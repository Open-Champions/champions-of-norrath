#include "comp_local_decompressor.cpp"
int main() {
    if (sizeof(compLocalDecompressor) != 80) return 1;
    for (unsigned char b : compLocalDecompressor) if (b != 0) return 2;
    return 0;
}
