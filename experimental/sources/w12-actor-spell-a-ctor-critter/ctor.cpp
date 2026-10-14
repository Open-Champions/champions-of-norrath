// Observed __7CritterPcG6Point3sPPc at 0x001efeb0, 1944 bytes: the Critter constructor. Arguments: the object, a name
// string, a position P (by value), a short type T and an argument list. Returns nothing meaningful (the register
// holds whatever finishConstruction left). In order:
//  1. A roll eRand__10GameObject(object) is taken; scale = (float)(roll % 14 + 4) (unsigned remainder). The Creature
//     constructor runs with (object, P, T, a null resource map, scale, argument list). Byte +40 = 10, the word at +1084
//     takes the float at +788 converted to an integer, bit 8 is OR-ed into +12, _vt$7Critter goes to +72, the float 1.2
//     to +168, 10.0 to +28, frameNm to +248, byte +172 = 18, word +1076 = 2, and +244, +176, +192, +236, the halfword at
//     +572 and +1080 are cleared. setParameters__8Creaturefffffi(object, 1.0 x5, 1) runs.
//  2. Words +1028 and +1020 = 180, +1024 = 60, and +1032..+1048 and +1052..+1072 are cleared.
//  3. The name is lowercased in place (each signed character whose _ctype_ entry (index c + 1) has bit 0 gets 32 added)
//     and copied with strcpy to a local N.
//  4. For each of the tags "lumpname", "wakname", "rawname", "idlename", "movename", "followname", "fleename",
//     "osidlename" and "osfleename": findTagString(object, tag, 0) is asked; when present, sprintf(buffer, F, that string)
//     is made (asked a second time for the string), otherwise sprintf(buffer, D, N). F/D are "%s.lmp"/"%s.lmp" for the
//     first tag (the lump name, kept in a local L), "%s.vif" for the second and "%s.tex" for the third, and "%s.anm"
//     (tag present) against "%s_idle.anm", "%s_move.anm", "%s_follow.anm", "%s_flee.anm", "%s_osidle.anm" and
//     "%s_osflee.anm" (tag absent) for the rest. Each of the last eight results is lumpFindResource__FPcT0(L, buffer),
//     stored at +1036, +1040, +1044, +1056, +1060, +1064, +1068 and +1072 in that order; if the +1068 result is null
//     the word at +1044 replaces it, and if the +1072 result is null the word at +1064 replaces it.
//  5. The tag "turnSpeed": when present it is read with sscanf and "%f" into the float at +1016, otherwise +1016 is 1.0.
//  6. animInitState__FP14AnimationState(object + 288). gameLineInitPathState__FP17gameLinePathStatePcP6Point3 receives
//     (object + 960, the string of tag "path" or null, object + 16).
//  7. When the word at +980 is non-zero: bit 5 is OR-ed into +12, animAdd__FP14AnimationStateP15AnimationHeaderi(object + 288,
//     word +1056, 2049), animRandomizeTime__FP14AnimationState(object + 288) and setState__7Critteri(object, 12) run. Otherwise,
//     when findTagInt__10GameObjectPci(object, "oneshot", 0) is non-zero: setState(13); else when +1044 is non-zero setState(0);
//     else when +1056 is non-zero setState(1).
//  8. For i = 0..31: sprintf(buffer, "change%d", i), and when that tag is present bit i is OR-ed into +244.
//  9. specificInit__7CritterPcT1(object, N, L); the 64-bit result of modelGetActiveChunks__FP12_modelHeaderi(word +1036,
//     word +244) goes to +552; objectAddToRunList runs when the halfword at +572 is 12, objectAddToSlowRunList otherwise;
//     finishConstruction__8Creature(object) runs last.
// Inferred: the tags select animation and model resource names for a creature. Unknown: the meaning of most fields.
// Known difference: the original keeps the name copy, lump name and resource name in adjacent 16-byte stack slots, so a
// name of 12 or more characters overruns them; the local buffers here are larger and do not reproduce that overrun
// (the paired runs use names of at most 11 characters).
// Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char _ctype_[];
extern int frameNm;
extern unsigned char vt_Critter[] __asm__("_vt$7Critter");
unsigned eRand__10GameObject(void* object);
void creature_construct(void* self, Point3 position, short type, void* map, float scale, char** arguments)
    __asm__("__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc");
void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);
char* strcpy(char* to, const char* from);
char* findTagString__10GameObjectPci(void* self, char* tag, int occurrence);
int findTagInt__10GameObjectPci(void* self, char* tag, int occurrence);
int sprintf(char* out, const char* format, ...);
int sscanf(const char* text, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void gameLineInitPathState__FP17gameLinePathStatePcP6Point3(void* state, char* tag, Point3* position);
void animInitState__FP14AnimationState(void* state);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void animRandomizeTime__FP14AnimationState(void* state);
void setState__7Critteri(void* self, int state);
void specificInit__7CritterPcT1(void* self, char* name, char* lump);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);
void objectAddToRunList__FP10GameObject(void* object);
void objectAddToSlowRunList__FP10GameObject(void* object);
void finishConstruction__8Creature(void* self);
void __7CritterPcG6Point3sPPc(void* self, char* name, Point3 position, short type, char** arguments);
}

namespace {
inline __attribute__((always_inline)) void make_name(void* self, char* tag, const char* tag_format, const char* default_format, char* buffer,
                                                      const char* name_copy) {
    if (findTagString__10GameObjectPci(self, tag, 0) != nullptr)
        sprintf(buffer, tag_format, findTagString__10GameObjectPci(self, tag, 0));
    else
        sprintf(buffer, default_format, name_copy);
}
}  // namespace

extern "C" void __7CritterPcG6Point3sPPc(void* self, char* name, Point3 position, short type, char** arguments) {
    using namespace w11;
    const float scale = static_cast<float>(static_cast<int>(eRand__10GameObject(self) % 14u + 4u));
    creature_construct(self, position, type, nullptr, scale, arguments);
    u8(self, 40) = 10;
    i32(self, 1084) = static_cast<int>(f32(self, 788));
    u32(self, 12) |= 8u;
    u32(self, 72) = word(vt_Critter);
    f32(self, 168) = 1.2f;
    f32(self, 28) = 10.0f;
    u32(self, 248) = static_cast<unsigned>(frameNm);
    u8(self, 172) = 18;
    u32(self, 1076) = 2;
    u32(self, 244) = 0;
    u32(self, 176) = 0;
    u32(self, 192) = 0;
    u32(self, 236) = 0;
    u16(self, 572) = 0;
    u32(self, 1080) = 0;
    setParameters__8Creaturefffffi(self, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1);
    u32(self, 1028) = 180;
    u32(self, 1024) = 60;
    u32(self, 1020) = 180;
    u32(self, 1032) = 0;
    u32(self, 1036) = 0;
    u32(self, 1040) = 0;
    u32(self, 1072) = 0;
    u32(self, 1068) = 0;
    u32(self, 1064) = 0;
    u32(self, 1060) = 0;
    u32(self, 1056) = 0;
    u32(self, 1052) = 0;
    u32(self, 1048) = 0;
    u32(self, 1044) = 0;
    for (char* p = name; *p != 0; ++p) {
        int c = static_cast<signed char>(*p);
        if ((_ctype_[c + 1] & 1) != 0) c += 32;
        *p = static_cast<char>(c);
    }
    char name_copy[32];
    char lump[48];
    char resource[64];
    strcpy(name_copy, name);
    char* const lump_tag = const_cast<char*>("lumpname");
    make_name(self, lump_tag, "%s.lmp", "%s.lmp", lump, name_copy);
    make_name(self, const_cast<char*>("wakname"), "%s.vif", "%s.vif", resource, name_copy);
    u32(self, 1036) = word(lumpFindResource__FPcT0(lump, resource));
    make_name(self, const_cast<char*>("rawname"), "%s.tex", "%s.tex", resource, name_copy);
    u32(self, 1040) = word(lumpFindResource__FPcT0(lump, resource));
    make_name(self, const_cast<char*>("idlename"), "%s.anm", "%s_idle.anm", resource, name_copy);
    u32(self, 1044) = word(lumpFindResource__FPcT0(lump, resource));
    make_name(self, const_cast<char*>("movename"), "%s.anm", "%s_move.anm", resource, name_copy);
    u32(self, 1056) = word(lumpFindResource__FPcT0(lump, resource));
    make_name(self, const_cast<char*>("followname"), "%s.anm", "%s_follow.anm", resource, name_copy);
    u32(self, 1060) = word(lumpFindResource__FPcT0(lump, resource));
    make_name(self, const_cast<char*>("fleename"), "%s.anm", "%s_flee.anm", resource, name_copy);
    u32(self, 1064) = word(lumpFindResource__FPcT0(lump, resource));
    make_name(self, const_cast<char*>("osidlename"), "%s.anm", "%s_osidle.anm", resource, name_copy);
    void* found = lumpFindResource__FPcT0(lump, resource);
    u32(self, 1068) = word(found);
    if (found == nullptr) u32(self, 1068) = u32(self, 1044);
    make_name(self, const_cast<char*>("osfleename"), "%s.anm", "%s_osflee.anm", resource, name_copy);
    found = lumpFindResource__FPcT0(lump, resource);
    u32(self, 1072) = word(found);
    if (found == nullptr) u32(self, 1072) = u32(self, 1064);
    char* const speed_tag = findTagString__10GameObjectPci(self, const_cast<char*>("turnSpeed"), 0);
    if (speed_tag != nullptr)
        sscanf(speed_tag, "%f", &f32(self, 1016));
    else
        f32(self, 1016) = 1.0f;
    void* const state = static_cast<unsigned char*>(self) + 288;
    animInitState__FP14AnimationState(state);
    gameLineInitPathState__FP17gameLinePathStatePcP6Point3(static_cast<unsigned char*>(self) + 960, findTagString__10GameObjectPci(self, const_cast<char*>("path"), 0),
                                                           reinterpret_cast<Point3*>(static_cast<unsigned char*>(self) + 16));
    if (u32(self, 980) != 0) {
        u32(self, 12) |= 5u;
        animAdd__FP14AnimationStateP15AnimationHeaderi(state, pointer(u32(self, 1056)), 2049);
        animRandomizeTime__FP14AnimationState(state);
        setState__7Critteri(self, 12);
    } else if (findTagInt__10GameObjectPci(self, const_cast<char*>("oneshot"), 0) != 0) {
        setState__7Critteri(self, 13);
    } else if (u32(self, 1044) != 0) {
        setState__7Critteri(self, 0);
    } else if (u32(self, 1056) != 0) {
        setState__7Critteri(self, 1);
    }
    char change[32];
    for (int i = 0; i < 32; ++i) {
        sprintf(change, "change%d", i);
        if (findTagString__10GameObjectPci(self, change, 0) != nullptr) u32(self, 244) |= 1u << i;
    }
    specificInit__7CritterPcT1(self, name_copy, lump);
    *reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(self) + 552) = modelGetActiveChunks__FP12_modelHeaderi(pointer(u32(self, 1036)), static_cast<int>(u32(self, 244)));
    if (i16(self, 572) == 12)
        objectAddToRunList__FP10GameObject(self);
    else
        objectAddToSlowRunList__FP10GameObject(self);
    finishConstruction__8Creature(self);
}
