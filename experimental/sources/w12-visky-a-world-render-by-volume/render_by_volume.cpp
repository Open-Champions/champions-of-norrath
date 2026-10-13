// Observed: RenderByVolume__7VIWorldR15VISceneRendVars at 0x010fe5b0 is 112 bytes. With the world
// as this and the render-variables record as the second argument it copies this's word at byte
// offset 68 into word 0 of the record, then calls RenderByVolume__7VIWorldiR15VISceneRendVars(this,
// m, record) where m is 0 if this's word at +52 is nonzero and 0x40000000 if it is zero. A negative
// result is returned as -1. Otherwise the word at +68 is re-read after the call, incremented
// by one and stored, and 0 is returned.
// Inferred: +68 is a per-frame or per-pass counter published to the render variables before the
// call; m is a mode or flag word. Unknown: member names and the meaning of m.
extern "C" {
int RenderByVolume__7VIWorldiR15VISceneRendVars(unsigned char* self, int mode, unsigned char* vars);

int RenderByVolume__7VIWorldR15VISceneRendVars(unsigned char* self, unsigned char* vars) {
    *reinterpret_cast<unsigned*>(vars + 0) = *reinterpret_cast<const unsigned*>(self + 68);
    const int mode = *reinterpret_cast<const int*>(self + 52) != 0 ? 0 : 0x40000000;
    if (RenderByVolume__7VIWorldiR15VISceneRendVars(self, mode, vars) < 0) return -1;
    *reinterpret_cast<int*>(self + 68) = *reinterpret_cast<const int*>(self + 68) + 1;
    return 0;
}
}
