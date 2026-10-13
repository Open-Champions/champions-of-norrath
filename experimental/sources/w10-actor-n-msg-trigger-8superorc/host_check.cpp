// Host contract for msg_trigger__8SuperOrci; not an EE input. Includes the source.
#include "msg_trigger.cpp"
#include <cstdio>
#include <cstring>
#include <string>

char bossMeterName[128];
int bossMeterCurrent = -1;
static std::string calls;
static unsigned roll;
extern "C" void soundResetStream__Fi(int s) { calls += "reset" + std::to_string(s) + ";"; }
extern "C" unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
extern "C" int sprintf(char* out, const char* fmt, ...) {
    // Only the one format this function uses; the argument is read from the va_list.
    __builtin_va_list ap; __builtin_va_start(ap, fmt);
    int v = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    if (std::strcmp(fmt, "mx_boss_generic%d_l.vag") != 0) return -1;
    return std::snprintf(out, 32, "mx_boss_generic%d_l.vag", v);
}
extern "C" void soundStartMusic__FPci(const char* n, int f) {
    calls += std::string("music:") + n + ":" + std::to_string(f) + ";";
}
int main() {
    alignas(16) unsigned char object[600] = {};
    float v = 12.25f; std::memcpy(object + 560, &v, 4);
    std::memset(bossMeterName, 'x', sizeof bossMeterName);
    for (unsigned r : {0u, 1u, 2u, 3u, 4u, 0xfffffffeu}) {
        calls.clear(); roll = r;
        msg_trigger__8SuperOrci(object, 9);
        std::string want = "reset0;rand;music:mx_boss_generic" + std::to_string((r & 3) + 1) + "_l.vag:259;";
        if (calls != want) { std::puts(calls.c_str()); return 1; }
    }
    if (std::strcmp(bossMeterName, "SuperOrc") != 0 || bossMeterName[9] != 'x' || bossMeterCurrent != 12) return 2;
    return 0;
}
