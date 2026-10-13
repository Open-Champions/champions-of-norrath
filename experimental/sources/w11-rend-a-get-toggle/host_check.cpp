#include <cstring>
extern "C" int GetToggle__6Camera(unsigned char* camera);
int main() {
    unsigned char cam[512] = {};
    const int v[] = {0, 1, 2, -1, 0x7fffffff};
    for (int x : v) {
        std::memcpy(cam + 84, &x, 4);
        if (GetToggle__6Camera(cam) != x) return 1;
    }
    cam[83] = 0x7f; cam[88] = 0x7f;
    int zero = 0; std::memcpy(cam + 84, &zero, 4);
    if (GetToggle__6Camera(cam) != 0) return 2;
    return 0;
}
