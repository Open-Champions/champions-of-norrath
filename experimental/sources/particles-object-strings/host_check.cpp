// Host contract for objectStrings. Not an EE plan input.
#include <cstdio>
extern "C" unsigned objectStrings;
int main() {
    if (sizeof objectStrings != 4u) return 1;
    if (objectStrings != 0u) {
        std::fprintf(stderr, "fail: objectStrings\n");
        return 1;
    }
    return 0;
}
