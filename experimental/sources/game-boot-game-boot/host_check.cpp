// Host contract for gameBoot. Not an EE plan input.
#include "game_boot.cpp"

#include <cstdio>

extern "C" unsigned char norrath_game_boot_camera[384] asm("c.1089") = {};
extern "C" unsigned char* gameCam = nullptr;
extern "C" unsigned gameNetReplicateQuestItems = 7u;

static int failures = 0;
static int constructors = 0;
static int ramps = 0;
static unsigned char* constructed = nullptr;
static unsigned guard_at_construct = 99u;
static unsigned char* cam_at_ramps = nullptr;
static unsigned guard_at_ramps = 99u;
static unsigned replicate_at_ramps = 99u;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void __6Camera(unsigned char* object) {
    ++constructors;
    constructed = object;
    guard_at_construct = norrath_game_boot_camera_guard;
}

extern "C" void gameInitSkillRamps__Fv() {
    ++ramps;
    cam_at_ramps = gameCam;
    guard_at_ramps = norrath_game_boot_camera_guard;
    replicate_at_ramps = gameNetReplicateQuestItems;
}

static void reset_calls() {
    constructors = 0;
    ramps = 0;
    constructed = nullptr;
    guard_at_construct = 99u;
    cam_at_ramps = nullptr;
    guard_at_ramps = 99u;
    replicate_at_ramps = 99u;
}

int main() {
    norrath_game_boot_camera_guard = 0u;
    gameCam = nullptr;
    gameNetReplicateQuestItems = 7u;
    reset_calls();
    gameBoot__Fv();
    expect(constructors == 1 && constructed == norrath_game_boot_camera, "construct the static camera");
    expect(guard_at_construct == 0u, "guard still clear inside the constructor");
    expect(norrath_game_boot_camera_guard == 1u, "guard becomes one");
    expect(ramps == 1 && cam_at_ramps == norrath_game_boot_camera, "camera published before skill ramps");
    expect(guard_at_ramps == 1u && replicate_at_ramps == 7u, "replicate flag still old during skill ramps");
    expect(gameNetReplicateQuestItems == 0u, "replicate flag cleared after skill ramps");

    gameNetReplicateQuestItems = 4u;
    gameCam = nullptr;
    reset_calls();
    gameBoot__Fv();
    expect(constructors == 0 && ramps == 1, "guard one skips construction");
    expect(gameCam == norrath_game_boot_camera && gameNetReplicateQuestItems == 0u, "second call still publishes and clears");

    norrath_game_boot_camera_guard = 2u;
    gameNetReplicateQuestItems = 3u;
    reset_calls();
    gameBoot__Fv();
    expect(constructors == 0 && norrath_game_boot_camera_guard == 2u, "any nonzero guard skips construction");
    expect(ramps == 1 && gameNetReplicateQuestItems == 0u, "nonzero guard still runs skill ramps");
    if (failures != 0) return 1;
    std::puts("game boot contract passed");
    return 0;
}
