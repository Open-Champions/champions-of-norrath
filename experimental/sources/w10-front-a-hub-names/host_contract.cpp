#include "hub_names.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof hubNames == 5 * sizeof(const char*), "five entries");
    const char* want[] = {"kelethin", "gnomehub", "beachhub", "elfhub", "airhub"};
    for (int i = 0; i < 5; ++i) {
        if (std::strcmp(hubNames[i], want[i]) != 0) return 1 + i;
    }
    std::puts("hub names passed");
    return 0;
}
