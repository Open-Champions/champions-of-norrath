#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
struct Tex { short width, height; };
struct Entry { int unobserved; Tex* texture; };
unsigned long glistp, gstart;
int GLISTSIZE;
static std::map<std::string, Entry> entries;
static std::map<Tex*, std::string> names;
static std::vector<std::string> calls;
static int loads;
extern "C" void* lumpLoad__FPc(char* name) { loads++; calls.push_back(std::string("load ") + name); return (void*)0x1000; }
extern "C" Entry* lumpFind__FPvPc(void* lump, char* name) {
    if (lump != (void*)0x1000) calls.push_back("BADLUMP");
    Entry& e = entries[name];
    if (!e.texture) {
        e.texture = new Tex{8, 8};
        std::string n = name;
        if (n.find("center") != std::string::npos) *e.texture = Tex{6, 5};
        names[e.texture] = n;
    }
    return &e;
}
extern "C" void _textureDrawColor__FP8_textureiiiUi(Tex* t, int x, int y, int extra, unsigned color) {
    char b[200]; snprintf(b, sizeof b, "color %s %d %d %d %u", names[t].c_str(), x, y, extra, color); calls.push_back(b);
}
extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(Tex* t, int x, int y, int extra, unsigned color, int l, int tp, int r, int b, char c1, char c2) {
    char s[200]; snprintf(s, sizeof s, "super %s %d %d %d %u clip %d %d %d %d %d %d", names[t].c_str(), x, y, extra, color, l, tp, r, b, c1, c2); calls.push_back(s);
}
extern "C" void textureSmoothBorder__FiiiiiUiiii(int, int, int, int, int, unsigned, int, int, int);

static std::vector<std::string> run(int x, int y, int w, int h, int style, int skipFill, int innerOnly, int room = 3000) {
    calls.clear(); entries.clear(); names.clear(); loads = 0;
    glistp = 0x100000; gstart = glistp - 8 * 40; GLISTSIZE = 40 + room;
    textureSmoothBorder__FiiiiiUiiii(x, y, w, h, 77, 0x80112233u, style, skipFill, innerOnly);
    return calls;
}
static std::string color(const char* n, int x, int y) { char b[200]; snprintf(b, sizeof b, "color %s %d %d 77 %u", n, x, y, 0x80112233u); return b; }
static std::string super(const char* n, int x, int y, int l, int t, int r, int b) { char s[200]; snprintf(s, sizeof s, "super %s %d %d 77 %u clip %d %d %d %d 0 0", n, x, y, 0x80112233u, l, t, r, b); return s; }

int main() {
    // Too little room: nothing is looked up.
    if (!run(10, 20, 30, 26, 0, 0, 0, 2999).empty()) return 1;
    // style 0, corners 8 wide: full expected sequence.
    std::vector<std::string> want = {"load interfac.lmp"};
    want.push_back(color("ctilebotleft.tex", 10, 37));
    want.push_back(color("ctilebotright.tex", 32, 37));
    want.push_back(color("ctiletopleft.tex", 10, 19));
    want.push_back(color("ctiletopright.tex", 32, 19));
    for (int sx : {18, 26}) {
        want.push_back(super("ctiletop.tex", sx, 20, 10, 20, 32, 38));
        want.push_back(super("ctilebot.tex", sx, 38, 10, 20, 32, 46));
    }
    for (int sy : {28, 36}) {
        want.push_back(super("ctileleft.tex", 10, sy, 10, 20, 40, 38));
        want.push_back(super("ctileright.tex", 32, sy, 10, 20, 40, 38));
    }
    for (int row : {0, 5}) for (int col : {0, 6, 12})
        want.push_back(super("ctilecenter.tex", 18 + col, 28 + row, 10, 20, 32, 38));
    auto got = run(10, 20, 30, 26, 0, 0, 0);
    // Compare ignoring the lookups: only draws and the load are recorded by the stubs.
    if (got != want) { for (size_t i = 0; i < got.size() && i < want.size(); i++) if (got[i] != want[i]) { fprintf(stderr, "%zu got %s want %s\n", i, got[i].c_str(), want[i].c_str()); break; } fprintf(stderr, "sizes %zu %zu\n", got.size(), want.size()); return 2; }
    // style 1 uses the btile prefix.
    got = run(10, 20, 30, 26, 1, 0, 0);
    if (got[1] != color("btilebotleft.tex", 10, 37)) return 3;
    // innerOnly skips the corners and edges but still tiles the centre.
    got = run(10, 20, 30, 26, 0, 0, 1);
    std::vector<std::string> centre(want.end() - 6, want.end());
    centre.insert(centre.begin(), "load interfac.lmp");
    if (got != centre) return 4;
    // skipFill skips only the centre.
    got = run(10, 20, 30, 26, 0, 1, 0);
    std::vector<std::string> edges(want.begin(), want.end() - 6);
    if (got != edges) return 5;
    // Both: only the load.
    if (run(10, 20, 30, 26, 0, 1, 1) != std::vector<std::string>{"load interfac.lmp"}) return 6;
    // Inner height not positive: no side pieces and no centre; narrow width: no top/bottom pieces.
    got = run(10, 20, 30, 16, 0, 0, 0);
    for (auto& s : got) if (s.find("left.tex") != std::string::npos && s.find("super") == 0) return 7;
    for (auto& s : got) if (s.find("center") != std::string::npos) return 8;
    got = run(10, 20, 16, 26, 0, 0, 0);
    for (auto& s : got) if (s.find("super ctiletop.tex") == 0 || s.find("super ctilebot.tex") == 0) return 9;
    // style 2: tile prefix, +4 bottom clip, centre starts 26/25 back and is clipped one past.
    got = run(10, 20, 30, 26, 2, 0, 0);
    if (got[1] != color("tilebotleft.tex", 10, 37)) return 10;
    if (got[5] != super("tiletop.tex", 18, 20, 10, 20, 32, 38)) return 11;
    if (got[6] != super("tilebot.tex", 18, 38, 10, 20, 36, 46)) return 12;
    // centre: rows start at y + c - 26 = 2, columns at x + c - 25 = -7, clip right = 40 - (-17) + 1 = 58, bottom = 46 - (-18) = 64.
    // row range 26 - 16 + 50 = 60 -> offsets 0,5,...,60 (13 rows); column range 14 + 52 = 66 -> offsets 0,6,...,60 (11 columns).
    int centers = 0;
    for (auto& s : got) if (s.find("super tilecenter.tex") == 0) centers++;
    if (centers != 13 * 11) return 13;
    if (got.size() < 5 + 4 + 4 + 1 + 1) return 14;
    std::string firstCenter = super("tilecenter.tex", -7, 2, 10, 20, 58, 64);
    bool found = false; for (auto& s : got) if (s == firstCenter) found = true;
    if (!found) return 15;
    std::string lastCenter = super("tilecenter.tex", -7 + 60, 2 + 60, 10, 20, 58, 64);
    found = false; for (auto& s : got) if (s == lastCenter) found = true;
    if (!found) return 16;
    // style 2 with a negative row range (h - 2c + 50 < 0) fills nothing; style 0 with a
    // non-positive inner height fills nothing either.
    got = run(10, 20, 30, -40, 2, 0, 1);
    for (auto& s : got) if (s.find("super") == 0) return 17;
    got = run(10, 20, 30, 16, 0, 0, 1);
    for (auto& s : got) if (s.find("super") == 0) return 18;
    got = run(10, 20, 30, -34, 2, 0, 1);   // -34 - 16 + 50 = 0: one row of centre tiles
    int zeroRow = 0;
    for (auto& s : got) if (s.find("super tilecenter.tex") == 0) zeroRow++;
    if (zeroRow != 11) return 19;
    return 0;
}
