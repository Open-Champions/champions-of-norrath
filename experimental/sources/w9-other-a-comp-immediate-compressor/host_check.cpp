#include "comp_immediate_compressor.cpp"
int main() {
    if (sizeof(compImmediateCompressor) != 80) return 1;
    for (unsigned char b : compImmediateCompressor) if (b != 0) return 2;
    return 0;
}
