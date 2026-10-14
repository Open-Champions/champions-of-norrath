// Host check only; not a build input.
#include "buffer_clear.cpp"
int main() {
    alignas(8) unsigned char f[200];
    for (unsigned char& b : f) b = 0xaa;
    if (BufferClear__6VIFile(f) != 0) return 1;
    for (int i = 0; i < 200; ++i) {
        bool cleared = (i >= 136 && i < 172) || (i >= 176 && i < 184);
        if (cleared ? f[i] != 0 : f[i] != 0xaa) return 2;
    }
    return 0;
}
