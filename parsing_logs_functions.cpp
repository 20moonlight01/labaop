#include "parsing_logs_functions.h"

void GetLine(std::ifstream& file, char* line) {
    char ch;
    int i = 0;
    file.get(ch);
    while ((ch != '\n') && (!file.eof())) {
        line[i] = ch;
        file.get(ch);
        i++;
    }
    line[i] = '\0';
}

void ParseLog(char* line, LogArguments* log) {
    int i = 0;
    int loc_cnt = 0;
    char value[kMaxStringLength];
    char zero[] = "0";
    while (line[i] != ' ') {
        if (line[i] == '\0') {
            (*log).remote_addr[0] = '\0';
            return;
        }
        (*log).remote_addr[loc_cnt] = line[i];
        i++;
        loc_cnt++;
    }
    (*log).remote_addr[loc_cnt] = '\0';
    loc_cnt = 0;
    while (line[i] != '[') {
        if (line[i] == '\0') {
            (*log).time[0] = '\0';
            return;
        }
        value[loc_cnt] = line[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    char test_str[] = " - - ";
    if (!CompareString(value, test_str)) {
        (*log).time[0] = '\0';
        return;
    }
    loc_cnt = 0;
    i++;
    while (line[i] != ']') {
        if (line[i] == '\0') {
            (*log).time[0] = '\0';
            return;
        }
        (*log).time[loc_cnt] = line[i];
        i++;
        loc_cnt++;
    }
    (*log).time[loc_cnt] = '\0';
    if (!(line[i + 1] == ' ' && line[i + 2] == '"')) {
        (*log).request[0] = '\0';
        return;
    }
    i += 3;
    loc_cnt = 0;
    while (line[i] != '"') {
        if (line[i] == '\0') {
            (*log).request[0] = '\0';
            return;
        }
        (*log).request[loc_cnt] = line[i];
        i++;
        loc_cnt++;
    }
    (*log).request[loc_cnt] = '\0';
    if (!(line[i + 1] == ' ') || (line[i + 2] == '\0')) {
        (*log).status = -1;
        return;
    }
    i += 2;
    loc_cnt = 0;
    while (line[i] != ' ') {
        if (line[i] == '\0') {
            (*log).status = -1;
            return;
        }
        value[loc_cnt] = line[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    if (std::atoi(value) == 0 && !CompareString(value, zero)) {
        (*log).status = -1;
        return;
    }
    (*log).status = std::atoi(value);
    if (line[i + 1] == '\0') {
        (*log).bytes_send = -1;
        return;
    }
    i++;
    loc_cnt = 0;
    while (line[i] != '\0') {
        value[loc_cnt] = line[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    char no_bytes[] = "-";
    if (std::atoi(value) == 0 && !CompareString(value, zero) && !CompareString(value, no_bytes)) {
        (*log).bytes_send = -1;
        return;
    }
    (*log).bytes_send = std::atoi(value);
}

void ParseTime(LogArguments* log) {
    int i = 0;
    int loc_cnt = 0;
    char value[kMaxStringLength];
    while ((*log).time[i] != '/') {
        value[loc_cnt] = (*log).time[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).date = std::atoi(value);
    loc_cnt = 0;
    i++;
    while ((*log).time[i] != '/') {
        value[loc_cnt] = (*log).time[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).month = GetMonthNumber(value);
    loc_cnt = 0;
    i++;
    while ((*log).time[i] != ':') {
        value[loc_cnt] = (*log).time[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).year = std::atoi(value);
    loc_cnt = 0;
    i++;
    while ((*log).time[i] != ':') {
        value[loc_cnt] = (*log).time[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).hours = std::atoi(value);
    loc_cnt = 0;
    i++;
    while ((*log).time[i] != ':') {
        value[loc_cnt] = (*log).time[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).mins = std::atoi(value);
    loc_cnt = 0;
    i++;
    while ((*log).time[i] != ' ') {
        value[loc_cnt] = (*log).time[i];
        i++;
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).secs = std::atoi(value);
    loc_cnt = 0;
    i++;
    for (int j = i; j < i + 3; j++) {
        value[loc_cnt] = (*log).time[j];
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).utc_hours = std::atoi(value);
    loc_cnt = 0;
    for (int j = i + 4; j < i + 6; j++) {
        value[loc_cnt] = (*log).time[j];
        loc_cnt++;
    }
    value[loc_cnt] = '\0';
    (*log).utc_mins = std::atoi(value);
}
