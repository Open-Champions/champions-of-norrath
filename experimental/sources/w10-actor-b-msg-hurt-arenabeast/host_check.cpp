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
static void *seen_self, *seen_source;
static float seen_amount;
static int seen_kind;
static char name_at_call[128];
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* source, int kind) {
    calls += "base;";
    seen_self = self; seen_source = source; seen_amount = amount; seen_kind = kind;
    std::memcpy(name_at_call, bossMeterName, sizeof name_at_call);
}
#include "msg_hurt.cpp"

int main() {
    int failures = 0;
    alignas(8) unsigned char object[600];
    const float healths[] = {0.0f, 1.5f, -2.5f, 3.99f, 100.0f, 123456.75f, -0.5f};
    for (float health : healths) {
        std::memset(object, 0x21, sizeof object);
        std::memcpy(object + 560, &health, 4);
        std::memset(bossMeterName, 'z', sizeof bossMeterName);
        bossMeterCurrent = -77;
        name_at_call[0] = 'q';
        calls.clear();
        int token = 0;
        msg_hurt__10ArenabeastfP10GameObjecti(object, 6.25f, &token, 9);
        if (calls != "base;") { std::fprintf(stderr, "fail: base call\n"); ++failures; }
        if (seen_self != object || seen_source != &token || seen_amount != 6.25f || seen_kind != 9) {
            std::fprintf(stderr, "fail: forwarded arguments\n"); ++failures;
        }
        if (name_at_call[0] != 'z') { std::fprintf(stderr, "fail: name written before the base call\n"); ++failures; }
        if (std::strcmp(bossMeterName, "ArenaBeast") != 0 || bossMeterName[11] != 'z') { std::fprintf(stderr, "fail: name\n"); ++failures; }
        if (bossMeterCurrent != static_cast<int>(health)) { std::fprintf(stderr, "fail: current %d\n", bossMeterCurrent); ++failures; }
        float after;
        std::memcpy(&after, object + 560, 4);
        if (after != health || object[559] != 0x21 || object[564] != 0x21) { std::fprintf(stderr, "fail: object written\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
