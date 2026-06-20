///@brief String.h but requires the caller pass available memory

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define debug0(x) printf(x)
#define debug(x,y) printf(x,y)
#define debug2(x,y,z) printf(x,y,z)

/// @brief A dynamic string
/// @param string The string to initialize the dstring with
/// @param length The length of the string
struct dstring {
    char *string;
    int length;
};

/// @brief Copies information from source to destination
/// @param source Dynamic string to copy from
/// @param destination Dynamic string to copy to
void dstring_copy(const struct dstring *source, struct dstring *destination);

/// @brief Removes all occurrences of a character from a dynamic string
/// @param dstring The dynamic string to remove the character from
/// @param c The character to remove
void dstring_remove(struct dstring *dstring, const char c);

/// @brief Replaces all occurrences of a character with another character in a dynamic string
/// @param dstring The dynamic string to replace the character in
/// @param x The character to replace
/// @param y The character to replace with
void dstring_replace(struct dstring *dstring, const char x, const char y);

/// @brief Gets the size of a dynamic string in bytes
/// @param dstring The dynamic string to get the size of
/// @return The size of the dynamic string in bytes
int dstring_sizeof(const struct dstring *dstring);

/// @brief Splits a dynamic string into two parts based on a delimiter
/// @param dstring The dstring to split
/// @param delimiter The character to split on
/// @param left A pointer to the left part of the split
/// @param right A pointer to the right part of the split
void dstring_split(const struct dstring *dstring, const char delimiter, struct dstring *left, struct dstring *right);

/// @brief Converts a dynamic string to lowercase
/// @param dstring The dstring to convert
void dstring_tolower(struct dstring *dstring);