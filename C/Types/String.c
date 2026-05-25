#include "String.h"

int beginsWith(const char *string, const char *prefix) {
    int stringLength = strlen(string);
    int prefixLength = strlen(prefix);

    if (prefixLength > stringLength) {
        return 0;
    }

    return strncmp(string, prefix, prefixLength) == 0;
}

int containChar(const char *string, const char c) {
    int length = strlen(string);

    for(int i = 0; i < length; i++) {
        if(string[i] == c) {
            return 1;
        }
    }

    return 0;
}

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

int endsWith(const char *string, const char *suffix) {
    int stringLength = strlen(string);
    int suffixLength = strlen(suffix);

    if (suffixLength > stringLength) {
        return 0;
    }

    return strcmp(string + stringLength - suffixLength, suffix) == 0;
}

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

void removeAll(const char *string, const char x, char *result) {
    int length = strlen(string);
    strncpy(result, string, length);
    int ptr = 0;
    int inString = 0;
    
    for(int i = 0; i < length; i++) {
        if(string[i] != x || inString) {
            result[ptr++] = string[i];
        }
    }
    result[ptr] = '\0';
}

void replace(const char *string, const char x, const char y, char *result) {
    int length = strlen(string);
    strncpy(result, string, length);
    result[length] = '\0';
    int inString = 0;

    for(int i = 0; i < length; i++) {
        if(result[i] == x || inString) {
            result[i] = y;
        }
    }
    
}

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
