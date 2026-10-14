// Host check only; not a build input. Also compared with the original in a private interpreter (1944 paired
// scenarios, 102 of 103 original words covered, the rest a nop). Here the globals are host variables, the text and
// font-size callees log, and the expected values come from a separate model of the description in the source
// comment, over cells -3..40, previous cells, snap values, the language byte, grid width and count.
#include "set_new_grid_position.cpp"
#include <cstdio>
#include <cstring>
#include <string>

int nameGridPos, nameGridW, nameGridCount;
float nameGridDstX, nameGridDstY, nameGridDstW, nameGridSrcX, nameGridSrcY, nameGridSrcW, otherHermiteTime;
char language;
void* mediumFont;
static std::string trace;
static int size_result;
static unsigned short strings[2][4];
extern "C" const unsigned short* textGetString__FPc(const char* key) {
    trace += std::string("K:") + key + ";";
    return strings[key[13] == 'D' ? 0 : 1];  // "selectSetNameDone" / "selectSetNameSpace"
}
extern "C" int fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int limit) {
    trace += std::string("S:") + (font == mediumFont ? "m" : "?") + (text == strings[0] ? "0" : text == strings[1] ? "1" : "?") + "," + std::to_string(limit) + ";";
    return size_result;
}
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }

int main() {
    bool ok = true;
    long scenarios = 0;
    int marker;
    mediumFont = &marker;
    for (int cell : {-3, -2, -1, 0, 1, 5, 11, 29, 40}) for (int old : {-3, -2, -1, 0, 5, 29}) for (int snap : {0, 1, 2}) for (int lang : {0, 1, -1})
    for (int width : {5, 7}) for (int count : {35, 42}) for (int size : {0, 63, 299}) {
        nameGridPos = old; nameGridW = width; nameGridCount = count; language = static_cast<char>(lang); size_result = size;
        nameGridDstX = 11.5f; nameGridDstY = -22.25f; nameGridDstW = 33.0f;
        nameGridSrcX = 1.0f; nameGridSrcY = 2.0f; nameGridSrcW = 3.0f;
        otherHermiteTime = 0.75f;
        trace.clear();
        setNewGridPosition__Fii(cell, snap);

        std::string want;
        float dx = 11.5f, dy = -22.25f, dw = 33.0f, sx = 1.0f, sy = 2.0f, sw = 3.0f, t = 0.75f;
        int pos = old;
        if (cell != old) {
            sx = dx; sy = dy; sw = dw;
            const int shift = lang != 0 ? 29 : 0;
            if (cell == -1 || cell == -2) {
                dx = static_cast<float>(shift + (cell == -1 ? 233 : 163));
                dy = static_cast<float>((count / width) * 20 + 122);
                want = std::string("K:") + (cell == -1 ? "selectSetNameDone" : "selectSetNameSpace") + ";S:m" + (cell == -1 ? "0" : "1") + ",1000000;";
                dw = static_cast<float>(size + 10);
            } else {
                dw = 20.0f;
                dx = static_cast<float>((cell % width) * 20 + shift + 61);
                dy = static_cast<float>((cell / width) * 20 + 122);
            }
            if (old == -3 || snap != 0) { sx = dx; sy = dy; sw = dw; }
            pos = cell; t = 0.0f;
        }
        if (trace != want || nameGridPos != pos || !same(nameGridDstX, dx) || !same(nameGridDstY, dy) || !same(nameGridDstW, dw) || !same(nameGridSrcX, sx) ||
            !same(nameGridSrcY, sy) || !same(nameGridSrcW, sw) || !same(otherHermiteTime, t) || nameGridW != width || nameGridCount != count) {
            ok = false;
            std::printf("MISMATCH cell=%d old=%d snap=%d lang=%d w=%d count=%d size=%d trace '%s'\n", cell, old, snap, lang, width, count, size, trace.c_str());
            break;
        }
        ++scenarios;
    }
    std::puts(ok ? ("ok (" + std::to_string(scenarios) + " scenarios)").c_str() : "FAIL");
    return !ok;
}
