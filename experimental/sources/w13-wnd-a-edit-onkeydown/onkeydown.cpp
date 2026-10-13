// Observed: OnKeyDown__9VIWndEditi at 0x010ac580 is 1612 bytes. With (this, key), ui being the word _5VIWnd$mUI
// (0x01192c70, read once at the start; the sound calls after a text change read it again), compact being the word
// sCompactKeyboard (0x01192bc8), and "sound(r)" meaning SoundPlay__4VIUI11VIUISoundId(ui, r != 0 ? 11 : 9) with the
// result r of the call just made. The word at +384 is cleared first. Then:
//  - keys 65..90, 97..122 and 48..57: Insert__9VIWndEditUs(this, key) then sound(result);
//  - a table over key - 4 (0..142), other keys do nothing:
//    keys 32 33 34 37 39 40 41 43 44 45 46 47 58 59 61 63 64 95 126: Insert(this, key), sound; key 8:
//    Backspace__9VIWndEdit(this), sound; key 133 (when compact is zero): when the 16-bit text at the pointer at
//    +424 is empty SoundPlay(ui, 9), else Backspace and sound; key 132 (compact zero): Insert(this, 32), sound;
//    key 13: Insert(this, 10) + sound when the word at +440 is nonzero, else the message call below with 259;
//    key 141 (compact zero): Insert(this, 10) + sound when +440 is nonzero; key 4 and 143: message 259 (143 plays
//    SoundPlay(ui, 11) first); keys 27 and 135: SoundPlay(ui, 8), then message 260 with zeros; key 142 (compact
//    zero): SoundPlay(ui, 11) and the word at +48 = 1; keys 17 and 18: the 64-bit value at +416 =
//    GetTime__7VITimer(), then SetCaretOffset__9VIWndEditi(this, CaretOffset__9VIWndEdit(this) -/+ 1), sound;
//    keys 19 and 20: MoveCaretUp / MoveCaretDown__9VIWndEdit(this), sound; keys 136, 137, 138, 139 (compact
//    zero): MoveCursor__9VIWndEditii(this, -1, 0 / 1, 0 / 0, -1 / 0, 1), then +384 = key and the float at +388 =
//    0.45f; key 146: OnKeyDown__5VIWndi(this, 146);
//    key 134 (unless compact equals 1): the words at +344 and +348 become the words at +328 and +332, the float at
//    +352 becomes 1.0f, and the byte c = sKeyMap[(word at +328) + 11 * (word at +332)] (sKeyMap at 0x01192b80,
//    66 bytes) selects: 1 Backspace + sound; 2 Insert 32 + sound; 3 SoundPlay(ui, 11) and the word at +52 = 1 -
//    itself; 5 SoundPlay(ui, 11) and message 259; 6 Insert 10 + sound; 7 MoveCaretUp; 8 MoveCaretDown (no sound
//    for 7 and 8); 9 and 10 the key 17 / 18 caret step without sound; 11, 12, 13, 14, 15, 16: Insert 45, 126,
//    44, 34, 95, 39 with sound; any other value (including 4 and 0) inserts the
//    byte as a character: an upper-case letter is lowered when the word at +44 is 0 and the 64-bit value at +48 is
//    0, or when the word at +52 is 1 and the word at +44 or the word at +48 equals 1; the byte is sign-extended
//    to 16 bits, Insert, sound.
//  "message m(p1, p2)": the object P at the word +20 has its table word at P + 32 followed to the entry at byte
//  offset 48 (halfword adjustment, then function address), which is called with (P + adjustment, this, m, p1, p2);
//  259 passes the words at +36 and +40, 260 passes zeros.
// Inferred: this is the key handler of a text-entry window with an on-screen keyboard grid (11 columns) next to
// ordinary key input; the words at +44, +48 and +52 act as caps/shift flags; sound 9 means a failed edit and 11
// a successful one.
// Unknown: key and code names, the exact meaning of messages 259 and 260, the words at +384/+388 (a held key and
// a repeat delay by inference), the result types of the called methods (only zero/nonzero is used) and the
// return type (void).
extern "C" {
int Insert__9VIWndEditUs(void* self, unsigned short ch);
int Backspace__9VIWndEdit(void* self);
int CaretOffset__9VIWndEdit(void* self);
int SetCaretOffset__9VIWndEditi(void* self, int offset);
int MoveCaretUp__9VIWndEdit(void* self);
int MoveCaretDown__9VIWndEdit(void* self);
void MoveCursor__9VIWndEditii(void* self, int dx, int dy);
void OnKeyDown__5VIWndi(void* self, int key);
unsigned long long GetTime__7VITimer();
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
extern unsigned _5VIWnd$mUI;
extern unsigned char sKeyMap[];
extern int sCompactKeyboard;

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, int, int);
};

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}
static inline void sound(int result) {
    SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), result != 0 ? 11 : 9);
}
static void send_message(void* self, int message, int p1, int p2) {
    unsigned char* parent = static_cast<unsigned char*>(pointer_of(static_cast<unsigned>(static_cast<int*>(self)[5])));
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(
        static_cast<unsigned char*>(pointer_of(*reinterpret_cast<unsigned*>(parent + 32))) + 48);
    entry->function(parent + entry->delta, self, message, p1, p2);
}
static int step_caret(void* self, int delta) {
    *reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(self) + 416) = GetTime__7VITimer();
    return SetCaretOffset__9VIWndEditi(self, CaretOffset__9VIWndEdit(self) + delta);
}
static void insert_key(void* self, unsigned short ch) {
    sound(Insert__9VIWndEditUs(self, ch));
}

void OnKeyDown__9VIWndEditi(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    const void* ui = pointer_of(_5VIWnd$mUI);
    words[96] = 0;
    if (static_cast<unsigned>(key - 65) < 26u) insert_key(self, static_cast<unsigned short>(key));
    if (static_cast<unsigned>(key - 97) < 26u) insert_key(self, static_cast<unsigned short>(key));
    if (static_cast<unsigned>(key - 48) < 10u) insert_key(self, static_cast<unsigned short>(key));
    if (static_cast<unsigned>(key - 4) >= 143u) return;
    switch (key) {
    case 32: case 33: case 34: case 37: case 39: case 40: case 41: case 43: case 44: case 45: case 46: case 47:
    case 58: case 59: case 61: case 63: case 64: case 95: case 126:
        insert_key(self, static_cast<unsigned short>(key));
        break;
    case 8:
        sound(Backspace__9VIWndEdit(self));
        break;
    case 27:
    case 135:
        SoundPlay__4VIUI11VIUISoundId(ui, 8);
        send_message(self, 260, 0, 0);
        break;
    case 13:
        if (words[110] != 0) insert_key(self, 10);
        else send_message(self, 259, words[9], words[10]);
        break;
    case 4:
        send_message(self, 259, words[9], words[10]);
        break;
    case 17:
        sound(step_caret(self, -1));
        break;
    case 18:
        sound(step_caret(self, 1));
        break;
    case 19:
        sound(MoveCaretUp__9VIWndEdit(self));
        break;
    case 20:
        sound(MoveCaretDown__9VIWndEdit(self));
        break;
    case 142:
        if (sCompactKeyboard != 0) return;
        SoundPlay__4VIUI11VIUISoundId(ui, 11);
        words[12] = 1;
        break;
    case 141:
        if (sCompactKeyboard != 0) return;
        if (words[110] != 0) insert_key(self, 10);
        break;
    case 146:
        OnKeyDown__5VIWndi(self, 146);
        break;
    case 136:
    case 137:
    case 138:
    case 139:
        if (sCompactKeyboard != 0) return;
        MoveCursor__9VIWndEditii(self, key == 136 ? -1 : key == 137 ? 1 : 0, key == 138 ? -1 : key == 139 ? 1 : 0);
        words[96] = key;
        *reinterpret_cast<float*>(bytes + 388) = 0.45f;
        break;
    case 132:
        if (sCompactKeyboard != 0) return;
        insert_key(self, 32);
        break;
    case 133:
        if (sCompactKeyboard != 0) return;
        if (**reinterpret_cast<unsigned short**>(bytes + 424) == 0) SoundPlay__4VIUI11VIUISoundId(ui, 9);
        else sound(Backspace__9VIWndEdit(self));
        break;
    case 143:
        SoundPlay__4VIUI11VIUISoundId(ui, 11);
        send_message(self, 259, words[9], words[10]);
        break;
    case 134: {
        if (sCompactKeyboard == 1) return;
        words[86] = words[82];
        *reinterpret_cast<float*>(bytes + 352) = 1.0f;
        words[87] = words[83];
        const unsigned char code = sKeyMap[words[82] + 11 * words[83]];
        switch (static_cast<signed char>(code)) {
        case 1:
            sound(Backspace__9VIWndEdit(self));
            break;
        case 2:
            insert_key(self, 32);
            break;
        case 3:
            SoundPlay__4VIUI11VIUISoundId(ui, 11);
            words[13] = 1 - words[13];
            break;
        case 5:
            SoundPlay__4VIUI11VIUISoundId(ui, 11);
            send_message(self, 259, words[9], words[10]);
            break;
        case 6:
            insert_key(self, 10);
            break;
        case 7:
            MoveCaretUp__9VIWndEdit(self);
            break;
        case 8:
            MoveCaretDown__9VIWndEdit(self);
            break;
        case 9:
            step_caret(self, -1);
            break;
        case 10:
            step_caret(self, 1);
            break;
        case 11:
            insert_key(self, 45);
            break;
        case 12:
            insert_key(self, 126);
            break;
        case 13:
            insert_key(self, 44);
            break;
        case 14:
            insert_key(self, 34);
            break;
        case 15:
            insert_key(self, 95);
            break;
        case 16:
            insert_key(self, 39);
            break;
        default: {
            unsigned char ch = code;
            if (static_cast<unsigned>(ch - 65) < 26u) {
                const int caps = words[11];
                bool lower;
                if (caps == 0 && *reinterpret_cast<unsigned long long*>(bytes + 48) == 0) {
                    lower = true;
                } else {
                    const int shifted = words[13];
                    if (shifted != 1) lower = false;
                    else if (caps == shifted) lower = true;
                    else lower = words[12] == shifted;
                }
                if (lower) ch = static_cast<unsigned char>(ch + 32);
            }
            insert_key(self, static_cast<unsigned short>(static_cast<signed char>(ch)));
            break;
        }
        }
        break;
    }
    default:
        break;
    }
}
}
