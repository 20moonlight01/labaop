#pragma once

const int kMaxStringLength = 1000;

struct TimeStruct {
    int dates;
    int month;
    int year;
    int hours;
    int mins;
    int secs;
};

struct LogArguments {
    char remote_addr[kMaxStringLength];
    char time[kMaxStringLength];
    long long int timestamp;
    int date;
    int month;
    int year;
    int hours;
    int mins;
    int secs;
    int utc_hours;
    int utc_mins;
    char request[kMaxStringLength];
    int status;
    int bytes_send;
};
