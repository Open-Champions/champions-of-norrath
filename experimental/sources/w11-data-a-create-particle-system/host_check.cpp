// Host contract for P_CreateParticleSystem__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char PS[16 * 312];
int psSequenceNm = 5;
int P_CreateParticleSystem__Fv();
static void* last_matrix;
void matIdent__FP8Matrix34(void* m) { last_matrix = m; }
}

static int fail(const char* what) { std::fprintf(stderr, "fail: %s\n", what); return 1; }
static int word(unsigned rec, unsigned off) { int v; std::memcpy(&v, PS + rec * 312 + off, 4); return v; }
static void set(unsigned rec, unsigned off, int v) { std::memcpy(PS + rec * 312 + off, &v, 4); }

int main() {
    set(0, 304, 9);
    set(1, 308, 77);
    const int id = P_CreateParticleSystem__Fv();
    if (id != ((5 << 8) | 1) || word(1, 304) != id) return fail("id");
    if (psSequenceNm != 6) return fail("sequence");
    if (word(1, 308) != 0) return fail("spare");
    if (last_matrix != PS + 312 + 256) return fail("matrix");
    for (unsigned i = 0; i < 16; ++i) set(i, 304, 1);
    last_matrix = nullptr;
    if (P_CreateParticleSystem__Fv() != 0 || psSequenceNm != 6 || last_matrix) return fail("full");
    set(15, 304, 0);
    if (P_CreateParticleSystem__Fv() != ((6 << 8) | 15)) return fail("last");
    return 0;
}
