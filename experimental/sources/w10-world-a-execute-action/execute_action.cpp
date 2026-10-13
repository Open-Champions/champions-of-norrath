// Observed executeAction__FP6Action (a local function) at 0x00187b38, 2484 bytes in SLUS_205.65.
// Runs one 20-byte cut-scene action record. Record layout as used here (offsets observed):
//   +0 float (the start time, used by the caller), +4 signed short `type`, +6 signed short `kind`,
//   +8, +12, +16 three words (`a`, `b`, `c`; integers, read as floats by some kinds).
// Dispatch on `type`:
//  * -100: only kind 6 acts: worldSetFarClip((float)(a * 12)). Every other kind returns.
//  * -99: kind 1 sets displayTextDisable to 1. Kind 0 starts a dialog line, kind 7 a music line
//    (both below). Other kinds return.
//  * 0 and above: `type` indexes the 44-byte records of the table `actor`; the record's first
//    word is an object address. Negative or other types, a null object or kind above 12 return.
// Dialog line (type -99, kind 0). Returns at once when dialogPacket is zero; otherwise sets
// displayTextDisable = 0 and returns when dlgDir is zero. Otherwise it also starts a frame figure of 0x10bda. With cutPlayMode set and cutSkipMode
// clear, soundContinueStream(2) is called; when displayDialogPos is inside 0..nmDialogNodes-1,
// "%s.vag" of the name at +49 of the 1096-byte dialog record is formatted, soundGetDialogSize
// gives the figure; when a next record exists, "%s.vag" of its name (+1145 from this one) is
// formatted and soundStartDialogStream(2, name, 40) is called. When displayDialogPos is not below
// nmDialogNodes the position is just advanced. Otherwise the current dialog record is used:
//   - with dialogActor set, its animation state (+288) loses the component found by flags 32
//     (animRemove with 0.0) and, if the state's count byte (+4) is positive, the last
//     component's float at +24 is clamped to >= 0 and the float at -16 relative to it to >= 1;
//   - the zero word of the anim table row (displayDialogPos & 1) ^ 1 is cleared (anim is two
//     rows of {word, word});
//   - a nonempty name at record +65 is looked up with objectFindObject: the old dialogActor loses
//     bit 28 of its 64-bit flags at +160; the new one (if it has bit 5 of its flags at +12) is
//     stored as dialogActor and gets that bit set, else dialogActor becomes 0;
//   - with a dialogActor, animations "idle" and "talk" are looked up and queued on it, either from
//     the anim row (when displayDialogPos < animationLoadPos and its second word is nonzero) or
//     from a computed length (frame figure * 7.29e-5 * 60 + 30, stored negated);
//   - displayDialogPos is incremented.
// Music line (type -99, kind 7): needs dlgDir and a nonzero first byte of its 80-byte record
// at displayMusicPos; with cutPlayMode set it either only advances (cutSkipMode set) or runs
// soundContinueStream(0), stores soundGetDialogSize(displayMusicPos) in musicSize and starts
// music displayMusicPos + 1 with 136; displayMusicPos is incremented.
// Object kinds (the object needs bit 5 of its flags at +12 where noted):
//   0 virtual +184 (1) with +570 byte cleared then masked 0xf7, sets +272,+276,+280 floats from a,b,c;
//   1 listDestroy(object+576); relink to actor[a]'s object; virtual +184 (3);
//   2 plays an animation chosen by `a` (see below); 3 virtual +24 (1); 4 virtual +24 (0);
//   5,6 nothing; 7 sets bit 3 of byte +570 and removes the object from the grid;
//   8 clears that bit and updates it in the grid; 9 objectAlwaysDraw; 10 moves the object (+16
//   +20 +24 from a,b,c) and updates the grid; 11 stores a as the short at +56; 12 toggles
//   bit 21 of the 64-bit flags at +160.
// Kind 2: with c != 0x0badfeed the a-th entry (zero based) of the object's list at +36 (pairs of
// {text, value}) whose text contains ".anm" gives the animation header (its value); with
// c == 0x0badfeed the lump named by the object (+960 for an NPC, else the first list text) is
// searched for "anm" entries and the one whose name hash (a one-at-a-time hash) equals a gives it.
// Then b chooses how it is played on the object's animation state: 0 and 3 one-shot, 1 and 2
// and 5 and 6 transitions (blend counts 4, 1, 3, 4), 4 nothing; blend times are float constants.
// Inferred: the field names in the notes above; the hash and the RTTI test are as observed
// (the type test compares the dynamic type with NPC).
// Unknown: the meaning of 0x0badfeed beyond its use here, the unit of the frame figure, and
// which record fields other code writes. The virtual slots' names are not claimed.
//
// Host builds define NORRATH_HOST_ARENA: object addresses are offsets into a host arena.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_base;
extern "C" void norrath_host_slot_call(unsigned function, unsigned char* self, int argument);
#endif

extern "C" {
extern signed char cutPlayMode;
extern signed char cutSkipMode;
extern int musicSize;
extern int displayTextDisable;
extern int displayMusicPos;
extern int displayDialogPos;
extern unsigned dlgDir;
extern unsigned dialogPacket;
extern int nmDialogNodes;
extern int animationLoadPos;
extern unsigned actor;
extern unsigned dialogActor;
extern unsigned anim[];

void worldSetFarClip__Ff(float);
void soundContinueStream__Fi(int);
int soundGetDialogSize__FPc(char*);
int soundGetDialogSize__Fi(int);
void soundStartDialogStream__FiPci(int, char*, int);
void soundStartMusic__Fii(int, int);
int animFindComponentWithFlags__FP14AnimationStatei(unsigned char*, int);
void animRemove__FP14AnimationStateP15AnimationHeaderf(unsigned char*, unsigned char*, float);
unsigned char* objectFindObject__FPc(char*);
unsigned char* objectFindAnimation__FP10GameObjectPc(unsigned char*, char*);
void animInitAnimation__FP15AnimationHeader(unsigned char*);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(unsigned char*, unsigned char*, float, int);
void animAddToEnd__FP14AnimationStateP15AnimationHeaderi(unsigned char*, unsigned char*, int);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(unsigned char*, unsigned char*, float, float, int);
void listDestroy__FP8ListHead(unsigned char*);
void listOnewayLink__FP8ListHeadT0Pv(unsigned char*, unsigned char*, unsigned char*);
void objectUpdateInGrid__FP10GameObject(unsigned char*);
void objectRemoveFromGrid__FP10GameObject(unsigned char*);
void objectAlwaysDraw__FP10GameObject(unsigned char*);
int sprintf(char*, const char*, ...);
char* strstr(const char*, const char*);
unsigned strlen(const char*);
unsigned char* lumpLoad__FPc(char*);
unsigned char* lumpFind__FPvPc(unsigned char*, char*);
unsigned char* lumpFindByType__FPvPci(unsigned char*, char*, int);
void* __tf3NPC();
int __eq__C9type_infoRC9type_info(void*, void*);
void __throw_bad_typeid();
}

namespace {
inline unsigned char* at(unsigned word) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline unsigned word_of(const void* pointer) {
#ifdef NORRATH_HOST_ARENA
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - norrath_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
template <class T> inline T& field(unsigned char* base, unsigned offset) {
    return *reinterpret_cast<T*>(base + offset);
}
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }

// Calls the virtual entry at (object's table at +72) + slot: a signed short this-adjustment,
// then the function word at +4.
inline void call_slot(unsigned char* object, unsigned slot, int argument) {
    unsigned char* entry = at(field<unsigned>(object, 72)) + slot;
    short adjust = field<short>(entry, 0);
    unsigned function = field<unsigned>(entry, 4);
#ifdef NORRATH_HOST_ARENA
    norrath_host_slot_call(function, object + adjust, argument);
#else
    reinterpret_cast<void (*)(unsigned char*, int)>(static_cast<__UINTPTR_TYPE__>(function))(object + adjust, argument);
#endif
}

inline float larger(float x, float y) { return x < y ? y : x; }
}

extern "C" void executeAction__FP6Action(unsigned char* record) {
    const int type = field<short>(record, 4);
    unsigned char* object = 0;

    if (type == -100) {
        if (field<short>(record, 6) == 6) worldSetFarClip__Ff(static_cast<float>(field<int>(record, 8) * 12));
        return;
    }
    if (type == -99) {
        const int kind = field<short>(record, 6);
        if (kind == 1) {
            displayTextDisable = kind;
            return;
        }
        if (kind == 0) {
            if (dialogPacket == 0) return;
            displayTextDisable = 0;  // the image stores this in the delay slot of the dlgDir test
            if (dlgDir == 0) return;
            int figure = 0x10bda;
            char name[80];
            if (cutPlayMode != 0 && cutSkipMode == 0) {
                soundContinueStream__Fi(2);
                int position = displayDialogPos;
                if (position >= 0 && position < nmDialogNodes) {
                    sprintf(name, "%s.vag", at(dialogPacket) + 1096 * position + 49);
                    figure = soundGetDialogSize__FPc(name);
                    position = displayDialogPos;
                }
                if (position + 1 < nmDialogNodes) {
                    sprintf(name, "%s.vag", at(dialogPacket) + 1096 * position + 1145);
                    soundStartDialogStream__FiPci(2, name, 40);
                }
            }
            int position = displayDialogPos;
            if (!(position < nmDialogNodes)) {
                displayDialogPos = position + 1;
                return;
            }
            unsigned char* line = at(dialogPacket) + 1096 * position;
            unsigned char* speaker = at(dialogActor);
            if (dialogActor != 0) {
                unsigned char* state = speaker + 288;
                int component = animFindComponentWithFlags__FP14AnimationStatei(state, 32);
                if (component >= 0) {
                    animRemove__FP14AnimationStateP15AnimationHeaderf(state, at(field<unsigned>(speaker, 296 + component * 32)), 0.0f);
                    int count = field<signed char>(state, 4);
                    if (count > 0) {
                        unsigned char* last = state + count * 32 - 24;
                        float low = larger(0.0f, field<float>(last, 24));
                        float high = larger(1.0f, field<float>(last, 8));
                        field<float>(last, 8) = high;
                        field<float>(last, 24) = low;
                    }
                }
            }
            anim[(displayDialogPos & 1) != 0 ? 1 : 3] = 0;
            if (field<signed char>(line, 65) != 0) {
                unsigned char* found = objectFindObject__FPc(reinterpret_cast<char*>(line + 65));
                unsigned char* previous = at(dialogActor);
                if (dialogActor != 0) {
                    field<unsigned long long>(previous, 160) &= ~0x10000000ull;
                }
                if (found == 0) {
                    dialogActor = 0;
                } else if ((field<int>(found, 12) & 0x20) == 0) {
                    dialogActor = 0;
                } else {
                    unsigned long long flags = field<unsigned long long>(found, 160);
                    dialogActor = word_of(found);
                    field<unsigned long long>(found, 160) = flags | 0x10000000ull;
                }
            }
            if (dialogActor != 0) {
                speaker = at(dialogActor);
                unsigned char* idle = objectFindAnimation__FP10GameObjectPc(speaker, const_cast<char*>("idle"));
                int row = displayDialogPos;
                if (row < animationLoadPos && anim[(row & 1) * 2 + 1] != 0) {
                    unsigned char* header = at(anim[(row & 1) * 2]);
                    animInitAnimation__FP15AnimationHeader(header);
                    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(speaker + 288, header, constant(0x3e999999u), 38);
                    anim[(displayDialogPos & 1) * 2 + 1] = 2;
                    if (idle != 0) {
                        unsigned char* state = at(dialogActor) + 288;
                        animAddToEnd__FP14AnimationStateP15AnimationHeaderi(state, idle, 3);
                        unsigned char* last = state + field<signed char>(state, 4) * 32 - 24;
                        float tail = field<float>(last, 24);
                        field<float>(last, 12) = constant(0x3d088888u);
                        field<int>(last, 8) = 0;
                        field<float>(last, 24) = tail + 30.0f;
                    }
                } else {
                    unsigned char* talk = objectFindAnimation__FP10GameObjectPc(at(dialogActor), const_cast<char*>("talk"));
                    if (talk != 0) {
                        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(at(dialogActor) + 288, talk, constant(0x3e999999u), 3);
                        if (idle != 0) {
                            float length = static_cast<float>(figure);
                            length = length * constant(0x3898eadbu);
                            length = length * 60.0f;
                            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(at(dialogActor) + 288, idle, constant(0x3eccccccu), 3);
                            length = length + 30.0f;
                            unsigned char* holder = at(dialogActor);
                            field<float>(holder, 288 + field<signed char>(holder, 292) * 32) = -length;
                        }
                    }
                }
            }
            displayDialogPos = displayDialogPos + 1;
            return;
        }
        if (kind == 7) {
            if (dlgDir == 0) return;
            int position = displayMusicPos;
            if (field<signed char>(at(dlgDir), 80 * position) == 0) return;
            if (cutPlayMode != 0) {
                if (cutSkipMode != 0) {
                    displayMusicPos = position + 1;
                    return;
                }
                soundContinueStream__Fi(0);
                musicSize = soundGetDialogSize__Fi(displayMusicPos);
                position = displayMusicPos;
                soundStartMusic__Fii(position + 1, 136);
                position = displayMusicPos;
            }
            displayMusicPos = position + 1;
        }
        return;
    }
    if (type >= 0) object = at(field<unsigned>(at(actor), type * 44));
    if (type < 0 || word_of(object) == 0) return;
    const int kind = field<short>(record, 6);
    if (static_cast<unsigned>(kind) >= 13) return;

    switch (kind) {
    case 0:
        if ((field<int>(object, 12) & 0x20) == 0) return;
        {
            signed char saved = field<signed char>(object, 570);
            field<signed char>(object, 570) = 0;
            call_slot(object, 184, 1);
            field<signed char>(object, 570) = saved & ~8;
            field<float>(object, 280) = static_cast<float>(field<int>(record, 16));
            field<float>(object, 272) = static_cast<float>(field<int>(record, 8));
            field<float>(object, 276) = static_cast<float>(field<int>(record, 12));
        }
        return;
    case 1:
        if ((field<int>(object, 12) & 0x20) == 0) return;
        listDestroy__FP8ListHead(object + 576);
        {
            int index = field<int>(record, 8);
            if (index < 0) return;
            unsigned char* target = at(field<unsigned>(at(actor), index * 44));
            listOnewayLink__FP8ListHeadT0Pv(object + 576, target, target);
            call_slot(object, 184, 3);
        }
        return;
    case 2: {
        if ((field<int>(object, 12) & 0x20) == 0) return;
        unsigned char* header = 0;
        if (field<int>(record, 16) != 0x0badfeed) {
            int occurrence = field<int>(record, 8);
            unsigned char* entry = at(field<unsigned>(object, 36));
            while (field<unsigned>(entry, 0) != 0 && occurrence >= 0) {
                if (strstr(reinterpret_cast<char*>(at(field<unsigned>(entry, 0))), ".anm") != 0) {
                    if (occurrence == 0) {
                        header = at(field<unsigned>(entry, 4));
                        break;
                    }
                    occurrence--;
                }
                entry += 8;
            }
        } else {
            void* own;
            if (object == 0) __throw_bad_typeid();
            own = reinterpret_cast<void* (*)()>(static_cast<__UINTPTR_TYPE__>(field<unsigned>(at(field<unsigned>(object, 72)), 4)))();
            char* lumpName;
            if (__eq__C9type_infoRC9type_info(own, __tf3NPC()) != 0) lumpName = reinterpret_cast<char*>(object + 960);
            else lumpName = reinterpret_cast<char*>(at(field<unsigned>(at(field<unsigned>(object, 36)), 0)));
            unsigned char* lump = lumpLoad__FPc(lumpName);
            int searched = 0;
            unsigned char* entry;
            while ((entry = lumpFindByType__FPvPci(lump, const_cast<char*>("anm"), searched)) != 0) {
                char* text = reinterpret_cast<char*>(at(field<unsigned>(entry, 0)));
                int hash = 0;
                int length = static_cast<int>(strlen(text));
                for (int i = 0; i < length; i++) {
                    hash += text[i];
                    hash += hash << 10;
                    hash ^= hash >> 6;
                }
                hash += hash << 3;
                hash ^= hash >> 11;
                hash += hash << 15;
                searched++;
                if (hash == field<int>(record, 8)) {
                    header = at(field<unsigned>(lumpFind__FPvPc(lump, text), 4));
                    break;
                }
            }
        }
        if (header == 0) return;
        const unsigned how = static_cast<unsigned>(field<int>(record, 12));
        if (how >= 7) return;
        switch (how) {
        case 0:
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(object + 288, header, constant(0x3e4cccccu), constant(0x3e4cccccu), 0);
            return;
        case 1:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(object + 288, header, constant(0x3e4cccccu), 4);
            return;
        case 2:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(object + 288, header, constant(0x3e4cccccu), 1);
            return;
        case 3:
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(object + 288, header, constant(0x3ca3d70au), constant(0x3ca3d70au), 0);
            return;
        case 5:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(object + 288, header, constant(0x3e4cccccu), 3);
            return;
        case 6:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(object + 288, header, constant(0x3e4cccccu), 4);
            return;
        }
        return;
    }
    case 3:
        call_slot(object, 24, 1);
        return;
    case 4:
        call_slot(object, 24, 0);
        return;
    case 7:
        if ((field<int>(object, 12) & 0x20) != 0) field<unsigned char>(object, 570) |= 8;
        objectRemoveFromGrid__FP10GameObject(object);
        return;
    case 8:
        if ((field<int>(object, 12) & 0x20) != 0) field<unsigned char>(object, 570) &= 0xf7;
        objectUpdateInGrid__FP10GameObject(object);
        return;
    case 9:
        objectAlwaysDraw__FP10GameObject(object);
        return;
    case 10:
        field<float>(object, 24) = static_cast<float>(field<int>(record, 16));
        field<float>(object, 16) = static_cast<float>(field<int>(record, 8));
        field<float>(object, 20) = static_cast<float>(field<int>(record, 12));
        objectUpdateInGrid__FP10GameObject(object);
        if ((field<int>(object, 12) & 0x20) == 0) return;
        field<float>(object, 60) = field<float>(object, 24);
        if (object == at(dialogActor)) return;
        {
            signed char saved = field<signed char>(object, 570);
            field<signed char>(object, 570) = 0;
            call_slot(object, 184, 0);
            field<signed char>(object, 570) = saved;
        }
        return;
    case 11:
        field<unsigned short>(object, 56) = field<unsigned short>(record, 8);
        return;
    case 12:
        if ((field<int>(object, 12) & 0x20) != 0) field<unsigned long long>(object, 160) ^= 0x200000ull;
        return;
    default:
        return;
    }
}
