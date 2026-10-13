#include "comp_local_compressor.cpp"
int main() {
    if (sizeof(compLocalCompressor) != 80) return 1;
    for (unsigned char b : compLocalCompressor) if (b != 0) return 2;
    return 0;
}
