#include "mca_file_open.cpp"

#include <cstdio>

int main() {
    if (sizeof(MCA_FileOpen) != 4 || MCA_FileOpen != 0) return 1;
    std::puts("mca file open passed");
    return 0;
}
