// Host check only; not a build input. Also compared with the original in a private interpreter (paired scenarios over
// texts with spaces, newlines, long words and zero-width characters, widths, row limits and the flag combinations;
// all words but five nops covered; those scenarios included widths of 12 and below, where a one-character line can already be
// too wide and the cut length becomes negative, which this host check avoids). The window and UI object live in the arena. The callees are stubs that log; a small
// reference model written from the description in the source comment predicts the whole call log and the result.
#define NORRATH_HOST_ARENA 1
#include "textbox.cpp"
#include "../w16-wnd-a-common/host_world.hpp"
#include <string>
#include <vector>

static const unsigned window_g = hw::BASE + 0x100, ui_g = hw::BASE + 0x2000, raster_g = hw::BASE + 0x3000, dictionary_g = hw::BASE + 0x3100, color_g = hw::BASE + 0x3200;
static std::string trace;
static int gh_value;
static std::string N(long v) { return std::to_string(v); }
static std::string narrow(const unsigned short* s, long limit = 100000) { std::string r; for (; *s && limit > 0; ++s, --limit) r += *s < 0x80 ? static_cast<char>(*s) : '?'; return r; }
static int width_of(const std::string& s) { int w = 0; for (char c : s) w += static_cast<unsigned char>(c) < 0x10 ? 0 : 5 + (c & 3); return w; }

extern "C" int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary) {
    trace += "G" + N(hw::guest(font) - ui_g) + "," + N(hw::guest(raster)) + "," + N(hw::guest(dictionary)) + ";";
    return gh_value;
}
extern "C" void DrawFrame__10VIUIWindowiiiiUi(void*, int a, int b, int c, int d, unsigned flags) { trace += "F" + N(a) + "," + N(b) + "," + N(c) + "," + N(d) + "," + N(flags) + ";"; }
extern "C" void CopyAsteriks__10VIUIWindowPUsii(void*, unsigned short* dst, int size, int length) {
    trace += "A" + N(size) + "," + N(length) + ";";
    for (int i = 0; i < length; ++i) dst[i] = 0x2a;
}
extern "C" void strncpy16__8VIStringPUsUiPCUsi(unsigned short* dst, unsigned size, const unsigned short* src, int length) {
    trace += "C" + N(size) + "," + N(length) + ",'" + narrow(src, length) + "';";
    for (int i = 0; i < length; ++i) dst[i] = src[i];
}
extern "C" int TextWidth__10VIUIWindowPCUs(void*, const unsigned short* text) {
    const std::string s = narrow(text);
    trace += "W'" + s + "';";
    return width_of(s);
}
extern "C" int strlen16__8VIStringPCUs(const unsigned short* text) { int n = 0; while (text[n]) ++n; trace += "L" + N(n) + ";"; return n; }
extern "C" void AppendEllipsis__10VIUIWindowPUsi(void*, unsigned short* text, int width) {
    trace += "E'" + narrow(text) + "'," + N(width) + ";";
    int n = 0; while (text[n]) ++n;
    for (int i = 0; i < 3; ++i) text[n + i] = 0x2e;
    text[n + 3] = 0;
}
extern "C" void DrawTextInternal__10VIUIWindowPCUsiiRC10VIColor32FUi(void*, const unsigned short* text, int x, int y, const VIColor32F& color, unsigned flags) {
    unsigned bits; std::memcpy(&bits, &color.v[0], 4);
    trace += "D'" + narrow(text) + "'," + N(x) + "," + N(y) + "," + N(bits) + "," + N(flags) + ";";
}
static unsigned& W(unsigned off) { return hw::w(window_g + off); }

// Reference model: returns the expected call log and result.
struct Model {
    std::string log;
    int result = 0;
};
static Model model(const std::string& text, int x, int row, int width, int rows, unsigned flags) {
    Model m;
    const bool measure = flags == 0xffffffffu;
    const int wx = 400, wy = 300, ww = 201, wh = 99, inner = 13;
    const std::string g = "G56," + N(raster_g) + "," + N(dictionary_g) + ";";
    m.log += g + g;
    const int left = wx - (ww >> 1) + x;
    if (!measure && (flags & 4)) m.log += "F" + N(x) + "," + N(row) + "," + N(width) + "," + N(rows) + "," + N(flags) + ";";
    const int skip = measure ? 0 : static_cast<int>(flags >> 24);
    int lines = 0, processed = 0, last_width = 0;
    size_t i = 0, start = 0, last_space = 0, last_grow = 0;
    std::string line;
    auto cut = [&](size_t from, size_t count) {
        const std::string piece = text.substr(from, count);
        if (!measure && (flags & 0x80)) { m.log += "A512," + N(static_cast<long>(count)) + ";"; line = std::string(count, '*'); }
        else { m.log += "C512," + N(static_cast<long>(count)) + ",'" + piece + "';"; line = piece; }
    };
    auto draw = [&]() {
        int px = left;
        if (flags & 8) { m.log += "W'" + line + "';"; px = left + (width >> 1) - (width_of(line) >> 1); }
        else if (flags & 0x10) { m.log += "W'" + line + "';"; px = left + width - width_of(line); }
        m.log += g;
        const int py = wy - (wh >> 1) + inner + (gh_value - 4) * (row + lines) + 5;
        unsigned bits; const float one = 0.75f; std::memcpy(&bits, &one, 4);
        m.log += "D'" + line + "'," + N(px) + "," + N(py) + "," + N(bits) + "," + N(flags) + ";";
    };
    auto finish_line = [&](size_t next) {
        if (!measure && (flags & 2) && lines + 1 == rows) {
            const long remaining = static_cast<long>(text.size() - next);
            m.log += "L" + N(remaining) + ";";
            if (remaining != 0) { m.log += "E'" + line + "'," + N(width) + ";"; line += "..."; }
        }
        if (processed >= skip) { if (!measure) draw(); ++lines; }
        ++processed;
    };
    while (i < text.size()) {
        if (text[i] == '\n') {
            cut(start, i - start);
            start = i + 1; i = start;
            finish_line(start);
        } else {
            cut(start, i - start);
            m.log += "W'" + line + "';";
            const int previous = last_width;
            last_width = width_of(line);
            if (last_width < width - 8) {
                if (previous != last_width) last_grow = i;
                if (text[i] == ' ') last_space = i;
                ++i;
                continue;
            }
            if (last_space != 0) { cut(start, last_space - start + 1); i = last_space + 1; }
            else { cut(start, last_grow - start); i = last_grow; }
            start = i;
            finish_line(start);
        }
        if (!(lines < rows)) { m.result = lines; return m; }
        last_space = 0; last_grow = 0;
    }
    if (i - start != 0) {
        cut(start, i - start);
        if (processed >= skip) { if (!measure) draw(); ++lines; }
    }
    m.result = lines;
    return m;
}

int main() {
    bool ok = true;
    long scenarios = 0;
    unsigned long long state = 12345;
    auto next = [&]() { state = state * 6364136223846793005ull + 1442695040888963407ull; return static_cast<unsigned>(state >> 33); };
    std::vector<std::string> texts = {"", "a", "\n", "a\n", "abc def ghi", std::string(30, 'W'), " " + std::string(10, 'a'), std::string(20, 'a') + "\n\n\n   " + std::string(7, 'b'), "ab ab ab ab ab ab ab ab ab ab", "\x01\x01\x01 \x01x y"};
    const char alphabet[] = {'a', 'b', 'W', 'i', ' ', ' ', ' ', '\n', '\x01', 'x', 'y'};
    for (int k = 0; k < 120; ++k) { std::string t; const int n = 1 + next() % 70; for (int j = 0; j < n; ++j) t += alphabet[next() % sizeof alphabet]; texts.push_back(t); }
    const unsigned flag_list[] = {0xffffffffu, 0, 2, 4, 8, 0x10, 0x80, 0x0a, 0x12, 0x86, 0x01000000u, 0x02000002u, 0x03000008u, 0x01000090u, 0x04000006u, 0x81};
    for (const std::string& text : texts) for (unsigned flags : flag_list) for (int width : {17, 20, 28, 60, 200}) for (int rows : {1, 2, 5, 256}) for (int xx : {0, -4}) for (int row : {0, 2}) {
        if ((next() & 7) != 0) continue;  // thin the product to a few thousand cases
        hw::clear();
        gh_value = 12 + next() % 9;
        W(0) = ui_g; W(12) = 400; W(16) = 300; W(20) = 201; W(24) = 99; W(28) = 13;
        hw::w(ui_g + 72) = raster_g; hw::w(ui_g + 80) = dictionary_g;
        const float color0 = 0.75f;
        std::memcpy(hw::at(color_g), &color0, 4);
        unsigned short units[300];
        for (size_t i = 0; i <= text.size(); ++i) units[i] = static_cast<unsigned char>(i < text.size() ? text[i] : 0);
        trace.clear();
        const int result = TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(hw::at(window_g), units, xx, row, width, rows, *reinterpret_cast<const VIColor32F*>(hw::at(color_g)), flags);
        const Model m = model(text, xx, row, width, rows, flags);
        if (result != m.result || trace != m.log) {
            ok = false;
            std::printf("MISMATCH text='%s' flags=%x width=%d rows=%d x=%d row=%d result %d/%d\n got  %s\n want %s\n", text.c_str(), flags, width, rows, xx, row, result, m.result, trace.c_str(), m.log.c_str());
            break;
        }
        ++scenarios;
    }
    std::puts(ok ? ("ok (" + std::to_string(scenarios) + " scenarios)").c_str() : "FAIL");
    return !ok;
}
