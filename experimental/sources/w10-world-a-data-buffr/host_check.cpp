#include "buffr.cpp"
#include <cstring>
int main() {
    if (sizeof(buffr) != 1280) return 1;
    for (unsigned i = 0; i < sizeof(buffr); i++) if (((const unsigned char*)&buffr[0])[i] != 0) return 2;
    return 0;
}
