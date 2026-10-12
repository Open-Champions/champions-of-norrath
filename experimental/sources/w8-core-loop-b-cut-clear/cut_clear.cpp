// Observed cutClear__Fv at 0x0018cda8, 224 bytes.
// Resets cutscene state: engineScissory2 = 447, cutMode = 0, engineScissory1 = 0, cutSkipMode = 0; removes the
// two engine tasks cutRunFrameTask__Fv (0x0018e428) and cutDrawFrame__Fv (0x0018c378); sets the far clip to
// 1800.0f; sets the frustum to (50.0f, 1.33f, 80.0f, 1.0f), where 1.33f is the float word 0x3faa3d70 loaded from
// the literal pool at 0x004801c8; sets the global character light scale to 1.0f.
// Then for the two eight-byte slots of anim (0x00790f60): if the first word is nonzero, wait for the CD when
// animationLoading is set (clearing it), free the block and zero both words. Finally thereIsACutPlayer = 0.
// Unknown: the field names of the anim slots (the second word is only ever zeroed here) and what 447 means.
extern "C" int engineScissory1;
extern "C" int engineScissory2;
extern "C" unsigned char cutMode;
extern "C" unsigned char cutSkipMode;
extern "C" unsigned char animationLoading;
extern "C" unsigned char thereIsACutPlayer;
struct CutAnimSlot { void* block; int second; };
extern "C" CutAnimSlot anim[2];
extern "C" void cutRunFrameTask__Fv();
extern "C" void cutDrawFrame__Fv();
extern "C" void engineRemoveTask__FPFv_v(void (*task)());
extern "C" void worldSetFarClip__Ff(float distance);
extern "C" void worldSetFrustum__Fffff(float first, float second, float third, float fourth);
extern "C" void lightGlobalCharLightScale__Ff(float scale);
extern "C" void cdWait__Fv();
extern "C" void memFree__FPv(void* block);

extern "C" void cutClear__Fv() {
    engineScissory2 = 447;
    cutMode = 0;
    cutSkipMode = 0;
    engineRemoveTask__FPFv_v(cutRunFrameTask__Fv);
    engineScissory1 = 0;
    engineRemoveTask__FPFv_v(cutDrawFrame__Fv);
    worldSetFarClip__Ff(1800.0f);
    worldSetFrustum__Fffff(50.0f, 1.33f, 80.0f, 1.0f);
    lightGlobalCharLightScale__Ff(1.0f);
    for (int i = 0; i < 2; ++i) {
        if (anim[i].block) {
            if (animationLoading) {
                animationLoading = 0;
                cdWait__Fv();
            }
            memFree__FPv(anim[i].block);
            anim[i].block = 0;
            anim[i].second = 0;
        }
    }
    thereIsACutPlayer = 0;
}
