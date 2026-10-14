#include "comp_immediate_decompressor.cpp"
int main() {
    if (sizeof(compImmediateDecompressor) != 80) return 1;
    for (unsigned char b : compImmediateDecompressor) if (b != 0) return 2;
    return 0;
}
