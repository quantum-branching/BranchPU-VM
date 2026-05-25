#include <string.h>
#include <stdlib.h>

/// @brief Checks if a string begins with a certain prefix
/// @param string The string to check if it begins with <prefix>
/// @param prefix The prefix that <string> may begin with
/// @return 1 if <string> begins with <prefix>, 0 otherwise
int beginsWith(const char *string, const char *prefix) {
    int stringLength = strlen(string);
    int prefixLength = strlen(prefix);

    if (prefixLength > stringLength) {
        return 0;
    }

    return strncmp(string, prefix, prefixLength) == 0;
}

/// @brief Checks if a string contains a certain character
/// @param string The string to check if it contains <c>
/// @param c The character that <string> may contain
/// @return 1 if <string> contains <c>, 0 otherwise
int containChar(const char *string, const char c) {
    int length = strlen(string);

    for(int i = 0; i < length; i++) {
        if(string[i] == c) {
            return 1;
        }
    }

    return 0;
}

/// @brief Checks if a string contains a certain character
/// @param string The string to check if it contains <c>
/// @param c The character that <string> may contain
/// @return 1 if <string> contains <c>, 0 otherwise
int containString(const char *string, const char *c) {
    int length = strlen(string);

    if (c[0] == '\0') {
        return 1;
    }
    
    for(int i = 0; i < length; i++) {
        if(string[i] == c[0]) {
            return containString(string + i, c + 1);
        }
    }
    
    return 0;
}


/// @brief Counts the number of occurences of a certain 
/// character in a string
/// @param string The string to count the number of occurences 
/// of <c> in
/// @param c The character to count the number of occurences 
/// of in <string>
/// @return The number of occurences of <c> in <string>
int count(const char *string, const char c) {
    int length = strlen(string);
    int count = 0;

    for(int i = 0; i < length; i++) {
        if(string[i] == c) {
            count++;
        }
    }

    return count;
}

/// @brief Checks if a string ends with a certain suffix
/// @param string The string to check if it ends with <suffix>
/// @param suffix The suffix that <string> may end with
/// @return 1 if <string> ends with <suffix>, 0 otherwise
int endsWith(const char *string, const char *suffix) {
    int stringLength = strlen(string);
    int suffixLength = strlen(suffix);

    if (suffixLength > stringLength) {
        return 0;
    }

    return strcmp(string + stringLength - suffixLength, suffix) == 0;
}

/// @brief Gets the <slice>th slice of a string
/// @param string The string to get the slice from
/// @param delimeter The delimiter used to determine the slices
/// @param slice The index of the slice to get
/// @param result The <slice>th slice of <string>
void getSlice(const char *string, const char delimeter, const int slice, char *result) {
    int length = strlen(string);
    int currentSlice = 0;
    int inString = 0;
    result[0] = '\0';

    for(int i = 0; i < length; i++) {
        if(string[i] == '"') {
            inString = !inString;
        }

        if(string[i] == delimeter && !inString) {
            currentSlice++;
        } else if(currentSlice == slice) {
            strncat(result, &string[i], 1);
        }
    }
    strncat(result, "\0", 1);
}

/// @brief Replaces all occurences of the character of <x> in 
/// <string> for <y>
/// @param string The string containing the characters to be replaced
/// @param x The character to be replaced with <y>
/// @param y The character to replace with <x>
/// @param result The string with occurences of <x> replaced with <y>
void replace(const char *string, const char x, const char y, char *result) {
    int length = strlen(string);
    strncpy(result, string, length);
    result[length] = '\0';

    for(int i = 0; i < length; i++) {
        if(result[i] == x) {
            result[i] = y;
        }
    }
    
}

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
void split(const char *string, const char delimeter, char *left, char *right) {
    int length = strlen(string);
    int inString = 0;

    for(int i = 0; i < length; i++) {
        if(string[i] == '"') {
            inString = !inString;
        }

        if(string[i] == delimeter && !inString) {
            strncpy(left, string, i);
            left[i] = '\0';
            strcpy(right, string + i + 1);
            return;
        }
    }

    strcpy(left, string);
    right[0] = '\0';
}

/// @brief Converts a all numeric characters in a string to 
/// an integer, within the range of a byte
/// @param string The string to convert to an integer
/// @return The byte value of a string, 0 if the string 
/// does not contain any numeric characters
int toByte(const char *string) {
    int length = strlen(string);
    int result = 0;

    for(int i = 0; i < length; i++) {
        if(string[i] >= '0' && string[i] <= '9') {
            result *= 10;
            result += string[i] - '0';
        }
    }

    result = abs(result) & 255;
    return result;
}

/// @brief Converts a all numeric characters in a string to an 
/// integer, within the range 0 - <max>
/// @param string The string to convert to an integer
/// @param max The maximum value for the result
/// @return The integer value of the string modulo <max>, 0 
/// if the string does not contain any numeric characters
int toInteger(const char *string, const int max) {
    int length = strlen(string);
    int result = 0;

    for(int i = 0; i < length; i++) {
        if(string[i] >= '0' && string[i] <= '9') {
            result *= 10;
            result += string[i] - '0';
        }
    }

    result = abs(result) % max;
    return result;
}
