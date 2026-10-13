// Host contract for msg_trigger__6Minioni; not an EE input. Callees are stubs that log.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
extern "C" {
char bossMeterName[128];
int bossMeterCurrent;
void soundResetStream__Fi(int s) { calls += "reset" + std::to_string(s) + ";"; }
void soundStartMusic__FPci(char* n, int v) { calls += std::string("music(") + n + "," + std::to_string(v) + ");"; }
}
#include "msg_trigger.cpp"
int main() {
    alignas(16) static unsigned char object[1024];
    int bad = 0;
    for (float f : {0.0f, 123.75f, -4.5f, 5000.0f}) {
        std::memset(bossMeterName, 'x', sizeof bossMeterName); bossMeterCurrent = -77; calls.clear();
        std::memcpy(object + 560, &f, 4);
        msg_trigger__6Minioni(object, 9);
        if (std::memcmp(bossMeterName, "GreaterMinion", 14) != 0) bad = 1;
        for (int i = 14; i < 128; ++i) if (bossMeterName[i] != 'x') bad = 1;
        if (bossMeterCurrent != static_cast<int>(f)) bad = 1;
        if (calls != "reset0;music(mx_boss_act4_grdemon_1.vag,259);") bad = 1;
    }
    if (bad) std::puts("fail");
    return bad;
}
