// Host check only; not an EE build input. The object is a byte array; the
// callees are recorders. Expected stores and call order were also compared with
// the original in a private interpreter run (5 argument sets, identical call
// logs, identical changed bytes).
#include "gold.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string calls;
static unsigned char object[256];
unsigned char _vt$4Gold[4];
unsigned char goldAnm[4];
unsigned frameNm = 0x1357u;
static unsigned char header_marker;

extern "C" {
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 p, void* map, char** args) {
    char buf[96];
    std::snprintf(buf, sizeof buf, "base(%d,%g,%g,%g,%d,%d);", self == object, p.x, p.y, p.z, map == nullptr, args == nullptr);
    calls += buf;
    return 0;
}
void animInitState__FP14AnimationState(void* s) { calls += s == object + 88 ? "init;" : "init?;"; }
void* get__11ResourceMap(void* m) { calls += m == goldAnm ? "get;" : "get?;"; return &header_marker; }
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int f) {
    calls += (s == object + 88 && h == &header_marker && f == 516) ? "add;" : "add?;";
}
unsigned objectAddToRunList__FP10GameObject(void* o) { calls += o == object ? "run;" : "run?;"; return 0x77; }
}

static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static unsigned word(int off) { unsigned v; std::memcpy(&v, object + off, 4); return v; }

int main() {
    std::memset(object, 0xa5, sizeof object);
    std::memset(_vt$4Gold, 0, sizeof _vt$4Gold);
    const unsigned before12 = word(12);
    const unsigned r = __4GoldG6Point3i(object, Point3{1.5f, -2.0f, 3.25f}, -5);
    expect(r == 0x77u, "returns the run-list result");
    expect(calls == "base(1,1.5,-2,3.25,1,1);init;get;add;run;", "call order and arguments");
    expect(word(72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$4Gold)), "vtable word");
    expect(object[93] == 1, "byte 93");
    expect(static_cast<int>(word(128)) == -5, "integer at 128");
    expect(word(12) == (before12 | 0x220cu), "flags at 12");
    expect(object[76] == 0, "byte 76");
    expect(word(80) == 0x1357u, "frameNm copy");
    expect(object[0] == 0xa5 && object[200] == 0xa5 && object[84] == 0xa5, "other bytes untouched");
    std::printf("%s\n", failures ? "FAIL" : "ok");
    return failures != 0;
}
