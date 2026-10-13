#include "cd_is_busy_streaming.cpp"
#include <cstdio>
signed char cdBusy;
int main() {
    const signed char values[] = {0, 1, 2, 127, -128, -1};
    for (signed char v : values) {
        cdBusy = v;
        if (cdIsBusyStreamingNonwithstanding__Fv() != v) return 1;
    }
    std::puts("cd_is_busy_streaming passed");
    return 0;
}
