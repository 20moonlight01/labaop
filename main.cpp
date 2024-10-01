#include <fstream>
#include <iostream>
#include <vector>

#include "merge_sort.h"
#include "parsing_logs_functions.h"
#include "string_functions.h"
#include "structures.h"
#include "timestamp_functions.h"
#include "vector_functions.h"

int main(int argc, char* argv[])
{
    if (argc == 0 || argc == 1) {
        std::cout << "error: no arguments found" << std::endl;
        return 1;
    }
    char output[kMaxStringLength]{'\0'};
    char input[kMaxStringLength]{'\0'};
    bool printing = 0;
    int stats = 10;
    int window = 0;
    int from = 0;
    int to = -1;
    char zero[]{'0'};
    char arguments[][kMaxStringLength] = {
        "--output=", "--stats=", "--window=", "--from=", "--to=",
        "--print",
        "-o", "-s", "-w", "-f", "-t",
        "-p",
        "--output", "--stats", "--window", "--from", "--to"
    };
    bool input_file_was_found = 0;
    int i = 1;

    while (i < argc) {
        int argument_index = -1;
        for (int k = 0; k < 5; k++) {
            if (CompareArguments(argv[i], arguments[k]) == 1) {
                argument_index = k;
            }
        }
        if (argument_index == -1) {
            for (int k = 5; k < 17; k++) {
                if (CompareArguments(argv[i], arguments[k]) == 0) {
                    argument_index = k;
                }
            }
        }
        if ((-1 < argument_index) && (argument_index < 5)) {
            int it_1 = 0;
            int it_2 = 0;
            bool equal_flag = 0;
            char argument_value[kMaxStringLength];
            while (argv[i][it_1] != '\0') {
                if (argv[i][it_1] == '=') {
                    equal_flag = 1;
                } else if (equal_flag) {
                    argument_value[it_2] = argv[i][it_1];
                    it_2++;
                }
                it_1++;
            }
            argument_value[it_2] = '\0';
            if (argument_index == 0) {
                ChangeValue(output, argument_value);
            } else if (std::atoi(argument_value) != 0) {
                int arg_val = std::atoi(argument_value);
                if (argument_index == 1) {
                    stats = arg_val;
                } else if (argument_index == 2) {
                    window = arg_val;
                } else if (argument_index == 3) {
                    from = arg_val;
                } else if (argument_index == 4) {
                    to = arg_val;
                }
            } else if (!CompareString(argument_value, zero)) {
                std::cout << "Error!" << std::endl;
                return 1;
            }
            i++;
        } else if ((argument_index == 5) || (argument_index == 11)) {
            printing = 1;
            i++;
        } else if (((5 < argument_index) && (argument_index < 11)) || ((11 < argument_index) && (argument_index < 17)) && (i < argc - 1)) {
            char argument_value[kMaxStringLength];
            ChangeValue(argument_value, argv[i + 1]);
            if (argument_index == 6 || argument_index == 12) {
                ChangeValue(output, argument_value);
            } else if (std::atoi(argument_value) != 0) {
                int arg_val = std::atoi(argument_value);
                if (argument_index == 7 || argument_index == 13) {
                    stats = arg_val;
                } else if (argument_index == 8 || argument_index == 14) {
                    window = arg_val;
                } else if (argument_index == 9 || argument_index == 15) {
                    from = arg_val;
                } else if (argument_index == 10 || argument_index == 16) {
                    to = arg_val;
                }
            } else if (!CompareString(argument_value, zero)) {
                std::cout << "Error!" << std::endl;
                return 1;
            }
            i += 2;
        } else if ((argument_index == -1) && (!input_file_was_found)) {
            ChangeValue(input, argv[i]);
            input_file_was_found = 1;
            i++;
        } else {
            std::cout << "Error!" << std::endl;
            return 1;
        }
    }

    if (output[0] == '\0') {
        std::cout << "no output file" << std::endl;
        return 0;
    }
    if ((stats <= 0) || (window < 0)) {
        std::cout << "error: stats or window value is less than zero" << std::endl;
        return 1;
    }
    if ((from < 0) || (to < from && to != -1)) {
        std::cout << "error: from or to argument is invalid" << std::endl;
        return 1;
    }
    if (input[0] == '\0') {
        std::cout << "error: input file not found" << std::endl;
        return 1;
    }

    char line[kMaxStringLength]{'\0'};
    std::ifstream input_file(input);
    std::ofstream output_file(output);
    std::vector<std::pair<char*, int>> for_stats_vector;
    std::vector<long long int> timestamps;
    long long int max_count_of_requests = 0;
    long long int cur_count_of_requests = 0;
    long long int first_value = -1;
    long long int max_from = 0;
    long long int max_to = 0;
    if ((input_file.is_open()) && (output_file.is_open())) {
        while (!input_file.eof()) {
            LogArguments log;
            GetLine(input_file, line);
            ParseLog(line, &log);
            if (log.remote_addr[0] == '\0') {
                continue;
            }
            if (log.time[0] == '\0') {
                continue;
            }
            if (log.request[0] == '\0') {
                continue;
            }
            if (log.status == -1) {
                continue;
            }
            if (log.bytes_send == -1) {
                continue;
            }
            ParseTime(&log);
            log.timestamp = TransferIntoTimestamp(
                log.date, log.month, log.year, log.hours, log.mins, log.secs, log.utc_hours, log.utc_mins
                );
            if (log.timestamp < from) {
                continue;
            }
            if ((log.timestamp > to) && (to != -1)) {
                break;
            }
            if ((500 <= log.status) && (log.status < 600)) {
                bool element_is_already_in_vector = 0;
                for (int i = 0; i < for_stats_vector.size(); i++) {
                    if (CompareString(log.request, for_stats_vector[i].first)) {
                        for_stats_vector[i].second++;
                        element_is_already_in_vector = 1;
                        break;
                    }
                }
                if (!element_is_already_in_vector) {
                    char* first_elem = new char[kMaxStringLength];
                    int j = 0;
                    while (log.request[j] != '\0') {
                        first_elem[j] = log.request[j];
                        j++;
                    }
                    first_elem[j] = '\0';
                    std::pair<char*, int> elem(first_elem, 1);
                    for_stats_vector.push_back(elem);
                }
            }
            if (window == 0) {
                continue;
            }
            if (first_value == -1) {
                first_value = log.timestamp;
            } else if (log.timestamp - first_value >= window) {
                if (cur_count_of_requests > max_count_of_requests) {
                    max_count_of_requests = cur_count_of_requests;
                    max_from = timestamps[0];
                    max_to = timestamps[timestamps.size() - 1];
                }
                while (timestamps[0] == first_value) {
                    PopFront(timestamps);
                    cur_count_of_requests--;
                    if (timestamps.size() == 0) {
                        break;
                    }
                }
                first_value = timestamps[0];
            }
            timestamps.push_back(log.timestamp);
            cur_count_of_requests++;
        }

        MergeSort(for_stats_vector, 0, for_stats_vector.size());
        int i = 0;
        output_file << "Most frequent requests finished with a 5XX error:\n";
        if (printing) {
            std::cout << "Most frequent requests finished with a 5XX error:" << std::endl;
        }
        while ((i < stats) && (i < for_stats_vector.size())) {
            output_file << for_stats_vector[i].first << ": " << for_stats_vector[i].second << "\n";
            if (printing) {
                std::cout << for_stats_vector[i].first << ": " << for_stats_vector[i].second << std::endl;
            }
            i++;
        }

        if (window != 0) {
            if (cur_count_of_requests > max_count_of_requests) {
                max_count_of_requests = cur_count_of_requests;
                max_from = timestamps[0];
                max_to = timestamps[timestamps.size() - 1];
            }
            if (max_to - max_from < window - 1) {
                max_to = max_from + (window - 1);
            }
            TimeStruct max_first_border = TransferFromTimestamp(max_from);
            TimeStruct max_second_border = TransferFromTimestamp(max_to);
            output_file << "Window: ";
            output_file << max_first_border.dates << "/" << max_first_border.month + 1 << "/" << max_first_border.year << " ";
            output_file << max_first_border.hours << "::" << max_first_border.mins << "::" << max_first_border.secs << "-";
            output_file << max_second_border.dates << "/" << max_second_border.month + 1 << "/" << max_second_border.year << " ";
            output_file << max_second_border.hours << "::" << max_second_border.mins << "::" << max_second_border.secs << "\n";
            output_file << "Requests: " << max_count_of_requests << "\n";
            if (printing) {
                std::cout << "Window: ";
                std::cout << max_first_border.dates << "/" << max_first_border.month + 1 << "/" << max_first_border.year << " ";
                std::cout << max_first_border.hours << "::" << max_first_border.mins << "::" << max_first_border.secs << "-";
                std::cout << max_second_border.dates << "/" << max_second_border.month + 1 << "/" << max_second_border.year << " ";
                std::cout << max_second_border.hours << "::" << max_second_border.mins << "::" << max_second_border.secs << "\n";
                std::cout << "Requests: " << max_count_of_requests << "\n";
            }
        }

        input_file.close();
        output_file.close();
        for (int i = 0; i < for_stats_vector.size(); i++) {
            delete [] for_stats_vector[i].first;
        }
    } else {
        if (input_file.is_open()) {
            input_file.close();
        }
        if (output_file.is_open()) {
            output_file.close();
        }
        std::cout << "error: input or output file cannot be opened";
        return 1;
    }

    return 0;
}
