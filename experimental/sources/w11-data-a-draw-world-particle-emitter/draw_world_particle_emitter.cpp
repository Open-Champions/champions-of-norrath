// drawWorldParticleEmitter__FP13PolysetHeader at 0x0017c930, 268 bytes, read
// from the pinned SLUS-20565 image.
// Observed, with p the argument and offsets in bytes:
//  - the position is the three words at p+40, +44 and +48, each converted from
//    int to float and multiplied by 0.25;
//  - the emitter data is the pointer in the word at p. Its word at +56 is set
//    from p+32 and its byte at +60 from the top bit of the word at p+4;
//  - the emitter record index is the low 31 bits of the word at p+4. The
//    definition handed over is ParticleEmitterArray + index * 152 + 72;
//  - drawParticleEmitter is called with the position, that definition, the
//    emitter data, frameNm, a zero angle, and bits 16, 17 and 18 of the word at
//    p+52 as three flags;
//  - afterwards p+32 receives the data word at +56, and the top bit of p+4 is
//    set when the signed byte at data+60 is nonzero and cleared otherwise.
// Fresh compilation is not an original-byte match.
extern "C" {
extern unsigned char ParticleEmitterArray[];
extern int frameNm;
}

struct Point3 {
    float x, y, z;
};

extern "C" void drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
    Point3 position, void* definition, void* data, int frame, short angle, bool a, bool b, bool c);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
int& wordAt(void* base, unsigned offset) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(base) + offset);
}
}

extern "C" void drawWorldParticleEmitter__FP13PolysetHeader(void* header) {
    Point3 position;
    position.x = static_cast<float>(wordAt(header, 40)) * 0.25f;
    position.y = static_cast<float>(wordAt(header, 44)) * 0.25f;
    position.z = static_cast<float>(wordAt(header, 48)) * 0.25f;
#if defined(NORRATH_HOST_CONTRACT)
    unsigned char* data = static_cast<unsigned char*>(norrath_host_pointer(static_cast<unsigned>(wordAt(header, 0))));
#else
    unsigned char* data = reinterpret_cast<unsigned char*>(wordAt(header, 0));
#endif
    wordAt(data, 56) = wordAt(header, 32);
    reinterpret_cast<signed char*>(data)[60] = static_cast<signed char>(static_cast<unsigned>(wordAt(header, 4)) >> 31);
    const int flags = wordAt(header, 52);
    const int index = wordAt(header, 4) & 0x7fffffff;
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
        position, ParticleEmitterArray + index * 152 + 72, data, frameNm, 0,
        ((flags >> 16) & 1) != 0, ((flags >> 17) & 1) != 0, ((flags >> 18) & 1) != 0);
    wordAt(header, 32) = wordAt(data, 56);
    if (reinterpret_cast<signed char*>(data)[60] != 0)
        wordAt(header, 4) = wordAt(header, 4) | static_cast<int>(0x80000000u);
    else
        wordAt(header, 4) = wordAt(header, 4) & 0x7fffffff;
}
