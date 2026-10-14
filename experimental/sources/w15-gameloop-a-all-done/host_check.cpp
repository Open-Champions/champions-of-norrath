// Host check for allDone__Fi. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "all_done.cpp"

#include <cstdio>
#include <string>
#include <vector>

void* gameCam = reinterpret_cast<void*>(0x40);
int playerSelRunning = 1, engineDisable30fps = 1, needEnforce = 1, gameBailFromGameLoop;
unsigned char playerSelectSuccessful;
char playerSelectPrevName[64], playerSelectNextName[64], levelName[322];
static std::vector<std::string> events;
extern "C" {
void Pop__6Camera(void* camera) { events.push_back(camera == gameCam ? "pop" : "pop?"); }
void SetMode__6Camerai(void* camera, int mode) { events.push_back(camera == gameCam ? "mode" + std::to_string(mode) : "mode?"); }
void engineRemoveTask__FPFv_v(void (*task)()) { events.push_back(task == playerSelectionTask__Fv ? "remove" : "remove?"); }
void playerSelectionTask__Fv() {}
void lightPopSceneLights__Fv() { events.push_back("lights"); }
void engineSetDisplayDisable__Fi(int frames) { events.push_back("disable" + std::to_string(frames)); }
void engineSyncVBlank__Fv() { events.push_back("sync"); }
void scriptSetGameState__FPci(char* name, int value) { events.push_back(std::string("state:") + name + std::to_string(value)); }
}
// The plain names are the host's; the source binds the image's strcmp and strcpy through labels, so the host check
// supplies the same two labels.
extern "C" int compareText(const char* a, const char* b) { return std::string(a).compare(b); }
extern "C" char* copyText(char* destination, const char* source) {
    events.push_back(std::string("copy:") + source);
    for (unsigned i = 0;; ++i) { destination[i] = source[i]; if (!source[i]) break; }
    return destination;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    const std::vector<std::string> common = {"pop", "mode1", "remove", "lights", "disable10", "sync"};
    // previous (nonzero): uses the previous name, clears needEnforce, does not run the game state
    std::snprintf(playerSelectPrevName, 64, "%s", "dungeon");
    std::snprintf(playerSelectNextName, 64, "%s", "town");
    allDone__Fi(1);
    std::vector<std::string> want = common; want.push_back("copy:dungeon");
    CHECK(events == want);
    CHECK(std::string(levelName) == "dungeon" && static_cast<unsigned char>(levelName[80]) == 0xff);
    CHECK(playerSelRunning == 0 && engineDisable30fps == 0 && needEnforce == 0 && gameBailFromGameLoop == 0 && playerSelectSuccessful == 0);
    // forward (zero): game state, success byte, next name
    events.clear(); needEnforce = 5;
    allDone__Fi(0);
    want = common; want.push_back("state:needPlayers0"); want.push_back("copy:town");
    CHECK(events == want);
    CHECK(std::string(levelName) == "town" && playerSelectSuccessful == 1 && needEnforce == 5);
    // empty names ask to leave the game loop and leave levelName alone
    events.clear(); playerSelectNextName[0] = 0;
    allDone__Fi(0);
    CHECK(gameBailFromGameLoop == 1 && std::string(levelName) == "town");
    gameBailFromGameLoop = 0; playerSelectPrevName[0] = 0;
    allDone__Fi(7);
    CHECK(gameBailFromGameLoop == 1 && needEnforce == 0);
    std::puts("ok");
    return 0;
}
