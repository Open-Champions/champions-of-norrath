// Host contract for msg_trigger__11SpiderQueeni; not an EE input. Includes the source.
#include "msg_trigger.cpp"
#include <cstdio>
#include <cstring>
#include <string>

char bossMeterName[128];
int bossMeterCurrent = -1;
static std::string calls;
extern "C" void soundResetStream__Fi(int s) { calls += "reset" + std::to_string(s) + ";"; }
extern "C" void soundStartMusic__FPci(const char* n, int f) {
    calls += std::string("music:") + n + ":" + std::to_string(f) + ";" + (std::strcmp(bossMeterName, "Shelox") == 0 && bossMeterCurrent == 77 ? "meter_ready;" : "meter_not_ready;");
}
int main() {
    alignas(16) unsigned char object[600] = {};
    float v = 77.9f; std::memcpy(object + 560, &v, 4);
    std::memset(bossMeterName, 'x', sizeof bossMeterName);
    msg_trigger__11SpiderQueeni(object, 5);
    if (calls != "reset0;music:mx_boss_act1_spidboss_l.vag:259;meter_ready;") { std::puts(calls.c_str()); return 1; }
    if (bossMeterName[7] != 'x' || bossMeterName[6] != 0) return 2;     // exactly 7 bytes written
    v = -3.5f; std::memcpy(object + 560, &v, 4); calls.clear();
    msg_trigger__11SpiderQueeni(object, 0);
    if (bossMeterCurrent != -3) return 3;                               // truncates toward zero
    return 0;
}
