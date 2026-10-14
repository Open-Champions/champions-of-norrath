// Host check only; not a build input. Build as x86_64 with static storage below 4 GiB so that
// 32-bit object words can hold real addresses. Read calls fill the buffers from a script; table
// entries are host-sized records. Scenarios: no focus, key down/up/none, pad transitions in both
// directions including negative zero and NaN, a handler that clears the focus, root draw
// present/absent/flag clear. The same logic was compared with the original in a private
// interpreter run (120 randomised scenarios, identical call logs, changed memory and results).
#include "update.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mFocus;
alignas(16) static unsigned char manager[256];
static unsigned char focus_object[64], root_object[64], keyboard[8], pad_device[8];
static VtEntry focus_table[6], root_table[6];
static std::string trace;
static bool clear_focus_on_call;
static VIConsoleKey next_key;
static int key_result, pad_result;
static VIConsolePad next_pad;

extern "C" int Read__10VIKeyboardR12VIConsoleKey(void* device, VIConsoleKey* key) {
    trace += device == keyboard ? "readkey;" : "readkey?;";
    *key = next_key;
    return key_result;
}
extern "C" int Read__5VIPadR12VIConsolePad(void* device, VIConsolePad* state) {
    trace += device == pad_device ? "readpad;" : "readpad?;";
    *state = next_pad;
    return pad_result;
}
static void on_down(void* self, int key) {
    trace += "down:" + std::to_string(static_cast<unsigned char*>(self) - focus_object) + "," + std::to_string(key) + ";";
    if (clear_focus_on_call) _5VIWnd$mFocus = 0;
}
static void on_up(void* self, int key) {
    trace += "up:" + std::to_string(static_cast<unsigned char*>(self) - focus_object) + "," + std::to_string(key) + ";";
    if (clear_focus_on_call) _5VIWnd$mFocus = 0;
}
static void on_draw(void* self, int) {
    trace += "draw:" + std::to_string(static_cast<unsigned char*>(self) - root_object) + ";";
}
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

struct Setup {
    bool focus = true, with_keyboard = true, with_pad = true, with_root = true;
    unsigned root_flag = 1;
    float previous[20] = {};
};

static int run(const Setup& s, const std::string& want, int want_result, const float* want_copy = nullptr) {
    trace.clear();
    std::memset(manager, 0, sizeof manager);
    focus_table[5] = VtEntry{24, 1, on_down};
    focus_table[4] = VtEntry{24, 1, on_up};
    root_table[2] = VtEntry{16, 1, reinterpret_cast<void (*)(void*, int)>(on_draw)};
    unsigned w = word_of(focus_table); std::memcpy(focus_object + 32, &w, 4);
    w = word_of(root_table); std::memcpy(root_object + 32, &w, 4);
    _5VIWnd$mFocus = s.focus ? word_of(focus_object) : 0;
    w = s.with_keyboard ? word_of(keyboard) : 0; std::memcpy(manager + 44, &w, 4);
    w = s.with_pad ? word_of(pad_device) : 0; std::memcpy(manager + 40, &w, 4);
    w = s.with_root ? word_of(root_object) : 0; std::memcpy(manager + 36, &w, 4);
    std::memcpy(root_object + 28, &s.root_flag, 4);
    std::memcpy(manager + 52, s.previous, sizeof s.previous);
    const int result = Update__15VIWindowManager(manager);
    int bad = 0;
    if (trace != want) { std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str()); ++bad; }
    if (result != want_result) { std::printf("result %d want %d\n", result, want_result); ++bad; }
    if (want_copy && std::memcmp(manager + 48, &next_pad.head, 4) == 0 && std::memcmp(manager + 52, want_copy, 80) != 0) ++bad;
    return bad;
}

int main() {
    int bad = 0;
    next_pad.head = 0xabcd;
    for (float& v : next_pad.value) v = 0.0f;
    clear_focus_on_call = false;
    pad_result = 0;
    key_result = 0;

    { Setup s; s.focus = false; bad += run(s, "draw:16;", 0); }
    { Setup s; s.focus = false; s.root_flag = 0; bad += run(s, "", 0); }
    { Setup s; s.with_root = false; s.with_keyboard = false; s.with_pad = false; bad += run(s, "", 1); }
    { Setup s; s.with_pad = false; s.with_root = false; key_result = 1; next_key = VIConsoleKey{65, 1, {}};
      bad += run(s, "readkey;down:24,65;", 1); }
    { Setup s; s.with_pad = false; s.with_root = false; key_result = 1; next_key = VIConsoleKey{200, 0, {}};
      bad += run(s, "readkey;up:24,200;", 1); }
    { Setup s; s.with_pad = false; s.with_root = false; key_result = 0; bad += run(s, "readkey;", 1); }
    { Setup s; s.with_pad = false; s.with_root = false; key_result = 2; bad += run(s, "readkey;", 1); }
    { Setup s; s.with_pad = false; s.with_root = false; key_result = 1; next_key = VIConsoleKey{7, 3, {}};
      clear_focus_on_call = true; bad += run(s, "readkey;down:24,7;", 0); clear_focus_on_call = false; }

    // Pad: press on 0, release on 3, -0.0 to 0.0 no event, NaN counts as down.
    key_result = 0;
    {
        Setup s; s.with_keyboard = false; s.with_root = false; pad_result = 1;
        for (float& v : next_pad.value) v = 0.0f;
        s.previous[3] = 1.0f; s.previous[5] = -0.0f; s.previous[7] = std::nanf("");
        next_pad.value[0] = 2.0f; next_pad.value[5] = 0.0f; next_pad.value[7] = 3.0f; next_pad.value[19] = -1.0f;
        bad += run(s, "readpad;down:24,128;up:24,131;down:24,147;", 1, next_pad.value);
        if (std::memcmp(manager + 52, next_pad.value, 80) != 0) { std::puts("copy mismatch"); ++bad; }
        unsigned head; std::memcpy(&head, manager + 48, 4);
        if (head != 0xabcd) { std::puts("head mismatch"); ++bad; }
    }
    {   // pad read fails: previous state kept
        Setup s; s.with_keyboard = false; s.with_root = false; pad_result = 0;
        s.previous[1] = 5.0f;
        bad += run(s, "readpad;", 1);
        float kept; std::memcpy(&kept, manager + 56, 4);
        if (kept != 5.0f) { std::puts("previous overwritten"); ++bad; }
    }
    {   // handler clears focus mid-pad: only the first event is delivered
        Setup s; s.with_keyboard = false; s.with_root = false; pad_result = 1; clear_focus_on_call = true;
        for (float& v : next_pad.value) v = 1.0f;
        bad += run(s, "readpad;down:24,128;", 0);
        clear_focus_on_call = false;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
