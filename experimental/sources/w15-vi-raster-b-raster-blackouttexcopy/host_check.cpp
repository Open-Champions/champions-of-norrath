// Host check only; not a build input.
#include "blackout_tex_copy.cpp"

int main() {
    if (sizeof(blackoutTexCopy) != 64) return 1;
    for (unsigned i = 0; i < sizeof(blackoutTexCopy); ++i) {
        if (blackoutTexCopy[i] != 0) return 2;
    }
    return 0;
}
