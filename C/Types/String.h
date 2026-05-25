#include <string.h>
#include <stdlib.h>

/// @brief Checks if a string begins with a certain prefix
/// @param string The string to check if it begins with <prefix>
/// @param prefix The prefix that <string> may begin with
/// @return 1 if <string> begins with <prefix>, 0 otherwise
int beginsWith(const char *string, const char *prefix);

/// @brief Checks if a string contains a certain character
/// @param string The string to check if it contains <c>
/// @param c The character that <string> may contain
/// @return 1 if <string> contains <c>, 0 otherwise
int containChar(const char *string, const char c);

/// @brief Checks if a string contains a certain character
/// @param string The string to check if it contains <c>
/// @param c The character that <string> may contain
/// @return 1 if <string> contains <c>, 0 otherwise
int containString(const char *string, const char *c);

/// @brief Counts the number of occurences of a certain 
/// character in a string
/// @param string The string to count the number of occurences 
/// of <c> in
/// @param c The character to count the number of occurences 
/// of in <string>
/// @return The number of occurences of <c> in <string>
int count(const char *string, const char c);

/// @brief Checks if a string ends with a certain suffix
/// @param string The string to check if it ends with <suffix>
/// @param suffix The suffix that <string> may end with
/// @return 1 if <string> ends with <suffix>, 0 otherwise
int endsWith(const char *string, const char *suffix);

/// @brief Gets the <slice>th slice of a string
/// @param string The string to get the slice from
/// @param delimeter The delimiter used to determine the slices
/// @param slice The index of the slice to get
/// @param result The <slice>th slice of <string>
void getSlice(const char *string, const char delimeter, const int slice, char *result);

/// @brief Removes all instances of <x> in <string>
/// @param string The string from which to remove <x> from
/// @param x The character from which to remove from <string>
/// @param result The string with all occurences of <x> removed
void removeAll(const char *string, const char x, char *result);

/// @brief Replaces all occurences of the character of <x> in 
/// <string> for <y>
/// @param string The string containing the characters to be replaced
/// @param x The character to be replaced with <y>
/// @param y The character to replace with <x>
/// @param result The string with occurences of <x> replaced with <y>
void replace(const char *string, const char x, const char y, char *result);

/// @brief Splits a string into two parts, with the first part 
/// being the substring before the first occurence of a character 
/// and the second part being the substring after the first 
/// occurence of a character
/// @param string The string to be split
/// @param delimeter The character to split the string at
/// @param left The string to store the part of the string 
/// before the first occurence of <delimeter>
/// @param right The string to store the part of the string 
/// after the first occurence of <delimiter>
void split(const char *string, const char delimeter, char *left, char *right);

/// @brief Converts a all numeric characters in a string to 
/// an integer, within the range of a byte
/// @param string The string to convert to an integer
/// @return The byte value of a string, 0 if the string 
/// does not contain any numeric characters
int toByte(const char *string);

/// @brief Converts a all numeric characters in a string to an 
/// integer, within the range 0 - <max>
/// @param string The string to convert to an integer
/// @param max The maximum value for the result
/// @return The integer value of the string modulo <max>, 0 
/// if the string does not contain any numeric characters
int toInteger(const char *string, const int max);
