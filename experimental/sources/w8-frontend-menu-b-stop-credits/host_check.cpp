#include "stop_credits.cpp"

#include <cstdio>
#include <cstring>

extern "C" {
int creditsPosY = 5;
int endTimer = 9;
int endInit = 1;
int lineShift = 7;
unsigned char frontEndStartInCredits = 1;
int menuNm = 60;
unsigned char* menu = nullptr;
unsigned char frontMenu[80];
}

static int calls;
static int fade_stream = -1;
static char music_name[40];
static int music_mode = -1;
static int fade_before_music;

extern "C" void soundFadeStream__Fi(int stream) {
    fade_stream = stream;
    fade_before_music = (calls++ == 0);
}
extern "C" void soundStartGlobalMusic__FPci(char* name, int mode) {
    std::strncpy(music_name, name, sizeof music_name - 1);
    music_mode = mode;
    calls++;
    // The scroll state is already reset by the time the music starts.
    if (creditsPosY != 448 || endTimer != 1 || frontEndStartInCredits != 0) music_mode = -2;
}

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

int main() {
    stopCredits__Fv();
    CHECK(fade_stream == 0);
    CHECK(fade_before_music == 1);
    CHECK(calls == 2);
    CHECK(creditsPosY == 448);
    CHECK(endTimer == 1);
    CHECK(endInit == 0);
    CHECK(lineShift == 0);
    CHECK(frontEndStartInCredits == 0);
    CHECK(std::strcmp(music_name, "mx_mainmenu_l.vag") == 0);
    CHECK(music_mode == 1);
    CHECK(menuNm == 0);
    CHECK(menu == frontMenu);
    if (failures == 0) std::printf("stop_credits ok\n");
    return failures != 0;
}
