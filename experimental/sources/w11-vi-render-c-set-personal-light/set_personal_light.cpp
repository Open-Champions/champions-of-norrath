// Observed SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F at 0x011450f8, 72 bytes. Three
// single-precision words are copied from the first reference argument to offsets 10776, 10780
// and 10784 of the scene; the float argument is stored at 10788; sixteen bytes are copied from
// the last reference argument to offset 10792 (read with unaligned loads, so the source needs
// no alignment). The result is zero.
// Inferred: the sixteen-byte object is four floats, as the name VIColor32F suggests; the copy
// does not look inside it. What the values mean is unknown.
extern "C" {
struct VIScene;
struct VIVect3 { float x, y, z; };
struct VIColor32F { unsigned char bytes[16]; };

int SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F(VIScene* scene, const VIVect3* position, float value,
                                                        const VIColor32F* color) {
    char* base = reinterpret_cast<char*>(scene);
    float* out = reinterpret_cast<float*>(base + 10776);
    out[0] = position->x;
    out[1] = position->y;
    out[2] = position->z;
    *reinterpret_cast<float*>(base + 10788) = value;
    __builtin_memcpy(base + 10792, color, 16);
    return 0;
}
}
