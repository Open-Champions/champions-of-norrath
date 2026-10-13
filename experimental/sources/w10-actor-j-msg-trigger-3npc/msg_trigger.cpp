// Observed msg_trigger__3NPCi at 0x002325b0, 264 bytes. "name" is the text at object+1068.
// Case-insensitive comparisons are against the lowercase texts "sylea", "firlow" and "hate3".
//  1. When the byte cutMode (0x00484318) is non-zero and name is "sylea": the word at +244
//     becomes 2 for a non-zero argument and 0 otherwise, the 64-bit result of
//     modelGetActiveChunks(model at +1056, word +244) is stored at +552, and the call ends.
//  2. Otherwise, when name is "firlow" and the text at levelName+81 is "hate3": the word at
//     +1136 becomes 1, the resource "idle1.anm" is looked up in the lump named at +960, and
//     it is added as a transition to the animation state at +288 with time 0.1 and flag 3.
//  3. Anything else is handed to the Creature routine with the same argument.
// What the texts and the cut-mode byte mean is unknown. Nothing here ran on the EE.
extern "C" {
extern signed char cutMode;
extern char levelName[];
int strcasecmp(const char* a, const char* b);
long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);
void* lumpFindResource__FPcT0(char* lump, char* name);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flag);
int msg_trigger__8Creaturei(void* self, int value);

void msg_trigger__3NPCi(void* self, int value) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const char* const name = reinterpret_cast<const char*>(o + 1068);
    if (cutMode != 0 && strcasecmp(name, "sylea") == 0) {
        *reinterpret_cast<int*>(o + 244) = (value != 0) ? 2 : 0;
        *reinterpret_cast<long long*>(o + 552) =
            modelGetActiveChunks__FP12_modelHeaderi(*reinterpret_cast<void**>(o + 1056), *reinterpret_cast<int*>(o + 244));
        return;
    }
    if (strcasecmp(name, "firlow") == 0 && strcasecmp(levelName + 81, "hate3") == 0) {
        *reinterpret_cast<int*>(o + 1136) = 1;
        void* const resource = lumpFindResource__FPcT0(reinterpret_cast<char*>(o + 960), const_cast<char*>("idle1.anm"));
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, resource, 0.1f, 3);
        return;
    }
    msg_trigger__8Creaturei(self, value);
}
}
