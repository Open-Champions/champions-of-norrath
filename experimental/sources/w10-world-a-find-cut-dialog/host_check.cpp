// Host contract for findCutDialog__Fv. Not an EE plan input. The library and lump routines are
// recording stubs; the check covers the names built, the extension cut and the node count.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#define sprintf w_sprintf
#define strcpy w_strcpy
#define strrchr w_strrchr
#include "find_cut_dialog.cpp"
#undef sprintf
#undef strcpy
#undef strrchr

static std::string trace;
static unsigned char resource[4];
static unsigned entryWords[4];
static bool exists;
extern "C" {
char engineCurrentLevelName[80];
char cutSceneName[80];
unsigned nmDialogNodes;
int w_sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f);
    int n = std::vsnprintf(d, 80, f, ap);
    va_end(ap); return n;
}
char* w_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
char* w_strrchr(const char* s, int c) { return const_cast<char*>(std::strrchr(s, c)); }
unsigned char* lumpFindResource__FPcT0(char* lump, char* name) {
    trace += std::string("find:") + lump + "," + name + ";";
    return exists ? resource : nullptr;
}
unsigned char* lumpLoad__FPc(char* lump) { trace += std::string("load:") + lump + ";"; return resource; }
unsigned* lumpFind__FPvPc(void* l, char* name) {
    trace += std::string("lf:") + name + ";";
    return entryWords;
}
}

int main() {
    std::strcpy(engineCurrentLevelName, "act1");
    std::strcpy(cutSceneName, "cut01.cut");
    entryWords[2] = 1096 * 7 + 1095;
    exists = true; nmDialogNodes = 99;
    if (findCutDialog__Fv() != resource) return 1;
    if (trace != "find:act1.lmp,cut01.dbin;load:act1.lmp;lf:cut01.dbin;") return 2;
    if (nmDialogNodes != 7) return 3;
    // only the last dot is removed
    trace.clear(); std::strcpy(cutSceneName, "a.b.c");
    findCutDialog__Fv();
    if (trace.rfind("find:act1.lmp,a.b.dbin;", 0) != 0) return 4;
    // no dot: the name is used whole
    trace.clear(); std::strcpy(cutSceneName, "plain");
    findCutDialog__Fv();
    if (trace.rfind("find:act1.lmp,plain.dbin;", 0) != 0) return 5;
    // resource missing: nothing is loaded and the count is zeroed
    trace.clear(); exists = false; nmDialogNodes = 5;
    if (findCutDialog__Fv() != nullptr) return 6;
    if (trace != "find:act1.lmp,plain.dbin;" || nmDialogNodes != 0) return 7;
    // size below one record gives zero nodes; a large size divides unsigned
    exists = true; entryWords[2] = 1095;
    findCutDialog__Fv();
    if (nmDialogNodes != 0) return 8;
    entryWords[2] = 0xffffffffu;
    findCutDialog__Fv();
    if (nmDialogNodes != 0xffffffffu / 1096u) return 9;
    return 0;
}
