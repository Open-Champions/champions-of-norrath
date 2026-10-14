#define NORRATH_EH_FRAME_SECTION __attribute__((used))
#include "eh_frame_begin.cpp"

#include <cstdio>

int main() {
    if (sizeof __EH_FRAME_BEGIN__ != 0) return 1;
    if (static_cast<void*>(__EH_FRAME_BEGIN__) == nullptr) return 2;
    std::puts("EH frame begin contract passed");
    return 0;
}
