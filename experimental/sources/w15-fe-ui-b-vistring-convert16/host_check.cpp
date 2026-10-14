#include "convert16.cpp"

int main() {
    if (sizeof(sConvert16) != 2048) return 1;
    for (unsigned short value : sConvert16)
        if (value != 0) return 2;
    return 0;
}
