// Host contract for cutDrawFrame__Fv. Not an EE plan input.
#include "cut_draw_frame.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

signed char coreLoadingScreenRunning;
int frontEndRunning, engineScissory1, engineScissory2, engineScreenSlide;
unsigned char cutInit;
int displayFrame, musicSize;
char levelName[200];
unsigned char* dialogPacket;
int displayDialogPos, nmDialogNodes;
void* smallFont;
int displayTextDisable, subtitlesOn, displayActionPos, nmActions;
unsigned char* action;

static std::string ev;
static int print_result;
struct Print { int x, y, w, a, b, c; unsigned short* text; };
static std::vector<Print> prints;
static unsigned short text_a[4], text_b[4];
static long last_key;
static int scissors;

static std::string fmt(const char* name, long a = 0, long b = 0, long c = 0, long d = 0) {
    char buf[128];
    std::snprintf(buf, sizeof buf, "%s(%ld,%ld,%ld,%ld) ", name, a, b, c, d);
    return buf;
}

extern "C" {
void resetScissor__Fv() { ev += fmt("reset"); }
void fontStart__FP4FontiUii(void* f, int s, unsigned color, int e) { ev += fmt("start", f == smallFont, s, color, e); }
unsigned short* textGetString__FPc(char* key) {
    last_key = (long)(key - (char*)dialogPacket);
    ev += fmt("text", last_key);
    return (last_key & 1) ? text_a : text_b;
}
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int w, int a, int b, int c) {
    prints.push_back({x, y, w, a, b, c, t});
    ev += fmt("print", x, y, w, a * 10000 + b) + fmt("c", c);
    return print_result;
}
void fontEnd__Fv() { ev += fmt("end"); }
void fontScissor__Fiiii(int a, int b, int c, int d) { ++scissors; ev += fmt("scissor", a, b, c, d); }
void hudLetterBox__Fv() { ev += fmt("letterbox"); }
}

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}

struct Act { float t; short h4, h6; int pad[3]; };
static_assert(sizeof(Act) == 20, "record size");

static void set_level(bool intro) {
    std::memset(levelName, 0, sizeof levelName);
    std::strcpy(levelName + 81, intro ? "actintro" : "other");
}

int main() {
    static unsigned char packet[1096 * 12];
    dialogPacket = packet;
    smallFont = &failures;
    static Act acts[12];
    action = reinterpret_cast<unsigned char*>(acts);

    // Early exits and scissor values.
    set_level(false);
    coreLoadingScreenRunning = 1; ev.clear(); frontEndRunning = 0;
    cutDrawFrame__Fv();
    expect(ev.empty(), "loading screen: nothing at all");
    coreLoadingScreenRunning = 0; cutInit = 0; engineScreenSlide = 7;
    cutDrawFrame__Fv();
    expect(ev == fmt("reset"), "cutInit zero stops after the scissor reset, no letterbox");
    expect(engineScissory1 == 51 && engineScissory2 == 382, "scissor values follow the slide");
    frontEndRunning = 1; engineScissory1 = engineScissory2 = 5; ev.clear(); cutInit = 1;
    subtitlesOn = 0; displayFrame = 0; musicSize = 0;
    cutDrawFrame__Fv();
    expect(engineScissory1 == 5 && engineScissory2 == 5, "front end leaves scissor words alone");
    expect(ev == fmt("reset"), "front end, nothing to draw, no letterbox");
    frontEndRunning = 0; ev.clear();
    cutDrawFrame__Fv();
    expect(ev == fmt("reset") + fmt("letterbox"), "letterbox when not in the front end");

    // Long-text block.
    set_level(true);
    musicSize = 50000; displayFrame = 600; displayDialogPos = 3; nmDialogNodes = 5; subtitlesOn = 1; displayTextDisable = 0;
    print_result = 100;
    prints.clear(); ev.clear();
    cutDrawFrame__Fv();
    expect(prints.size() == 2, "two prints for the long text");
    if (prints.size() == 2) {
        expect(prints[0].x == 320 && prints[0].y == 473 - 300 && prints[0].w == 620 && prints[0].a == -100 && prints[0].b == -100 && prints[0].c == 33, "measure print");
        const float rate = float(342000 + 100 * 855) / 50000.0f;
        expect(prints[1].y == 428 - int(600.0f * rate) && prints[1].a == -100 && prints[1].b == 580 && prints[1].c == 33, "scrolled print");
    }
    expect(displayTextDisable == 1, "text disabled afterwards");
    expect(ev.find("start(1,5,2153410179,-1)") != std::string::npos, "first font style");
    expect(ev.find("text(" + std::to_string(1096 * 3 - 1063) ) != std::string::npos || ev.find("text(2225") != std::string::npos, "text key offset");
    expect(ev.rfind("letterbox") == ev.size() - fmt("letterbox").size(), "letterbox last");
    // Subtitles stay off while the long text just disabled them, and for the intro level in general.
    expect(ev.find("scissor") == std::string::npos, "no scroll scissor");
    // Gate conditions for the long block.
    for (int gate = 0; gate < 6; ++gate) {
        musicSize = 50000; displayFrame = 600; displayDialogPos = 3; nmDialogNodes = 5; displayTextDisable = 0; subtitlesOn = 0;
        set_level(true);
        if (gate == 0) displayFrame = 1;
        if (gate == 1) musicSize = 40000;
        if (gate == 2) set_level(false);
        if (gate == 3) displayDialogPos = 0;
        if (gate == 4) displayDialogPos = 6;
        if (gate == 5) displayDialogPos = 5;
        prints.clear(); ev.clear();
        cutDrawFrame__Fv();
        const bool should = gate == 5;
        expect((prints.size() == 2) == should, "long text gate");
    }

    // Subtitle block, short text.
    set_level(false);
    displayDialogPos = 2; nmDialogNodes = 4; displayTextDisable = 0; subtitlesOn = 1; print_result = 40;
    prints.clear(); ev.clear();
    cutDrawFrame__Fv();
    expect(prints.size() == 2, "measure then draw");
    if (prints.size() == 2) {
        expect(prints[0].y == 390 && prints[0].a == -100 && prints[0].b == -90 && prints[0].c == 18, "subtitle measure");
        expect(prints[1].y == 388 && prints[1].a == -100 && prints[1].b == 580 && prints[1].c == 18, "short subtitle");
    }
    expect(ev.find("start(1,7,2153405018,-1)") != std::string::npos, "subtitle style");
    expect(scissors == 0, "no scissor for short text");
    // Gate conditions for subtitles.
    for (int gate = 0; gate < 7; ++gate) {
        set_level(false); displayDialogPos = 2; nmDialogNodes = 4; displayTextDisable = 0; subtitlesOn = 1; dialogPacket = packet;
        if (gate == 0) subtitlesOn = 0;
        if (gate == 1) dialogPacket = nullptr;
        if (gate == 2) displayTextDisable = 1;
        if (gate == 3) displayDialogPos = 0;
        if (gate == 4) displayDialogPos = 5;
        if (gate == 5) set_level(true), musicSize = 0;
        prints.clear(); ev.clear();
        cutDrawFrame__Fv();
        expect((prints.size() == 2) == (gate == 6), "subtitle gate");
    }
    dialogPacket = packet;

    // Scrolling subtitles: markers carry halfword -99 and 0.
    const float times[10] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45};
    for (int i = 0; i < 10; ++i) {
        acts[i].t = times[i];
        acts[i].h4 = (i % 2 == 0) ? -99 : 0;
        acts[i].h6 = 0;
    }
    acts[4].h6 = 1;   // looks like a marker but is not
    nmActions = 10;
    set_level(false); displayDialogPos = 2; nmDialogNodes = 4; displayTextDisable = 0; subtitlesOn = 1; print_result = 100;
    int worst = 0;
    for (int frame = 0; frame <= 2700; frame += 37) {
        const double t = frame / 60.0;
        int natural = 0;
        while (natural < 10 && times[natural] < t) ++natural;
        for (int start : {0, natural}) {
            displayFrame = frame; displayActionPos = start;
            prints.clear(); ev.clear(); scissors = 0;
            cutDrawFrame__Fv();
            // Reference: previous marker at or before t, next marker after t.
            double prev = -1, next = -1;
            for (int i = 0; i < 10; ++i) if (acts[i].h4 == -99 && acts[i].h6 == 0 && times[i] <= t) prev = times[i];
            for (int i = 9; i >= 0; --i) if (acts[i].h4 == -99 && acts[i].h6 == 0 && times[i] >= t + 0.001) next = times[i];
            if (prev < 0) continue;
            if (next < 0) next = times[9];
            const double span = next - prev, elapsed = t - prev, margin = (span - 0.5) * 0.5;
            int want = 0;
            if (!(elapsed < margin)) {
                double p = (elapsed - margin) / (span - 2 * margin);
                p = p < 0 ? 0 : (p > 1 ? 1 : p);
                want = int(p * 46.0);
            }
            expect(scissors == 1 && prints.size() == 2, "scrolling subtitle draws once after a scissor");
            if (prints.size() == 2) {
                expect(prints[1].a == 370 && prints[1].b == 442 && prints[1].c == 18 && prints[1].x == 320, "scroll print arguments");
                const int got = 388 - prints[1].y;
                if (std::abs(got - want) > worst) worst = std::abs(got - want);
                expect(std::abs(got - want) <= 1, "scroll offset");
                if (want == 0 || want == 46) expect(got == want, "clamped offsets are exact");
            }
        }
    }
    expect(ev.find("scissor(0,377,640,433)") != std::string::npos, "scissor rectangle");
    // The scroll starts at a measured height of 54 (extra 43) and not below.
    for (int r : {53, 54}) {
        print_result = r; displayFrame = 1200; displayActionPos = 5; scissors = 0; prints.clear(); ev.clear();
        cutDrawFrame__Fv();
        expect((scissors == 1) == (r == 54), "scroll threshold");
    }
    (void)worst;
    return failures ? 1 : 0;
}
