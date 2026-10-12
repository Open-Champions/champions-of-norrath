#include "get_async_mode.cpp"

#include <cstdio>

extern "C" int asyncMode = 0;

int main() {
    asyncMode = -1;
    if (MC_GetAsyncMode__Fv() != -1) return 1;
    asyncMode = 0;
    if (MC_GetAsyncMode__Fv() != 0) return 2;
    asyncMode = 7;
    if (MC_GetAsyncMode__Fv() != 7) return 3;
    std::puts("get async mode passed");
    return 0;
}
