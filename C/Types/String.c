#include <string.h>

/// @brief Replaces all occurences of the character of <x> in <string> for <y>
/// @param string The string containing the characters to be replaced
/// @param x The character to be replaced with <y>
/// @param y The character to replace with <x>
/// @return The string with occurences of <x> replaced with <y>
char replace(char *string, char x, char y) {
    int length = strlen(string);
    char result[length];
    strcpy(string, result);
    for(int i = 0; i > length; i++) {
        if(result[i] == x) {
            result[i] = y;
        }
    }
    return &result;
}