#include <cstdio>
#include "data_handle.cpp"
int main() {
    if (sizeof dataHandle != 4 || dataHandle != 0) return 1;
    dataHandle = -1;
    std::puts("ok");
    return 0;
}
