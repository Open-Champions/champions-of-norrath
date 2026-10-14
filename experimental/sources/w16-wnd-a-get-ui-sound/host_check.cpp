// Host check only; not a build input. Also compared with the original in a private interpreter (12 ids, all words
// covered). The table is a host array of 17 records; ids below 17 give their own record, any other value record 0.
#include "get_ui_sound.cpp"
#include <cstdio>
#include <initializer_list>

UISoundRecord UISounds[17];

int main() {
    for (int i = 0; i < 17; ++i) UISounds[i] = UISoundRecord{i, 0.25f * static_cast<float>(i)};
    bool ok = true;
    for (unsigned id = 0; id < 64; ++id) ok = ok && GetUISound__F11VIUISoundId(id) == &UISounds[id < 17 ? id : 0];
    for (unsigned id : {0x7fffffffu, 0x80000000u, 0xffffffffu, 17u, 16u}) ok = ok && GetUISound__F11VIUISoundId(id) == &UISounds[id < 17 ? id : 0];
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
