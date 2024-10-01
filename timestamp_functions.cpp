#include "string_functions.h"
#include "timestamp_functions.h"

int GetMonthNumber(char* month) {
    char months[][kMaxStringLength] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (CompareString(month, months[i])) {
            return i;
        }
    }

    return -1;
}

long long int TransferIntoTimestamp(int date, int month, int year, int hours, int mins, int secs, int utc_hours, int utc_mins) {
    long long int res = 0;
    int days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    res += (year - 1970) * 365 * 24 * 3600;
    res += days[month] * 24 * 3600;
    res += (date - 1) * 24 * 3600;
    res += hours * 3600;
    res += mins * 60;
    res += secs;
    res += (year - 1970) / 4 * 24 * 3600;
    if ((month > 1) && ((year % 400 == 0) || (year % 4 == 0))) {
        res += 24 * 3600;
    }
    if (utc_hours < 0) {
        res -= utc_hours * 3600;
        res += utc_mins * 60;
    } else {
        res += utc_hours * 3600;
        res -= utc_mins * 60;
    }

    return res;
}

TimeStruct TransferFromTimestamp(long long int timestamp) {
    long long int res = 0;
    int days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int odd_days[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
    TimeStruct time;
    int mins = timestamp / 60;
    time.secs = timestamp - mins * 60;
    int hours = mins / 60;
    time.mins = mins - hours * 60;
    int dates = hours / 24;
    time.hours = hours - dates * 24;
    int years = dates / 365;
    time.dates = dates - years * 365;
    time.dates -= years / 4;
    int months = 0;
    if ((1970 + years) % 4 == 0) {
        for (int i = 0; i < 12; i++) {
            if (time.dates >= odd_days[i]) {
                months = i;
            }
        }
        time.dates -= odd_days[months];
    } else {
        for (int i = 0; i < 12; i++) {
            if (time.dates >= days[i]) {
                months = i;
            }
        }
        time.dates -= days[months];
    }
    time.month = months;
    time.year = 1970 + years;
    time.dates++;
    return time;
}
