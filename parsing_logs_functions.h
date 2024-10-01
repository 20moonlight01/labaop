#pragma once

#include <fstream>

#include "string_functions.h"
#include "structures.h"
#include "timestamp_functions.h"

void GetLine(std::ifstream& file, char* line);

void ParseLog(char* line, LogArguments* log);

void ParseTime(LogArguments* log);
