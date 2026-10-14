#include "same_session.cpp"

#include <cstdio>
#include <cstring>

static int print_calls;
static char printed[64];

extern "C" int scePrintf(const char* format, ...) {
    ++print_calls;
    std::strncpy(printed, format, sizeof printed - 1);
    return 0;
}

int main() {
    if (VIIsMcSameSession__Fv() != 1) return 1;
    if (print_calls != 1 || std::strcmp(printed, "VIIsMcSameSessionId()\n") != 0) return 2;
    if (VIIsMcSameSession__Fv() != 1 || print_calls != 2) return 3;
    std::puts("mc same-session passed");
    return 0;
}
