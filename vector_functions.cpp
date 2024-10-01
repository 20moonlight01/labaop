#include "vector_functions.h"

void PopFront(std::vector<long long int>& a) {
    if (a.size() == 0) {
        return;
    }
    if (a.size() == 1) {
        a.pop_back();
        return;
    }
    for (int i = 1; i < a.size(); i++) {
        a[i - 1] = a[i];
    }
    a.pop_back();
}
