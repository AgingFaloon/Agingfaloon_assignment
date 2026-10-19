#include <iostream>
#include <string>

namespace {

const int kTypeCount = 4;
const char* const kTypeNames[kTypeCount] = {"printer", "scanner", "router", "server"};

struct Totals {
    int count = 0;
    int hours = 0;
};

int typeIndex(const std::string& type) {
    for (int i = 0; i < kTypeCount; ++i) {
        if (type == kTypeNames[i]) return i;
    }
    return -1;
}

int busiestIndex(const Totals byType[]) {
    int best = 0;
    for (int i = 1; i < kTypeCount; ++i) {
        if (byType[i].hours > byType[best].hours) best = i;
    }
    return best;
}

}  

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    if (!(std::cin >> n)) return 0;

    Totals byType[kTypeCount];
    int repairHours = 0;  

    for (int i = 0; i < n; ++i) {
        std::string type;
        std::string status;
        int hours = 0;
        if (!(std::cin >> type >> status >> hours)) break;

        const int idx = typeIndex(type);
        if (idx < 0) continue;

        byType[idx].count += 1;
        byType[idx].hours += hours;
        if (status == "repair") repairHours += hours;
    }

    for (int i = 0; i < kTypeCount; ++i) {
        std::cout << kTypeNames[i] << ' '
                  << byType[i].count << ' '
                  << byType[i].hours << '\n';
    }
    std::cout << "busiest " << kTypeNames[busiestIndex(byType)] << '\n';

    (void)repairHours;
    return 0;
}
