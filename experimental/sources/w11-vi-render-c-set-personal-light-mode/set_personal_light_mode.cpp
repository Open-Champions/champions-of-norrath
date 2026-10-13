// Observed SetPersonalLightMode__7VIScene19VIPersonalLightMode at 0x011450e8, 12 bytes. The mode
// argument is stored as a 32-bit word at offset 10772 of the scene and the result is zero.
// The meaning of the mode values is not established here.
extern "C" {
struct VIScene;
int SetPersonalLightMode__7VIScene19VIPersonalLightMode(VIScene* scene, int mode) {
    *reinterpret_cast<int*>(reinterpret_cast<char*>(scene) + 10772) = mode;
    return 0;
}
}
