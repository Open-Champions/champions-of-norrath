// Observed setState__3NPCi at 0x002326b8, 348 bytes. "self" offsets are bytes; the state is the
// halfword at +572. A word copy of the argument is passed to setStateProcessing__8CreaturePi,
// which may change it; a non-zero result ends the call with nothing else done. Afterwards, with
// n the (possibly changed) word:
//   n == 1: getSmartRandomDestination(result, self, 200) fills a 12-byte point that is copied to
//     +272. When the current state differs from 1, the resource "walk.anm" is looked up in the
//     lump named at +960, added as a transition to the animation state at +288 (time 0.1, flag
//     0x803) and animGlobalSync runs on that state.
//   n == 0: a header is chosen. When the word at +1136 is non-zero and the text at +1068 equals
//     "firlow" (case-insensitive), the resource "idle1.anm" is looked up in the lump at +960.
//     When there is no header yet, objectFindAnimation(self, "idle") supplies one. A header, if
//     present, is added as a transition (time 0.1, flag 3).
//   any other n: nothing further.
// In every case that gets this far the low halfword of the word is stored at +572. The
// meaning of the states is unknown. Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
int strcasecmp(const char* a, const char* b);
void* lumpFindResource__FPcT0(char* lump, char* name);
void* objectFindAnimation__FP10GameObjectPc(void* object, char* name);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flag);
void animGlobalSync__FP14AnimationState(void* state);

void setState__3NPCi(void* self, int state) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    int word = state;
    if (setStateProcessing__8CreaturePi(self, &word) != 0) return;
    const int n = word;
    if (n == 1) {
        float point[3];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        float* const target = reinterpret_cast<float*>(o + 272);
        target[0] = point[0]; target[1] = point[1]; target[2] = point[2];
        if (*reinterpret_cast<short*>(o + 572) != n) {
            void* const walk = lumpFindResource__FPcT0(reinterpret_cast<char*>(o + 960), const_cast<char*>("walk.anm"));
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, walk, 0.1f, 0x803);
            animGlobalSync__FP14AnimationState(o + 288);
        }
    } else if (n == 0) {
        void* header = nullptr;
        if (*reinterpret_cast<int*>(o + 1136) != 0 && strcasecmp(reinterpret_cast<const char*>(o + 1068), "firlow") == 0) {
            header = lumpFindResource__FPcT0(reinterpret_cast<char*>(o + 960), const_cast<char*>("idle1.anm"));
        }
        if (header == nullptr) header = objectFindAnimation__FP10GameObjectPc(self, const_cast<char*>("idle"));
        if (header != nullptr) animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, header, 0.1f, 3);
    }
    *reinterpret_cast<short*>(o + 572) = static_cast<short>(word);
}
}
