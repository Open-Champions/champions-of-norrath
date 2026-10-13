// Observed fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii at 0x001df560, 36 bytes.
// Arguments by the mangled name: a ball-lightning state, two game objects, a position (by value), a float and
// two ints. The image reserves 816 bytes of frame, copies the 12-byte position from its hidden pointer onto the
// stack and returns; no other memory is written, nothing is called and no result is produced. Nothing here ran
// on the EE (and the stack copy is not reproduced: it is dead).
struct Point3 { float x, y, z; };

extern "C" void fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii(void*, void*, void*, Point3, float, int, int) {}
