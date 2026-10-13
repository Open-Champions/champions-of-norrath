// Observed cutInitScene__FP9_cutscene at 0x0018d350, 3696 bytes. Nothing here
// was executed on the EE. Scene fields (byte offsets): +0 flag word, +4 offset
// of the action array, +8 action count, +12 offset of the actor array, +16
// actor count, +112 an int, +116 text. An action is 20 bytes: +0 float time,
// +4 and +6 halfwords (a kind and a type), +8, +12 and +16 words. An actor is
// 44 bytes: +0 object pointer, +4 a word, +8 name text, +28 three floats, +40
// a halfword, +42 and +43 bytes. `rot` below is -(signed halfword at +56 of
// the found origin object) passed to icos__Fi and isin__Fi.
// A. myYaw = -8192, myPitch = 10000, myRoll = 0, myFOV = 50.0, nmActors =
//    scene+16, actor = scene + scene+12, nmActions = scene+8, flags = scene+0,
//    action = scene + scene+4.
// B. When bit 0 of the scene flag word is clear: set it. If flag bit 2: move the
//    action array up by three records (memmove of count * 20 bytes to action
//    + 60), and add 3 to both the scene count and nmActions. If flag bit 4:
//    add 3 to both counts again. Look up the object named by cutSceneName
//    (0x00790f10) with objectFindObject__FPc; when found, with o = it
//    (position floats +16, +20, +24; halfword +56):
//    - each actor's three floats become (x*c + y*s + o.x, -x*s + y*c + o.y, z +
//      o.z) with c = icos(rot), s = isin(rot) (icos is called first), and its
//      halfword at +40 gets o's halfword added;
//    - each action: a record of kind -100 uses type 0 as "position" and type 1
//      as "yaw" (other types untouched); any other kind uses types 0, 5 and 10
//      as "position" and 6 and 11 as "yaw" (others untouched). A position
//      record has its words +8, +12, +16 converted to floats, transformed like
//      an actor position (icos, then isin, called per record) and stored back
//      as (int)(value + 0.5). A yaw record: h = (unsigned short)(0 -
//      (unsigned short)word8); h = (unsigned short)(h + o.halfword56); word8 =
//      -(int)(short)h; icos and isin are called but their results unused.
// C. When flag bit 2 is set and netNetworkMode is zero: actions 0, 1, 2 are
//    written as kind -100 records with time 0.0 and types 0, 1, 2: type 0 holds
//    the truncated ints of worldCameraPos at +8, +12, +16; type 1 holds
//    GetYaw__6Camera(gameCam) at +8; type 2 holds GetPitch__6Camera(gameCam)
//    at +8. When flag bit 4 is set and netNetworkMode is zero: the same three
//    records go into the last three actions, with time = (time of the action
//    four from the end) + 2.0.
// D. strcpy(cutStartMusic (0x00790f70), scene+116); displayTextDisable = 2;
//    soundGetDialogDir__Fv() -> dlgDir; cutLoadedDialogStream = 0;
//    dialogPacket = findCutDialog__Fv(); displayDialogPos = 0; displayMusicPos =
//    0; animationLoadPos = 0. When dlgDir is nonzero, displayMusicPos becomes
//    the index of the first 80-byte dlgDir entry (ending at a zero first byte)
//    whose text equals cutStartMusic. When dialogPacket and dlgDir are
//    nonzero: for each dialog node i < nmDialogNodes (records of 1096 bytes,
//    text at +49) the first dlgDir entry whose text equals sprintf("%s.anm",
//    that text) gives size = word at entry+144 - word at entry+64, and the
//    largest size is rounded up to a multiple of 2048; anim[0].block and
//    anim[1].block get memAlloc__FiPc(size, "anim1") and memAlloc__FiPc(size,
//    "anim2"), their second words 0; and when displayDialogPos <
//    nmDialogNodes: soundStartDialogStream(2, sprintf("%s.vag", text of node
//    displayDialogPos), 40).
// E. When any action has kind word 0xff9d and type word 7 (the word at +4 is
//    0x0007ff9d): soundResetStream(0) and soundStartMusic(displayMusicPos,
//    136). When the scene word +112 is zero it becomes 10.
// F. objectIterateRunLists(showHiddenObjects__FP10GameObject). For each actor
//    i: eRandState = 0x392fa + i is set before a string test for those with
//    byte +43 nonzero; the actor words +0 and +4 are cleared first.
//    - byte +43 nonzero: name "Player": if player[0]'s float at +172 > 0,
//      blockAlloc__Fi(1408) and the Player constructor __6PlayerG6Point3sii(
//      block, &copy of the position, halfword +40, 0, 1) and the block becomes
//      the object (a null block leaves it null); otherwise null. Name "Player2":
//      null when nmPlayers < 2, else the same with player[1]'s float and index 1.
//      Any other name: objectCreateByName__FPcG6Point3sPPc(name, &copy of the
//      position, halfword +40, the word at +4).
//      A nonzero object gets 0x88000 or'ed into its flags word at +12.
//    - byte +43 zero: objectFindInBox__FG6Point3T0PP10GameObjecti with the box
//      (x-240, y-240, z-100000) .. (x+240, y+240, z+100000) and up to 64
//      objects; among them skip objects with 0x80000 in +12, pick the nearest
//      in the xy plane within 600.0, requiring either the object's text pointer
//      at +52 to equal the actor name, or (only while no named match is held) its
//      class name, taken as the text of the type record returned by the function
//      at the +4 entry of its table at +72 with leading digits skipped, to equal
//      the actor name. A named match is held from the moment it becomes the
//      nearest, and class names are then no longer consulted. A winner is stored as the actor's object and word
//      +4 = winner's word +44; unless findTagString__10GameObjectPci finds the
//      tags "autoprop" or "autodoor" its halfword +56 and floats +16..+24 are
//      set from the actor and objectUpdateInGrid is called; its flags word
//      gets 0x88000 or'ed. No winner leaves words +0 and +4 zero.
//    - then for a nonzero object whose flag word +12 has 0x20: byte +569 =
//      actor byte +42, a virtual call through the entry at table(+72) + 184
//      (adjustment halfword, function pointer at +4) with argument 0, and bit
//      0 of byte +570 set.
// G. objectIterateRunLists(hideHiddenObjects__FP10GameObject). When flag bit 3
//    (8): objectIterateRunLists(removeMonster__FP10GameObject), else
//    sleepMonsters__8creaturev(). When flags & 6 is zero and frontEndRunning
//    is zero: flags |= 0x10. When flags & 0x10 and subtitlesOn: engineScreenSlide
//    = 16. Finally, for the first action of kind -100 and type 6 whose time is
//    at least 0.0: when its time is exactly 0.0, worldSetFarClip((float)(word8
//    * 12)).
// Not reproduced: the original contains an unreachable null test calling
// __throw_bad_typeid before the class-name call. The player 'float at +172'
// is read as in cutStop. Unknown: what the kinds, types, flag bits and the
// -99 / -100 markers mean.
extern "C" {
void* memmove(void* to, const void* from, unsigned long count);
int strcmp(const char* a, const char* b);
char* strcpy(char* to, const char* from);
int sprintf(char* to, const char* format, ...);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* objectFindObject__FPc(char* name);
void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*));
void showHiddenObjects__FP10GameObject(void* object);
void hideHiddenObjects__FP10GameObject(void* object);
void removeMonster__FP10GameObject(void* object);
void sleepMonsters__8creaturev();
int GetYaw__6Camera(void* camera);
int GetPitch__6Camera(void* camera);
char* soundGetDialogDir__Fv();
unsigned char* findCutDialog__Fv();
void* memAlloc__FiPc(int size, char* name);
void soundStartDialogStream__FiPci(int stream, char* name, int volume);
void soundResetStream__Fi(int stream);
void soundStartMusic__Fii(int index, int volume);
void* blockAlloc__Fi(int size);
void __6PlayerG6Point3sii(void* self, float* position, short yaw, int index, int extra);
void* objectCreateByName__FPcG6Point3sPPc(char* name, float* position, short yaw, char** tags);
void objectFindInBox__FG6Point3T0PP10GameObjecti(float* low, float* high, void** out, int count);
char* findTagString__10GameObjectPci(void* object, char* tag, int index);
void objectUpdateInGrid__FP10GameObject(void* object);
void worldSetFarClip__Ff(float distance);
extern short myYaw;
extern short myPitch;
extern short myRoll;
extern float myFOV;
extern int nmActors;
extern unsigned char* actor;
extern int nmActions;
extern unsigned char* action;
extern int flags;
extern signed char netNetworkMode;
extern void* gameCam;
extern float worldCameraPos[3];
extern char cutSceneName[];
extern char cutStartMusic[];
extern int displayTextDisable;
extern char* dlgDir;
extern int cutLoadedDialogStream;
extern unsigned char* dialogPacket;
extern int displayDialogPos;
extern int displayMusicPos;
extern int animationLoadPos;
extern int nmDialogNodes;
struct CutAnimSlot { void* block; int second; };
extern CutAnimSlot anim[2];
extern int eRandState;
extern unsigned char player[];
extern int nmPlayers;
extern int frontEndRunning;
extern int subtitlesOn;
extern int engineScreenSlide;
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;
typedef unsigned __attribute__((may_alias)) AliasedUnsigned;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;

#define WORD(base, off) (*reinterpret_cast<AliasedInt*>((base) + (off)))
#define FLOAT(base, off) (*reinterpret_cast<AliasedFloat*>((base) + (off)))
#define HALF(base, off) (*reinterpret_cast<AliasedShort*>((base) + (off)))
#define UHALF(base, off) (*reinterpret_cast<AliasedUShort*>((base) + (off)))

static inline unsigned char* Ptr(unsigned word) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word)); }
static inline unsigned Word(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

struct VirtualEntry {
    short adjust;
    short unused;
    void (*function)(void* self, int argument);
};
struct TypeEntry {
    short adjust;
    short unused;
    const unsigned char* (*function)();
};
#if defined(__mips__)
static_assert(__builtin_offsetof(VirtualEntry, function) == 4, "function pointer at +4");
static_assert(__builtin_offsetof(TypeEntry, function) == 4, "function pointer at +4");
#endif

static void transformPosition(unsigned char* record, unsigned char* origin) {
    const float x = static_cast<float>(WORD(record, 8));
    const float y = static_cast<float>(WORD(record, 12));
    const float z = static_cast<float>(WORD(record, 16));
    const int rot = -static_cast<int>(HALF(origin, 56));
    const float c = icos__Fi(rot);
    const float s = isin__Fi(-static_cast<int>(HALF(origin, 56)));
    float nx = x * c + y * s;
    nx = nx + FLOAT(origin, 16);
    float ny = (-x) * s + y * c;
    ny = ny + FLOAT(origin, 20);
    float nz = z + FLOAT(origin, 24);
    WORD(record, 8) = static_cast<int>(nx + 0.5f);
    WORD(record, 12) = static_cast<int>(ny + 0.5f);
    WORD(record, 16) = static_cast<int>(nz + 0.5f);
}

static void transformYaw(unsigned char* record, unsigned char* origin) {
    unsigned short h = static_cast<unsigned short>(0u - UHALF(record, 8));
    icos__Fi(-static_cast<int>(HALF(origin, 56)));
    isin__Fi(-static_cast<int>(HALF(origin, 56)));
    h = static_cast<unsigned short>(h + UHALF(origin, 56));
    WORD(record, 8) = -static_cast<int>(static_cast<short>(h));
}

static void writeCameraRecords(unsigned char* first, float time) {
    unsigned char* a = first;
    unsigned char* b = first + 20;
    unsigned char* c = first + 40;
    const float x = worldCameraPos[0];
    const float y = worldCameraPos[1];
    const float z = worldCameraPos[2];
    FLOAT(a, 0) = time;
    HALF(a, 4) = -100;
    HALF(a, 6) = 0;
    HALF(b, 6) = 1;
    WORD(a, 8) = static_cast<int>(x);
    WORD(a, 12) = static_cast<int>(y);
    FLOAT(b, 0) = time;
    WORD(a, 16) = static_cast<int>(z);
    HALF(b, 4) = -100;
    WORD(b, 8) = GetYaw__6Camera(gameCam);
    HALF(c, 6) = 2;
    HALF(c, 4) = -100;
    FLOAT(c, 0) = time;
    WORD(c, 8) = GetPitch__6Camera(gameCam);
}

extern "C" void cutInitScene__FP9_cutscene(unsigned char* scene) {
    const int sceneFlags = WORD(scene, 0);
    const int sceneActions = WORD(scene, 8);
    myYaw = -8192;
    myPitch = 10000;
    myFOV = 50.0f;
    nmActors = WORD(scene, 16);
    actor = scene + WORD(scene, 12);
    myRoll = 0;
    nmActions = sceneActions;
    flags = sceneFlags;
    action = scene + WORD(scene, 4);

    if ((sceneFlags & 1) == 0) {
        WORD(scene, 0) = sceneFlags | 1;
        if ((sceneFlags & 2) != 0) {
            memmove(action + 60, action, static_cast<unsigned long>(sceneActions) * 20);
            WORD(scene, 8) = WORD(scene, 8) + 3;
            nmActions = nmActions + 3;
        }
        if ((flags & 4) != 0) {
            WORD(scene, 8) = WORD(scene, 8) + 3;
            nmActions = nmActions + 3;
        }
        unsigned char* origin = static_cast<unsigned char*>(objectFindObject__FPc(cutSceneName));
        if (origin != nullptr) {
            for (int i = 0; i < nmActors; ++i) {
                unsigned char* record = actor + 44 * i;
                const int rot = -static_cast<int>(HALF(origin, 56));
                const float c = icos__Fi(rot);
                const float s = isin__Fi(-static_cast<int>(HALF(origin, 56)));
                UHALF(record, 40) = static_cast<unsigned short>(UHALF(record, 40) + UHALF(origin, 56));
                const float x = FLOAT(record, 28);
                const float y = FLOAT(record, 32);
                const float z = FLOAT(record, 36);
                float nx = x * c + y * s;
                nx = nx + FLOAT(origin, 16);
                float ny = (-x) * s + y * c;
                ny = ny + FLOAT(origin, 20);
                FLOAT(record, 28) = nx;
                FLOAT(record, 32) = ny;
                FLOAT(record, 36) = z + FLOAT(origin, 24);
            }
            for (int i = 0; i < nmActions; ++i) {
                unsigned char* record = action + 20 * i;
                const int kind = HALF(record, 4);
                const int type = HALF(record, 6);
                if (kind == -100) {
                    if (type == 0) transformPosition(record, origin);
                    else if (type == 1) transformYaw(record, origin);
                } else if (static_cast<unsigned>(type) < 12u) {
                    if (type == 0 || type == 5 || type == 10) transformPosition(record, origin);
                    else if (type == 6 || type == 11) transformYaw(record, origin);
                }
            }
        }
    }

    if ((flags & 2) != 0 && netNetworkMode == 0) writeCameraRecords(action, 0.0f);
    if ((flags & 4) != 0 && netNetworkMode == 0) {
        unsigned char* last = action + 20 * (nmActions - 3);
        const float time = FLOAT(action + 20 * (nmActions - 4), 0) + 2.0f;
        writeCameraRecords(last, time);
    }

    strcpy(cutStartMusic, reinterpret_cast<char*>(scene + 116));
    displayTextDisable = 2;
    cutLoadedDialogStream = 0;
    dlgDir = soundGetDialogDir__Fv();
    dialogPacket = findCutDialog__Fv();
    displayDialogPos = 0;
    displayMusicPos = 0;
    animationLoadPos = 0;
    if (dlgDir != nullptr) {
        int index = 0;
        for (int offset = 0; dlgDir[offset] != 0; offset += 80, ++index) {
            if (strcmp(cutStartMusic, dlgDir + offset) == 0) {
                displayMusicPos = index;
                break;
            }
        }
    }
    if (dialogPacket != nullptr && dlgDir != nullptr) {
        int size = 0;
        char name[80];
        for (int i = 0; i < nmDialogNodes; ++i) {
            sprintf(name, "%s.anm", reinterpret_cast<char*>(dialogPacket + 1096 * i + 49));
            if (dlgDir[0] != 0) {
                for (int offset = 0; dlgDir[offset] != 0; offset += 80) {
                    if (strcmp(name, dlgDir + offset) == 0) {
                        const int length = WORD(reinterpret_cast<unsigned char*>(dlgDir) + offset, 144) -
                                           WORD(reinterpret_cast<unsigned char*>(dlgDir) + offset, 64);
                        if (!(length < size)) size = length;
                        break;
                    }
                }
            }
        }
        size = (size + 2047) & -2048;
        anim[0].block = memAlloc__FiPc(size, const_cast<char*>("anim1"));
        anim[0].second = 0;
        anim[1].block = memAlloc__FiPc(size, const_cast<char*>("anim2"));
        anim[1].second = 0;
        if (displayDialogPos < nmDialogNodes) {
            char stream[80];
            sprintf(stream, "%s.vag", reinterpret_cast<char*>(dialogPacket + 1096 * displayDialogPos + 49));
            soundStartDialogStream__FiPci(2, stream, 40);
        }
    }

    bool musicCue = false;
    for (int i = 0; i < nmActions; ++i) {
        if (WORD(action + 20 * i, 4) == 0x0007ff9d) musicCue = true;
    }
    if (musicCue) {
        soundResetStream__Fi(0);
        soundStartMusic__Fii(displayMusicPos, 136);
    }
    if (WORD(scene, 112) == 0) WORD(scene, 112) = 10;

    objectIterateRunLists__FPFP10GameObject_v(showHiddenObjects__FP10GameObject);
    for (int i = 0; i < nmActors; ++i) {
        unsigned char* record = actor + 44 * i;
        const signed char create = static_cast<signed char>(record[43]);
        WORD(record, 0) = 0;
        WORD(record, 4) = 0;
        if (create != 0) {
            eRandState = 0x392fa + i;
            if (strcmp(reinterpret_cast<char*>(record + 8), "Player") == 0) {
                if (FLOAT(player, 172) > 0.0f) {
                    void* block = blockAlloc__Fi(1408);
                    float position[3];
                    position[0] = FLOAT(actor + 44 * i, 28);
                    position[1] = FLOAT(actor + 44 * i, 32);
                    position[2] = FLOAT(actor + 44 * i, 36);
                    if (block != nullptr) __6PlayerG6Point3sii(block, position, HALF(actor + 44 * i, 40), 0, 1);
                    WORD(actor + 44 * i, 0) = static_cast<int>(Word(block));
                } else {
                    WORD(actor + 44 * i, 0) = 0;
                }
            } else if (strcmp(reinterpret_cast<char*>(actor + 44 * i + 8), "Player2") == 0) {
                if (nmPlayers < 2) {
                    WORD(actor + 44 * i, 0) = 0;
                } else if (FLOAT(player, 23984 + 172) > 0.0f) {
                    void* block = blockAlloc__Fi(1408);
                    float position[3];
                    position[0] = FLOAT(actor + 44 * i, 28);
                    position[1] = FLOAT(actor + 44 * i, 32);
                    position[2] = FLOAT(actor + 44 * i, 36);
                    if (block != nullptr) __6PlayerG6Point3sii(block, position, HALF(actor + 44 * i, 40), 1, 1);
                    WORD(actor + 44 * i, 0) = static_cast<int>(Word(block));
                } else {
                    WORD(actor + 44 * i, 0) = 0;
                }
            } else {
                unsigned char* current = actor + 44 * i;
                float position[3];
                position[0] = FLOAT(current, 28);
                position[1] = FLOAT(current, 32);
                position[2] = FLOAT(current, 36);
                void* created = objectCreateByName__FPcG6Point3sPPc(reinterpret_cast<char*>(current + 8), position,
                                                                    HALF(current, 40), reinterpret_cast<char**>(Ptr(WORD(current, 4))));
                WORD(actor + 44 * i, 0) = static_cast<int>(Word(created));
            }
            unsigned char* object = Ptr(WORD(actor + 44 * i, 0));
            if (object == nullptr) continue;
            WORD(object, 12) = WORD(object, 12) | 0x88000;
        } else {
            unsigned char* current = actor + 44 * i;
            const float cx = FLOAT(current, 28);
            const float cy = FLOAT(current, 32);
            const float cz = FLOAT(current, 36);
            float low[3] = {cx - 240.0f, cy - 240.0f, cz - 100000.0f};
            float high[3] = {cx + 240.0f, cy + 240.0f, cz + 100000.0f};
            void* found[64];
            objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
            unsigned char* best = nullptr;
            bool bestNamed = false;
            float bestDistance = 600.0f;
            for (int k = 0; found[k] != nullptr; ++k) {
                unsigned char* candidate = static_cast<unsigned char*>(found[k]);
                if ((WORD(candidate, 12) & 0x80000) != 0) continue;
                char* text = reinterpret_cast<char*>(Ptr(WORD(candidate, 52)));
                bool named = false;
                if (text != nullptr) named = strcmp(text, reinterpret_cast<char*>(actor + 44 * i + 8)) == 0;
                if (!named) {
                    if (bestNamed) continue;
                    const TypeEntry* entry = reinterpret_cast<const TypeEntry*>(
                        *reinterpret_cast<unsigned char* const*>(candidate + 72));
                    const unsigned char* type = entry->function();
                    const char* className = *reinterpret_cast<const char* const*>(type);
                    while (static_cast<unsigned>(*className - '0') < 10u) ++className;
                    if (strcmp(className, reinterpret_cast<char*>(actor + 44 * i + 8)) != 0) continue;
                }
                const float dx = FLOAT(actor + 44 * i, 28) - FLOAT(candidate, 16);
                const float dy = FLOAT(actor + 44 * i, 32) - FLOAT(candidate, 20);
                const float distance = __builtin_sqrtf(dx * dx + dy * dy);
                if (distance < bestDistance) {
                    bestDistance = distance;
                    best = candidate;
                    bestNamed = named;
                }
            }
            if (best != nullptr) {
                unsigned char* current2 = actor + 44 * i;
                WORD(current2, 4) = WORD(best, 44);
                WORD(current2, 0) = static_cast<int>(Word(best));
                if (findTagString__10GameObjectPci(best, const_cast<char*>("autoprop"), 0) == nullptr &&
                    findTagString__10GameObjectPci(best, const_cast<char*>("autodoor"), 0) == nullptr) {
                    unsigned char* rec = actor + 44 * i;
                    UHALF(best, 56) = UHALF(rec, 40);
                    FLOAT(best, 16) = FLOAT(rec, 28);
                    FLOAT(best, 20) = FLOAT(rec, 32);
                    FLOAT(best, 24) = FLOAT(rec, 36);
                    objectUpdateInGrid__FP10GameObject(best);
                }
                WORD(best, 12) = WORD(best, 12) | 0x88000;
            } else {
                WORD(actor + 44 * i, 4) = 0;
                WORD(actor + 44 * i, 0) = 0;
            }
        }
        unsigned char* record2 = actor + 44 * i;
        unsigned char* object = Ptr(WORD(record2, 0));
        if (object != nullptr && (WORD(object, 12) & 0x20) != 0) {
            object[569] = record2[42];
            const VirtualEntry* entry = reinterpret_cast<const VirtualEntry*>(
                *reinterpret_cast<unsigned char* const*>(object + 72) + 184);
            entry->function(object + entry->adjust, 0);
            object[570] = static_cast<unsigned char>(object[570] | 1);
        }
    }
    objectIterateRunLists__FPFP10GameObject_v(hideHiddenObjects__FP10GameObject);
    if ((flags & 8) != 0) objectIterateRunLists__FPFP10GameObject_v(removeMonster__FP10GameObject);
    else sleepMonsters__8creaturev();
    if ((flags & 6) == 0 && frontEndRunning == 0) flags = flags | 0x10;
    if ((flags & 0x10) != 0 && subtitlesOn != 0) engineScreenSlide = 16;

    unsigned char* farRecord = nullptr;
    for (int i = 0; i < nmActions; ++i) {
        unsigned char* record = action + 20 * i;
        if (HALF(record, 4) != -100) continue;
        if (HALF(record, 6) != 6) continue;
        if (0.0f <= FLOAT(record, 0)) {
            farRecord = record;
            break;
        }
    }
    if (farRecord != nullptr && FLOAT(farRecord, 0) == 0.0f) {
        worldSetFarClip__Ff(static_cast<float>(WORD(farRecord, 8) * 12));
    }
}
