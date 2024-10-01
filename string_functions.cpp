#include "string_functions.h"

bool CompareString(char* string_1, char* string_2) {
    int j = 0;
    bool are_equal = 1;
    while (string_2[j] != '\0') {
        if ((string_1[j] == '\0') || (string_1[j] != string_2[j])) {
            are_equal = 0;
            break;
        }
        j++;
    }
    if ((string_2[j] == '\0') && ('\0' == string_1[j]) && (are_equal)) {
        return 1;
    }

    return 0;
}

int CompareArguments(char* string_1, char* string_2) {
    int j = 0;
    bool are_equal = 1;
    while (string_2[j] != '\0') {
        if ((string_1[j] == '\0') || (string_1[j] != string_2[j])) {
            are_equal = 0;
            break;
        }
        j++;
    }
    if ((string_2[j] == '\0') && ('\0' == string_1[j]) && (are_equal)) {
        return 0;
    } else if (are_equal) {
        return 1;
    }
    
    return 2;
}

void ChangeValue(char* value, char* argument_value) {
    int i = 0;
    while (argument_value[i] != '\0') {
        value[i] = argument_value[i];
        i++;
    }
    value[i] = '\0';
}
