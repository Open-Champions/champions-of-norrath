// Host contract for msg_draw__10WaterSpout; not an EE input. Includes the source.
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string calls;
static unsigned char arena[128] __attribute__((aligned(16)));
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    calls += (o == arena ? "run;" : "bad;");
    calls += arena[88] == 0 ? "b88=0;" : "b88!=0;";
}
int main() {
    for (int b88 : {0, 1, 2, 0x7f, 0x80, 0xff}) {
        for (int b89 : {0, 1, 0xff}) {
            std::memset(arena, 0xa5, sizeof arena); calls.clear();
            arena[88] = static_cast<unsigned char>(b88); arena[89] = static_cast<unsigned char>(b89);
            msg_draw__10WaterSpout(arena);
            std::string want = b88 == 0 ? "run;b88=0;" : "";
            unsigned char want88 = b88 == 0 ? 1 : b88;
            if (calls != want || arena[88] != want88 || arena[89] != 0) { std::printf("fail %d %d: %s\n", b88, b89, calls.c_str()); return 1; }
            for (int i = 0; i < 128; ++i) if (i != 88 && i != 89 && arena[i] != 0xa5) return 2;
        }
    }
    return 0;
}
