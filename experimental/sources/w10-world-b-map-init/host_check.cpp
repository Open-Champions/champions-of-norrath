// Host contract for init__3mapi. Not an EE plan input.
#include <cstdio>
int _3map$iconListCount = 7;
int _3map$revealed = 9;
#include "map_init.cpp"
int main() {
    init__3mapi(5);
    if (_3map$iconListCount != 0 || _3map$revealed != 0) return 1;
    _3map$iconListCount = -1; _3map$revealed = -1;
    init__3mapi(0);
    if (_3map$iconListCount != 0 || _3map$revealed != 0) return 2;
    std::puts("init__3mapi check passed");
    return 0;
}
