#include <cstdio>
#include <cstring>
#include "front_end_words.cpp"

static int fail(int code) {
    std::printf("frontend words check failed: %d\n", code);
    return code;
}

int main() {
    if (sizeof frontMenu != 5 * sizeof(norrath_frontend_record)) return fail(1);
    if (sizeof(void*) == 4 && (sizeof frontMenu != 80 || sizeof creditMenu != 16)) return fail(2);
    const char* labels[4] = {"menu_startnewgame", "menu_loadsavedgame", "menu_options", "menu_credits"};
    for (int index = 0; index < 4; ++index) {
        if (std::strcmp(frontMenu[index].label, labels[index]) != 0) return fail(4 + index);
    }
    if (frontMenu[4].label != 0 || creditMenu[0].label != 0) return fail(8);
    if (creditsPosY != 448) return fail(9);
    if (frontEndRunning != 0 || engineHoldMpeg != 0 || autodemoTimer != 0 || lineShift != 0) return fail(10);
    if (frontEndSuspend != 0 || frontEndStartInCredits != 0 || frontendControllerConfigRunning != 0) return fail(11);
    if (attemptLoadGame != 0 || expWarnAlpha != 0.0f || frontEndWasRunning != 0) return fail(12);
    return 0;
}
