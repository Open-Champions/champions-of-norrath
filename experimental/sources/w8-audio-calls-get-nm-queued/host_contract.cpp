#include "get_nm_queued.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char stream[3 * 452] = {};
int main() {
    for (int i = 0; i < 3; ++i) {
        int v = 100 + i;
        std::memcpy(stream + i * 452 + 448, &v, 4);
        int other = -7;  // neighboring words must not be read
        std::memcpy(stream + i * 452 + 444, &other, 4);
    }
    for (int i = 0; i < 3; ++i)
        if (soundGetNmQueued__Fi(i) != 100 + i) return 1;
    int neg = -5;
    std::memcpy(stream + 452 + 448, &neg, 4);
    if (soundGetNmQueued__Fi(1) != -5) return 2;
    std::puts("get_nm_queued passed");
    return 0;
}
