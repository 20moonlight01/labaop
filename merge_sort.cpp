#include "merge_sort.h"

void Merge(std::vector<std::pair<char*, int>>& a, int left, int mid, int right) {
    int it_1 = 0;
    int it_2 = 0;
    std::vector<std::pair<char*, int>> result(right - left);
    while ((left + it_1 < mid) && (mid + it_2 < right)) {
        if (a[left + it_1].second > a[mid + it_2].second) {
            result[it_1 + it_2] = a[left + it_1];
            it_1 += 1;
        } else {
            result[it_1 + it_2] = a[mid + it_2];
            it_2 += 1;
        }
    }
    while (left + it_1 < mid) {
        result[it_1 + it_2] = a[left + it_1];
        it_1 += 1;
    }
    while (mid + it_2 < right) {
        result[it_1 + it_2] = a[mid + it_2];
        it_2 += 1;
    }
    for (int i = 0; i < it_1 + it_2; i++) {
        a[left + i] = result[i];
    }
}

void MergeSort(std::vector<std::pair<char*, int>>& a, int left, int right) {
    if (left + 1 >= right) {
        return;
    }
    int mid = (left + right) / 2;
    MergeSort(a, left, mid);
    MergeSort(a, mid, right);
    Merge(a, left, mid, right);
}
