#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <thread>

std::istream& operator>>(std::istream& in, __int128& value) {
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        value = value * 10 + (s[i] - '0');
    }
    if (negative) value = -value;
    return in;
}

std::ostream& operator<<(std::ostream& out, __int128 value) {
    if (value == 0) {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    while (value > 0) {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

void find_factors(long long n, long long start, long long end, std::vector<long long>& factors) {
    for (long long i = start; i <= end; ++i) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
}

int main() {
    long long n;
    std::cin >> n;
    if (n <= 1) {
        return 0;
    }

    long long sqrt_n = (long long)(std::sqrt((long double)(n))) + 1;
    int num_threads = 16;
    long long range_size = sqrt_n / num_threads;

    std::vector<std::thread> threads;
    std::vector< std::vector < long long > > factors_(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        long long start = 2 + i * range_size;
        long long end = (i == num_threads - 1 ? sqrt_n : start + range_size - 1);

        threads.emplace_back(find_factors, n, start, end, std::ref(factors_[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    long long prod = 1;
    long long prev = -1;
    std::vector< long long > res;
    for (const auto& factors: factors_) {
        for (const auto& factor: factors) {
            if (factor == prev || std::__gcd(factor, prod) == 1) {
                prod *= factor;
                prev = factor;
                res.push_back(factor);
            }
        }
    }

    if (prod != n) {
        res.push_back(n / prod);
    }

    for (auto factor : res) {
        std::cout << factor << ' ';
    }
    std::cout << '\n';

    return 0;
}