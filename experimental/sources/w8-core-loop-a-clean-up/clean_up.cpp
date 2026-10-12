// Observed cleanUp__Fv at 0x0017e370, 968 bytes (file-local in the original). It is called from the pause
// menu code.
//   1. objectIterateGrid is called with cleanUpFunc2__FP10GameObject, then with cleanUpFunc1__FP10GameObject.
//   2. For each of three groups g = 0, 1, 2 of the player record `player`: entries start at byte 328 + 4616 * g,
//      are 72 bytes each, and the entry count is the word at byte 4936 + 4616 * g, re-read at every step.
//      Sixteen integer scores start at zero. Each entry is copied to a local 72-byte buffer and sorted into a
//      class from the byte fields of the description its first word points at:
//        g == 0: class 1 when the signed byte at +30 is 4, else class 0;
//        g == 1: class = the signed byte at +28;
//        g == 2: class = the signed byte at +28, except that 15 gives -1.
//      A negative class is ignored. First pass over the entries: score[class] becomes
//      max(score[class], getValue(copy)). Second pass, from entry 0: when getValue(copy) is less than
//      score[class] the entry is removed - the gold total grows by (int)((float)value * 0.4f)
//      (bits 0x3ecccccc), removeItemFromInventory(0, index, g, 0) is called, and the same index is looked at
//      again; otherwise score[class] is raised by one and the entry stays. So per class only the first entry
//      holding the maximum value is kept.
//   3. playerRecomputeWeight(0) is called, and the total is added to the signed word at byte 232 of `player`.
// Return: the original leaves a register value behind; no caller uses it, so it is declared void here.
// Inferred: that removing entry k shifts later entries down (the loop re-examines index k); the word at +232
//   behaves like a running amount of money.
// Unknown: the item and description types, what the classes mean, the second argument of getValue (the
//   original passes nothing), the other bytes of an entry and of its description, and what happens when a
//   class byte is 16 or more (the score array has 16 slots and no bound is applied).
extern "C" {
extern unsigned char player[];
void objectIterateGrid__FPFP10GameObject_v(void (*visit)(unsigned char*));
void cleanUpFunc1__FP10GameObject(unsigned char* object);
void cleanUpFunc2__FP10GameObject(unsigned char* object);
int getValue__4itemPQ24item11description(unsigned char* item);
void removeItemFromInventory__Fiiii(int a, int index, int group, int b);
void playerRecomputeWeight__Fi(int a);
}

namespace {
int load_int(const unsigned char* bytes) {
    const unsigned value = static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
                           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(value);
}

void store_int(unsigned char* bytes, int value) {
    const unsigned v = static_cast<unsigned>(value);
    bytes[0] = static_cast<unsigned char>(v);
    bytes[1] = static_cast<unsigned char>(v >> 8);
    bytes[2] = static_cast<unsigned char>(v >> 16);
    bytes[3] = static_cast<unsigned char>(v >> 24);
}

const unsigned char* load_pointer(const unsigned char* bytes) {
    const unsigned char* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

float bits_to_float(unsigned bits) {
    float value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}

int classify(int group, const unsigned char* entry) {
    const unsigned char* description = load_pointer(entry);
    if (group == 1) return static_cast<signed char>(description[28]);
    if (group == 0) return static_cast<signed char>(description[30]) == 4 ? 1 : 0;
    if (group == 2) {
        const int c = static_cast<signed char>(description[28]);
        return c != 15 ? c : -1;
    }
    return 0;
}

void copy_entry(unsigned char* to, const unsigned char* from) {
    for (int i = 0; i < 72; ++i) to[i] = from[i];
}
}

extern "C" void cleanUp__Fv() {
    int total = 0;
    objectIterateGrid__FPFP10GameObject_v(cleanUpFunc2__FP10GameObject);
    objectIterateGrid__FPFP10GameObject_v(cleanUpFunc1__FP10GameObject);
    const float factor = bits_to_float(0x3ecccccc);
    for (int group = 0; group < 3; ++group) {
        int score[16];
        for (int i = 0; i < 16; ++i) score[i] = 0;
        unsigned char* entries = player + 328 + 4616 * group;
        const unsigned char* count = player + 4936 + 4616 * group;
        alignas(8) unsigned char copy[72];

        for (int i = 0; i < load_int(count); ++i) {
            copy_entry(copy, entries + 72 * i);
            const int slot = classify(group, copy);
            if (slot >= 0) {
                const int value = getValue__4itemPQ24item11description(copy);
                if (score[slot] < value) score[slot] = value;
            }
        }
        for (int i = 0; i < load_int(count); ++i) {
            copy_entry(copy, entries + 72 * i);
            const int slot = classify(group, copy);
            if (slot >= 0) {
                const int value = getValue__4itemPQ24item11description(copy);
                if (value < score[slot]) {
                    total += static_cast<int>(static_cast<float>(value) * factor);
                    removeItemFromInventory__Fiiii(0, i, group, 0);
                    --i;
                } else {
                    score[slot] = score[slot] + 1;
                }
            }
        }
    }
    playerRecomputeWeight__Fi(0);
    store_int(player + 232, load_int(player + 232) + total);
}
