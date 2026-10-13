// Host check for ChangeNetCam__6Camerai. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
#include "change_net_cam.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int nmPlayersAlive;
int playersAlive[8];
}
static std::string trace;
extern "C" void Reset__6Cameraii(void* camera, int a, int b) {
    char buf[40];
    std::snprintf(buf, sizeof buf, "reset %d %d %d;", camera != nullptr, a, b);
    trace += buf;
}
extern "C" void coreStartSwitchScene__Fi(int scene) {
    char buf[24];
    std::snprintf(buf, sizeof buf, "scene %d;", scene);
    trace += buf;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

// returns the followed byte after one call
static int step(unsigned char (&cam)[400], int cur, int by, const char* expectTrace) {
    cam[353] = static_cast<unsigned char>(cur);
    trace.clear();
    ChangeNetCam__6Camerai(cam, by);
    if (trace != expectTrace) std::printf("trace %s, wanted %s\n", trace.c_str(), expectTrace);
    return trace == expectTrace ? static_cast<signed char>(cam[353]) : 1000;
}

int main() {
    unsigned char cam[400];
    std::memset(cam, 0xcc, sizeof cam);
    nmPlayersAlive = 3;
    playersAlive[0] = 0; playersAlive[1] = 2; playersAlive[2] = 3; playersAlive[3] = 1;  // [3] is past the count
    const char* sw = "reset 1 0 0;scene 10;";
    CHECK(step(cam, 2, 1, sw) == 3);           // forward
    CHECK(step(cam, 3, 1, sw) == 0);           // forward past the end wraps to the first entry
    CHECK(step(cam, 0, -1, sw) == 3);          // backward past the start wraps to the last entry
    CHECK(step(cam, 3, -1, sw) == 2);          // backward
    CHECK(step(cam, 0, 2, sw) == 3);           // larger steps
    CHECK(step(cam, 2, 2, sw) == 0);           // larger step past the end goes to the first entry, not further
    CHECK(step(cam, 2, -2, sw) == 3);          // a larger step back wraps only to count - 1
    CHECK(step(cam, 0, -5, sw) == 3);
    CHECK(step(cam, 2, 0, "") == 2);           // zero keeps the position: no reset
    // a value that is not in the list starts from entry 0
    CHECK(step(cam, 7, 1, sw) == 2);
    CHECK(step(cam, 7, 0, sw) == 0);
    CHECK(step(cam, 7, -1, sw) == 3);
    CHECK(step(cam, 0, 0, "") == 0);
    // the followed byte is compared as a signed value and nothing else in the camera changes
    unsigned char before[400];
    std::memcpy(before, cam, sizeof cam);
    cam[353] = 2; before[353] = 0;
    trace.clear();
    ChangeNetCam__6Camerai(cam, 1);
    for (int i = 0; i < 400; ++i) if (i != 353) CHECK(cam[i] == before[i]);
    // a single-entry list: any step lands on the same player (no reset); step 1 wraps to it, step -1 to it
    nmPlayersAlive = 1; playersAlive[0] = 5;
    CHECK(step(cam, 5, 1, "") == 5);
    CHECK(step(cam, 5, -1, "") == 5);
    CHECK(step(cam, 4, 0, sw) == 5);
    // the list can hold a value that makes the byte negative: the comparison is signed
    nmPlayersAlive = 2; playersAlive[0] = 0xff; playersAlive[1] = 1;
    CHECK(step(cam, 1, 1, sw) == -1);
    CHECK(step(cam, -1, 0, "") == -1);
    CHECK(step(cam, -1, 1, sw) == 1);
    // an empty list: the entry read is playersAlive[0] after the wrap rules, with a step of zero or
    // positive, and [-1] for a negative step (not exercised here)
    nmPlayersAlive = 0; playersAlive[0] = 9;
    CHECK(step(cam, 3, 0, sw) == 9);
    CHECK(step(cam, 3, 1, sw) == 9);
    std::puts("change net cam passed");
    return 0;
}
