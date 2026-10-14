#include "last_dir_count.cpp"

#include <cstdio>

int main() {
    if (sizeof MCA_LastDirCount != 4 || MCA_LastDirCount != 0) return 1;
    MCA_LastDirCount = 3;
    if (MCA_LastDirCount != 3) return 2;
    std::puts("last dir count passed");
    return 0;
}
