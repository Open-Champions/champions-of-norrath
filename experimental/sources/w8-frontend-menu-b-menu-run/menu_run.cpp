// Observed: menuRun__FP4MenuP3padPi at 0x00192f38 is 1392 bytes.
// Menu layout seen here: records of 16 bytes, ended by a record whose first
// word is zero. Per record: word +0 (nonzero for a live entry), word +4 the
// kind, word +8 a pointer to the value, word +12 flags. Kind 5 and flag bit 0
// make an entry unselectable. The terminator's word +4 keeps the cursor: the
// selected index in bits 0 to 6 and the previous index shifted left by 8.
// Each call advances the file-local float at 0x004844e8 by a constant, caps
// it at 1.0, stores it, and does nothing more until it has reached 1.0. A
// moved cursor resets it to zero.
// Input: with a pad argument, the words at pad+8 and pad+16 are used as
// two button masks (call them held and pressed); with a null pad both are
// OR-ed over the four pads of the global `pad` array (88-byte stride). The
// pressed mask moves the cursor by -1 (0x101000) or +1 (0x204000), clamps it
// to the live entries and skips unselectable ones in the movement direction;
// if that fails it falls back to the old cursor, then to entry zero. A cursor
// change plays "menu_main_toggle.anm" from "fx.lmp" unless
// gameBailFromGameLoop is 101. Pressed 0x408000 and 0x802000 edit the
// selected entry's value by kind: 1 is an int (-1 or +1), 2 is a float
// (-0.1f or +0.1f), 3 flips an int between 0 and 1; each plays the toggle
// sound. Kind 3 also flips on padAcceptButton and plays "menu_main_accept.anm".
// The held mask with 0x408000 or 0x802000 nudges a kind 4 float by -1/120 or
// +1/120 and clamps it to 0..1. The optional out int receives 1 when the
// cursor moved. The return value is the cursor.
// Inferred: kind 4 is a slider; kind 5 a separator. Field names are guesses
// from use only.
// Unknown: what the label word holds, the meaning of the other kinds, why
// the sound is suppressed for the value 101, and the Menu and pad types.
extern "C" {
struct AnimationHeader;
struct GameObject;
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(AnimationHeader* header, GameObject* object,
                                                          float volume);
extern int gameBailFromGameLoop;
extern unsigned padAcceptButton;
extern unsigned char pad[];
extern float norrath_menu_local_t_004844e8;
}

namespace {
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned word);
#endif

unsigned load_word(const unsigned char* at) {
    return static_cast<unsigned>(at[0]) | (static_cast<unsigned>(at[1]) << 8) |
           (static_cast<unsigned>(at[2]) << 16) | (static_cast<unsigned>(at[3]) << 24);
}

int load_int(const unsigned char* at) { return static_cast<int>(load_word(at)); }

#ifdef NORRATH_HOST_ARENA
unsigned char* target_of(const unsigned char* record) { return norrath_host_pointer(load_word(record + 8)); }
#else
unsigned char* target_of(const unsigned char* record) {
    return reinterpret_cast<unsigned char*>(load_word(record + 8));
}
#endif

void play(const char* name) {
    void* sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(static_cast<AnimationHeader*>(sequence), 0, 1.0f);
}

void play_toggle() { play("menu_main_toggle.anm"); }

bool skipped(const unsigned char* record) {
    return load_int(record + 4) == 5 || (load_word(record + 12) & 1u) != 0;
}

void edit(const unsigned char* record, int direction) {
    int kind = load_int(record + 4);
    if (kind == 1) {
        int* value = reinterpret_cast<int*>(target_of(record));
        *value += direction;
        play_toggle();
    } else if (kind == 2) {
        float* value = reinterpret_cast<float*>(target_of(record));
        *value += direction * 0.1f;
        play_toggle();
    } else if (kind == 3) {
        int* value = reinterpret_cast<int*>(target_of(record));
        *value = (*value == 0);
        play_toggle();
    }
}
}

extern "C" int menuRun__FP4MenuP3padPi(unsigned char* menu, unsigned char* pad_in, int* changed) {
    int count = 0;
    int terminator = 0;
    if (load_word(menu) != 0) {
        do {
            count += 1;
            terminator = count * 16;
        } while (load_word(menu + terminator) != 0);
    }

    float tween = norrath_menu_local_t_004844e8 + 0x1.9ff1bcp-3f;
    int state = load_int(menu + terminator + 4);
    int cursor = state & 0x7f;
    int previous = state >> 8;
    int old = cursor;
    unsigned held = 0;
    unsigned pressed = 0;
    if (tween >= 1.0f) tween = 1.0f;
    norrath_menu_local_t_004844e8 = tween;
    if (tween >= 1.0f) {
        if (pad_in != 0) {
            held = load_word(pad_in + 8);
            pressed = load_word(pad_in + 16);
        } else {
            for (int index = 0; index < 4; ++index) {
                held |= load_word(pad + index * 88 + 8);
                pressed |= load_word(pad + index * 88 + 16);
            }
        }
        int direction = 0;
        if ((pressed & 0x101000u) != 0) {
            cursor -= 1;
            direction = -1;
        }
        if ((pressed & 0x204000u) != 0) {
            cursor += 1;
            direction = 1;
        }
        int last = count - 1;
        if (cursor < 0) cursor = 0;
        if (cursor > last) cursor = last;
        while (direction != 0) {
            if (!skipped(menu + cursor * 16)) break;
            cursor += direction;
            if (cursor < 0) {
                cursor = 0;
                break;
            }
            if (cursor > last) {
                cursor = last;
                break;
            }
        }
        if (skipped(menu + cursor * 16)) cursor = old;
        if (skipped(menu + cursor * 16)) cursor = 0;
        if (cursor != old && gameBailFromGameLoop != 101) play_toggle();

        const unsigned char* record = menu + cursor * 16;
        if ((pressed & 0x408000u) != 0) edit(record, -1);
        if ((held & 0x408000u) != 0 && load_int(record + 4) == 4) {
            float* value = reinterpret_cast<float*>(target_of(record));
            float next = *value - 0x1.11111p-7f;
            *value = next;
            if (next < 0.0f) *value = 0.0f;
        }
        if ((held & 0x802000u) != 0 && load_int(record + 4) == 4) {
            float* value = reinterpret_cast<float*>(target_of(record));
            float next = *value + 0x1.11111p-7f;
            if (1.0f < next) {
                *value = next;
                *value = 1.0f;
            } else {
                *value = next;
            }
        }
        if ((pressed & padAcceptButton) != 0 && load_int(record + 4) == 3) {
            int* value = reinterpret_cast<int*>(target_of(record));
            *value = (*value == 0);
            play("menu_main_accept.anm");
        }
        if ((pressed & 0x802000u) != 0) edit(record, 1);

        if (cursor != old) {
            norrath_menu_local_t_004844e8 = 0.0f;
            previous = old;
        }
        *reinterpret_cast<int*>(menu + terminator + 4) = cursor | (previous << 8);
    }
    if (changed != 0) *changed = (old != cursor);
    return cursor;
}
