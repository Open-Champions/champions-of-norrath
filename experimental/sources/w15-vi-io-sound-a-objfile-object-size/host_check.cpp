// Host check only; not a build input.
#include "object_size.cpp"
int main() {
    alignas(8) unsigned char o[512] = {};
    *reinterpret_cast<int*>(o + 232) = -1;
    *reinterpret_cast<unsigned*>(o + 244) = 100;
    if (ObjectSize__9VIObjFile(o) != 0) return 1;
    *reinterpret_cast<int*>(o + 232) = 0;
    if (ObjectSize__9VIObjFile(o) != 100) return 2;
    *reinterpret_cast<int*>(o + 232) = 4;
    *reinterpret_cast<unsigned*>(o + 4 * 32 + 244) = 0x80000001u;
    if (ObjectSize__9VIObjFile(o) != 0x80000001u) return 3;
    return 0;
}
