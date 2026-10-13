// Observed: Init__13VIBattleMusicP7VIScene at 0x0106de60 is 52 bytes. With the battle-music
// object as this and the scene pointer as the argument it stores: the scene pointer at byte
// offset 0, -1 at +4, 0 at +8, 0 at +12, the float 0.01 (bit pattern 0x3c23d70a) at +16, -1 at +24
// and -1 at +28. Bytes +20 and everything after +32 are untouched. It returns 0.
// Inferred: -1 marks "no track / no handle yet" in the words at +4, +24 and +28.
// Unknown: member names and what the float at +16 controls.
extern "C" {
int Init__13VIBattleMusicP7VIScene(unsigned char* self, unsigned char* scene) {
    *reinterpret_cast<unsigned*>(self + 0) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(scene));
    *reinterpret_cast<int*>(self + 4) = -1;
    *reinterpret_cast<int*>(self + 8) = 0;
    *reinterpret_cast<int*>(self + 12) = 0;
    *reinterpret_cast<unsigned*>(self + 16) = 0x3c23d70au;
    *reinterpret_cast<int*>(self + 24) = -1;
    *reinterpret_cast<int*>(self + 28) = -1;
    return 0;
}
}
