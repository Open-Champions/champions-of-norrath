// Host check only; not a build input.
#include <string>
static unsigned char arena[0x1000];
#define NORRATH_WORD_ADDRESS(word) (static_cast<void*>(arena + (word)))
#include "parse_zone_base_obj.cpp"
static std::string tr; static int failStep; static unsigned version; static int step;
static int result(const char* mark) { tr += mark; ++step; return step == failStep ? -1 : 0; }
static void* arrayCtor; static void* arrayUsed; static int dtorFlags;
extern "C" void __t7VIArray1Zi(void* array) { arrayCtor = array; tr += "c;"; }
extern "C" void _$_t7VIArray1Zi(void* array, int flags) { tr += array == arrayCtor ? "d;" : "d?;"; dtorFlags = flags; }
extern "C" int RemoveActorsFromZone__7VISceneR6VIZoneRt7VIArray1Zi(void* scene, void* zone, void* array) {
    arrayUsed = array; return result(scene == arena + 0x10 && zone == arena + 0x400 && array == arrayCtor ? "r;" : "r?;");
}
extern "C" int ParseZoneTree__10VIESFParseR10VIZoneTree(void*, void* tree) { return result(tree == arena + 0x400 + 68 ? "t;" : "t?;"); }
extern "C" int ParseZonePreTranslations__10VIESFParseR6VIZone(void*, void* zone) { return result(zone == arena + 0x400 ? "p;" : "p?;"); }
extern "C" int ParseZoneRooms__10VIESFParseR6VIZone(void*, void* zone) { return result(zone == arena + 0x400 ? "o;" : "o?;"); }
extern "C" int ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(void*, void* table) { return result(table == arena + 0x400 + 100 ? "s;" : "s?;"); }
extern "C" int ParseZoneStaticTable__10VIESFParseRt7VIArray1Z21VIZoneStaticTableElem(void*, void* table) { return result(table == arena + 0x400 + 108 ? "a;" : "a?;"); }
extern "C" unsigned ObjectVersion__9VIObjFile(void*) { tr += "v;"; return version; }
extern "C" int ParseZoneFlora__10VIESFParse(void*) { return result("f;"); }
extern "C" void Precalc__17VIZoneRadialFloraR7VIScene(void* flora, void* scene) { tr += flora == arena + 0x400 + 116 && scene == arena + 0x10 ? "l;" : "l?;"; }
extern "C" int UpdateActors__7VISceneRt7VIArray1Zi(void* scene, void* array) { return result(scene == arena + 0x10 && array == arrayCtor ? "u;" : "u?;"); }
extern "C" int ReadEnd__9VIObjFile(void*) { return result("e;"); }
static int run(unsigned char* parse) { tr.clear(); step = 0; dtorFlags = 0; return ParseZoneBaseObj__10VIESFParse(parse); }
int main() {
    alignas(8) unsigned char parse[64] = {0};
    unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[6] = 0x10; w[7] = 0x400; w[9] = 0x55;
    version = 0; failStep = 0;
    if (run(parse) != 0 || tr != "c;r;t;p;o;s;a;v;u;e;d;" || dtorFlags != 2) return 1;
    version = 3;
    if (run(parse) != 0 || tr != "c;r;t;p;o;s;a;v;f;l;u;e;d;") return 2;
    version = 0;
    const char* expectNoFlora[] = {"", "c;r;d;", "c;r;t;d;", "c;r;t;p;d;", "c;r;t;p;o;d;", "c;r;t;p;o;s;d;", "c;r;t;p;o;s;a;d;"};
    // failing step: 1 remove, 2 tree, 3 pre-translations, 4 rooms, 5 resources, 6 static table; then without flora 7 update, 8 read end
    for (int s = 1; s <= 6; ++s) {
        failStep = s;
        if (run(parse) != -1 || tr != expectNoFlora[s] || dtorFlags != 2) return 10 + s;
    }
    failStep = 7;
    if (run(parse) != -1 || tr != "c;r;t;p;o;s;a;v;u;d;") return 20;
    failStep = 8;
    if (run(parse) != -1 || tr != "c;r;t;p;o;s;a;v;u;e;d;") return 21;
    version = 1; failStep = 7;
    if (run(parse) != -1 || tr != "c;r;t;p;o;s;a;v;f;d;") return 22;
    failStep = 8;
    if (run(parse) != -1 || tr != "c;r;t;p;o;s;a;v;f;l;u;d;") return 23;
    failStep = 9;
    if (run(parse) != -1 || tr != "c;r;t;p;o;s;a;v;f;l;u;e;d;") return 24;
    return 0;
}
