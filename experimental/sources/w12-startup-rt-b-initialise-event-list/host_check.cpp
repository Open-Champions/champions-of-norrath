// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <cstdio>
#include <cstring>
#include "initialise_event_list.cpp"
int main() {
    unsigned char record[80], before[80];
    for (int i = 0; i < 80; ++i) record[i] = (unsigned char)(0xc0 + i);
    std::memcpy(before, record, sizeof record);
    initialise_event_list(record);
    for (int i = 0; i < 80; ++i) {
        const bool cleared = i >= 36 && i < 40;
        if (cleared ? record[i] != 0 : record[i] != before[i]) {
            std::printf("initialise_event_list wrong at byte %d\n", i);
            return 1;
        }
    }
    std::printf("initialise_event_list cases passed\n");
    return 0;
}
