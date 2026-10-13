// Host contract for msg_hurt__9MummyKingfP10GameObjecti; not an EE input.
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
static std::string calls;
extern "C" {
char bossMeterName[128];
int bossMeterCurrent;
void msg_hurt__8CreaturefP10GameObjecti(void*, float a, void* who, int v) { calls += "hurt(" + std::to_string(a) + "," + (who ? "obj" : "null") + "," + std::to_string(v) + ");"; }
}
#include "msg_hurt.cpp"
int main() {
    alignas(16) static unsigned char o[1024];
    int bad = 0;
    int dummy;
    for (unsigned long long flags : {0ull, 0x04000000ull, 0xfbffffffull, 0x0400000000000000ull, 0x04000000ffffffffull}) {
        for (float f : {0.0f, 77.9f}) {
            std::memset(o, 0, sizeof o); std::memset(bossMeterName, 'x', sizeof bossMeterName); bossMeterCurrent = -9; calls.clear();
            std::memcpy(o + 160, &flags, 8); std::memcpy(o + 560, &f, 4);
            msg_hurt__9MummyKingfP10GameObjecti(o, 3.5f, &dummy, 12);
            const bool skip = (flags & 0x04000000ull) != 0;
            const bool ok = skip ? (calls.empty() && bossMeterCurrent == -9 && bossMeterName[0] == 'x')
                                 : (calls == "hurt(3.500000,obj,12);" && std::memcmp(bossMeterName, "MummyKing", 10) == 0 && bossMeterName[10] == 'x' && bossMeterCurrent == static_cast<int>(f));
            if (!ok) { std::printf("fail flags=%llx\n", flags); bad = 1; }
        }
    }
    return bad;
}
