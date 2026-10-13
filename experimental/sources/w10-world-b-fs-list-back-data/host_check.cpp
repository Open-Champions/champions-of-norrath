// Host check for fsListBack. Not an EE plan input.
#include <cstdio>
#include "fs_list_back_data.cpp"
int main() {
    if (fsListBack != nullptr) return 1;
    static unsigned char node[4]; fsListBack = node;
    if (fsListBack != node) return 2;
    std::puts("fsListBack check passed");
    return 0;
}
