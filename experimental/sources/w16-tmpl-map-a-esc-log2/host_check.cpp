// Host check only; not a build input. Paired-run compared with the original in a private interpreter.
#include "log2.cpp"
#include <cstdio>
int main() {
    const int in[] = {-5, 0, 1, 2, 3, 4, 7, 8, 255, 256, 0x7fffffff};
    const int want[] = {0, 0, 1, 2, 2, 3, 3, 4, 8, 9, 31};
    for (unsigned i = 0; i < sizeof in / sizeof in[0]; ++i)
        if (Log2__13VIEESurfCachei(nullptr, in[i]) != want[i]) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
