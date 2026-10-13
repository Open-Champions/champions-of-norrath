// Host contract for spawn__FG6Point3iisPcbPPci. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
// The libc calls are replaced by recording stand-ins (the host libc names are redirected).
#define sscanf host_sscanf
#define strcasecmp host_strcasecmp
#include "spawn.cpp"
#undef sscanf
#undef strcasecmp
int eRandState;
int pmlistptrsused;
char* pmlistptrs[64];
int (*userFilterFunction)(char*);
static std::vector<std::string> ev;
static int clear_result = 77, found_result = 1;
static unsigned char* created;
static char* created_at;
static int filter_result;
static std::vector<std::string> pooled;
static char* pool_cells[64];
// "%[^set]" reader: copies up to the first delimiter from the set in the format.
extern "C" int host_sscanf(const char* text, const char* format, ...) {
    va_list ap; va_start(ap, format);
    char* out = va_arg(ap, char*);
    va_end(ap);
    std::string fmt = format;
    std::string set = fmt.substr(3, fmt.size() - 4);   // between "%[^" and "]"
    size_t n = 0;
    while (text[n] && set.find(text[n]) == std::string::npos) ++n;
    std::memcpy(out, text, n); out[n] = 0;
    ev.push_back("scan:" + fmt + ":" + std::string(out));
    return n ? 1 : 0;
}
static const char* wanted[5] = {"scorpion", "firebeetle", "bbeetle", "ant", "lbeetle"};
static std::string case_hit;
extern "C" int host_strcasecmp(const char* a, const char* b) {
    ev.push_back(std::string("case:") + b);
    return std::strcmp(a, b);
}
extern "C" char* addpmlist__FPc(char* text) {
    pooled.push_back(text);
    ev.push_back(std::string("pool:") + text);
    return pool_cells[pooled.size() - 1] = reinterpret_cast<char*>(0x1000 + pooled.size());
}
static char* cap_value = reinterpret_cast<char*>(0x2222);
extern "C" char** addpmlistptrscap__Fv() { ev.push_back("cap"); return reinterpret_cast<char**>(cap_value); }
extern "C" int getClearradius__FPCc(const char* t) { ev.push_back(std::string("radius:") + t); return clear_result; }
extern "C" void objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(float* result, const float* point, int radius, int line, int type, int clear, char* found) {
    ev.push_back("spot:" + std::to_string(point[0]) + "," + std::to_string(radius) + "," + std::to_string(line) + "," + std::to_string(type) + "," + std::to_string(clear));
    result[0] = 9; result[1] = 8; result[2] = 7; *found = static_cast<char>(found_result);
}
extern "C" unsigned char* objectCreateByName__FPcG6Point3sPPc(char* text, const float* point, short angle, char** args) {
    ev.push_back(std::string("create:") + (text ? text : "null") + ":" + std::to_string(point[0]) + ":" + std::to_string(angle) + ":" + std::to_string(reinterpret_cast<unsigned long>(args)));
    return created;
}
static int filter(char* t) { ev.push_back(std::string("filter:") + t); return filter_result; }
int main() {
    int failures = 0;
    static char text[256];
    unsigned char point[12]; sf_(point, 1.0f); sf_(point + 4, 2.0f); sf_(point + 8, 3.0f);
    unsigned char object[64]; std::memset(object, 0, sizeof object);
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); for (auto& e : ev) std::printf("   %s\n", e.c_str()); ++failures; } };
    auto run = [&](const char* name, bool use_filter, int clear, char** args = reinterpret_cast<char**>(0x5555)) {
        std::strcpy(text, name);
        ev.clear(); pooled.clear(); pmlistptrsused = 0; std::memset(pmlistptrs, 0, sizeof pmlistptrs);
        eRandState = 0x12345678; created = object; sf_(object + 28, 100.0f); found_result = 1; filter_result = 1; clear_result = 100;
        userFilterFunction = filter;
        spawn__FG6Point3iisPcbPPci(point, 3000, 7, static_cast<short>(0xfff9), text, use_filter, args, clear);
    };
    const int next_state = static_cast<int>(0x12345678u * 0x19660du + 0x3c6ef35fu);
    const short next_angle = static_cast<short>(next_state >> 16);
    // Plain name: no parameters.
    run("scorpion,x-y", false, 100);
    expect(ev.size() == 3 && ev[0] == "scan:%[^,-]:scorpion" && ev[1].rfind("spot:1.0", 0) == 0 && ev[1].find(",3000,7,-7,100") != std::string::npos, "plain name: first scan, spot args");
    expect(ev[2] == "create:scorpion,x-y:9.000000:" + std::to_string(next_angle) + ":21845", "created with the strstr hit, the spot, the angle and the argument list unchanged");
    expect(eRandState == next_state, "random state advanced");
    // A clear radius of -1 is looked up before the spot search and its result is passed on.
    run("ant", false, -1);
    expect(ev[1] == "radius:ant" && ev[2].find(",100") != std::string::npos && ev[2].find(",-1") == std::string::npos, "looked-up radius is used for the spot search");
    // Redo with a differing radius so the chain runs: class "ant" matches the 4th entry.
    sf_(object + 28, 55.0f); std::strcpy(text, "ant"); ev.clear(); eRandState = 1; found_result = 1; clear_result = 100; created = object; userFilterFunction = nullptr;
    spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, false, nullptr, -1);
    {
        std::vector<std::string> calls; for (auto& e : ev) if (e.rfind("case:", 0) == 0) calls.push_back(e);
        expect(calls.size() == 4 && calls[3] == "case:ant", "strcasecmp chain stops at the first match (ant, fourth)");
    }
    std::strcpy(text, "zebra"); ev.clear(); eRandState = 1; spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, false, nullptr, -1);
    { int n = 0; for (auto& e : ev) if (e.rfind("case:", 0) == 0) ++n; expect(n == 5, "no match runs all five comparisons"); }
    std::strcpy(text, "scorpion"); ev.clear(); eRandState = 1; spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, false, nullptr, -1);
    { int n = 0; for (auto& e : ev) if (e.rfind("case:", 0) == 0) ++n; expect(n == 1, "first name matches at once"); }
    std::strcpy(text, "ant"); ev.clear(); eRandState = 1; spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, false, nullptr, 100);
    { int n = 0; for (auto& e : ev) if (e.rfind("case:", 0) == 0) ++n; expect(n == 0, "a supplied radius never runs the chain"); }
    // Nothing found.
    run("ant", false, 100); found_result = 1;
    std::strcpy(text, "ant"); ev.clear(); found_result = 0; eRandState = 5;
    spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, true, nullptr, 100);
    expect(ev.size() == 2 && eRandState == 5, "no spot found: nothing created, state untouched");
    // Filter.
    run("ant", true, 100); expect(ev[2] == "filter:ant" && ev[3].rfind("create:", 0) == 0, "filter called with the class, then create");
    std::strcpy(text, "ant"); ev.clear(); eRandState = 5; filter_result = 0; userFilterFunction = filter;
    spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, true, nullptr, 100);
    expect(ev.size() == 3 && ev[2] == "filter:ant" && eRandState == 5, "filter refusal stops before the random step");
    std::strcpy(text, "ant"); ev.clear(); eRandState = 5; userFilterFunction = nullptr;
    spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, true, nullptr, 100);
    expect(ev.size() == 2 && eRandState == 5, "a null filter function with the flag set stops");
    std::strcpy(text, "ant"); ev.clear(); eRandState = 5; userFilterFunction = nullptr; created = object;
    spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, false, nullptr, 100);
    expect(ev.back().rfind("create:", 0) == 0, "a null filter function with the flag clear does not matter");
    // Null object stops.
    std::strcpy(text, "ant"); ev.clear(); created = nullptr;
    spawn__FG6Point3iisPcbPPci(point, 3000, 7, 5, text, false, nullptr, -1);
    expect(ev.back().rfind("create:", 0) == 0, "null object: nothing after create");
    created = object;
    // Parenthesised name with parameters.
    run("(ant-x;p1;a(b)c;d)", false, 100);
    expect(ev[0] == "scan:%[^,;-]:ant" && ev[1] == "pool:p1" && ev[2] == "pool:a(b)c" && ev[3] == "pool:d" && ev[4] == "cap", "parameters are pooled in order, nested parentheses kept");
    expect(pmlistptrsused == 3 && pmlistptrs[0] == pool_cells[0] && pmlistptrs[2] == pool_cells[2], "pool results appended to pmlistptrs");
    expect(ev[6].rfind("create:", 0) == 0 && ev[6].find(":8738") != std::string::npos, "the capped list replaces the arguments (0x2222)");
    expect(ev[6].rfind("create:ant-x;p1;a(b)c;d):", 0) == 0, "strstr finds the class inside the text (one past the parenthesis)");
    run("(ant)", false, 100);
    expect(ev[0] == "scan:%[^,;-]:ant)", "the class scan stops at the delimiter set only (a ) stays in the name)");
    expect(ev[1] == "cap" && pooled.empty(), "no parameters: only the cap");
    run("(ant;)", false, 100);
    expect(ev[1] == "pool:" && ev[2] == "cap" && pooled.size() == 1, "an empty parameter is pooled");
    run("(ant;;x)", false, 100);
    expect(ev[1] == "pool:" && ev[2] == "pool:x" && ev[3] == "cap", "empty then x");
    // Text without a closing parenthesis would run past the terminator in the original; not tested.
    if (failures) return 1;
    std::printf("spawn check passed\n");
    return 0;
}
