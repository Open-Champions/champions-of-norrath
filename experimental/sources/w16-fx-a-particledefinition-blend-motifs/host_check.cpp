// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 14];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "blend_motifs.cpp"
#include <cmath>
#include <initializer_list>
#include <cstring>
static unsigned motifs[4];
static int motifCount, firstCalls, nextCalls;
extern "C" unsigned FirstMotif__C22VIParticleDefinitionEx(const unsigned char*) { ++firstCalls; return motifCount ? motifs[0] : 0; }
extern "C" unsigned NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(const unsigned char*, unsigned motif) {
    ++nextCalls;
    for (int i = 0; i + 1 < motifCount; ++i) if (motifs[i] == motif) return motifs[i + 1];
    return 0;
}
static float& f(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }
static unsigned& w(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
int main() {
    alignas(8) static unsigned char definition[800], output[800];
    unsigned char* m0 = arena + 0x100;
    unsigned char* m1 = arena + 0x500;
    unsigned char* m2 = arena + 0x900;
    for (int i = 0; i < 724; i += 4) { f(definition, i) = static_cast<float>(i); f(m0, i) = static_cast<float>(i) + 8.0f; f(m1, i) = static_cast<float>(i) - 4.0f; f(m2, i) = 1000.0f; }
    for (int i = 0; i < 724; i += 4) f(output, i) = -1.0f;
    // triples: 612 all zero (flag 0); 624 has only z non-zero (flag 1); 636 only y; 648 only x; 672 all zero
    for (int t : {612, 624, 636, 648, 672}) for (int k = 0; k < 3; ++k) { f(definition, t + 4 * k) = 0.0f; f(m0, t + 4 * k) = 0.0f; f(m1, t + 4 * k) = 0.0f; f(m2, t + 4 * k) = 0.0f; }
    f(definition, 624 + 8) = 2.0f; f(m0, 624 + 8) = 2.0f; f(m1, 624 + 8) = 2.0f; f(m2, 624 + 8) = 2.0f;
    f(definition, 636 + 4) = 3.0f; f(m0, 636 + 4) = 3.0f; f(m1, 636 + 4) = 3.0f; f(m2, 636 + 4) = 3.0f;
    f(definition, 648) = 4.0f; f(m0, 648) = 4.0f; f(m1, 648) = 4.0f; f(m2, 648) = 4.0f;
    f(m0, 712) = 0.5f; f(m1, 712) = 2.0f; f(m2, 712) = 0.0f;
    motifs[0] = 0x100; motifs[1] = 0x500; motifs[2] = 0x900; motifCount = 3;
    BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes(definition, output);
    if (firstCalls != 1 || nextCalls != 3) return 1;
    // field 0: base 0, motif 0 offset +8 at weight 0.5, motif 1 offset -4 at weight 2: 0 + 4 - 8 = -4
    if (f(output, 0) != -4.0f) return 2;
    if (f(output, 400) != 400.0f + 4.0f - 8.0f) return 3;          // the third motif has weight zero and is skipped
    // the weight word at 712 is copied from the definition, not blended; the float at 720 is blended
    if (f(output, 712) != f(definition, 712)) return 4;
    if (f(output, 720) != 720.0f + 4.0f - 8.0f) return 5;
    if (w(output, 684) != 0 || w(output, 688) != 1 || w(output, 692) != 1 || w(output, 696) != 1 || w(output, 700) != 0) return 6;
    // words in 684..719 that are not flags are copied from the definition
    w(definition, 704) = 0x1234; w(definition, 708) = 0x5678; motifCount = 0; firstCalls = 0;
    BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes(definition, output);
    if (w(output, 704) != 0x1234 || w(output, 708) != 0x5678 || f(output, 0) != 0.0f || firstCalls != 1) return 7;
    return 0;
}
