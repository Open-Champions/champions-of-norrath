// Host check for sorted_dir.cpp; not an EE plan input.
#include "sorted_dir.cpp"

int main() {
    if (sortedDir[0] != 0 || sortedDir[639] != 0) return 1;
    sortedDir[33] = 7;
    if (sortedDir[33] != 7) return 2;
    return 0;
}
