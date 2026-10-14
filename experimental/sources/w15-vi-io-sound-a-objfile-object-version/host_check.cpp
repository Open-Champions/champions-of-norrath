// Host check only; not a build input.
#include "object_version.cpp"
int main() {
    alignas(8) unsigned char o[512] = {};
    *reinterpret_cast<int*>(o + 232) = -1;
    *reinterpret_cast<unsigned short*>(o + 242) = 9;
    if (ObjectVersion__9VIObjFile(o) != 0) return 1;
    *reinterpret_cast<int*>(o + 232) = 0;
    if (ObjectVersion__9VIObjFile(o) != 9) return 2;
    *reinterpret_cast<int*>(o + 232) = 2;
    *reinterpret_cast<unsigned short*>(o + 2 * 32 + 242) = 0xfffe;
    if (ObjectVersion__9VIObjFile(o) != 0xfffe) return 3;
    return 0;
}
