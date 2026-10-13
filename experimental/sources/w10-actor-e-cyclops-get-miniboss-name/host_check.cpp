// Host contract for getMinibossName__7Cyclops. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "get_miniboss_name.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[96] = {};
    char* first = nullptr;
    const struct { unsigned char byte; const char* text; } rows[] = {
        {1, "MBCyclops1"}, {9, "MBCyclops9"}, {0, "MBCyclops0"}, {127, "MBCyclops127"},
        {128, "MBCyclops-128"}, {255, "MBCyclops-1"}};
    for (const auto& row : rows) {
        object[76] = row.byte;
        char* name = getMinibossName__7Cyclops(object);
        if (!first) first = name;
        if (name != first || std::strcmp(name, row.text) != 0) { std::printf("fail %u: %s\n", row.byte, name); return 1; }
    }
    return 0;
}
