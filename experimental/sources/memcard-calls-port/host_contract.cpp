#include "port.cpp"

#include <cstdio>

int main() {
    if (sizeof MCA_Port != 4 || MCA_Port != 0) return 1;
    MCA_Port = -1;
    if (MCA_Port != -1) return 2;
    std::puts("mca port passed");
    return 0;
}
