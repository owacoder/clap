#include "string.h"
#include "stdlib.h"

#ifdef CLAP_SAVE_SPACE
int iscntrl(int chr) {
    if (chr == EOF)
        return 0;

    return chr < 32 || chr == 127;
}

int isprint(int chr) {
    return chr >= 32 && chr < 127;
}

int isspace(int chr) {
    return chr == 32 || (chr >= 9 && chr <= 13);
}

int isblank(int chr) {
    return chr == 32 || chr == 9;
}

int isgraph(int chr) {
    return chr > 32 && chr < 127;
}

int ispunct(int chr) {
    return (chr > 32 && chr < 48) || (chr > 57 && chr < 65) || (chr > 90 && chr < 97) || (chr > 122 && chr < 127);
}

int isupper(int chr) {
    return chr > 64 && chr < 91;
}

int islower(int chr) {
    return chr > 96 && chr < 123;
}

int isalpha(int chr) {
    return isupper(chr) || islower(chr);
}

int isdigit(int chr) {
    return chr > 47 && chr < 58;
}

int isxdigit(int chr) {
    return isdigit(chr) || (chr > 64 && chr < 71) || (chr > 96 && chr < 103);
}

int isalnum(int chr) {
    return isalpha(chr) || isdigit(chr);
}
#endif

#define ATOI \
    int negative = 0;                               \
                                                    \
    if (*str == '-' || *str == '+')                 \
        negative = *str++ == '-';                   \
                                                    \
    if (negative) {                                 \
        for (; isdigit(*str); ++str)                \
            result = (result * 10) - (*str - '0');  \
    } else {                                        \
        for (; isdigit(*str); ++str)                \
            result = (result * 10) + (*str - '0');  \
    }

int atoi(const char *str) {
    int result = 0;

    ATOI

    return result;
}

long atol(const char *str) {
    long result = 0;

    ATOI

    return result;
}

long long atoll(const char *str) {
    long long result = 0;

    ATOI

    return result;
}

void *memchr(const void *mem, int chr, size_t size) {
    const unsigned char *ptr = mem;

    while (size-- > 0) {
        if (*ptr == (unsigned char) chr)
            return (void *) ptr;

        ++ptr;
    }

    return NULL;
}

int memcmp(const void *lhs, const void *rhs, size_t size) {
    const unsigned char *lhsptr = lhs;
    const unsigned char *rhsptr = rhs;

    while (size-- > 0) {
        if (*lhsptr < *rhsptr)
            return -1;
        else if (*lhsptr > *rhsptr)
            return 1;

        ++lhsptr;
        ++rhsptr;
    }

    return 0;
}

void *memset(void *mem, int chr, size_t size) {
    unsigned char *ptr = mem;

    while (size-- > 0) {
        *ptr++ = (unsigned char) chr;
    }

    return mem;
}

void *memcpy(void *dest, const void *src, size_t size) {
    unsigned char *destptr = dest;
    const unsigned char *srcptr = src;

    while (size-- > 0) {
        *destptr++ = *srcptr++;
    }

    return dest;
}

void *memmove(void *dest, const void *src, size_t size) {
    unsigned char *destptr = dest;
    const unsigned char *srcptr = src;

    if (destptr == srcptr)
        return dest;
    else if (destptr < srcptr || (size_t) (destptr - srcptr) >= size)
        return memcpy(dest, src, size);

    /* Copy backwards from src to dest */
    while (size-- > 0) {
        destptr[size] = srcptr[size];
    }

    return dest;
}

size_t strlen(const char *str) {
    const char *start = str;

    for (; *str; ++str);

    return str - start;
}

int strcmp(const char *lhs, const char *rhs) {
    for (; *lhs && *rhs; ++lhs, ++rhs) {
        if ((unsigned char) *lhs < (unsigned char) *rhs)
            return -1;
        else if ((unsigned char) *lhs > (unsigned char) *rhs)
            return 1;
    }

    if (*lhs)
        return 1;
    else if (*rhs)
        return -1;
    else
        return 0;
}

int strncmp(const char *lhs, const char *rhs, size_t size) {
    for (; *lhs && *rhs && size > 0; ++lhs, ++rhs, --size) {
        if ((unsigned char) *lhs < (unsigned char) *rhs)
            return -1;
        else if ((unsigned char) *lhs > (unsigned char) *rhs)
            return 1;
    }

    if (size == 0)
        return 0;
    else if (*lhs)
        return 1;
    else if (*rhs)
        return -1;
    else
        return 0;
}

int strcoll(const char *lhs, const char *rhs) {
    return strcmp(lhs, rhs);
}

char *strchr(const char *str, int chr) {
    for (; *str; ++str) {
        if (*str == (char) chr)
            return (char *) str;
    }

    return chr == 0? (char *) str: NULL;
}

char *strrchr(const char *str, int chr) {
    const char *lastFound = NULL;

    for (; *str; ++str) {
        if (*str == (char) chr)
            lastFound = str;
    }

    return (char *) (chr == 0? str: lastFound);
}

size_t strspn(const char *dest, const char *src) {
    const char *save = dest;

    for (; *dest; ++dest) {
        if (NULL == strchr(src, *dest))
            break;
    }

    return dest - save;
}

size_t strcspn(const char *dest, const char *src) {
    const char *save = dest;

    for (; *dest; ++dest) {
        if (NULL != strchr(src, *dest))
            break;
    }

    return dest - save;
}

char *strpbrk(const char *dest, const char *src) {
    for (; *dest; ++dest) {
        if (NULL != strchr(src, *dest))
            return (char *) dest;
    }

    return NULL;
}

char *strstr(const char *str, const char *substr) {
    for (; *str; ++str) {
        const char *haystack = str;
        const char *needle = substr;

        for (; *haystack && *needle; ++haystack, ++needle) {
            if (*haystack != *needle)
                goto next_try;
        }

        return *needle? NULL: (char *) str;

next_try: ;
    }

    return NULL;
}

char *strtok(char *str, const char *delim) {
    static char *save = NULL;
    char *temp;

    if (str == NULL) {
        if (save == NULL)
            return NULL;
        str = save;
    }

    temp = strpbrk(str, delim);
    if (temp == NULL) {
        save = NULL;
        return NULL;
    }

    save = temp + strspn(temp, delim);
    if (*save)
        *save++ = 0;
    else
        save = NULL;

    return (char *) temp;
}

char *strcpy(char *dest, const char *src) {
    char *destptr = dest;

    while ((*destptr++ = *src++));

    return dest;
}

char *strncpy(char *dest, const char *src, size_t size) {
    char *destptr = dest;

    while (size-- > 0 && (*destptr++ = *src++));

    return dest;
}

char *strcat(char *dest, const char *src) {
    return strcpy(dest + strlen(dest), src);
}

char *strncat(char *dest, const char *src, size_t size) {
    char *destptr = dest;
    const char *srcptr = src;

    destptr += strlen(destptr);

    while (size-- > 0) {
        *destptr++ = *srcptr++;
    }

    *destptr = 0;

    return dest;
}

/* Supposed to transform the string into something that lets strcmp compare the same as strcoll for the current locale.
 * (see https://en.cppreference.com/w/c/string/byte/strxfrm)
 * Since locales aren't implemented yet (if ever), this just copies the string
 */
size_t strxfrm(char *dest, const char *src, size_t size) {
    if (size == 0)
        return strlen(src);

    strncpy(dest, src, size);
    return size;
}

#ifdef CLAP_DYNAMIC_MEMORY
char *strdup(const char *str) {
    size_t len = strlen(str);
    char *newstr = malloc(len + 1);

    if (newstr != NULL)
        memcpy(newstr, str, len);

    return newstr;
}

char *strndup(const char *str, size_t size) {
    const char *strptr = str;

    /* Short-circuit the string length */
    while (*strptr && strptr - str <= size)
        ++strptr;

    size = *strptr? size: strptr - str;
    char *newstr = malloc(size + 1);
    if (newstr != NULL) {
        memcpy(newstr, str, size);
        newstr[size] = 0;
    }

    return newstr;
}
#endif
