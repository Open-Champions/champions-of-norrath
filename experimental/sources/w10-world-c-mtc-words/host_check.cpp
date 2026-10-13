// Host contract for the mtcStart and mtcExit storage. Not an EE plan input.
#include <cstdio>
#include "mtc_words.cpp"

int main() {
    if (mtcStart != 0 || mtcExit != 0 || sizeof(mtcStart) != 4 || sizeof(mtcExit) != 4) {
        std::fprintf(stderr, "fail: two zero words\n");
        return 1;
    }
    std::printf("mtc words contract passed\n");
    return 0;
}
