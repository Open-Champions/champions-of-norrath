// Host contract for msg_trigger__12UndeadKnighti; not an EE input. Includes the source.
#include "msg_trigger.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
char bossMeterName[32];
int bossMeterCurrent;
static std::string calls; static unsigned roll;
extern "C" {
void soundResetStream__Fi(int s) { calls += "reset" + std::to_string(s) + ";"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
int sprintf(char* buffer, const char* format, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, format);
    const int value = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    calls += std::string("fmt(") + format + "," + std::to_string(value) + ");";
    return std::snprintf(buffer, 32, "name%d", value);
}
void soundStartMusic__FPci(char* name, int v) { calls += std::string("music(") + name + "," + std::to_string(v) + ");"; }
}
int main() {
    alignas(8) unsigned char obj[600];
    for (unsigned r : {0u, 1u, 2u, 3u, 4u, 7u, 65535u}) {
        for (int value : {0, 1, 5, -1}) {
            std::memset(obj, 0, sizeof obj); std::memset(bossMeterName, 'x', sizeof bossMeterName); bossMeterCurrent = -77;
            const float v = 12.75f; std::memcpy(obj + 560, &v, 4);
            roll = r; calls.clear();
            msg_trigger__12UndeadKnighti(obj, value);
            const std::string want = "reset0;rand;fmt(mx_boss_generic%d_l.vag," + std::to_string((r & 3) + 1) + ");music(name" + std::to_string((r & 3) + 1) + ",259);";
            if (calls != want) { std::printf("fail\n  %s\n  %s\n", calls.c_str(), want.c_str()); return 1; }
            if (std::memcmp(bossMeterName, "UndeadKnight", 13) != 0 || bossMeterName[13] != 'x' || bossMeterCurrent != 12) return 2;
        }
    }
    return 0;
}
