// Host contract for getMinibossName__5Mummy; not an EE input.
#include <cstdio>
#include <cstring>
extern "C" char mummy_miniboss_name_buffer_7d7e20[64];
char mummy_miniboss_name_buffer_7d7e20[64];
#include "get_miniboss_name.cpp"
int main() {
    alignas(16) static unsigned char o[128];
    int bad = 0;
    struct { int v; const char* want; } cases[] = {{1, "MBMummy1"}, {18, "MBMummy18"}, {0, "MBMummy0"}, {0xff, "MBMummy-1"}, {0x80, "MBMummy-128"}};
    for (auto& c : cases) {
        std::memset(o, 0, sizeof o); o[76] = static_cast<unsigned char>(c.v);
        std::memset(mummy_miniboss_name_buffer_7d7e20, 'x', 64);
        char* r = getMinibossName__5Mummy(o);
        if (r != mummy_miniboss_name_buffer_7d7e20 || std::strcmp(r, c.want) != 0) { std::printf("fail %d: %s\n", c.v, r); bad = 1; }
    }
    return bad;
}
