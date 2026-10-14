#include "char_light_state.cpp"
template <typename T, unsigned N> static bool zero(const T (&a)[N]) {
    for (unsigned i = 0; i < N; ++i) if (a[i]) return false;
    return true;
}
int main() {
    if (sizeof(charAmbient) != 8 || sizeof(charDirect) != 8 || sizeof(charDirect2) != 8) return 1;
    if (sizeof(charDirectDir) != 16 || sizeof(charDirectDir2) != 16) return 2;
    if (sizeof(frameStatePacket) != 0x600 || sizeof(commandQueue) != 0x1000) return 3;
    if (sizeof(charScaleR) != 4 || sizeof(charScaleG) != 4 || sizeof(charScaleB) != 4) return 4;
    if (!zero(charAmbient) || !zero(charDirect) || !zero(charDirect2) || !zero(charDirectDir) || !zero(charDirectDir2)) return 5;
    if (!zero(frameStatePacket) || !zero(commandQueue)) return 6;
    if (charScaleR != 0.0f || charScaleG != 0.0f || charScaleB != 0.0f) return 7;
    return 0;
}
