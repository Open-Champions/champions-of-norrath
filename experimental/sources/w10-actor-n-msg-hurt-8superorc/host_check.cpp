// Host contract for msg_hurt__8SuperOrcfP10GameObjecti; not an EE input. Includes the source.
#include "msg_hurt.cpp"
#include <cstdio>
#include <cstring>
#include <string>

char bossMeterName[128];
char engineCurrentLevelName[80];
int bossMeterCurrent = -1;
static std::string calls;
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* s, float a, void* src, int k) {
    calls += "base(" + std::to_string(static_cast<int>(a)) + "," + std::to_string(k) + (src == reinterpret_cast<void*>(8) ? ",src" : ",other") + ");";
}
extern "C" int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}
int main() {
    alignas(16) unsigned char object[600] = {};
    float v = 41.5f; std::memcpy(object + 560, &v, 4);
    std::strcpy(engineCurrentLevelName, "gothic1");
    std::memset(bossMeterName, 'x', sizeof bossMeterName);
    msg_hurt__8SuperOrcfP10GameObjecti(object, 6.0f, reinterpret_cast<void*>(8), 3);
    if (calls != "base(6,3,src);") { std::puts(calls.c_str()); return 1; }
    if (bossMeterCurrent != -1 || bossMeterName[0] != 'x') return 2;      // that level: no meter change
    std::strcpy(engineCurrentLevelName, "gothic2");
    msg_hurt__8SuperOrcfP10GameObjecti(object, 6.0f, reinterpret_cast<void*>(8), 3);
    if (std::strcmp(bossMeterName, "SuperOrc") != 0 || bossMeterName[9] != 'x' || bossMeterCurrent != 41) return 3;
    std::strcpy(engineCurrentLevelName, "gothic");                          // prefix of the name is different
    bossMeterCurrent = -1;
    msg_hurt__8SuperOrcfP10GameObjecti(object, 6.0f, reinterpret_cast<void*>(8), 3);
    if (bossMeterCurrent != 41) return 4;
    return 0;
}
