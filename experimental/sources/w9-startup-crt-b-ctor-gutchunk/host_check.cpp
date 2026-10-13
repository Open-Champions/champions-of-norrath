// Host check only; not an EE build input. The callee is a recorder. The same
// stores and call arguments were compared with the original in a private
// interpreter run (5 argument sets with random table contents; identical call
// logs and changed bytes).
#include "gutchunk.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>

char* fxResourceMap[28];
unsigned char _vt$8GutChunk[4];
static unsigned char object[640];
static char names[28];
static struct { int calls; bool ok; int third; } ph;

extern "C" void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 p, char* a, char* b, char* c,
                                                              long l, int i, void* d, float f) {
    ++ph.calls;
    ph.ok = self == object && p.x == 1.0f && p.y == 2.0f && p.z == 3.0f && a == &names[0] && b == &names[20] &&
            l == 41 && i == 2625 && d == nullptr && f == 1.0f && c == &names[ph.third];
}

static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

int main() {
    for (int i = 0; i < 28; ++i) fxResourceMap[i] = &names[i];
    for (int selector : {3, 0}) {
        std::memset(object, 0x3c, sizeof object);
        for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(i + 1);
        ph = {0, false, selector ? 24 : 22};
        __8GutChunkG6Point3iif(object, Point3{1.0f, 2.0f, 3.0f}, 41, selector, 9.5f);
        expect(ph.calls == 1 && ph.ok, selector ? "PhProp arguments, bit 1 set" : "PhProp arguments, bit 1 clear");
        int w584, w604;
        std::memcpy(&w584, object + 584, 4);
        std::memcpy(&w604, object + 604, 4);
        expect(w584 == selector, "selector stored at 584");
        expect(w604 == 2 && object[608] == 0, "constants at 604 and 608");
        expect(std::memcmp(object + 592, object + 16, 12) == 0, "position copy at 592");
        expect(object[0] == 0x3c && object[300] == 0x3c && object[609] == 0x3c, "other bytes untouched");
    }
    std::printf("%s\n", failures ? "FAIL" : "ok");
    return failures != 0;
}
