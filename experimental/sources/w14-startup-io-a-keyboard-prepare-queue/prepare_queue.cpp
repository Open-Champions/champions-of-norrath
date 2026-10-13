// Observed: PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey at 0x002cb9e8 is 504 bytes
// and returns nothing. The key record is two bytes, {code, direction}. It uses two
// words of the keyboard object, +528 (a pending dead-key code, 0 for none) and +532
// (the last code taken as input while one was pending), and the table sFrDeadkey of
// sUsDeadkeyCount words that hold dead-key codes (a match compares the byte against
// the whole word). Every queued record goes to QueueKeyEvent__10VIKeyboardP12VIConsoleKey.
// Direction nonzero (down):
//   * with +528 nonzero: +532 = code; r = CombineDeadkey__10VIKeyboardUc(this, code).
//     If r is nonzero the records {r, 1} and {r, 0} are queued. If r is zero the
//     records {pending, 1}, {pending, 0}, {code, 1} and {code, 0} are queued (the low
//     byte of +528 is the pending code). Afterwards +528 = 0.
//   * with +528 zero: if the code equals any of the first sUsDeadkeyCount table words,
//     +528 = code and nothing is queued, except that a code of 0 is queued as normal;
//     otherwise the incoming record itself is queued.
// Direction zero (up): a code that matches the table is never queued; any other code
// is queued unless it equals +532. Whenever the code equals +532, +532 becomes 0.
// Inferred: this implements dead-key (accent) composition for the keyboard queue
// that Read__10VIKeyboardR12VIConsoleKey fills and DequeueKeyEvent drains.
// Unknown: the table contents beyond what the image holds, and what the uses of the
// +532 word are outside this function.
extern "C" {
extern int sFrDeadkey[];
extern int sUsDeadkeyCount;
unsigned char CombineDeadkey__10VIKeyboardUc(void* keyboard, unsigned char code);
void QueueKeyEvent__10VIKeyboardP12VIConsoleKey(void* keyboard, void* record);

static bool isDeadkey(unsigned code, int count) {
    for (int index = 0; index < count; ++index) {
        if (code == static_cast<unsigned>(sFrDeadkey[index])) return true;
    }
    return false;
}

void PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey(void* object, void* record) {
    int* words = static_cast<int*>(object);
    unsigned char* key = static_cast<unsigned char*>(record);
    int count = sUsDeadkeyCount;
    unsigned char out[2];
    if (key[1] != 0) {
        if (words[132] != 0) {
            words[133] = key[0];
            unsigned char combined = CombineDeadkey__10VIKeyboardUc(object, key[0]);
            if (combined == 0) {
                out[0] = static_cast<unsigned char>(words[132]);
                out[1] = 1;
                QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, out);
                out[1] = 0;
                QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, out);
                out[0] = key[0];
                out[1] = 1;
                QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, out);
                out[1] = 0;
                QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, out);
            } else {
                out[0] = combined;
                out[1] = 1;
                QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, out);
                out[1] = 0;
                QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, out);
            }
            words[132] = 0;
        } else {
            unsigned code = key[0];
            bool matched = isDeadkey(code, count);
            if (matched) words[132] = static_cast<int>(code);
            if (!matched || code == 0) QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, record);
        }
    } else {
        unsigned code = key[0];
        bool matched = isDeadkey(code, count) && code != 0;
        if (!matched && words[133] != static_cast<int>(code)) QueueKeyEvent__10VIKeyboardP12VIConsoleKey(object, record);
        if (words[133] == static_cast<int>(key[0])) words[133] = 0;
    }
}
}
