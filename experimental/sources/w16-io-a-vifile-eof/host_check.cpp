// Host check only; not a build input.
#include "eof.cpp"
int main() {
    alignas(8) unsigned char f[64] = {};
    *reinterpret_cast<int*>(f + 8) = 5;
    if (Eof__6VIFile(f) != 5) return 1;
    *reinterpret_cast<int*>(f) = -1;
    if (Eof__6VIFile(f) != -1) return 2;
    *reinterpret_cast<int*>(f + 48) = 1;
    if (Eof__6VIFile(f) != 5) return 3;
    *reinterpret_cast<int*>(f + 48) = 0;
    *reinterpret_cast<int*>(f + 56) = 1;
    if (Eof__6VIFile(f) != 5) return 4;
    *reinterpret_cast<int*>(f) = 0;
    *reinterpret_cast<int*>(f + 56) = 0;
    if (Eof__6VIFile(f) != 5) return 5;
    return 0;
}
