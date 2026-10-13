// Host contract for the three-name Prop constructor; not an EE input. Every callee is a stub that
// logs; the bounding-box stub fills the six integers from a table so the centre arithmetic,
// the shift rounding of negative sums and the flag-4 branch can be checked against a recomputation.
#include "ctor.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char vt_Prop[8];
static std::string calls; static unsigned char* the_object; static int box[6]; static void* lump_result[2]; static int lump_n;
void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls += "go(" + std::string(self == the_object ? "self" : "other") + "," + std::to_string(p.x) + "," + std::to_string(reinterpret_cast<unsigned long>(map)) + "," + std::to_string(reinterpret_cast<unsigned long>(names)) + ");";
}
extern "C" char* strcpy(char* to, const char* from) {
    calls += "strcpy(" + std::to_string(to - reinterpret_cast<char*>(the_object)) + "," + from + ");";
    std::memcpy(to, from, std::strlen(from) + 1); return to;
}
void* lumpFindResource__FPcT0(char* a, char* b) {
    calls += "lump(" + std::to_string(a - reinterpret_cast<char*>(the_object)) + "," + std::to_string(b - reinterpret_cast<char*>(the_object)) + ");";
    return lump_result[(lump_n++) & 1];
}
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* h, long long l, int* a, int* b, int* c, int* d, int* e, int* f) {
    calls += "bbox(" + std::string(h == lump_result[0] ? "first" : "other") + "," + std::to_string(l) + ");";
    *a = box[0]; *b = box[1]; *c = box[2]; *d = box[3]; *e = box[4]; *f = box[5];
}
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void init__4Propi(void*, int f) { calls += "init" + std::to_string(f) + ";"; }
int main() {
    alignas(16) static unsigned char o[512];
    char n1[] = "lump", n2[] = "mdl", n3[] = "tex";
    int bad = 0;
    the_object = o;
    lump_result[0] = reinterpret_cast<void*>(0x1000); lump_result[1] = reinterpret_cast<void*>(0x2000);
    const int boxes[][6] = {{-10, -20, -30, 10, 20, 30}, {-7, -5, -3, 2, 4, 6}, {1, 2, 3, 4, 5, 6}, {-9, -9, -9, 0, 0, 0}};
    for (const auto& bx : boxes) for (int flags : {0, 4, 5, 3, 7}) {
        std::memcpy(box, bx, sizeof box);
        std::memset(o, 0x11, sizeof o); calls.clear(); lump_n = 0;
        float pos[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(o + 16, pos, 12);
        const unsigned seed = 0x12345678u; std::memcpy(o + 68, &seed, 4);
        const long long big = -0x123456789abcdefLL;
        construct(o, Point3{9.0f, 8.0f, 7.0f}, n1, n2, n3, big, flags);
        std::string want = "go(self,9.000000,0,0);strcpy(84,lump);strcpy(100,mdl);strcpy(164,tex);lump(84,100);lump(84,164);bbox(first," + std::to_string(big) + ");";
        if (flags & 4) want += "grid;";
        want += "init" + std::to_string(flags) + ";";
        if (calls != want) { if (!bad) std::printf("calls %s\nwant  %s\n", calls.c_str(), want.c_str()); bad = 1; }
        const unsigned w = seed * 0x19660du + 0x3c6ef35fu;
        unsigned w68; unsigned short h56; long long q232; float f48, f252, f256, f240, f244, f248, f264, f16, f20, f24; short h268; unsigned p76; unsigned p80;
        std::memcpy(&w68, o + 68, 4); std::memcpy(&h56, o + 56, 2); std::memcpy(&q232, o + 232, 8); std::memcpy(&f48, o + 48, 4);
        std::memcpy(&f252, o + 252, 4); std::memcpy(&f256, o + 256, 4); std::memcpy(&f240, o + 240, 4); std::memcpy(&f244, o + 244, 4);
        std::memcpy(&f248, o + 248, 4); std::memcpy(&f264, o + 264, 4); std::memcpy(&h268, o + 268, 2); std::memcpy(&p76, o + 76, sizeof p76); std::memcpy(&p80, o + 80, sizeof p80);
        std::memcpy(&f16, o + 16, 4); std::memcpy(&f20, o + 20, 4); std::memcpy(&f24, o + 24, 4);
        const float cx = float((bx[0] + bx[3]) >> 1), cy = float((bx[1] + bx[4]) >> 1), cz = float((bx[2] + bx[5]) >> 1);
        if (w68 != w || h56 != (w >> 16) || q232 != big || f48 != 0 || f252 != 0 || f256 != 0 || h268 != 0) bad = 1;
        if (f240 != cx || f244 != cy || f248 != cz || f264 != cz - float(bx[2])) bad = 1;
        if (p76 != 0x1000u || p80 != 0x2000u) bad = 1;
        if (f16 != 1.0f + ((flags & 4) ? cx : 0) || f20 != 2.0f + ((flags & 4) ? cy : 0) || f24 != 3.0f + ((flags & 4) ? cz : 0)) bad = 1;
    }
    return bad;
}
