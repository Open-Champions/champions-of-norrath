#include <cstdio>
extern "C" unsigned* dmaGif;
int main() {
    if (dmaGif != nullptr) return 1;
    std::puts("dmaGif contract passed");
    return 0;
}
