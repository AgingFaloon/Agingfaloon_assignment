#include "format.h"
#include <iostream>

void print_report(int count, long long total, int smallest, int largest) {
    std::cout << "count=" << count << '\n'
              << "sum=" << total << '\n'
              << "min=" << smallest << '\n'
              << "max=" << largest << '\n';
}
