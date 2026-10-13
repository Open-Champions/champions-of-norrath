// Host contract for marble. Not an EE plan input.
#include <cstdio>
extern "C" void* marble;
int main() {
    if (sizeof marble != sizeof(void*)) return 1;
    if (marble != nullptr) {
        std::fprintf(stderr, "fail: marble\n");
        return 1;
    }
    return 0;
}
