// Observed: Read__10VIKeyboardR12VIConsoleKey at 0x002cb4a0 is 1352 bytes and
// returns the result of DequeueKeyEvent__10VIKeyboardP12VIConsoleKey(this, out),
// which every path ends with. Offsets are in this unless marked. The word at 0
// is a state (0 closed, 1 open); the word at 4 a device handle; 16 + 2 * code is
// a table of 256 byte pairs (last translated key, key-down flag).
// State 0: gKeyboardAttached = 0; word 528 = 0; lgKbdEnumerate(0, buffer). If it
//   returns nonzero, nothing else. Otherwise lgKbdOpen(0, this + 4); on failure
//   word 4 = -1, on success lgKbdSetRepeatRate(0, 0), the 512 bytes at 16 are
//   cleared, word 12 = 0 and the state becomes 1.
// State 1: gKeyboardAttached = 1; lgKbdEnumHint(&hint). If it fails or the hint
//   word equals 1 the device is closed (lgKbdClose(word 4), state 0). Otherwise
//   n = lgKbdRead(word 4, buffer). The values 0x80000000, 0x80000004,
//   0x80000006 and 0x8000000d close the device; for 0x8000000d first
//   printf("Keyboard Buffer Overflow? Resetting"). Zero processes events; any
//   other value does nothing. Any other state does nothing.
// Events: the read buffer has a 4-byte header whose halfword at 2 is the event
//   count, then 16-byte events. For each event the halfwords at event offsets 4,
//   8, 10 and 12 are code, flags A, flags B (the one tested with 0x22) and
//   direction (1 means down). A one-byte key is chosen as below; if it is
//   nonzero, PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey is called with the
//   two-byte record {key, direction == 1}. Key 255 means "none".
//   * code 88 gives 4 or 13 (4 if flags B & 0x22) whatever flags A is. For codes
//     89 to 97 with flags A bit 0 clear: 89 -> 23, 90 -> 20, 91 -> 22, 92 -> 17,
//     93 -> none, 94 -> 18, 95 -> 24, 96 -> 19, 97 -> 21. Other codes from 83 to
//     103 (and these with flags A bit 0 set) use the translate path.
//   * code 40 -> 4 or 13; 73 -> 25, 74 -> 24, 75 -> 21, 76 -> 26, 77 -> 23,
//     78 -> 22, 79 -> 18, 80 -> 17, 81 -> 20, 82 -> 19; other codes 41 to 72 and
//     codes outside 40 to 103 use the translate path with an unconditional flag
//     store.
//   Translate path: down -> key = TranslateEvent__10VIKeyboardP11_lgKbdEvent(this,
//   event), stored at 16 + 2 * code; up -> key = the stored byte, which is then
//   cleared. The flag byte at 17 + 2 * code takes the direction (for codes 83 to
//   103 only when key is nonzero).
// Not byte-identical: the compiler lays out the two original jump tables as
// switches. Inferred: the code values are scan-code-like numbers from lgKbdRead;
// the key numbers are the game's own and are not named here.
// Unknown: names of the flag halfwords, the word at 12 and 528, the sizes of
// the enumerate buffer, and the 0x80000000 / 0x80000004 error meanings.
extern "C" {
extern int gKeyboardAttached;
int lgKbdEnumerate(int index, void* buffer);
int lgKbdOpen(int index, void* handle);
int lgKbdSetRepeatRate(int index, int rate);
int lgKbdEnumHint(void* hint);
int lgKbdRead(int handle, void* buffer);
int lgKbdClose(int handle);
void* memset(void* destination, int value, unsigned count);
int printf(const char* format, ...);
unsigned char TranslateEvent__10VIKeyboardP11_lgKbdEvent(void* keyboard, void* event);
void PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey(void* keyboard, void* key);
int DequeueKeyEvent__10VIKeyboardP12VIConsoleKey(void* keyboard, void* key);

static unsigned half(const unsigned char* element, int offset) {
    return *reinterpret_cast<const unsigned short*>(element + offset);
}

// The key for code 88 and code 40 depends on the flags B halfword.
static unsigned char flags_key(const unsigned char* element) {
    return (half(element, 10) & 0x22) != 0 ? 4 : 13;
}

// Reads or writes the byte pair for a code and returns the translated key.
static unsigned char translate(unsigned char* keyboard, unsigned char* element, unsigned code, bool down) {
    unsigned char* slot = keyboard + 16 + 2 * code;
    unsigned char key;
    if (down) {
        key = TranslateEvent__10VIKeyboardP11_lgKbdEvent(keyboard, element);
        slot[0] = key;
    } else {
        key = slot[0];
        slot[0] = 0;
    }
    return key;
}

static void process_event(unsigned char* keyboard, unsigned char* element) {
    unsigned code = half(element, 4);
    unsigned flags_a = half(element, 8);
    bool down = half(element, 12) == 1;
    unsigned char key = 0;
    if (code - 83u < 21u) {
        if ((flags_a & 1) == 0) {
            switch (code) {
            case 88: key = flags_key(element); break;
            case 89: key = 23; break;
            case 90: key = 20; break;
            case 91: key = 22; break;
            case 92: key = 17; break;
            case 93: key = 255; break;
            case 94: key = 18; break;
            case 95: key = 24; break;
            case 96: key = 19; break;
            case 97: key = 21; break;
            default: break;
            }
        }
        if (key == 255) return;
        if (key == 0) {
            if (code == 88) {
                key = flags_key(element);
            } else {
                key = translate(keyboard, element, code, down);
                if (key != 0) keyboard[17 + 2 * code] = down ? 1 : 0;
            }
        }
    } else {
        switch (code) {
        case 40: key = flags_key(element); break;
        case 73: key = 25; break;
        case 74: key = 24; break;
        case 75: key = 21; break;
        case 76: key = 26; break;
        case 77: key = 23; break;
        case 78: key = 22; break;
        case 79: key = 18; break;
        case 80: key = 17; break;
        case 81: key = 20; break;
        case 82: key = 19; break;
        default:
            key = translate(keyboard, element, code, down);
            keyboard[17 + 2 * code] = down ? 1 : 0;
            break;
        }
    }
    if (key != 0) {
        unsigned char record[2] = {key, static_cast<unsigned char>(down ? 1 : 0)};
        PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey(keyboard, record);
    }
}

int Read__10VIKeyboardR12VIConsoleKey(void* object, void* out) {
    unsigned char* keyboard = static_cast<unsigned char*>(object);
    int* words = static_cast<int*>(object);
    if (words[0] == 0) {
        alignas(16) unsigned char scratch[16];
        gKeyboardAttached = 0;
        words[132] = 0;
        if (lgKbdEnumerate(0, scratch) == 0) {
            if (lgKbdOpen(0, keyboard + 4) != 0) {
                words[1] = -1;
            } else {
                lgKbdSetRepeatRate(0, 0);
                memset(keyboard + 16, 0, 512);
                words[3] = 0;
                words[0] = 1;
            }
        }
    } else if (words[0] == 1) {
        alignas(16) int hint[4];
        gKeyboardAttached = 1;
        bool close_device = lgKbdEnumHint(hint) != 0 || hint[0] == 1;
        if (!close_device) {
            alignas(16) unsigned char buffer[208];
            int result = lgKbdRead(words[1], buffer);
            unsigned status = static_cast<unsigned>(result);
            if (status == 0x80000000u || status == 0x80000004u || status == 0x80000006u) {
                close_device = true;
            } else if (status == 0x8000000du) {
                printf("Keyboard Buffer Overflow? Resetting");
                close_device = true;
            } else if (result == 0) {
                for (int index = 0; index < static_cast<int>(half(buffer, 2)); ++index)
                    process_event(keyboard, buffer + 4 + 16 * index);
            }
        }
        if (close_device) {
            lgKbdClose(words[1]);
            words[0] = 0;
        }
    }
    return DequeueKeyEvent__10VIKeyboardP12VIConsoleKey(object, out);
}
}
