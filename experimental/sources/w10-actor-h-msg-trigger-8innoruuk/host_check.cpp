// Host contract for Innoruuk msg_trigger; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
extern "C" char bossMeterName[128];
extern "C" int bossMeterCurrent;
char bossMeterName[128];
int bossMeterCurrent;
#include "msg_trigger.cpp"
static std::string calls; static int resetArg = -1, musicMode; static std::string musicName; static std::string nameAtMusic; static int meterAtMusic;
extern "C" void soundResetStream__Fi(int s) { calls += "reset;"; resetArg = s; }
extern "C" void soundStartMusic__FPci(char* n, int m) { calls += "music;"; musicName = n; musicMode = m; nameAtMusic = bossMeterName; meterAtMusic = bossMeterCurrent; }
int main() {
    int bad = 0;
    alignas(16) unsigned char object[1100];
    const float values[] = {1.5f, -7.9f, 0.0f, 123456.0f, 1000.7f};
    for (float v : values) {
        std::memset(object, 0x21, sizeof object); std::memset(bossMeterName, 'Z', sizeof bossMeterName); calls.clear(); bossMeterCurrent = -5;
        std::memcpy(object + 560, &v, 4);
        msg_trigger__8Innoruuki(object, 77);
        if (calls != "reset;music;" || resetArg != 0) { std::puts("fail: call order or reset argument"); bad = 1; }
        if (musicName != "mx_boss_act5_innoruuk_l.vag" || musicMode != 259) { std::puts("fail: music arguments"); bad = 1; }
        if (std::memcmp(bossMeterName, "Innoruuk\0ZZZ", 12) != 0) { std::puts("fail: name copy of exactly nine bytes"); bad = 1; }
        if (nameAtMusic != "Innoruuk" || meterAtMusic != static_cast<int>(v) || bossMeterCurrent != static_cast<int>(v)) { std::puts("fail: meter value or ordering"); bad = 1; }
    }
    if (!bad) std::puts("innoruuk msg_trigger ok");
    return bad;
}
