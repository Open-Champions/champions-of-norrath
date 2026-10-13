// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -o /tmp/c host_contract.cpp lgaud_enum_hint.cpp
#include <cstdarg>
#include <cstdio>
#include <cstring>
extern "C" {
int norrath_lgaud_semaphore_view = 5;
unsigned* norrath_lgaud_reply_words_view;
static char printed[128]; static int prints;
int printf(const char* format, ...) {
    va_list a; va_start(a, format); std::vsnprintf(printed, sizeof printed, format, a); va_end(a); ++prints; return 0;
}
unsigned lgAudEnumHint(unsigned* hint);
}
int main() {
    unsigned reply[4] = {0x1234, 0x5678, 0, 0}, hint = 0xaaaa;
    norrath_lgaud_reply_words_view = reply;
    norrath_lgaud_semaphore_view = -1;
    if (lgAudEnumHint(&hint) != 0x8000000au || hint != 0xaaaa || reply[0] != 0x1234 || prints != 0) return 1;
    norrath_lgaud_semaphore_view = 5;
    if (lgAudEnumHint(nullptr) != 0x80000004u || std::strcmp(printed, "lgAudEnumHint: invalid parameter (hint)\n") != 0 || reply[0] != 0x1234) return 2;
    if (lgAudEnumHint(&hint) != 0 || hint != 0x1234 || reply[0] != 0 || reply[1] != 0x5678) return 3;   // only word 0 is taken and cleared
    if (lgAudEnumHint(&hint) != 0 || hint != 0) return 4;                                                 // cleared word reads back zero
    return 0;
}
