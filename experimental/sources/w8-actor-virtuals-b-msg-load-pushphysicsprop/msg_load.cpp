// Observed msg_load__15PushPhysicsPropPc at 0x00150100, 384 bytes.
// Reads the stream as consecutive 32-bit words and returns the 68 bytes consumed:
//   +0   float, stored at object+256 (call it f; stored before any call);
//   +4   +8  +12   each passed to objectSaveGetFromStringPool; results stored at
//                  object+268, +272, +276 in that order;
//   then lumpFindResource(object[268], object[272]) -> object+260, and
//        lumpFindResource(object[268], object[276]) -> object+264;
//   object+32 = f + f; object+28 = f * 1.19999992847 (bits 0x3f999999, read from the
//   shared float constant at 0x0047fdb0);
//   bodyInit(object+80, 1.0, 1.0, f+f, f+f, f+f) -- the body record sits at object+80;
//   +16 .. +64 (13 floats) stored at object+96, +100, ... +144;
//   bodyUpdateMatrix(object+80).
// The float arguments to bodyInit beyond the first two are all 2*f in the image.
// Unknown: what the stream floats mean; the body record layout.

extern "C" {
unsigned objectSaveGetFromStringPool__Fi(int index);
unsigned lumpFindResource__FPcT0(unsigned name, unsigned kind);
void bodyInit__FP5_bodyfffff(void* body, float a, float b, float c, float d, float e);
void bodyUpdateMatrix__FP5_body(void* body);
}

namespace {
typedef int Word __attribute__((may_alias));
typedef unsigned UWord __attribute__((may_alias));
typedef float FloatWord __attribute__((may_alias));
inline float bits_to_float(unsigned bits) {
    union {
        unsigned bits;
        float value;
    } converted;
    converted.bits = bits;
    return converted.value;
}
}

extern "C" int msg_load__15PushPhysicsPropPc(void* self, const char* stream) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* in = reinterpret_cast<const unsigned char*>(stream);
    const float f = *reinterpret_cast<const FloatWord*>(in);
    *reinterpret_cast<FloatWord*>(object + 256) = f;
    *reinterpret_cast<UWord*>(object + 268) = objectSaveGetFromStringPool__Fi(*reinterpret_cast<const Word*>(in + 4));
    *reinterpret_cast<UWord*>(object + 272) = objectSaveGetFromStringPool__Fi(*reinterpret_cast<const Word*>(in + 8));
    *reinterpret_cast<UWord*>(object + 276) = objectSaveGetFromStringPool__Fi(*reinterpret_cast<const Word*>(in + 12));
    *reinterpret_cast<UWord*>(object + 260) =
        lumpFindResource__FPcT0(*reinterpret_cast<UWord*>(object + 268), *reinterpret_cast<UWord*>(object + 272));
    *reinterpret_cast<UWord*>(object + 264) =
        lumpFindResource__FPcT0(*reinterpret_cast<UWord*>(object + 268), *reinterpret_cast<UWord*>(object + 276));
    const float stored = *reinterpret_cast<const FloatWord*>(object + 256);
    const float doubled = stored + stored;
    *reinterpret_cast<FloatWord*>(object + 32) = doubled;
    *reinterpret_cast<FloatWord*>(object + 28) = stored * bits_to_float(0x3f999999u);
    bodyInit__FP5_bodyfffff(object + 80, 1.0f, 1.0f, doubled, doubled, doubled);
    for (unsigned i = 0; i < 13u; ++i)
        *reinterpret_cast<FloatWord*>(object + 96 + 4 * i) = *reinterpret_cast<const FloatWord*>(in + 16 + 4 * i);
    bodyUpdateMatrix__FP5_body(object + 80);
    return 68;
}
