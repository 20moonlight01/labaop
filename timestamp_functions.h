#pragma once

#include "structures.h"

int GetMonthNumber(char* month);

long long int TransferIntoTimestamp(int date, int month, int year, int hours, int mins, int secs, int utc_hours, int utc_mins);

TimeStruct TransferFromTimestamp(long long int timestamp);
