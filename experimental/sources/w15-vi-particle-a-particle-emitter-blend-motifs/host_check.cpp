// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "blend_motifs.cpp"
#include <cstring>
#include <string>
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
static std::string tr; static int motifs_left;
static unsigned char* mot(int i) { return arena + 0x1000 + 0x400 * i; }
extern "C" void* FirstMotif__C22VIParticleDefinitionEx(void* d) { tr += "F" + std::to_string((unsigned char*)d - arena) + ";"; return motifs_left > 0 ? mot(0) : nullptr; }
extern "C" void* NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(void*, void* m) {
    int i = ((unsigned char*)m - mot(0)) / 0x400; tr += "N" + std::to_string(i) + ";"; return i + 1 < motifs_left ? mot(i + 1) : nullptr; }
extern "C" void BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes(void* d, void* a) { tr += "B" + std::to_string((unsigned char*)d - arena) + "," + std::to_string((unsigned char*)a - arena) + ";"; }
extern "C" void RecomputeStaticBBox__17VIParticleEmitter(void* e) { tr += "R" + std::to_string((unsigned char*)e - arena) + ";"; }
static void put(unsigned char* b, int off, unsigned v) { memcpy(b + off, &v, 4); }
static void putf(unsigned char* b, int off, float v) { memcpy(b + off, &v, 4); }
static unsigned get(unsigned char* b, int off) { unsigned v; memcpy(&v, b + off, 4); return v; }
int main() {
    unsigned char* e = arena + 0x200;       // emitter
    unsigned char* def = arena + 0x600;
    unsigned char* n0 = arena + 0x800; unsigned char* n1 = arena + 0x840; unsigned char* sent = arena + 0x880;
    memset(arena, 0x33, sizeof arena);
    put(e, 84, 0x600); put(e, 176, 0x800); put(e, 912, 0);
    put(def, 716, 5);
    put(n0, 0, 0x840); put(n0, 16, 0x1000); putf(n0, 20, 2.0f); put(n0, 24, 1);
    put(n1, 0, 0x880); put(n1, 16, 0x1400); putf(n1, 20, 0.0f); put(n1, 24, 0);
    put(sent, 0, 0); put(sent, 16, 0x1800); putf(sent, 20, 9.0f); put(sent, 24, 1);
    put(mot(0), 716, 7); put(mot(1), 716, 11); put(mot(2), 716, 13);
    motifs_left = 2;
    BlendMotifs__17VIParticleEmitter(e);
    // motifs 0 and 1 cleared first, then node weights stored; total = 7 (node 0) + 5 = 12
    if (tr != "F1536;N0;N1;B1536,700;R512;") return 1;
    if (get(e, 912) != 12) return 2;
    if (get(n0, 24) != 0 || get(sent, 24) != 1) return 3;
    if (get(mot(0), 712) != 0x40000000u || get(mot(1), 712) != 0) return 4;
    if (get(mot(2), 712) != 0x33333333u) return 5;   // sentinel node not processed
    tr.clear();
    BlendMotifs__17VIParticleEmitter(e);            // nothing changed and the total matches
    if (tr != "F1536;N0;N1;") return 6;
    putf(n1, 20, 3.0f); put(mot(1), 716, 20);       // total becomes 7 + 20 + 5 = 32
    tr.clear();
    BlendMotifs__17VIParticleEmitter(e);
    if (tr != "F1536;N0;N1;B1536,700;R512;" || get(e, 912) != 32) return 7;
    put(n0, 24, 4); tr.clear();                     // flag alone forces the blend
    BlendMotifs__17VIParticleEmitter(e);
    if (tr != "F1536;N0;N1;B1536,700;R512;" || get(n0, 24) != 0) return 8;
    return 0;
}
