#include <cstdio>
extern "C" int fullFrameMode;
int main() {
    if (fullFrameMode != 0) return 1;
    std::puts("fullFrameMode contract passed");
    return 0;
}
