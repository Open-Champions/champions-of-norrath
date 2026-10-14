// Host contract for the Critter constructor; not an EE input. Stubs record every call as text; the expected text is built
// from a table of the nine resource tags, independently of the source's call structure.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#define sprintf w12_sprintf
#define sscanf w12_sscanf
#define strcpy w12_strcpy
#include "ctor.cpp"
#undef sprintf
#undef sscanf
#undef strcpy
#include <cstdarg>
#include <cstdio>
#include <set>
#include <map>
using namespace w11_host;
unsigned char _ctype_[300];
int frameNm;
unsigned char vt_Critter[8];
static unsigned char* obj;
static std::string& log() { return calls(); }
static std::map<std::string, std::string> tags;
static std::set<std::string> missing;
static std::map<std::string, int> ints;
static unsigned rolls[1];
static std::string id(const void* p) { return std::to_string(p == obj); }
extern "C" unsigned eRand__10GameObject(void* o) { log() += "rand(" + id(o) + ");"; return rolls[0]; }
extern "C" void creature_construct(void* self, Point3 p, short type, void* map, float scale, char** args) {
    char b[160];
    std::snprintf(b, sizeof b, "creature(%s,%.2f,%.2f,%.2f,%d,%d,%.1f,%d);", id(self).c_str(), p.x, p.y, p.z, type, map == nullptr, scale, args == reinterpret_cast<char**>(at(0x700)));
    log() += b;
    std::memset(self, 0x21, 0x480);
    w11::u32(self, 12) = 0x100;
    w11::f32(self, 788) = 7.5f;
    w11::u32(self, 980) = 0;
}
extern "C" void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int f) {
    char t[96]; std::snprintf(t, sizeof t, "setp(%s,%g,%g,%g,%g,%g,%d);", id(self).c_str(), a, b, c, d, e, f); log() += t;
}
extern "C" char* w12_strcpy(char* to, const char* from) { log() += std::string("strcpy(") + from + ");"; std::strcpy(to, from); return to; }
extern "C" char* findTagString__10GameObjectPci(void* self, char* tag, int occurrence) {
    log() += "tag(" + id(self) + "," + tag + "," + std::to_string(occurrence) + ");";
    static char store[64][64]; static int n;
    auto it = tags.find(tag);
    if (it == tags.end()) return nullptr;
    char* d = store[n++ % 64];
    std::strcpy(d, it->second.c_str());
    return d;
}
extern "C" int findTagInt__10GameObjectPci(void* self, char* tag, int occurrence) { log() += "int(" + id(self) + "," + tag + "," + std::to_string(occurrence) + ");"; return ints[tag]; }
extern "C" int w12_sprintf(char* out, const char* format, ...) {
    va_list ap; va_start(ap, format);
    const int n = std::vsnprintf(out, 128, format, ap);
    va_end(ap);
    log() += std::string("fmt(") + format + "=" + out + ");";
    return n;
}
extern "C" int w12_sscanf(const char* text, const char* format, ...) {
    va_list ap; va_start(ap, format);
    float* f = va_arg(ap, float*);
    va_end(ap);
    log() += std::string("scan(") + text + "," + format + ");";
    *f = 2.5f;
    return 1;
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    log() += std::string("lump(") + lump + "," + name + ");";
    return missing.count(name) ? nullptr : at(0x800 + 16 * (std::strlen(name) % 7 + 1));
}
extern "C" void gameLineInitPathState__FP17gameLinePathStatePcP6Point3(void* state, char* tag, Point3* p) {
    log() += "path(" + std::to_string(static_cast<unsigned char*>(state) - obj) + "," + (tag ? tag : "null") + "," + std::to_string(reinterpret_cast<unsigned char*>(p) - obj) + ");";
}
extern "C" void animInitState__FP14AnimationState(void* s) { log() += "animInit(" + std::to_string(static_cast<unsigned char*>(s) - obj) + ");"; }
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int f) { log() += "animAdd(" + std::to_string(static_cast<unsigned char*>(s) - obj) + "," + std::to_string(h == w11::pointer(w11::u32(obj, 1056))) + "," + std::to_string(f) + ");"; }
extern "C" void animRandomizeTime__FP14AnimationState(void*) { log() += "animRand;"; }
extern "C" void setState__7Critteri(void* s, int st) { log() += "state(" + id(s) + "," + std::to_string(st) + ");"; }
extern "C" void specificInit__7CritterPcT1(void* s, char* name, char* lump) { log() += std::string("spec(") + name + "," + lump + ");"; w11::i16(obj, 572) = 12; }
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int mask) {
    log() += "chunks(" + std::to_string(m == w11::pointer(w11::u32(obj, 1036))) + "," + std::to_string(mask) + ");";
    return 0x1122334455667788ull;
}
extern "C" void objectAddToRunList__FP10GameObject(void*) { log() += "run;"; }
extern "C" void objectAddToSlowRunList__FP10GameObject(void*) { log() += "slow;"; }
extern "C" void finishConstruction__8Creature(void* s) { log() += "finish(" + id(s) + ");"; }

int main() {
    obj = at(0x100);
    for (int i = 0; i < 300; ++i) _ctype_[i] = 0;
    for (int c = 'A'; c <= 'Z'; ++c) _ctype_[c + 1] = 1;
    frameNm = 4242;
    int bad = 0;
    struct Res { const char* tag; const char* tag_fmt; const char* def_fmt; int offset; int fallback; };
    const Res table[] = {{"wakname", "%s.vif", "%s.vif", 1036, 0}, {"rawname", "%s.tex", "%s.tex", 1040, 0}, {"idlename", "%s.anm", "%s_idle.anm", 1044, 0},
                         {"movename", "%s.anm", "%s_move.anm", 1056, 0}, {"followname", "%s.anm", "%s_follow.anm", 1060, 0}, {"fleename", "%s.anm", "%s_flee.anm", 1064, 0},
                         {"osidlename", "%s.anm", "%s_osidle.anm", 1068, 1044}, {"osfleename", "%s.anm", "%s_osflee.anm", 1072, 1064}};
    const char* const tag_names[] = {"lumpname", "wakname", "rawname", "idlename", "movename", "followname", "fleename", "osidlename", "osfleename", "turnSpeed"};
    for (int k = 0; k < 24; ++k) {
        std::memset(arena, 0x5a, sizeof arena);
        calls().clear(); tags.clear(); missing.clear(); ints.clear();
        char name[16] = "WolfDIRE";
        if (k & 1) std::strcpy(name, "A");
        const std::string lower = (k & 1) ? "a" : "wolfdire";
        rolls[0] = 30 + k;
        for (int i = 0; i < 10; ++i)
            if (((k * 7 + i * 5) % 3) != 0) tags[tag_names[i]] = "t" + std::to_string(i);
        if (k & 2) tags["path"] = "pathx";
        if (k & 4) tags["change3"] = "x";
        if (k & 8) tags["change31"] = "x";
        if (k % 5 == 0) missing = {lower + "_osidle.anm", lower + "_osflee.anm", "t7", "t8"};
        if (k % 5 == 1) missing = {"t3", lower + "_idle.anm", lower + "_move.anm", "t4"};
        if (k % 5 == 2) missing = {"t3", "t4", lower + "_idle.anm"};
        ints["oneshot"] = (k % 3 == 0) ? 1 : 0;
        __7CritterPcG6Point3sPPc(obj, name, Point3{1.0f, 2.0f, 3.0f}, static_cast<short>(k - 8), reinterpret_cast<char**>(at(0x700)));
        // independent expected log
        char b[200];
        std::string want = "rand(1);";
        std::snprintf(b, sizeof b, "creature(1,1.00,2.00,3.00,%d,1,%.1f,1);", k - 8, static_cast<float>((30 + k) % 14 + 4));
        want += b;
        want += "setp(1,1,1,1,1,1,1);strcpy(" + lower + ");";
        std::string lumpname, resname;
        bool found[8];
        auto emit = [&](const char* tag, const char* tag_fmt, const char* def_fmt, std::string& outname) {
            auto it = tags.find(tag);
            want += std::string("tag(1,") + tag + ",0);";
            if (it != tags.end()) {
                want += std::string("tag(1,") + tag + ",0);";
                std::snprintf(b, sizeof b, tag_fmt, it->second.c_str());
            } else {
                std::snprintf(b, sizeof b, def_fmt, lower.c_str());
            }
            outname = b;
            want += std::string("fmt(") + (it != tags.end() ? tag_fmt : def_fmt) + "=" + b + ");";
        };
        emit("lumpname", "%s.lmp", "%s.lmp", lumpname);
        for (int i = 0; i < 8; ++i) {
            emit(table[i].tag, table[i].tag_fmt, table[i].def_fmt, resname);
            want += "lump(" + lumpname + "," + resname + ");";
            found[i] = missing.count(resname) == 0;
        }
        // effective presence of idle (index 2), move (3), flee (5)
        const bool idle = found[2], move = found[3];
        const auto st = tags.find("turnSpeed");
        want += "tag(1,turnSpeed,0);";
        if (st != tags.end()) want += "scan(" + st->second + ",%f);";
        want += "animInit(288);";
        want += std::string("tag(1,path,0);path(960,") + (tags.count("path") ? "pathx" : "null") + ",16);";
        want += "int(1,oneshot,0);";
        if (ints["oneshot"]) want += "state(1,13);";
        else if (idle) want += "state(1,0);";
        else if (move) want += "state(1,1);";
        unsigned mask = 0;
        for (int i = 0; i < 32; ++i) {
            std::snprintf(b, sizeof b, "fmt(change%%d=change%d);tag(1,change%d,0);", i, i);
            want += b;
            if (tags.count("change" + std::to_string(i))) mask |= 1u << i;
        }
        want += "spec(" + lower + "," + lumpname + ");chunks(1," + std::to_string(static_cast<int>(mask)) + ");run;finish(1);";
        std::string got = calls();
        // the stub writes a deterministic order for tag(path): the real call order has the path tag lookup before gameLineInitPathState
        if (got != want) { std::fprintf(stderr, "case %d\n%s\n%s\n", k, got.c_str(), want.c_str()); bad = 1; }
        const bool ok = w11::u32(obj, 244) == mask && w11::u32(obj, 248) == 4242u && w11::u8(obj, 172) == 18 && w11::u32(obj, 1076) == 2 && w11::u32(obj, 1020) == 180 &&
                        w11::u32(obj, 1024) == 60 && w11::u32(obj, 1028) == 180 && w11::i32(obj, 1084) == 7 && w11::f32(obj, 168) == 1.2f && w11::f32(obj, 28) == 10.0f &&
                        w11::u8(obj, 40) == 10 && w11::u32(obj, 72) == w11::word(vt_Critter) && w11::u32(obj, 12) == (0x100u | 8u) &&
                        w11::f32(obj, 1016) == (st != tags.end() ? 2.5f : 1.0f) && *reinterpret_cast<unsigned long long*>(obj + 552) == 0x1122334455667788ull &&
                        w11::u32(obj, 176) == 0 && w11::u32(obj, 1080) == 0 && w11::u32(obj, 1052) == 0 && w11::u32(obj, 1048) == 0 && w11::u32(obj, 1032) == 0 &&
                        (w11::u32(obj, 1036) != 0) == found[0] && (w11::u32(obj, 1040) != 0) == found[1] && (w11::u32(obj, 1044) != 0) == found[2] &&
                        (w11::u32(obj, 1056) != 0) == found[3] && (w11::u32(obj, 1060) != 0) == found[4] && (w11::u32(obj, 1064) != 0) == found[5] &&
                        (found[6] ? w11::u32(obj, 1068) != 0 : w11::u32(obj, 1068) == w11::u32(obj, 1044)) && (found[7] ? w11::u32(obj, 1072) != 0 : w11::u32(obj, 1072) == w11::u32(obj, 1064)) &&
                        w11::i16(obj, 572) == 12;
        if (!ok) { std::fprintf(stderr, "case %d fields\n", k); bad = 1; }
        if (lower != name) { std::fprintf(stderr, "case %d name not lowercased in place: %s\n", k, name); bad = 1; }
    }
    if (!bad) std::puts("critter ctor ok");
    return bad;
}
