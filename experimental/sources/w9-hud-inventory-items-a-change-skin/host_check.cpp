// Host contract for changeSkin__Fii. Not an EE plan input.
#include "change_skin.cpp"
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
static std::string calls;
static unsigned char objects[10][1100];
extern "C" {
SkinTarget targets[10]; signed char skinSetting[16]; unsigned char* targetObjs[10]; const char* npcLumpName = "lump";
int sprintf(char* out, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt); int n = vsnprintf(out, 96, fmt, ap); va_end(ap); return n;
}
void* lumpFindResource__FPcT0(const char* lump, char* name) {
    calls += std::string(lump) + ":" + name + ";";
    return reinterpret_cast<void*>(0x1000 + calls.size());
}
}
int main() {
    for (int i = 0; i < 10; ++i) targetObjs[i] = objects[i];
    targets[4].name = "ab"; targets[4].count = 3;
    if (changeSkin__Fii(4, 1) != 1 || skinSetting[4] != 1) return 1;
    if (calls != "lump:ab_bodyv1.vif;lump:ab_bodyv2.tex;") return 2;
    if (*reinterpret_cast<void**>(objects[4] + 1056) == 0 || *reinterpret_cast<void**>(objects[4] + 1060) == 0) return 3;
    if (changeSkin__Fii(4, 1) != 1 || skinSetting[4] != 2) return 4;
    if (changeSkin__Fii(4, 1) != 0 || skinSetting[4] != 2) return 5;
    calls.clear();
    if (changeSkin__Fii(4, -1) != 1 || skinSetting[4] != 1) return 6;
    if (calls != "lump:ab_bodyv1.vif;lump:ab_bodyv2.tex;") return 7;
    if (changeSkin__Fii(4, -1) != 1 || skinSetting[4] != 0) return 8;
    if (changeSkin__Fii(4, -1) != 0 || skinSetting[4] != 0) return 9;
    skinSetting[4] = 2;
    calls.clear();
    if (changeSkin__Fii(4, 0) != 0 || skinSetting[4] != 0) return 10;
    if (calls != "lump:ab_bodyv1.vif;lump:ab_bodyv1.tex;") return 11;
    return 0;
}
