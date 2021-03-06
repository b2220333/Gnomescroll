/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef _WIN32
# include <sys/types.h>
# include <pwd.h>
#endif

#include <common/mallox.hpp>

#ifndef UINT8_MAX
# define UINT8_MAX  (255U)
#endif

#ifndef UINT16_MAX
# define UINT16_MAX (65535U)
#endif

template<typename T>
bool in_array(const T elem, const T* arr, size_t len)
{
    for (size_t i=0; i<len; i++)
        if (arr[i] == elem)
            return true;
    return false;
}

template<typename T>
size_t get_array_len(const T* arr, const T terminator)
{   // determines the length of a terminator-terminated array
    size_t len = 0;
    while (arr[len] != terminator)
        len++;
    return len;
}

int count_digits(int n)
{
    int digits = 0;
    do
    {
        digits++;
        n /= 10;
    } while (n > 0);
    return digits;
}

void address_from_string(char* ip, int address[4]);
void address_from_uint32(uint32_t ip, uint8_t address[4]);

char* get_time_str();
time_t atott(const char* str);

time_t utc_now();

char* strip_whitespace(const char* str)
{
    IF_ASSERT(str == NULL) return NULL;
    if (str[0] == '\0')
        return (char*)calloc(1, sizeof(char));
    int i=0;
    while (isspace(str[i])) i++;
    int start = i;
    i = int(strlen(str)) - 1;
    while (isspace(str[i])) i--;
    int end = i;
    int len = end-start+1;
    char* nstr = (char*)malloc((len+1) * sizeof(char));
    strncpy(nstr, &str[start], len);
    nstr[len] = '\0';
    return nstr;
}

unsigned char* hexstr_to_char(const char* hexstr)
{
    size_t len = strlen(hexstr);
    IF_ASSERT(len % 2 != 0)
        return NULL;
    size_t final_len = len / 2;
    unsigned char* chrs = (unsigned char*)malloc((final_len+1) * sizeof(*chrs));
    for (size_t i=0, j=0; j<final_len; i+=2, j++)
        chrs[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i+1] % 32 + 9) % 25;
    chrs[final_len] = '\0';
    return chrs;
}

static unsigned int strhash(const char* s)
{
    unsigned int highorder;
    unsigned int h = 0;
    for (int i=0; s[i] != '\0'; i++)
    {
         highorder = h & 0xf8000000;    // extract high-order 5 bits from h
                                        // 0xf8000000 is the hexadecimal representation
                                        //   for the 32-bit number with the first five
                                        //   bits = 1 and the other bits = 0
         h = h << 5;                    // shift h left by 5 bits
         h = h ^ (highorder >> 27);     // move the highorder 5 bits to the low-order
                                        //   end and XOR into h
         h = h ^ (unsigned int)s[i];                  // XOR h and ki
    }
    return h;
}

ALWAYS_INLINE bool is_strict_char(const char c)
{
    return (c >= 33 && c <= 126);
}

bool str_starts_with(const char* str, const char* match)
{
    if (match[0] == '\0') return true;
    if (str[0] == '\0') return false;
    int i=0;
    char c = '\0';
    char d = '\0';
    while ((c = str[i]) != '\0' && (d = match[i]) != '\0')
    {
        if (c != d) return false;
        i++;
    }
    return (match[i] == '\0');
}

bool str_ends_with(const char* str, const char* match)
{
    if (match[0] == '\0') return true;
    if (str[0] == '\0') return false;
    int i = (int)strlen(str);
    int j = (int)strlen(match);
    if (j > i) return false;
    for (i=i-1, j=j-1; i >= 0 && j >= 0; i--, j--)
        if (str[i] != match[j]) return false;
    return true;
}

static long long int parse_int(const char* str, bool& err)
{
    char* endptr = NULL;
    long long int val = strtoll(str, &endptr, 10);
    err = (*str == '\0' || *endptr != '\0');
    return val;
}

bool is_valid_name_char(char c)
{   // For string names used in configs. Do not fuck with this
    return (isalnum(c) || c == '_' || c == '-');
}

bool is_valid_name(const char* name)
{   // for string names using in config. don't change
    size_t len = strlen(name);
    if (len <= 0 || len > DAT_NAME_MAX_LENGTH) return false;
    for (size_t i=0; i<len; i++)
        if (!is_valid_name_char(name[i]))
            return false;
    return true;
}

void _test_common()
{
    GS_ASSERT(str_ends_with("foobar", "bar"));
    GS_ASSERT(str_ends_with("foobar", "foobar"));
    GS_ASSERT(str_ends_with("foobar", ""));
    GS_ASSERT(!str_ends_with("foobar", "ba"));
    GS_ASSERT(!str_ends_with("foobar", "foo"));
    GS_ASSERT(!str_ends_with("foobar", "afoobar"));
    GS_ASSERT(!str_ends_with("", "afoobar"));

    GS_ASSERT(str_starts_with("foobar", "foo"));
    GS_ASSERT(str_starts_with("foobar", ""));
    GS_ASSERT(str_starts_with("foobar", "foobar"));
    GS_ASSERT(!str_starts_with("foobar", "oo"));
    GS_ASSERT(!str_starts_with("foobar", "bar"));
    GS_ASSERT(!str_starts_with("foobar", "foobarr"));
    GS_ASSERT(!str_starts_with("", "foobarr"));
}

size_t title_string(const char* str, char* out, size_t size)
{   // WARNING: does not copy null terminator over if size is too short
    size_t i=0;
    bool should_upper = true;
    for (; i<size && str[i] != '\0'; i++)
    {
        char c = str[i];
        if (c == '_') c = ' ';
        if (should_upper && islower(c))
        {
            c = toupper(c);
            should_upper = false;
        }
        else if (!isalpha(c))
            should_upper = true;
        out[i] = c;
    }
    if (i < size) out[i] = '\0';
    return i;
}

void make_pretty_name(const char* src, char* dest)
{   // WARNING: make sure dest is at least as long as src
    // capitalize string
    size_t len = strlen(src);
    MALLOX(char, title, len+1);
    size_t wrote = title_string(src, title, len);
    title[wrote] = '\0';
    // remove numbers
    size_t j=0;
    for (size_t i=0; title[i] != '\0'; i++)
        if (!isdigit(title[i]))
            dest[j++] = title[i];
    dest[j] = '\0';
}

char* make_pretty_name(const char* src)
{
    char* dest = (char*)malloc(strlen(src) + 1);
    make_pretty_name(src, dest);
    return dest;
}

float round_half_from_zero(float f)
{
    return (f > 0.0f) ? floorf(f + 0.5f) : ceilf(f - 0.5f);
}

float ceil_from_zero(float f)
{
    return (f > 0.0f) ? ceilf(f) : floorf(f);
}

template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline bool is_equal(float a, float b)
{   // NOTE: epsilon isnt scaled to the values in question
    static const float epsilon = 0.0005f;
    return (fabsf(a - b) < epsilon);
}

void copy_string(char* dest, const char* src, size_t n)
{
    IF_ASSERT(n == 0 || dest == NULL || src == NULL) return;
    strncpy(dest, src, n);
    GS_ASSERT(dest[n - 1] == '\0');
    dest[n - 1] = '\0';
}
