// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
char bossMeterName[128];
int bossMeterCurrent;
}
static std::string calls;
static int reset_stream;
static std::string music_name;
static int music_flags;
extern "C" void soundResetStream__Fi(int stream) { calls += "reset;"; reset_stream = stream; }
extern "C" void soundStartMusic__FPci(char* name, int flags) { calls += "music;"; music_name = name; music_flags = flags; }
#include "msg_trigger.cpp"

int main() {
    int failures = 0;
    alignas(8) unsigned char object[600];
    const float healths[] = {0.0f, 1.5f, -2.5f, 99999.5f};
    const int rolls[] = {0, 1, 2};  // repeated calls must not depend on earlier ones
    for (float health : healths) {
        for (int roll : rolls) {
            std::memset(object, 0x21, sizeof object);
            std::memcpy(object + 560, &health, 4);
            std::memset(bossMeterName, 'z', sizeof bossMeterName);
            bossMeterCurrent = -77 - roll;
            calls.clear();
            msg_trigger__10Arenabeasti(object, 12345);
            if (calls != "reset;music;") { std::fprintf(stderr, "fail: order %s\n", calls.c_str()); ++failures; }
            if (music_name != "mx_boss_act4_arenabeast_l.vag" || music_flags != 259 || reset_stream != 0) {
                std::fprintf(stderr, "fail: music %s\n", music_name.c_str()); ++failures;
            }
            if (std::strcmp(bossMeterName, "ArenaBeast") != 0 || bossMeterName[11] != 'z') { std::fprintf(stderr, "fail: name\n"); ++failures; }
            if (bossMeterCurrent != static_cast<int>(health)) { std::fprintf(stderr, "fail: current\n"); ++failures; }
            if (object[559] != 0x21 || object[564] != 0x21) { std::fprintf(stderr, "fail: object written\n"); ++failures; }
        }
    }
    return failures ? 1 : 0;
}
