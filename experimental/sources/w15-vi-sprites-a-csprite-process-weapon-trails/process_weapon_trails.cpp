// Observed: ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44 at 0x010afbe8, 1484 bytes. Arguments (this, camera, scene, matrix). Four
// zero-filled 16-aligned VIMatrix44 locals (m1..m4) and two zeroed three-float vectors (a, b) are kept; every exit destroys m4, m3, m2, m1 in that order
// with the destructor flag 2. Words are 32-bit at the stated offsets from this unless noted.
//  1. When the word at +4544 is zero the function returns 0. It then reads GetTime__7VIScene(scene); when that minus the 64-bit value at +4568 is
//     below 50 (signed) it returns 0.
//  2. When the word at +4576 (a trail handle) is non-zero and the word at +4580 is neither -1 nor equal to the word at +4584, it calls
//     VITrailFxEnd__FP9VITrailFx(handle) and clears +4576.
//  3. The word at +4492 minus 22, if below 23, selects a slot s: values 22, 24, 27, 40 give 1; 23, 25, 26, 28, 29, 41, 42, 44 give 0; 30 gives 2; every
//     other value (and anything outside 22..44) returns 0.
//  4. When +4584 differs from +4580, or is -1, it calls VITrailFxBegin__Flif(GetTime(scene), 25, 0.39999998f) and stores the result at +4576; a zero
//     result returns -1, otherwise +4580 takes the value of +4584.
//  5. m3 = Mul(matrix, camera+176); CalcTransAboutCenter__9VIHSprite(this, m3, scene, m2). The word at +4400+4*s is an attachment number; when it is -1
//     the function returns 0. GetAttachSprite__C9VIHSpritei(this, number) gives a scene sprite id; unless GetSpriteType(scene, id) is 11 it returns 0.
//     That sprite (Sprite__7VIScenei) is a group: its count is the word at +64 (re-read every pass) and the member ids are at +68+32*i. For each member
//     whose GetSpriteType is 2: CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44(group, i, m4); m3 = Mul(m4, R) where R is the matrix at offset 48 of
//     the 256-byte record number (word at +4364+12*s) in the table whose base is the word at (this+64)+8; m4 = Mul(m3, m2). If the member sprite's word
//     at +64 is 0 the translation row m4[12..14] is copied to a and a counter is incremented; if it is 1 the same row goes to b and the counter is
//     incremented. As soon as the counter is 2 after a member it calls AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3(handle at +4576, a, b,
//     the colour at this+4548, the colour (0, 0, 0, 1)) and leaves the loop. It returns 0 whichever way the loop ends.
// Inferred: the trail is a ribbon between two member positions of the equipped weapon's group sprite; +4580/+4584 are the current and wanted trail
// kinds, +4568 the time of the last segment and +4544 an enable flag.
// Unknown: what the values 22..44 of +4492 mean (they select the slot), what the sprite kinds 0 and 1 in the group members are, what kind 11 and 2 are,
// and the unit of the time (a trail segment is added at most every 50 of them).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
long long GetTime__7VIScene(void* scene);
void VITrailFxEnd__FP9VITrailFx(int trail);
int VITrailFxBegin__Flif(long long time, int count, float value);
void AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3(int trail, const float* a, const float* b, const void* color, const float* secondColor);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b);
void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(void* self, const void* matrix, void* scene, void* out);
int GetAttachSprite__C9VIHSpritei(const void* self, int index);
int GetSpriteType__7VIScenei(void* scene, int id);
unsigned char* Sprite__7VIScenei(void* scene, int id);
void CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44(void* group, int index, void* out);
void _$_10VIMatrix44(void* self, int flags);

static int& word(unsigned char* bytes, int offset) { return *reinterpret_cast<int*>(bytes + offset); }

static void release(float* m1, float* m2, float* m3, float* m4) {
    _$_10VIMatrix44(m4, 2);
    _$_10VIMatrix44(m3, 2);
    _$_10VIMatrix44(m2, 2);
    _$_10VIMatrix44(m1, 2);
}

int ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(void* object, void* camera, void* scene, void* matrix) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    alignas(16) float m1[16];
    alignas(16) float m2[16];
    alignas(16) float m3[16];
    alignas(16) float m4[16];
    float a[3] = {0.0f, 0.0f, 0.0f};
    float b[3] = {0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 16; ++i) m1[i] = 0.0f;
    for (int i = 0; i < 16; ++i) m2[i] = 0.0f;
    for (int i = 0; i < 16; ++i) m3[i] = 0.0f;
    for (int i = 0; i < 16; ++i) m4[i] = 0.0f;
    if (word(bytes, 4544) == 0) {
        release(m1, m2, m3, m4);
        return 0;
    }
    if (GetTime__7VIScene(scene) - *reinterpret_cast<long long*>(bytes + 4568) < 50) {
        release(m1, m2, m3, m4);
        return 0;
    }
    int trail = word(bytes, 4576);
    if (trail != 0) {
        int kind = word(bytes, 4580);
        if (kind != -1 && kind != word(bytes, 4584)) {
            VITrailFxEnd__FP9VITrailFx(trail);
            word(bytes, 4576) = 0;
        }
    }
    static const signed char kSlots[23] = {1, 0, 1, 0, 0, 1, 0, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 0, 0, -1, 0};
    unsigned selector = static_cast<unsigned>(word(bytes, 4492) - 22);
    int slot = selector < 23 ? kSlots[selector] : -1;
    if (slot < 0) {
        release(m1, m2, m3, m4);
        return 0;
    }
    if (word(bytes, 4584) != word(bytes, 4580) || word(bytes, 4584) == -1) {
        int started = VITrailFxBegin__Flif(GetTime__7VIScene(scene), 25, 0.39999998f);
        word(bytes, 4576) = started;
        if (started == 0) {
            release(m1, m2, m3, m4);
            return -1;
        }
        word(bytes, 4580) = word(bytes, 4584);
    }
    Mul__10VIMatrix44RC10VIMatrix44T1(m3, matrix, static_cast<unsigned char*>(camera) + 176);
    CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(object, m3, scene, m2);
    int found = 0;
    int attachment = word(bytes, 4400 + 4 * slot);
    if (attachment != -1) {
        int id = GetAttachSprite__C9VIHSpritei(object, attachment);
        if (GetSpriteType__7VIScenei(scene, id) == 11) {
            unsigned char* group = Sprite__7VIScenei(scene, id);
            for (int i = 0; i < word(group, 64); ++i) {
                int memberId = word(group, 68 + 32 * i);
                if (GetSpriteType__7VIScenei(scene, memberId) != 2) continue;
                unsigned char* member = Sprite__7VIScenei(scene, memberId);
                CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44(group, i, m4);
                unsigned record = static_cast<unsigned>(word(bytes, 64 + 8)) + (static_cast<unsigned>(word(bytes, 4364 + 12 * slot)) << 8) + 48u;
                Mul__10VIMatrix44RC10VIMatrix44T1(m3, m4, NORRATH_WORD_TO_POINTER(record));
                Mul__10VIMatrix44RC10VIMatrix44T1(m4, m3, m2);
                int memberKind = word(member, 64);
                if (memberKind == 0) {
                    a[0] = m4[12];
                    a[1] = m4[13];
                    a[2] = m4[14];
                    ++found;
                } else if (memberKind == 1) {
                    b[0] = m4[12];
                    b[1] = m4[13];
                    b[2] = m4[14];
                    ++found;
                }
                if (found == 2) {
                    float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
                    AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3(word(bytes, 4576), a, b, bytes + 4548, color);
                    break;
                }
            }
        }
    }
    release(m1, m2, m3, m4);
    return 0;
}
}
