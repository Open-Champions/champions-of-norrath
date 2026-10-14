#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "yes_no.cpp"

struct Font { int id; };
extern "C" {
int frontEndRunning;
Font font_a{1}, font_b{2};
Font* mediumFont = &font_a;
float coreInterfaceBrightness[4] = {1.0f, 1.0f, 1.0f, 1.0f};
int yesNoPos[4];
int activeDelay[4] = {3, 3, 3, 3};
norrath_pad_record pad[4];
int padAcceptButton = 0x4000;
int nmPlayers = 4;
}
static std::vector<std::string> log_;
static int local_mask;          // bit i set -> player i is local
static unsigned short text_buf[4], yes_buf[1], no_buf[1];
static int wrap_height = 30;
static int flares, flare_arg = -9, sounds;
static unsigned cs(unsigned c, float f) { return c ^ static_cast<unsigned>(f * 1000.0f); }
static std::string num(long v) { return std::to_string(v); }
extern "C" {
Font* lumpFindResource__FPcT0(const char* l, const char* n) {
    log_.push_back(std::string("lump:") + l + ":" + n);
    if (!std::strcmp(n, "gamefontsmall.fnt")) return &font_b;
    return reinterpret_cast<Font*>(&text_buf[2]);
}
unsigned colorScale__FUif(unsigned c, float f) { return cs(c, f); }
void fontStart__FP4FontiUii(Font* f, int m, unsigned c, int l) {
    log_.push_back("fontStart:" + num(f->id) + ":" + num(m) + ":" + num(c) + ":" + num(l));
}
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int w, int a, int b, int c) {
    log_.push_back("wrap:" + num(x) + ":" + num(y) + ":" + num(t == text_buf) + ":" + num(w) + ":" + num(a) + ":" + num(b) + ":" + num(c));
    return wrap_height;
}
void fontColor__FUiii(unsigned c, int a, int b) { log_.push_back("fontColor:" + num(c) + ":" + num(a) + ":" + num(b)); }
unsigned short* textGetString__FPc(const char* k) { log_.push_back(std::string("text:") + k); return !std::strcmp(k, "yes") ? yes_buf : no_buf; }
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int s) {
    log_.push_back("center:" + num(x) + ":" + num(y) + ":" + (t == yes_buf ? "yes" : t == no_buf ? "no" : "?") + ":" + num(s));
}
void fontEnd__Fv() { log_.push_back("fontEnd"); }
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int m, unsigned c) {
    log_.push_back("tex:" + num(t == &text_buf[2]) + ":" + num(x) + ":" + num(y) + ":" + num(m) + ":" + num(c));
}
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned c, const char* l, int b, int d) {
    log_.push_back("border:" + num(x) + ":" + num(y) + ":" + num(w) + ":" + num(h) + ":" + num(a) + ":" + num(c) + ":" + (l ? l : "null") + ":" + num(b) + ":" + num(d));
}
int netPlayerIsLocal__Fi(int i) { return (local_mask >> i) & 1; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float g) {
    ++sounds; log_.push_back("sound:" + num(h == &text_buf[2]) + ":" + num(o == 0) + ":" + num(g == 1.0f)); return 0;
}
void coreInterfaceFlare__Fi(int i) { ++flares; flare_arg = i; }
}
static void reset() {
    log_.clear(); frontEndRunning = 0; mediumFont = &font_a; local_mask = 0; flares = 0; sounds = 0; flare_arg = -9;
    for (int i = 0; i < 4; ++i) { coreInterfaceBrightness[i] = 1.0f; yesNoPos[i] = 0; activeDelay[i] = 0; pad[i].word12 = 0; }
}
static std::vector<std::string> expected_draw(int x, int adj, int p, int yo, int pos, bool fe, float br) {
    std::vector<std::string> v;
    int width = adj + 400, top = yo + 150, box = wrap_height + 48, row = top + box;
    if (fe) v.push_back("lump:frontend.lmp:gamefontsmall.fnt");
    v.push_back("fontStart:" + num(fe ? 2 : 1) + ":7:" + num(cs(0x801e6464u, br)) + ":-1");
    v.push_back("wrap:" + num(x + 320) + ":" + num(top) + ":1:" + num(width) + ":-100:580:21");
    (void)p;
    if (pos == 1) v.push_back("fontColor:" + num(cs(0x801e6464u, br * 1.4f)) + ":-1:0");
    v.push_back("text:yes");
    v.push_back("center:" + num(x + 255) + ":" + num(row - 32) + ":yes:1000000");
    v.push_back("fontColor:" + num(cs(0x801e6464u, pos != 0 ? br : br * 1.4f)) + ":-1:0");
    v.push_back("text:no");
    v.push_back("center:" + num(x + 377) + ":" + num(row - 32) + ":no:1000000");
    v.push_back("fontEnd");
    const char* lump = fe ? "lump:frontend.lmp:button.tex" : "lump:hud.lmp:button.tex";
    unsigned s1 = pos == 1 ? 200 : 128, s2 = pos == 0 ? 200 : 128;
    v.push_back(lump);
    v.push_back("tex:1:" + num(x + 215) + ":" + num(row - 37) + ":7:" + num(cs(s1 * 0x010101u | 0x80000000u, br)));
    v.push_back(lump);
    v.push_back("tex:1:" + num(x + 337) + ":" + num(row - 37) + ":7:" + num(cs(s2 * 0x010101u | 0x80000000u, br)));
    v.push_back("border:" + num(x - width / 2 + 312) + ":" + num(top - 8) + ":" + num(width + 16) + ":" + num(box + 16) + ":7:" +
                num(cs(0x80808080u, br)) + ":" + (fe ? "frontend.lmp" : "null") + ":0:0");
    return v;
}
static bool draw_ok(int x, int adj, int p, int yo, int pos, bool fe, float br, size_t extra = 0) {
    auto v = expected_draw(x, adj, p, yo, pos, fe, br);
    if (log_.size() != v.size() + extra) { std::printf("size %zu vs %zu\n", log_.size(), v.size() + extra); return false; }
    for (size_t i = 0; i < v.size(); ++i) if (log_[i] != v[i]) { std::printf("[%zu] %s != %s\n", i, log_[i].c_str(), v[i].c_str()); return false; }
    return true;
}
int main() {
    // layout, undecided, three argument sets, both fonts
    for (int fe = 0; fe < 2; ++fe)
        for (int pos = 0; pos < 2; ++pos) {
            reset(); frontEndRunning = fe; yesNoPos[1] = pos; coreInterfaceBrightness[1] = 0.5f;
            int r = hudRunYesNo__FPUsiiii(text_buf, -50, 1, 10, -20);
            if (r != 0 || !draw_ok(10, -50, 1, -20, pos, fe, 0.5f)) return 1;
        }
    reset(); wrap_height = 31; coreInterfaceBrightness[0] = 2.0f; yesNoPos[0] = 1;
    if (hudRunYesNo__FPUsiiii(text_buf, 7, -1, -4, 9) != 0 || !draw_ok(-4, 7, 0, 9, 1, false, 2.0f)) return 2;   // odd width: division rounds toward zero
    wrap_height = 30;
    // delay
    reset(); activeDelay[2] = 2; pad[2].word12 = 0xffffffffu; yesNoPos[2] = 1;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 2, 0, 0) != 0 || activeDelay[2] != 1 || sounds || flares || yesNoPos[2] != 1) return 3;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 2, 0, 0) != 0 || activeDelay[2] != 0 || flares) return 4;
    // movement
    reset(); yesNoPos[3] = 1; pad[3].word12 = 0x2000;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 3, 0, 0) != 0 || yesNoPos[3] != 0 || sounds != 1) return 5;
    if (log_.back() != "sound:1:1:1") return 6;
    reset(); yesNoPos[3] = 0; pad[3].word12 = 0x2000;                    // already there: no sound
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 3, 0, 0) != 0 || yesNoPos[3] != 0 || sounds) return 7;
    reset(); yesNoPos[0] = 0; pad[0].word12 = 0x800000;
    hudRunYesNo__FPUsiiii(text_buf, 0, 0, 0, 0);
    if (yesNoPos[0] != 0 || sounds) return 8;
    reset(); yesNoPos[0] = 1; pad[0].word12 = 0x800000;
    hudRunYesNo__FPUsiiii(text_buf, 0, 0, 0, 0);
    if (yesNoPos[0] != 0 || sounds != 1) return 9;
    reset(); yesNoPos[0] = 0; pad[0].word12 = 0x8000;
    hudRunYesNo__FPUsiiii(text_buf, 0, 0, 0, 0);
    if (yesNoPos[0] != 1 || sounds != 1) return 10;
    reset(); yesNoPos[0] = 0; pad[0].word12 = 0x400000;
    hudRunYesNo__FPUsiiii(text_buf, 0, 0, 0, 0);
    if (yesNoPos[0] != 1 || sounds != 1) return 11;
    reset(); yesNoPos[0] = 1; pad[0].word12 = 0x400000;
    hudRunYesNo__FPUsiiii(text_buf, 0, 0, 0, 0);
    if (yesNoPos[0] != 1 || sounds) return 12;
    // both directions pressed: moves to 0 then back to 1, two sounds
    reset(); yesNoPos[0] = 1; pad[0].word12 = 0x802000 | 0x408000;
    hudRunYesNo__FPUsiiii(text_buf, 0, 0, 0, 0);
    if (yesNoPos[0] != 1 || sounds != 2) return 13;
    // accept
    reset(); yesNoPos[1] = 1; pad[1].word12 = 0x4000;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 1, 0, 0) != 2 || yesNoPos[1] != 0 || activeDelay[1] != 3 || flares != 1 || flare_arg != 1) return 14;
    reset(); yesNoPos[1] = 0; pad[1].word12 = 0x80;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 1, 0, 0) != 1 || yesNoPos[1] != 0 || activeDelay[1] != 3) return 15;
    reset(); padAcceptButton = 0x10000; pad[1].word12 = 0x4000;           // the accept mask is padAcceptButton, not a fixed bit
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 1, 0, 0) != 0 || flares) return 16;
    pad[1].word12 = 0x10000;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, 1, 0, 0) != 1) return 17;
    padAcceptButton = 0x4000;
    // any local player (-1): only local pads count, flare gets index 0
    reset(); local_mask = 0x5; pad[1].word12 = 0x4000; pad[2].word12 = 0;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, -1, 0, 0) != 0) return 18;     // player 1 is not local
    pad[2].word12 = 0x4000; yesNoPos[0] = 1;
    if (hudRunYesNo__FPUsiiii(text_buf, 0, -1, 0, 0) != 2 || flare_arg != 0 || activeDelay[0] != 3) return 19;
    reset(); local_mask = 0xf; nmPlayers = 2; pad[3].word12 = 0x4000;       // only nmPlayers entries are scanned
    if (hudRunYesNo__FPUsiiii(text_buf, 0, -1, 0, 0) != 0) return 20;
    nmPlayers = 4;
    std::puts("hudRunYesNo__FPUsiiii check passed");
    return 0;
}
