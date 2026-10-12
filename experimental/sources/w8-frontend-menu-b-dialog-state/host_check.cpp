#include "dialog_state.cpp"

#include <cstdio>

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(showOptions == 0 && showMore == 0 && optionIndex == 0 && scrollTarget == 0 && scrollY == 0 &&
          scrollEnd == 0 && nameChars == 0 && optionCount == 0 && scrollSpan == 0.0f && optionsShowing == 0);
    CHECK(sizeof optionLinePos == 48);
    for (int i = 0; i < 12; ++i) CHECK(optionLinePos[i] == 0);
    if (failures == 0) std::printf("dialog_state ok\n");
    return failures != 0;
}
