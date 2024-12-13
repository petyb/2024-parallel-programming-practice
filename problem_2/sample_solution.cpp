#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <thread>

std::vector<size_t> read_array() {
    size_t length, a, b, p;
    std::cin >> length >> a >> b >> p;
    std::vector<size_t> result(length);
    result[0] = a % p;
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = (result[i - 1] * a + b) % p;
    }
    return result;
}

void parallel_sort(std::vector<size_t>& a, int l, int r) {
    if (l + 10 <= r) {
        sort(a.begin() + l, a.begin() + r);
        return;
    }
    int m = (l + r) / 2;
    std::thread lthread(parallel_sort, std::ref(a), l, m);
    std::thread rthread(parallel_sort, std::ref(a), m, r);
    lthread.join();
    rthread.join();
    std::inplace_merge(a.begin() + l, a.begin() + m, a.begin() + r);
}

int main() {
    auto array = read_array();
    int n = array.size();
    parallel_sort(array, 0, array.size());
    size_t k;
    std::cin >> k;
    for (size_t i = k - 1; i < n; i += k) {
        std::cout << array[i] << ' ';
    }
    std::cout << "\n";

    return 0;
}