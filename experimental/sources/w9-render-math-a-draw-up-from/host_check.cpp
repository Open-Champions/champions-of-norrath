// Host check only; not a build input. Callees log their arguments (strings are logged by content); the
// expected log comes from a separate model of the numbered step list in the source comment, over null and
// non-null text and label, an empty string, long strings, several limits, heights and widths, and colours
// that exercise every byte of the dimming step.
#include "draw_up_from.cpp"
#include <cstdio>
#include <string>
#include <vector>

void* smallFont = reinterpret_cast<void*>(0x1234);
static std::string trace;
static int stub_width, stub_height;
static std::string N(long v) { return std::to_string(v); }
static std::string u(const unsigned short* s) { std::string r; for (; *s; ++s) r += static_cast<char>(*s); return r; }

extern "C" int fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int limit) {
    trace += std::string("W") + (font == smallFont ? "" : "?") + ",'" + u(text) + "'," + N(limit) + ";";
    return stub_width;
}
extern "C" int fontPrintWrapu__FiiPUsiiii(int x, int y, const unsigned short* text, int width, int a, int b, int c) {
    trace += "P" + N(x) + "," + N(y) + ",'" + u(text) + "'," + N(width) + "," + N(a) + "," + N(b) + "," + N(c) + ";";
    return stub_height;
}
extern "C" void fontColor__FUiii(unsigned color, int a, int b) { trace += "C" + N(color) + "," + N(a) + "," + N(b) + ";"; }

int main() {
    bool ok = true;
    long scenarios = 0;
    const std::string long_label(200, 'L'), long_text(250, 'T');
    const char* texts[] = {nullptr, "", "hello", "A\x01\x7f\xff", long_text.c_str()};
    const char* labels[] = {nullptr, "", "Name", "\xe9x", long_label.c_str()};
    const unsigned colors[] = {0u, 0xffffffffu, 0x01020304u, 0xfe80017fu, 0x80ff7f01u, 0xaabbccddu};
    for (const char* text : texts) for (const char* label : labels) for (int width : {0, 37, 500}) for (int height : {0, 14, 99}) for (int limit : {-5, 0, 640})
    for (int x : {0, 12, -300}) for (int y : {5, 400}) for (unsigned color : colors) {
        stub_width = width;
        stub_height = height;
        trace.clear();
        const int result = drawUpFrom__FiiPCUcT2iUi(x, y, reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>(label), limit, color);

        std::string a, b;
        if (!text) { a = " "; b = ""; }
        else {
            a = text;
            b = label ? std::string(label) + ": " : "> ";
        }
        const int left = x + width, room = limit - width, new_y = y - height;
        const unsigned dimmed = (((color >> 24) & 0xffu) << 24) | ((((color >> 16) & 0xffu) >> 1) << 16) | ((((color >> 8) & 0xffu) >> 1) << 8) | ((color & 0xffu) >> 1);
        const std::string want = "W,'" + b + "',1000000;P" + N(left) + "," + N(y) + ",'" + a + "'," + N(room) + ",0,0,21;C" + N(color) + ",-1,0;P" + N(x) + "," + N(new_y) +
                                 ",'" + b + "'," + N(limit) + ",230,580,21;C" + N(dimmed) + ",-1,0;P" + N(left) + "," + N(new_y) + ",'" + a + "'," + N(room) + ",230,580,21;";
        if (trace != want || result != new_y) { ok = false; std::fputs(("MISMATCH\n got  " + trace.substr(0, 300) + "\n want " + want.substr(0, 300) + "\n").c_str(), stdout); break; }
        ++scenarios;
    }
    std::puts(ok ? ("ok (" + std::to_string(scenarios) + " scenarios)").c_str() : "FAIL");
    return !ok;
}
