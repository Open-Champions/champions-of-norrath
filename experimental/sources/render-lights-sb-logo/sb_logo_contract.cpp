// Host contract for displaySBLogoScreen__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char pad[352];
extern "C" int frameNm;
extern "C" int skipIntro;
extern "C" int displaySBLogoScreen__Fv();

unsigned char pad[352];
int frameNm = 0;
int skipIntro = 0;

static int failures = 0;
static int finds = 0;
static int draws = 0;
static char lump_text[32];
static char name_text[32];
static char* found = nullptr;
static char* drawn = nullptr;
static int drawn_x = -1;
static int drawn_y = -1;
static int drawn_mode = -1;
static unsigned drawn_color = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    ++finds;
    std::snprintf(lump_text, sizeof lump_text, "%s", lump);
    std::snprintf(name_text, sizeof name_text, "%s", name);
    return found;
}

extern "C" void textureDrawColor__FP8_textureiiiUi(char* texture, int x, int y, int mode, unsigned color) {
    ++draws;
    drawn = texture;
    drawn_x = x;
    drawn_y = y;
    drawn_mode = mode;
    drawn_color = color;
}

static void clear_pad() {
    std::memset(pad, 0, sizeof pad);
}

int main() {
    char texture = 0;
    found = &texture;
    clear_pad();
    frameNm = 301;
    skipIntro = 5;
    pad[12] = 1;
    int result = displaySBLogoScreen__Fv();
    expect(result == 1 && skipIntro == 5, "frame 301 returns 1 without skipIntro");
    expect(finds == 1 && std::strcmp(lump_text, "sblogo.lmp") == 0, "sb lump");
    expect(std::strcmp(name_text, "snowlogo.tex") == 0, "snow texture");
    expect(draws == 1 && drawn == &texture && drawn_x == 0 && drawn_y == 0, "draw origin");
    expect(drawn_mode == 3 && drawn_color == 0x80808080u, "draw mode and color");

    clear_pad();
    frameNm = 300;
    skipIntro = 7;
    result = displaySBLogoScreen__Fv();
    expect(result == 0 && skipIntro == 7, "quiet pad returns 0");

    clear_pad();
    pad[12] = 1;
    frameNm = 300;
    skipIntro = 0;
    result = displaySBLogoScreen__Fv();
    expect(result == 1 && skipIntro == 1, "pad+12 sets skipIntro");

    clear_pad();
    pad[103] = 0x80;
    frameNm = 300;
    skipIntro = 0;
    result = displaySBLogoScreen__Fv();
    expect(result == 1 && skipIntro == 1, "pad+100 high bit sets skipIntro");

    clear_pad();
    frameNm = -1;
    skipIntro = 4;
    result = displaySBLogoScreen__Fv();
    expect(result == 0 && skipIntro == 4, "negative frame still inspects pad");

    frameNm = 1000;
    pad[100] = 1;
    skipIntro = 9;
    result = displaySBLogoScreen__Fv();
    expect(result == 1 && skipIntro == 9, "later frame leaves skipIntro");
    if (failures != 0) return 1;
    std::printf("sb-logo contract passed\n");
    return 0;
}
