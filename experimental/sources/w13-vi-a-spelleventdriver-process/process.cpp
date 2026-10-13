// Observed: Process__18VISpellEventDriver at 0x01181df0 is 1356 bytes. Argument: driver (a0).
// Fields (all words unless stated): +16 effect driver pointer, +20 scene pointer, +24 int, +28 int
// index of the next scripted event, +32 pointer to the active wait event, +36 and +48 non-zero
// flags/pointers, +72 pointer to the event table (int count at table+4, the record array pointer at
// table+8, records of 44 bytes), +76 head word of an active-event list and +88 head word of a
// second list (both lists end with a node whose first word is zero), +80/+92 not touched.
//   1. If +36 is non-zero it calls ProcessProjectileEvent__18VISpellEventDriver(driver); if +36 is
//      still non-zero afterwards the function returns 1. If +48 is non-zero it calls
//      ProcessLightningEvent__18VISpellEventDriver(driver); if +32 is non-zero it calls
//      ProcessWaitEvent__18VISpellEventDriver(driver); then it always calls
//      ProcessInactiveEvents__18VISpellEventDriver(driver).
//   2. For every node of the list at +76 whose first word (next) is non-zero (the last node is
//      not visited) it takes event = the word at node+116 and its int type = *event:
//      3 ProcessCreateEmitter(driver, node, 1), 6 ProcessSetEmitterMotif(driver, node),
//      9 ProcessCreateSprite(driver, node, 1), 11 ProcessCreateSound(driver, node, 1),
//      14 ProcessFadeSprite(driver, node) (all __18VISpellEventDriverP18VISpellActiveEvent[b]); other
//      types do nothing.
//   3. If +32 is zero and the index (+28) is below the table count, the record r = table[+8] + 44 *
//      index is handled by its int type r[0] (types outside 1..17 only advance the index):
//      1  creates a wait event: p = __builtin_new(184); when non-null it calls
//         __18VISpellActiveEventP12VISpellEventP7VIScene(p, r, scene); stores p at +32 and stores
//         the 64-bit result of GetTime__7VIScene(scene) at p+176 (through the +32 word);
//      2  NewSetAction, 3 NewCreateEmitter, 6 NewSetEmitterMotif, 8 NewPlayTargetProjectile, 9
//         NewCreateSprite, 10 NewDestroySprite, 11 NewCreateSound, 12 NewDestroySound, 13
//         NewPlayTargetLightning, 14 NewFadeSprite, 15 NewPlayAttack, 17 NewShakeCamera (all
//         __18VISpellEventDriverP12VISpellEvent, called with (driver, r));
//      4  searches the +76 list for the first node (next non-zero) whose event word (node+116) has
//         int [4] equal to r[4] and int [0] equal to 3; if found, Remove__7VILNode(node) and
//         AddTail__7VILListP7VILNode(driver + 88, node); if not found it only advances the index;
//      5  the same search; when found stores r[8] at node+132 and the float r[20] at node+136;
//      7  with flags = r[4]: if bit 1 is set it calls AddSpellEventDriver__19VISpellEffectDriverP
//         16VISpellEventListi(e, *e + 24, e[12]) with e = the word at +16; if bit 2 is set it calls
//         the same function for i in 0 .. e[16] (re-reading e and the count after every call) with
//         (e, *e + 36, e[20 + 4 * i]);
//      16 only when e[96] == the int at +24 (e the word at +16; otherwise only advances) and the
//         function word e[84] is non-zero (otherwise only advances): it calls e[84](a0, a1, a2,
//         e[88]) with (1, 100, 0) by default, a0 = byte r[8], a1 = byte r[12], a2 = r[16] when
//         r[4] == 0, (1, 80, 200) when r[4] == 1 and (1, 220, 140) when r[4] == 2.
//      After the handlers that do not "only advance" the table pointer (+72) and the index (+28)
//      are re-read, and the index is stored incremented (the advance-only paths store the old
//      index + 1 instead).
//   4. If the index equals the table count, and the first words of the nodes at +76 and +88 are
//      both zero, and +32 and +48 are both zero, it returns 0; otherwise it returns 1.
// Inferred: the per-frame update of a scripted spell: the event table is a timeline played one
// record at a time (stopping while a wait event is active), active events (emitters, sprites,
// sounds) are updated from the +76 list and the function reports whether anything is still
// running (1) or the spell is finished (0). The pairs of "New..." and "Process..." functions
// create and update the individual event kinds.
// Unknown: the VISpellEvent record layout beyond the offsets above, the type names of the 17
// record types (only the called function names), what +36/+48 hold, and the meaning of types 4
// and 5 beyond "stop/update the matching emitter".
extern "C" {
void ProcessProjectileEvent__18VISpellEventDriver(void* driver);
void ProcessLightningEvent__18VISpellEventDriver(void* driver);
void ProcessWaitEvent__18VISpellEventDriver(void* driver);
void ProcessInactiveEvents__18VISpellEventDriver(void* driver);
void ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb(void* driver, void* node, int flag);
void ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent(void* driver, void* node);
void ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb(void* driver, void* node, int flag);
void ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb(void* driver, void* node, int flag);
void ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent(void* driver, void* node);
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);
void NewSetAction__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewCreateEmitter__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewCreateSprite__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewDestroySprite__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewCreateSound__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewDestroySound__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewFadeSprite__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewPlayAttack__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void NewShakeCamera__18VISpellEventDriverP12VISpellEvent(void* driver, void* event);
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);
void AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(void* effect, void* list, int arg);
}

// Words that hold 32-bit target addresses: a host check defines NORRATH_HOST_ARENA, so they hold
// arena offsets / function ids and the helpers below are provided by the check.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
extern "C" unsigned norrath_host_word(const void* pointer);
#define WORD_POINTER(word) norrath_host_pointer(word)
#define WORD_FUNCTION(word) norrath_host_function(word)
#define POINTER_WORD(pointer) norrath_host_word(pointer)
#else
#define WORD_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define WORD_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#define POINTER_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

static inline unsigned char* pointerAt(const unsigned char* p) {
    return WORD_POINTER(*reinterpret_cast<const unsigned*>(p));
}
static inline int intAt(const unsigned char* p) { return *reinterpret_cast<const int*>(p); }

// Walks the +76 list for the first visited node whose event is type 3 with the given id.
static inline unsigned char* findEmitterNode(unsigned char* driver, int id) {
    unsigned char* node = pointerAt(driver + 76);
    unsigned char* next = pointerAt(node);
    if (next != 0) {
        for (;;) {
            unsigned char* event = pointerAt(node + 116);
            if (intAt(event + 4) == id && intAt(event) == 3) {
                return node;
            }
            node = next;
            next = pointerAt(node);
            if (next == 0) {
                break;
            }
        }
    }
    return 0;
}

typedef void (*CameraCallback)(int a, int b, int c, int d);

extern "C" {
int Process__18VISpellEventDriver(unsigned char* driver) {
    if (intAt(driver + 36) != 0) {
        ProcessProjectileEvent__18VISpellEventDriver(driver);
        if (intAt(driver + 36) != 0) {
            return 1;
        }
    }
    if (intAt(driver + 48) != 0) {
        ProcessLightningEvent__18VISpellEventDriver(driver);
    }
    if (intAt(driver + 32) != 0) {
        ProcessWaitEvent__18VISpellEventDriver(driver);
    }
    ProcessInactiveEvents__18VISpellEventDriver(driver);

    unsigned char* node = pointerAt(driver + 76);
    unsigned char* next = pointerAt(node);
    if (next != 0) {
        do {
            switch (intAt(pointerAt(node + 116))) {
            case 3:
                ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb(driver, node, 1);
                break;
            case 6:
                ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent(driver, node);
                break;
            case 9:
                ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb(driver, node, 1);
                break;
            case 11:
                ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb(driver, node, 1);
                break;
            case 14:
                ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent(driver, node);
                break;
            default:
                break;
            }
            node = next;
            next = pointerAt(node);
        } while (next != 0);
    }

    unsigned char* table = pointerAt(driver + 72);
    int index;
    if (intAt(driver + 32) != 0) {
        index = intAt(driver + 28);
    } else {
        index = intAt(driver + 28);
        if (index < intAt(table + 4)) {
            unsigned char* record = pointerAt(table + 8) + 44 * index;
            // advance: 0 = the index is re-read and incremented, 1 = incremented from the old value
            int advanceOld = 0;
            switch (intAt(record)) {
            case 1: {
                void* created = __builtin_new(184);
                if (created != 0) {
                    __18VISpellActiveEventP12VISpellEventP7VIScene(created, record, pointerAt(driver + 20));
                }
                *reinterpret_cast<unsigned*>(driver + 32) = POINTER_WORD(created);
                long long now = GetTime__7VIScene(pointerAt(driver + 20));
                *reinterpret_cast<long long*>(pointerAt(driver + 32) + 176) = now;
                break;
            }
            case 2:
                NewSetAction__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 3:
                NewCreateEmitter__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 4: {
                unsigned char* found = findEmitterNode(driver, intAt(record + 4));
                if (found == 0) {
                    advanceOld = 1;
                } else {
                    Remove__7VILNode(found);
                    AddTail__7VILListP7VILNode(driver + 88, found);
                }
                break;
            }
            case 5: {
                unsigned char* found = findEmitterNode(driver, intAt(record + 4));
                if (found == 0) {
                    advanceOld = 1;
                } else {
                    *reinterpret_cast<int*>(found + 132) = intAt(record + 8);
                    *reinterpret_cast<float*>(found + 136) = *reinterpret_cast<float*>(record + 20);
                }
                break;
            }
            case 6:
                NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 7: {
                int flags = intAt(record + 4);
                if ((flags & 1) != 0) {
                    unsigned char* effect = pointerAt(driver + 16);
                    AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(effect, pointerAt(effect) + 24, intAt(effect + 12));
                    flags = intAt(record + 4);
                }
                if ((flags & 2) != 0) {
                    unsigned char* effect = pointerAt(driver + 16);
                    if (intAt(effect + 16) > 0) {
                        int i = 0;
                        do {
                            AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(effect, pointerAt(effect) + 36, intAt(effect + 20 + 4 * i));
                            ++i;
                            effect = pointerAt(driver + 16);
                        } while (i < intAt(effect + 16));
                    }
                }
                break;
            }
            case 8:
                NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 9:
                NewCreateSprite__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 10:
                NewDestroySprite__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 11:
                NewCreateSound__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 12:
                NewDestroySound__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 13:
                NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 14:
                NewFadeSprite__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 15:
                NewPlayAttack__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            case 16: {
                unsigned char* effect = pointerAt(driver + 16);
                if (intAt(effect + 96) != intAt(driver + 24)) {
                    advanceOld = 1;
                } else if (intAt(effect + 84) == 0) {
                    advanceOld = 1;
                } else {
                    int mode = intAt(record + 4);
                    int a = 1, b = 100, c = 0;
                    if (mode == 1) {
                        b = 80;
                        c = 200;
                    } else if (mode < 2) {
                        if (mode == 0) {
                            c = intAt(record + 16);
                            a = record[8];
                            b = record[12];
                        }
                    } else if (mode == 2) {
                        b = 220;
                        c = 140;
                    }
                    reinterpret_cast<CameraCallback>(WORD_FUNCTION(*reinterpret_cast<unsigned*>(effect + 84)))(a, b, c, intAt(effect + 88));
                }
                break;
            }
            case 17:
                NewShakeCamera__18VISpellEventDriverP12VISpellEvent(driver, record);
                break;
            default:
                advanceOld = 1;
                break;
            }
            if (advanceOld) {
                *reinterpret_cast<int*>(driver + 28) = index + 1;
                index = index + 1;
            } else {
                table = pointerAt(driver + 72);
                index = intAt(driver + 28) + 1;
                *reinterpret_cast<int*>(driver + 28) = index;
            }
        }
    }
    if (index != intAt(table + 4)) {
        return 1;
    }
    if (intAt(pointerAt(driver + 76)) != 0) {
        return 1;
    }
    if (intAt(pointerAt(driver + 88)) != 0) {
        return 1;
    }
    if (intAt(driver + 32) != 0) {
        return 1;
    }
    return intAt(driver + 48) != 0 ? 1 : 0;
}
}
