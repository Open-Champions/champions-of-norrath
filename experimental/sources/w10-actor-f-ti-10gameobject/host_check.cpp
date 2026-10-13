// Host contract for __ti10GameObject: the storage is eight zero bytes with word alignment. Not an EE input.
#include "ti.cpp"
int main() {
    if (sizeof(__ti10GameObject) != 8) return 1;
    if ((reinterpret_cast<__UINTPTR_TYPE__>(__ti10GameObject) & 3u) != 0) return 2;
    for (unsigned i = 0; i < sizeof(__ti10GameObject); ++i)
        if (__ti10GameObject[i] != 0) return 3;
    return 0;
}
