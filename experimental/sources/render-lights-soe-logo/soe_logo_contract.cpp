// Host contract for displaySOELogo__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char pad[352];
extern "C" int frameNm;
extern "C" int skipIntro;
extern "C" int displaySOELogo__Fv();

unsigned char pad[352];
int frameNm = 0;
int skipIntro = 0;

static int failures = 0;
static int finds = 0;
static int draws = 0;
static char lumps[2][32];
static char names[2][32];
static char* found_queue[2];
static int found_index = 0;
static char* drawn_texture[2];
static int drawn_x[2];
static int drawn_y[2];
static int drawn_mode[2];
static unsigned drawn_color[2];

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    int slot = finds++;
    if (slot < 2) {
        std::snprintf(lumps[slot], sizeof lumps[slot], "%s", lump);
        std::snprintf(names[slot], sizeof names[slot], "%s", name);
    }
    char* result = found_index < 2 ? found_queue[found_index] : nullptr;
    ++found_index;
    return result;
}

extern "C" void textureDrawColor__FP8_textureiiiUi(char* texture, int x, int y, int mode, unsigned color) {
    int slot = draws++;
    if (slot < 2) {
        drawn_texture[slot] = texture;
        drawn_x[slot] = x;
        drawn_y[slot] = y;
        drawn_mode[slot] = mode;
        drawn_color[slot] = color;
    }
}

int main() {
    char url = 0;
    char splash = 0;
    found_queue[0] = &url;
    found_queue[1] = &splash;
    std::memset(pad, 0, sizeof pad);
    frameNm = 0;
    skipIntro = 3;
    int result = displaySOELogo__Fv();
    expect(result == 0 && skipIntro == 3, "quiet pad returns 0");
    expect(finds == 2 && draws == 2, "two finds and two draws");
    expect(std::strcmp(lumps[0], "soelogo.lmp") == 0 && std::strcmp(lumps[1], "soelogo.lmp") == 0, "soe lump");
    expect(std::strcmp(names[0], "soe_splash_url.tex") == 0, "url texture");
    expect(std::strcmp(names[1], "soe_splash.tex") == 0, "splash texture");
    expect(drawn_texture[0] == &url && drawn_x[0] == 192 && drawn_y[0] == 330, "url placement");
    expect(drawn_texture[1] == &splash && drawn_x[1] == 64 && drawn_y[1] == 50, "splash placement");
    expect(drawn_mode[0] == 3 && drawn_mode[1] == 3, "draw mode");
    expect(drawn_color[0] == 0x80808080u && drawn_color[1] == 0x80808080u, "draw color");

    pad[12] = 2;
    frameNm = 300;
    skipIntro = 0;
    result = displaySOELogo__Fv();
    expect(result == 1 && skipIntro == 1, "pad+12 sets skipIntro");

    std::memset(pad, 0, sizeof pad);
    pad[100] = 9;
    frameNm = -4;
    skipIntro = 0;
    result = displaySOELogo__Fv();
    expect(result == 1 && skipIntro == 1, "negative frame still inspects pad");

    frameNm = 301;
    skipIntro = 8;
    pad[12] = 1;
    result = displaySOELogo__Fv();
    expect(result == 1 && skipIntro == 8, "frame 301 leaves skipIntro");
    expect(draws == 8, "draws still run after the frame gate");
    if (failures != 0) return 1;
    std::printf("soe-logo contract passed\n");
    return 0;
}
