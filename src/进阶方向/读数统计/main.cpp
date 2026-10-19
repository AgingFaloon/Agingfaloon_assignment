#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        return 1; 
    }
    
    int sum = 0;
    int min_val = 1001; 
    int max_val = -1001; 
    int val;

    for (int i = 0; i < n; ++i) {
        std::cin >> val;
        sum += val;
        if (val < min_val) {
            min_val = val;
        }
        if (val > max_val) {
            max_val = val;
        }
    }
    
    std::cout << "count=" << n << "\n";
    std::cout << "sum=" << sum << "\n";
    std::cout << "min=" << min_val << "\n";
    std::cout << "max=" << max_val << "\n";

    return 0;
}

