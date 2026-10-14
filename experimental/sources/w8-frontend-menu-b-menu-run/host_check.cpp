#define NORRATH_HOST_ARENA
#include "menu_run.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int gameBailFromGameLoop = 0;
unsigned padAcceptButton = 0x4;
unsigned char pad[352];
float norrath_menu_local_t_004844e8 = 1.0f;
}

static unsigned char* slots[8];
extern "C" unsigned char* norrath_host_pointer(unsigned word) { return slots[word]; }

static std::vector<std::string> sounds;
static std::string last_lump;
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    last_lump = lump;
    static char holder[64];
    std::strncpy(holder, name, 63);
    return holder;
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(AnimationHeader* header, GameObject*, float volume) {
    if (volume != 1.0f) sounds.push_back("badvolume");
    sounds.push_back(reinterpret_cast<const char*>(header));
}

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

static void put(unsigned char* at, unsigned v) {
    at[0] = v; at[1] = v >> 8; at[2] = v >> 16; at[3] = v >> 24;
}
static unsigned get(const unsigned char* at) { return at[0] | (at[1] << 8) | (at[2] << 16) | (at[3] << 24); }

// Menu: 0 plain(kind 0), 1 int counter, 2 float, 3 toggle, 4 slider, 5 separator,
// 6 disabled int (flag bit 0), 7 plain; then the terminator.
static unsigned char menu[9 * 16];
static int counter, toggle, disabled_value;
static float fvalue, slider;
static unsigned char pad_a[88];

static void setup() {
    std::memset(menu, 0, sizeof menu);
    unsigned kinds[8] = {0, 1, 2, 3, 4, 5, 1, 0};
    for (int i = 0; i < 8; ++i) {
        put(menu + i * 16, 100 + i);
        put(menu + i * 16 + 4, kinds[i]);
        put(menu + i * 16 + 8, i);
    }
    put(menu + 6 * 16 + 12, 1);
    slots[1] = reinterpret_cast<unsigned char*>(&counter);
    slots[2] = reinterpret_cast<unsigned char*>(&fvalue);
    slots[3] = reinterpret_cast<unsigned char*>(&toggle);
    slots[4] = reinterpret_cast<unsigned char*>(&slider);
    slots[6] = reinterpret_cast<unsigned char*>(&disabled_value);
    counter = 5; toggle = 0; fvalue = 0.5f; slider = 0.5f; disabled_value = 9;
    sounds.clear();
    norrath_menu_local_t_004844e8 = 1.0f;
    gameBailFromGameLoop = 0;
    std::memset(pad_a, 0, sizeof pad_a);
    std::memset(pad, 0, sizeof pad);
    put(menu + 8 * 16 + 4, 0);
}

static void buttons(unsigned held, unsigned pressed) { put(pad_a + 8, held); put(pad_a + 16, pressed); }

int main() {
    // Terminator state is read: cursor 3 in the low bits, 2 in the high byte.
    setup();
    put(menu + 8 * 16 + 4, 3 | (2 << 8));
    buttons(0, 0);
    int changed = 7;
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 3);
    CHECK(changed == 0);
    CHECK(sounds.empty());
    CHECK(get(menu + 8 * 16 + 4) == (3u | (2u << 8)));

    // Down moves one entry, plays the toggle sound, stores previous and resets the tween.
    setup();
    put(menu + 8 * 16 + 4, 0);
    buttons(0, 0x204000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 1);
    CHECK(changed == 1);
    CHECK(sounds.size() == 1 && sounds[0] == "menu_main_toggle.anm");
    CHECK(last_lump == "fx.lmp");
    CHECK(get(menu + 8 * 16 + 4) == (1u | (0u << 8)));
    CHECK(norrath_menu_local_t_004844e8 == 0.0f);

    // While the tween is below 1 nothing but the tween is touched.
    buttons(0, 0x204000);
    sounds.clear();
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 1);
    CHECK(changed == 0);
    CHECK(sounds.empty());
    CHECK(norrath_menu_local_t_004844e8 > 0.2f && norrath_menu_local_t_004844e8 < 0.21f);
    for (int i = 0; i < 3; ++i) menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(norrath_menu_local_t_004844e8 < 1.0f);
    // The call that reaches 1.0 clamps it, handles the held press and resets the tween.
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 2);
    CHECK(norrath_menu_local_t_004844e8 == 0.0f);

    // Moving past a separator and a disabled entry skips them (down from 4 lands on 7).
    setup();
    put(menu + 8 * 16 + 4, 4);
    buttons(0, 0x204000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 7);
    // Up from 7 skips back over 6 and 5 to 4.
    norrath_menu_local_t_004844e8 = 1.0f;
    buttons(0, 0x101000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 4);

    // Hitting the end with only skipped entries beyond returns to the old cursor.
    setup();
    put(menu + 8 * 16 + 4, 7);
    buttons(0, 0x204000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 7);
    CHECK(changed == 0);
    CHECK(sounds.empty());

    // A trailing run of unselectable entries falls back to the old cursor.
    setup();
    put(menu + 7 * 16 + 4, 5);
    put(menu + 8 * 16 + 4, 4);
    buttons(0, 0x204000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 4);
    CHECK(changed == 0);
    // A stored cursor on a separator with no input falls back to entry zero.
    setup();
    put(menu + 8 * 16 + 4, 5);
    buttons(0, 0);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 0);

    // Up at the top stays at zero.
    setup();
    buttons(0, 0x1000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 0);

    // Up from 4 whose predecessors 3..0 are fine moves normally; but with 5 and 6
    // unselectable a downward walk from the end clamps to last (7) when pressed on 7.
    setup();
    put(menu + 8 * 16 + 4, 7);
    buttons(0, 0x4000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, nullptr) == 7);

    // No toggle sound when gameBailFromGameLoop is 101.
    setup();
    gameBailFromGameLoop = 101;
    buttons(0, 0x4000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 1);
    CHECK(sounds.empty());

    // Value editing on the selected entry: kind 1 int.
    setup();
    put(menu + 8 * 16 + 4, 1);
    buttons(0, 0x8000);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 1);
    CHECK(counter == 4);
    CHECK(sounds.size() == 1 && sounds[0] == "menu_main_toggle.anm");
    buttons(0, 0x2000);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    buttons(0, 0x2000);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(counter == 6);
    // Kind 2 float.
    put(menu + 8 * 16 + 4, 2);
    buttons(0, 0x400000 | 0x8000);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(fvalue > 0.39f && fvalue < 0.41f);
    buttons(0, 0x802000);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(fvalue > 0.49f && fvalue < 0.51f);
    // Kind 3 toggles with either direction and with the accept button.
    put(menu + 8 * 16 + 4, 3);
    sounds.clear();
    buttons(0, 0x8000);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(toggle == 1);
    buttons(0, 0x2000);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(toggle == 0);
    CHECK(sounds.size() == 2);
    sounds.clear();
    buttons(0, 4);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(toggle == 1);
    CHECK(sounds.size() == 1 && sounds[0] == "menu_main_accept.anm");
    // Accept does nothing for other kinds.
    put(menu + 8 * 16 + 4, 1);
    sounds.clear();
    int before = counter;
    buttons(0, 4);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(counter == before);
    CHECK(sounds.empty());

    // Held buttons nudge a slider and clamp.
    put(menu + 8 * 16 + 4, 4);
    slider = 0.5f;
    buttons(0x802000, 0);
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(slider > 0.508f && slider < 0.509f);
    slider = 0.999f;
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(slider == 1.0f);
    buttons(0x408000, 0);
    slider = 0.001f;
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(slider == 0.0f);
    slider = 0.5f;
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(slider > 0.49f && slider < 0.492f);
    // Held does not edit counters, pressed does not move the slider.
    buttons(0, 0x802000);
    slider = 0.5f;
    menuRun__FP4MenuP3padPi(menu, pad_a, &changed);
    CHECK(slider == 0.5f);

    // A null pad ORs the four global pads.
    setup();
    put(pad + 88 * 2 + 16, 0x4000);
    CHECK(menuRun__FP4MenuP3padPi(menu, nullptr, &changed) == 1);
    CHECK(changed == 1);
    setup();
    put(pad + 88 * 3 + 8, 0x802000);
    put(menu + 8 * 16 + 4, 4);
    slider = 0.25f;
    menuRun__FP4MenuP3padPi(menu, nullptr, nullptr);
    CHECK(slider > 0.258f && slider < 0.259f);

    // The state word masks bit 7 and bits above 15 are replaced by the previous index.
    setup();
    put(menu + 8 * 16 + 4, 0x80 | 2 | (5 << 8));
    buttons(0, 0);
    CHECK(menuRun__FP4MenuP3padPi(menu, pad_a, &changed) == 2);
    CHECK(get(menu + 8 * 16 + 4) == (2u | (5u << 8)));

    if (failures == 0) std::printf("menu_run ok\n");
    return failures != 0;
}
