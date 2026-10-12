// Observed preConfigure at 0x004289d0, 0x50 bytes of initialized data between
// creditMenu and creditsText: five 16-byte records. The first four hold a
// pointer to the strings menu_playerone, menu_playertwo, menu_playerthree and
// menu_playerfour (the target addresses are 0x0046ad50, 0x0046ad60, 0x0046ad70
// and 0x0046ad88) followed by twelve zero bytes. The fifth record is all zero.
// The pause menu walks records of this size until a zero first word. Other
// meanings of the three words after each name are unknown. The target build
// has four-byte pointers; the host check compares names only.
extern "C" {
struct PreConfigureRecord {
    const char* name;
    unsigned words[3];
};
PreConfigureRecord preConfigure[5] = {
    {"menu_playerone", {0, 0, 0}},
    {"menu_playertwo", {0, 0, 0}},
    {"menu_playerthree", {0, 0, 0}},
    {"menu_playerfour", {0, 0, 0}},
    {nullptr, {0, 0, 0}},
};
}
