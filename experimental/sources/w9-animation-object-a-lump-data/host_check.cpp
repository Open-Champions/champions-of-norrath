// Host check for the lump pointer storage. Not an EE plan input.
#include <cstdio>
#include "lump_data.cpp"
int main() {
    if (domeLump != nullptr || dialogLumpName != nullptr || eqLumpHeader != nullptr) {
        std::fprintf(stderr, "fail: pointers do not start null\n");
        return 1;
    }
    if (sizeof domeLump != sizeof(void*) || sizeof dialogLumpName != sizeof(char*)) { std::fprintf(stderr, "fail: widths\n"); return 2; }
    static char name[] = "x";
    dialogLumpName = name;
    if (dialogLumpName[0] != 'x') { std::fprintf(stderr, "fail: text pointer\n"); return 3; }
    std::printf("lump pointer storage check passed\n");
    return 0;
}
