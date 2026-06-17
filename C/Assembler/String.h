#include <stdlib.h>
#include <string.h>

#define DSTRING_EMPTY dstring_new("")

struct dstring {
    char *string;
    int length;
};

/// @brief Copies a dstring
/// @param dstring The dstring to copy
/// @return A new dstring with the same content
struct dstring *dstring_copy(struct dstring *dstring);

/// @brief Frees a dstring
/// @param dstring The dstring to free
void dstring_free(struct dstring *dstring);

/// @brief  Creates a new dstring
/// @param string The string to initialize the dstring with
/// @return A new dstring with the same content
struct dstring *dstring_new(const char *string);

/// @brief Removes a character from a dstring
/// @param dstring The dstring to remove the character from
/// @param c The character to remove
/// @return A new dstring with the character removed
struct dstring *dstring_remove(struct dstring *dstring, const char c);

/// @brief Replaces a character with a different character in a dstring
/// @param dstring The string to replace the character in
/// @param x The character to replace
/// @param y The character to replace with
/// @return A new dstring with the characters replaced
struct dstring *dstring_replace(struct dstring *dstring, const char x, const char y);

/// @brief Splits a dstring into two parts based on a delimiter
/// @param dstring The dstring to split
/// @param delimiter The character to split on
/// @param left A pointer to the left part of the split
/// @param right A pointer to the right part of the split
void dstring_split(const struct dstring *dstring, const char delimiter, struct dstring *left, struct dstring *right);