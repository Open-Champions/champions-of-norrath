// Host contract for msg_draw__3Orc; not an EE input. The four callees are stubs that append all
// of their arguments to a log. Pointers inside the object are host pointers here, so the
// contract builds a table of marker values and compares the log with the sequence stated in
// msg_draw.cpp for several combinations of the four index words.
#include <cstdio>
#include <cstring>
#include <string>
#include "msg_draw.cpp"
static std::string calls;
static unsigned char markers[400];
static std::string id(const void* p) {
    if (!p) return "null";
    const unsigned char* c = static_cast<const unsigned char*>(p);
    if (c >= markers && c < markers + sizeof markers) return "m" + std::to_string(c - markers);
    return "p";
}
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* t2) {
    calls += "draw(" + id(m) + "," + id(t) + "," + id(t2) + ");"; static unsigned char rec; return &rec;
}
extern "C" void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* pm, void* r, unsigned long f, int b, int c) {
    calls += "att(" + id(m) + "," + id(t) + "," + num(a) + "," + id(pm) + "," + (r ? "rec" : "null") + "," + num(static_cast<double>(f == ~0ul)) + "," + num(b) + "," + num(c) + ");";
}
extern "C" void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short ang, void* m2, void* an, float f1, int a, int b, float f2, Matrix34* mx) {
    (void)d; (void)s; (void)an;
    calls += "fx(" + id(m) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(ang) + "," + id(m2) + "," + num(f1) + "," + num(a) + "," + num(b) + "," + num(f2) + "," + (mx ? "mx" : "null") + ");";
}
int main() {
    alignas(16) static unsigned char o[1200];
    alignas(16) static unsigned char block[64];
    alignas(16) static unsigned char base[400];
    int bad = 0;
    for (int i = 0; i < 400; ++i) markers[i] = static_cast<unsigned char>(i);
    // block: model header marker at +12, texture marker at +20 (host pointers to markers)
    const unsigned char* mh = markers + 1; const unsigned char* tx = markers + 2;
    std::memcpy(block + 12, &mh, sizeof mh); std::memcpy(block + 20, &tx, sizeof tx);
    // Table: the slot at base+4+8*i holds &markers[100+i] and the slot at base+140+8*i holds
    // &markers[200+i]. The slots at +140 and +276 are read by the fixed calls; slot +140 is both
    // header 17 and texture 0, so texture 0 (marker 200) is written last.
    std::memset(base, 0, sizeof base);
    for (int i = 0; i < 18; ++i) {
        const unsigned char* h = markers + 100 + i;
        std::memcpy(base + 4 + 8 * i, &h, sizeof h);
    }
    for (int i = 0; i < 18; ++i) {
        const unsigned char* t = markers + 200 + i;
        std::memcpy(base + 140 + 8 * i, &t, sizeof t);
    }
    struct Case { unsigned idx, j, k4, w1124; short state; };
    const Case cases[] = {{0, 0, 0, 0, 0}, {3, 0, 0, 0, 6}, {0, 5, 0, 0, 1}, {2, 7, 4, 0, 6}, {0, 0, 4, 9, 2}, {1, 1, 4, 1, 6}, {4, 0, 3, 1, 6}};
    for (const Case& c : cases) {
        std::memset(o, 0, sizeof o); calls.clear();
        unsigned char* bp = base; unsigned char* tp = block;
        std::memcpy(o + 36, &tp, sizeof tp);
        std::memcpy(o + 972, &c.idx, 4); std::memcpy(o + 1112, &c.j, 4); std::memcpy(o + 1116, &c.k4, 4); std::memcpy(o + 1124, &c.w1124, 4);
        std::memcpy(o + 1136, &bp, sizeof bp);
        const Point3 p = {1.5f, 2.5f, -3.5f}; std::memcpy(o + 16, &p, 12);
        const short ang = 1234; std::memcpy(o + 56, &ang, 2);
        std::memcpy(o + 572, &c.state, 2);
        const float f2 = 7.25f; std::memcpy(o + 224, &f2, 4);
        msg_draw__3Orc(o);
        auto att = [&](int hi, int ti) { return "att(m" + std::to_string(hi) + ",m" + std::to_string(ti) + ",0,m1,rec,1,-1,0);"; };
        std::string w2;
        if (c.idx) w2 += "fx(m" + std::to_string(100 + c.idx) + ",1.5:2.5:-3.5,1234,m1,0.5," + (c.state == 6 ? "1" : "0") + ",0,7.25,null);";
        w2 += "draw(m1,m2,null);";
        if (c.idx) w2 += att(100 + c.idx, 200 + c.idx);
        if (c.j) w2 += att(100 + c.j, 200 + c.j);
        if (c.k4 == 4) {
            w2 += att(106, 206);
            if (c.w1124) w2 += att(200, 217);
        }
        if (calls != w2) { std::printf("case idx%u j%u k%u w%u\n got  %s\n want %s\n", c.idx, c.j, c.k4, c.w1124, calls.c_str(), w2.c_str()); bad = 1; }
    }
    return bad;
}
