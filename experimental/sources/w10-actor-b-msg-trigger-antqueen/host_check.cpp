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
static unsigned rand_value;
static void* rand_self;
static int reset_stream;
static std::string music_name;
static int music_flags;
extern "C" void soundResetStream__Fi(int stream) { calls += "reset;"; reset_stream = stream; }
extern "C" unsigned eRand__10GameObject(void* self) { calls += "rand;"; rand_self = self; return rand_value; }
extern "C" void soundStartMusic__FPci(char* name, int flags) { calls += "music;"; music_name = name; music_flags = flags; }
#include "msg_trigger.cpp"

int main() {
    int failures = 0;
    alignas(8) unsigned char object[600];
    const float healths[] = {0.0f, 1.5f, -2.5f, 99999.5f};
    const unsigned rolls[] = {0u, 1u, 2u, 3u, 4u, 7u, 0x80000001u, 0xffffffffu, 0x12345676u};
    for (float health : healths) {
        for (unsigned roll : rolls) {
            std::memset(object, 0x21, sizeof object);
            std::memcpy(object + 560, &health, 4);
            std::memset(bossMeterName, 'z', sizeof bossMeterName);
            bossMeterCurrent = -77;
            rand_value = roll;
            calls.clear();
            msg_trigger__8AntQueeni(object, 12345);
            if (calls != "reset;rand;music;") { std::fprintf(stderr, "fail: order %s\n", calls.c_str()); ++failures; }
            char expected[40];
            std::snprintf(expected, sizeof expected, "mx_boss_generic%u_l.vag", (roll & 3u) + 1u);
            if (music_name != expected || music_flags != 259 || reset_stream != 0 || rand_self != object) {
                std::fprintf(stderr, "fail: music %s\n", music_name.c_str()); ++failures;
            }
            if (std::strcmp(bossMeterName, "AntQueen") != 0 || bossMeterName[9] != 'z') { std::fprintf(stderr, "fail: name\n"); ++failures; }
            if (bossMeterCurrent != static_cast<int>(health)) { std::fprintf(stderr, "fail: current\n"); ++failures; }
            if (object[559] != 0x21 || object[564] != 0x21) { std::fprintf(stderr, "fail: object written\n"); ++failures; }
        }
    }
    return failures ? 1 : 0;
}
