// Observed inventoryUnpack__Fiib at 0x001b10e8, 288 bytes (int player, int screen,
// bool skipShop). `inventory` (0x00427288) and `statsScreen` (0x004281a8) are
// arrays of 116-byte screen records; animState is an array of 264-byte
// AnimationState records indexed by player. In order:
//   1. animInitState__FP14AnimationState(&animState[player]);
//      inventorySetAnim__FiP14AnimationStatei(player, &animState[player], 1);
//      animGlobalSync__FP14AnimationState(&animState[player]).
//   2. In inventory[screen]: word +0 = screen * 320; word +52 and word +56 =
//      player; float +68 = (float)(screen * 320) + 100.0f; float +72 = 200.0f;
//      word +76 = 0 (the mode word that inventoryDraw tests against 2).
//   3. The word at +52 of statsScreen[screen] = player.
//   4. skipShop false: shopUnpackToGrid__Fiii(player, screen, 0). The call is the
//      last action, and nothing is returned.
// Unknown: what the stored fields mean beyond their use elsewhere (+52 is the
// owning player, +76 a mode).
extern "C" {
extern unsigned char inventory[] __attribute__((aligned(4)));
extern unsigned char statsScreen[];
extern unsigned char animState[];
void animInitState__FP14AnimationState(void*);
void animGlobalSync__FP14AnimationState(void*);
void inventorySetAnim__FiP14AnimationStatei(int, void*, int);
void shopUnpackToGrid__Fiii(int, int, int);

void inventoryUnpack__Fiib(int owner, int screen, bool skipShop) {
    void* state = animState + owner * 264;
    animInitState__FP14AnimationState(state);
    inventorySetAnim__FiP14AnimationStatei(owner, state, 1);
    animGlobalSync__FP14AnimationState(state);
    unsigned char* s = inventory + screen * 116;
    const int x = screen * 320;
    *reinterpret_cast<int*>(s) = x;
    *reinterpret_cast<int*>(s + 52) = owner;
    *reinterpret_cast<int*>(s + 56) = owner;
    *reinterpret_cast<float*>(s + 68) = static_cast<float>(x) + 100.0f;
    *reinterpret_cast<float*>(s + 72) = 200.0f;
    *reinterpret_cast<int*>(s + 76) = 0;
    *reinterpret_cast<int*>(statsScreen + screen * 116 + 52) = owner;
    if (!skipShop) shopUnpackToGrid__Fiii(owner, screen, 0);
}
}
