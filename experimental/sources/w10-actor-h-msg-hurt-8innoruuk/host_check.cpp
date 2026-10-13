// Host contract for Innoruuk msg_hurt; not an EE input.
#include <cstdio>
#include <cstring>
extern "C" int bossMeterCurrent = 7;
#include "msg_hurt.cpp"
static int calls; static void* sSelf; static float sAmount; static void* sAttacker; static int sKind;
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* s, float a, void* at, int k) { ++calls; sSelf = s; sAmount = a; sAttacker = at; sKind = k; }
int main() {
    int bad = 0, dummy;
    alignas(16) unsigned char object[1100];
    const float values[] = {1.5f, -7.9f, 0.0f, 123456.0f, 0.99f, -0.5f};
    const unsigned gates[] = {0, 1, 0x10000, 0xffffffffu};
    for (float v : values) for (unsigned gate : gates) {
        std::memset(object, 0x21, sizeof object); calls = 0; bossMeterCurrent = 7;
        std::memcpy(object + 984, &gate, 4); std::memcpy(object + 560, &v, 4);
        msg_hurt__8InnoruukfP10GameObjecti(object, 3.25f, &dummy, 9);
        if (gate != 0) {
            if (calls != 0 || bossMeterCurrent != 7) { std::puts("fail: nonzero gate does nothing"); bad = 1; }
        } else {
            if (calls != 1 || sSelf != object || sAmount != 3.25f || sAttacker != &dummy || sKind != 9) { std::puts("fail: base call arguments"); bad = 1; }
            if (bossMeterCurrent != static_cast<int>(v)) { std::printf("fail: meter %d for %f\n", bossMeterCurrent, v); bad = 1; }
        }
    }
    if (!bad) std::puts("innoruuk msg_hurt ok");
    return bad;
}
