// Host check only; not a build input.
#include "num_sub_objects.cpp"
int main() {
    alignas(8) unsigned char o[512] = {};
    *reinterpret_cast<int*>(o + 232) = -1;
    if (NumSubObjects__9VIObjFile(o) != -1) return 1;
    *reinterpret_cast<int*>(o + 232) = 0;
    *reinterpret_cast<int*>(o + 248) = 7;
    if (NumSubObjects__9VIObjFile(o) != 7) return 2;
    *reinterpret_cast<int*>(o + 232) = 3;
    *reinterpret_cast<int*>(o + 3 * 32 + 248) = 11;
    if (NumSubObjects__9VIObjFile(o) != 11) return 3;
    return 0;
}
