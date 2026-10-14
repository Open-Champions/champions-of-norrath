// Observed fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii at 0x001df560, 36 bytes. By its mangled name the
// arguments are a ball-lightning state, two game objects, a position (by value, so passed by address), a float and two integers. The
// image reserves 816 bytes of frame, copies the 12-byte position onto the stack and returns. It writes no other memory, calls nothing
// and leaves no result. The stack copy is dead and is not reproduced. Unknown: what the state would hold, and why the body is empty.
// Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" void fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii(void*, void*, void*, Point3, float, int, int) {}
