//Please open this by VS Code or Visual Studio,or it's annotations can't be read
#include <iostream>
#include <map>
#include <string>

int main() {
    int count = 0;
    if (!(std::cin >> count)) return 1;

    // 两张表：编号 -> 在架册数 / 借出册数。
    // 数量统一用 long long，copies 也按 64 位读入，避免册数累加后溢出。
    std::map<std::string, long long> onShelf;
    std::map<std::string, long long> borrowed;

    std::string operation;
    std::string code;
    for (int i = 0; i < count; i++) {
        if (!(std::cin >> operation >> code)) return 1;

        // return 只有两个字段，只有 add/borrow 才读第三个字段 copies，
        // 读入部分与模板保持一致。
        long long copies = 0;
        if (operation != "return") {
            if (!(std::cin >> copies)) return 1;
        }

        // 编号首次出现时取引用即在两张表里登记，两张表初始数量都是 0；
        // map 的键序就是编号字典序，输出明细前不用再排序，也不会出现重复编号。
        long long& shelf = onShelf[code];
        long long& lent = borrowed[code];

        // 任一操作不能执行时立刻输出一行 rejected 并 return 0：
        // 此前成功的操作不输出，也不先扣减再加回去，避免留下中间态。
        if (operation == "add") {
            // add 把 copies 累加到在架。
            shelf += copies;
        } else if (operation == "borrow") {
            // borrow 先比较在架是否足够，够了才扣在架、加借出。
            if (shelf < copies) {
                std::cout << "rejected\n";
                return 0;
            }
            shelf -= copies;
            lent += copies;
        } else if (operation == "return") {
            // return 先确认该编号借出至少 1 册，再借出减 1、在架加 1。
            if (lent < 1) {
                std::cout << "rejected\n";
                return 0;
            }
            lent -= 1;
            shelf += 1;
        } else {
            // 未定义的操作一律视为不能执行。
            std::cout << "rejected\n";
            return 0;
        }
    }

    // 全部成功后先汇总再输出明细：只统计在架或借出至少有一个大于 0 的编号，
    // titles 是这些编号的个数，onShelf、borrowed 是当前在架总册数和借出总册数。
    long long titles = 0;
    long long totalShelf = 0;
    long long totalBorrowed = 0;
    for (const auto& entry : onShelf) {
        const long long shelf = entry.second;
        const long long lent = borrowed.at(entry.first);
        if (shelf <= 0 && lent <= 0) continue;
        titles++;
        totalShelf += shelf;
        totalBorrowed += lent;
    }

    std::cout << "titles=" << titles << "\n";
    std::cout << "onShelf=" << totalShelf << "\n";
    std::cout << "borrowed=" << totalBorrowed << "\n";

    // 明细逐行输出「编号 在架册数 借出册数」，顺序即 map 的键序，
    // 在架为 0 但借出大于 0 的编号也要保留。
    for (const auto& entry : onShelf) {
        const long long shelf = entry.second;
        const long long lent = borrowed.at(entry.first);
        if (shelf <= 0 && lent <= 0) continue;
        std::cout << entry.first << " " << shelf << " " << lent << "\n";
    }
    return 0;
}
