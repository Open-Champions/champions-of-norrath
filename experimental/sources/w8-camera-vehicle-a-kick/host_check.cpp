// Host contract for Kick__7Vehicle. Not an EE plan input.
#include "kick.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int calls = 0;
static int slots[8];
static void* owners[8];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

char levelName[322];

extern "C" void removePassenger__7Vehiclei(void* vehicle, int slot) {
    if (calls < 8) {
        slots[calls] = slot;
        owners[calls] = vehicle;
    }
    ++calls;
}

static void run(const char* text, const char* prefix_noise, int want_calls) {
    std::memset(levelName, 0, sizeof levelName);
    std::memcpy(levelName, prefix_noise, std::strlen(prefix_noise));
    std::strcpy(levelName + 81, text);
    calls = 0;
    int vehicle_marker;
    Kick__7Vehicle(&vehicle_marker);
    expect(calls == want_calls, text);
    if (want_calls == 4) {
        for (int i = 0; i < 4; ++i) expect(slots[i] == i && owners[i] == &vehicle_marker, "slots 0..3 in order on this vehicle");
    }
}

int main() {
    run("lava2", "lava2", 4);
    run("lava2", "", 4);
    run("lava3", "lava2", 0);
    run("lava", "lava2", 0);
    run("lava22", "lava2", 0);
    run("Lava2", "lava2", 0);
    run("", "lava2", 0);
    std::memset(levelName, 0, sizeof levelName);
    std::strcpy(levelName, "lava2");
    calls = 0;
    int marker;
    Kick__7Vehicle(&marker);
    expect(calls == 0, "text at offset 0 is not what is compared");
    return failures ? 1 : 0;
}
