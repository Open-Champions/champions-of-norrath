#include <cstring>
struct ItemObserved {
    const char* name;
    unsigned observed[15];
};
static const ItemObserved rows[] = {
#include "rows.hpp"
};
static int find_name(const char* name) {
    unsigned index = 0;
    for (;;) {
        if (rows[index].name == nullptr) return 0;
        if (std::strcmp(rows[index].name, name) == 0) return static_cast<int>(index);
        index += 1;
    }
}
int main() {
    if (sizeof rows / sizeof rows[0] != 429u) return 1;
    if (std::strcmp(rows[0].name, "shortbow") != 0) return 2;
    static const unsigned want[] = {0x1u, 0x40000000u, 0x80001u, 0xcu, 0x0u, 0xa0000u, 0x40002u, 0x4u, 0x0u, 0x40800800u, 0x0u, 0x4832d0u, 0x1eu, 0x0u, 0x0u};
    for (unsigned index = 0; index < 15; ++index)
        if (rows[0].observed[index] != want[index]) return 3;
    if ((rows[0].observed[2] & 0xffffu) != (0x1u)) return 4;
    if (rows[428].name != nullptr) return 5;
    if (find_name("missing-item-name") != 0) return 6;
    if (find_name(rows[1].name) != 1) return 7;
    return 0;
}
