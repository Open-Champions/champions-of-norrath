#include "session_id.cpp"

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
    sSessionId = 0x1122334455667788LL;
    VISetMcSessionId__FUl(0x1234u);
    if (print_calls != 1 || std::strcmp(printed, "VISetMcSessionId()\n") != 0) return 1;
    if (sSessionId != 0x1234) return 2;
    VISetMcSessionId__FUl(0);
    if (sSessionId != 0) return 3;
    VISetMcSessionId__FUl(0x7fffffffu);
    if (sSessionId != 0x7fffffff) return 4;
    if (print_calls != 3) return 5;
    std::puts("mc session-id passed");
    return 0;
}
