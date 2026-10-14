// Host check only; not a build input.
#include "foregrnd_tex_copy.cpp"

int main() {
    if (sizeof(foregrndTexCopy) != 64) return 1;
    for (unsigned i = 0; i < sizeof(foregrndTexCopy); ++i) {
        if (foregrndTexCopy[i] != 0) return 2;
    }
    return 0;
}
