// Host check for sorted_dir_count.cpp; not an EE plan input.
#include "sorted_dir_count.cpp"

int main() {
    if (sortedDirCount != 0) return 1;
    sortedDirCount = 10;
    if (sortedDirCount != 10) return 2;
    return 0;
}
