// Host contract for msg_load__7CyclopsPc. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_load.cpp"
#include <cstdio>
static int calls = 0, result = 0;
static void* seen_self = nullptr; static char* seen_data = nullptr;
static unsigned char* watch = nullptr; static unsigned byte40_during = 0;
extern "C" int msg_load__8CreaturePc(void* self, char* data) {
    ++calls; seen_self = self; seen_data = data; byte40_during = static_cast<unsigned char*>(self)[40]; return result;
}
int main() {
    const struct { int half; int count; } rows[] = {{10, 17}, {9, 3}, {11, 0}, {0, -5}, {-10, 99}, {0x100a & 0x7fff, 4}};
    for (const auto& row : rows) {
        unsigned char object[600]; char data[8];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x33;
        vslots_b::write_u16(vslots_b::address(object) + 572u, static_cast<unsigned>(row.half) & 0xffffu);
        calls = 0; result = row.count;
        const int got = msg_load__7CyclopsPc(object, data);
        if (got != row.count || calls != 1 || seen_self != object || seen_data != data || byte40_during != 0x33u) { std::printf("fail call %d\n", row.half); return 1; }
        const unsigned want = row.half == 10 ? 120u : 0x33u;
        if (object[40] != want) { std::printf("fail byte %d\n", row.half); return 1; }
        for (unsigned i = 0; i < sizeof object; ++i)
            if (i != 40 && i != 572 && i != 573 && object[i] != 0x33) { std::printf("stray %u\n", i); return 1; }
    }
    return 0;
}
