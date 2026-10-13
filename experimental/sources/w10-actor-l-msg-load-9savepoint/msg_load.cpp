// Observed msg_load__9SavepointPc at 0x0021c3d0, 204 bytes. Reads a 32-bit word and two bytes from
// the buffer and returns 6. In order: the word goes to object+128. When it is nonzero: the member
// at +136 is activated (lightActivate__FP5Light), the byte at +196 is set to 1, the model
// address found through the group word at +36 (its word at +12) is queried with
// modelGetActiveChunks(model, 1), the byte at +80 is cleared, the 64-bit mask is stored at +120,
// and the animation "float.anm" in the lump "savepnt.lmp" is added to the state at +76 with
// animAdd(state, header, 3). Whether or not it was nonzero, the object is then added to the run
// list; the first buffer byte (signed) goes to the byte at +197 and the second to +198; and when
// the word at +128 is nonzero the byte at +197 is set to 1 afterwards. Unknown: what the fields
// mean. Nothing here ran on the EE.
#include "../w10-actor-l-common/access.hpp"
extern "C" {
void lightActivate__FP5Light(void* light);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);
char* lumpFindResource__FPcT0(char* lump, char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int count);
void objectAddToRunList__FP10GameObject(void* object);

int msg_load__9SavepointPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int word = *reinterpret_cast<int*>(buffer);
    w10l::sword(o, 128) = word;
    if (word != 0) {
        lightActivate__FP5Light(o + 136);
        o[196] = 1;
        const unsigned long long chunks = modelGetActiveChunks__FP12_modelHeaderi(
            w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12)), 1);
        o[80] = 0;
        w10l::dword(o, 120) = chunks;
        animAdd__FP14AnimationStateP15AnimationHeaderi(
            o + 76, lumpFindResource__FPcT0(const_cast<char*>("savepnt.lmp"), const_cast<char*>("float.anm")), 3);
    }
    objectAddToRunList__FP10GameObject(self);
    o[197] = static_cast<unsigned char>(buffer[4]);
    o[198] = static_cast<unsigned char>(buffer[5]);
    if (w10l::sword(o, 128) != 0) o[197] = 1;
    return 6;
}
}
