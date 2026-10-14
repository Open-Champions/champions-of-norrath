// Host contract for linePos. Not an EE plan input.
#include <cstdio>
extern "C" int linePos;
int main() {
    if (sizeof linePos != 4u) return 1;
    if (linePos != 0) {
        std::fprintf(stderr, "fail: linePos\n");
        return 1;
    }
    return 0;
}
