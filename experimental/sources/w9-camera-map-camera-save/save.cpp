// Observed Save__6CameraPc at 0x0026c4d0, 264 bytes.
// Serializes camera state into the byte buffer `out`, writing sequentially, and
// returns the number of bytes written (196). In order: the three floats of
// worldCameraPos (0x0037ee80); sixteen floats read from worldCameraMatrix
// (0x0037ef00) as four groups of four whose starting float index advances by
// three (groups start at float 0, 3, 6 and 9, so neighbouring groups overlap
// and the last float read is index 12, past the 12 floats the symbol is known
// to hold); the halfwords at camera+104 and camera+106; the words at camera+84
// and camera+88; the floats at camera+144, +212 and +140; and the 96 bytes
// starting at camera+256. Nothing in the camera is modified. Why the matrix is
// read this way is not established. Nothing here was executed on the EE.
extern "C" {
extern float worldCameraPos[3];
extern float worldCameraMatrix[16];
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" int Save__6CameraPc(const unsigned char* camera, unsigned char* out) {
    unsigned char* p = out;
    const float* pos = worldCameraPos;
    for (int i = 0; i < 3; ++i) {
        *reinterpret_cast<AliasedFloat*>(p) = pos[i];
        p += 4;
    }
    const float* mat = worldCameraMatrix;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            *reinterpret_cast<AliasedFloat*>(p) = mat[row * 3 + col];
            p += 4;
        }
    }
    *reinterpret_cast<AliasedShort*>(p) = *reinterpret_cast<const AliasedShort*>(camera + 104);
    p += 2;
    const float zoom_blend = *reinterpret_cast<const AliasedFloat*>(camera + 144);
    *reinterpret_cast<AliasedShort*>(p) = *reinterpret_cast<const AliasedShort*>(camera + 106);
    p += 2;
    *reinterpret_cast<AliasedInt*>(p) = *reinterpret_cast<const AliasedInt*>(camera + 84);
    p += 4;
    *reinterpret_cast<AliasedInt*>(p) = *reinterpret_cast<const AliasedInt*>(camera + 88);
    p += 4;
    *reinterpret_cast<AliasedFloat*>(p) = zoom_blend;
    p += 4;
    *reinterpret_cast<AliasedFloat*>(p) = *reinterpret_cast<const AliasedFloat*>(camera + 212);
    p += 4;
    *reinterpret_cast<AliasedFloat*>(p) = *reinterpret_cast<const AliasedFloat*>(camera + 140);
    p += 4;
    for (int i = 0; i < 96; ++i) *p++ = camera[256 + i];
    return static_cast<int>(p - out);
}
