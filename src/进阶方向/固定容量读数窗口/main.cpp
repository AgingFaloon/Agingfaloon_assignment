#include <iostream>

namespace
{
	const int CAPACITY = 8;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    if (!(std::cin >> n)) return 0;

    int window[CAPACITY] = {0};
    int kept = 0;
    int sum = 0;
    int last = 0;
    int read = 0;

    for (int i = 0; i < n; ++i) {
        int value = 0;
        if (!(std::cin >> value)) break;

        const int slot = read % CAPACITY;
        if (kept == CAPACITY) {
            sum -= window[slot];
        } else {
            ++kept;
        }
        window[slot] = value;
        sum += value;
        last = value;
        ++read;
    }

    const int oldestSlot = (read >= CAPACITY) ? (read % CAPACITY) : 0;
    const int oldest = window[oldestSlot];

    std::cout << "kept=" << kept << '\n'
              << "oldest=" << oldest << '\n'
              << "last=" << last << '\n'
              << "sum=" << sum << '\n';

    return 0;
}
