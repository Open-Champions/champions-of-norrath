#include "jump_gate_task_draw.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Font { int x; };
static Font font_object;
Font* smallFont = &font_object;
int flareAlpha = 255, thisLevel = 1, jumping = 1;
int padAcceptButtonFontChar = 7, padBackButtonFontChar = 9;
float cursorTime = 0.5f;
signed char jumpGateDestCount = 2, lastDest = 0, curDest = 1;
char jumpGateEntries[480];
unsigned short hudFooterUni[64];
static int task_active;
static std::vector<std::string> events;
static unsigned short good[8] = {'a', 'b', 'x', 0}, colon[8] = {'a', 'b', ':', 0};
static int colon_budget;
static float hermite_result = 0.25f;
static float last_us_a, last_us_b;
static int jump_calls, rect_top, rect_bottom, rect_color;
static unsigned last_border_color;

extern "C" {
int engineTaskActive__FPc(const char*) { return task_active; }
void textureDrawBorder__FiiiiiUiPcii(int, int, int, int, int, unsigned c, char*, int, int) {
    last_border_color = c;
    events.push_back("border");
}
void fontStart__FP4FontiUii(Font*, int, unsigned, int) { events.push_back("start"); }
void fontColor__FUiii(unsigned c, int, int) { events.push_back("color" + std::to_string(c >> 24)); }
void fontEnd__Fv() { events.push_back("end"); }
unsigned short* textGetString__FPc(const char* key) {
    events.push_back(std::string("text ") + key);
    if (colon_budget > 0) { --colon_budget; return colon; }
    return good;
}
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int) {
    events.push_back("print " + std::to_string(x) + "," + std::to_string(y) + (t == colon ? "c" : "g"));
}
float evalHermite__Ffffff(float t, float, float, float, float) { return hermite_result + 0 * t; }
void rectangleDraw__FiiiiiUi(int, int y0, int, int y1, int, unsigned c) {
    rect_top = y0; rect_bottom = y1; rect_color = static_cast<int>(c >> 24);
}
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float a, float b, float, float,
                              float, float, float, float) {
    last_us_a = a; last_us_b = b;
}
void hudShowMsg__FPUsN20iii(unsigned short*, unsigned short*, unsigned short*, int, int, int) {
    events.push_back("show");
}
void doJumpEffect__Fi(int) { ++jump_calls; }
}

int main() {
    std::strcpy(jumpGateEntries, "ab");
    std::strcpy(jumpGateEntries + 32, "cd");
    task_active = 1;
    jumpGateTaskDraw__Fv();
    if (!events.empty() || jump_calls) return 1;
    task_active = 0;
    colon_budget = 1;  // entry 0: first lookup ends in ':', so one character is dropped
    jumpGateTaskDraw__Fv();
    if (events.empty() || events[0] != "border" || events[1] != "start") return 2;
    // entry 0 (not the current level): "ab" is looked up, ends in ':', the last character is
    // dropped, the shortened name is under two characters so the loop ends and the colon
    // text is the one printed
    if (events[2] != "text ab" || events[3] != "print 320,122c") return 3;
    // entry 1 is the current level: colour change, lookup, print, colour change
    if (events[4] != "color255" || events[5] != "text cd" || events[6] != "print 320,143g" || events[7] != "color255")
        return 4;
    if (events[8] != "end" || events[9] != "text Back" || events[10] != "text JumpGateTitle" || events[11] != "show")
        return 5;
    if (last_us_a != 9.0f || rect_color != 127) return 6;
    // lastDest 0 and curDest 1: from 125, delta 21, hermite 0.25
    if (rect_top != 130 || rect_bottom != 153) return 7;
    if (last_border_color != 0xff808080u || jump_calls != 1) return 8;
    events.clear();
    curDest = 0;
    flareAlpha = 0;
    jumping = 0;
    jumpGateTaskDraw__Fv();
    if (!events.empty() || jump_calls != 1) return 9;
    flareAlpha = 4;
    jumping = 0;
    jumpGateTaskDraw__Fv();
    bool accept = false;
    for (auto& s : events) accept = accept || s == "text AcceptBack";
    if (!accept || last_us_a != 7.0f || last_us_b != 9.0f) return 10;
    std::puts("jump gate task draw passed");
    return 0;
}
