#include <cstdio>

#define NORRATH_HOST_CONTRACT 1
#include "set_display_disable.cpp"

extern "C" {
int engineDisplayDisable;
}

namespace {
unsigned failures;
void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
}  // namespace

int main() {
    engineDisplayDisable = 9;
    engineSetDisplayDisable__Fi(-4);
    expect(engineDisplayDisable == -4, "negative value is stored");
    engineSetDisplayDisable__Fi(2);
    expect(engineDisplayDisable == 2, "the value simpleDrawLoop passes first");
    engineSetDisplayDisable__Fi(1);
    expect(engineDisplayDisable == 1, "the value after the loop");
    engineSetDisplayDisable__Fi(0);
    expect(engineDisplayDisable == 0, "zero is stored");
    if (failures != 0) return 1;
    std::printf("display-disable checks passed\n");
    return 0;
}
