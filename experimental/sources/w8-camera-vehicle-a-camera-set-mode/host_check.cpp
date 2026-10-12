// Host contract for SetMode__6Camerai. Not an EE plan input.
#include "set_mode.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int reveal_calls = 0;
static int mode_seen_by_reveal = 0;
static unsigned char* camera_for_reveal;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int gameShowEnemiesInMap;

extern "C" void revealEntireMap__3mapv() {
    ++reveal_calls;
    std::memcpy(&mode_seen_by_reveal, camera_for_reveal + 52, 4);
}

int main() {
    static unsigned char camera[256];
    camera_for_reveal = camera;
    for (int mode = -3; mode <= 9; ++mode) {
        std::memset(camera, 0x77, sizeof camera);
        unsigned char before[256];
        std::memcpy(before, camera, sizeof camera);
        gameShowEnemiesInMap = 42;
        reveal_calls = 0;
        mode_seen_by_reveal = -999;
        SetMode__6Camerai(camera, mode);
        int stored;
        std::memcpy(&stored, camera + 52, 4);
        expect(stored == mode, "mode stored at +52");
        const bool reveal = mode >= 3 && mode <= 5;
        expect(reveal_calls == (reveal ? 1 : 0), "reveal for modes 3..5 only");
        if (reveal) expect(mode_seen_by_reveal == 0x77777777, "mode is stored after the reveal call");
        expect(gameShowEnemiesInMap == (reveal ? 1 : 42), "flag set to 1 for modes 3..5 only");
        std::memcpy(before + 52, camera + 52, 4);
        if (mode == 1) {
            std::memset(before + 56, 0, 8);
            std::memset(before + 152, 0, 4);
        }
        expect(std::memcmp(before, camera, sizeof camera) == 0, "only the expected camera words change");
    }
    return failures ? 1 : 0;
}
