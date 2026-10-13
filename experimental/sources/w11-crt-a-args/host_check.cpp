// Host contract for _args. Not an EE plan input.
#include <cstdio>
#include "args.cpp"
int main() {
    if (sizeof(_args) != 0x180 || (reinterpret_cast<__UINTPTR_TYPE__>(_args) & 15) != 0) return 1;
    for (unsigned i = 0; i < sizeof(_args); ++i) if (_args[i] != 0) return 2;
    std::puts("_args ok");
    return 0;
}
