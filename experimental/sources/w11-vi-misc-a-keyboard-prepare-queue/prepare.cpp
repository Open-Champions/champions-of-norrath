// Observed: PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey at 0x002cb9e8 is 504
// bytes and returns nothing. The key record is two bytes: key at 0 and a down flag
// at 1. In the keyboard object the word at 528 holds a pending dead key (0 = none)
// and the word at 532 the key whose release is to be swallowed. The dead-key set
// is the first sUsDeadkeyCount words of the table at 0x00442148 (the symbol table
// gives sFrDeadkey, sUkDeadkey and sUsDeadkey at that address; four words
// 0x5e 0x60 0x7e 0xa8 follow in the image), compared against the key byte.
// Key down:
//  - A dead key is pending: word 532 = key; c = CombineDeadkey__10VIKeyboardUc(this,
//    key). If c is nonzero it queues {c, down} then {c, up}. If c is zero it queues
//    the pending dead key as {dead, down}, {dead, up} (low byte of word 528) and
//    then the key itself as {key, down}, {key, up}. Either way word 528 is cleared.
//  - No dead key is pending: a dead key becomes pending (word 528 = key) and
//    nothing is queued; any other key is queued unchanged by QueueKeyEvent.
// Key up: if word 532 equals the key it is cleared and nothing is queued. Otherwise
// a dead key is ignored and any other key is queued unchanged.
// Inferred: this implements keyboard dead-key composition. Unknown: which layout the
// table belongs to and what sets sUsDeadkeyCount (it is 0 in the file image).
extern "C" {
extern const int sUsDeadkey[];
extern int sUsDeadkeyCount;
int CombineDeadkey__10VIKeyboardUc(void* keyboard, unsigned char key);
void QueueKeyEvent__10VIKeyboardP12VIConsoleKey(void* keyboard, void* record);

static bool is_dead_key(unsigned char key) {
    int count = sUsDeadkeyCount;
    for (int index = 0; index < count; ++index)
        if (key == sUsDeadkey[index]) return true;
    return false;
}

static void queue_pair(void* keyboard, unsigned char* record, unsigned char key) {
    record[0] = key;
    record[1] = 1;
    QueueKeyEvent__10VIKeyboardP12VIConsoleKey(keyboard, record);
    record[1] = 0;
    QueueKeyEvent__10VIKeyboardP12VIConsoleKey(keyboard, record);
}

void PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey(void* keyboard, void* event) {
    int* words = static_cast<int*>(keyboard);
    unsigned char* key = static_cast<unsigned char*>(event);
    alignas(16) unsigned char record[2];
    if (key[1] != 0) {
        if (words[132] != 0) {
            words[133] = key[0];
            int combined = CombineDeadkey__10VIKeyboardUc(keyboard, key[0]);
            if (combined != 0) {
                queue_pair(keyboard, record, static_cast<unsigned char>(combined));
            } else {
                queue_pair(keyboard, record, static_cast<unsigned char>(words[132]));
                queue_pair(keyboard, record, key[0]);
            }
            words[132] = 0;
        } else if (is_dead_key(key[0])) {
            words[132] = key[0];
        } else {
            QueueKeyEvent__10VIKeyboardP12VIConsoleKey(keyboard, event);
        }
    } else {
        bool dead = is_dead_key(key[0]);
        if (words[133] == key[0]) {
            words[133] = 0;
        } else if (!dead) {
            QueueKeyEvent__10VIKeyboardP12VIConsoleKey(keyboard, event);
        }
    }
}
}
