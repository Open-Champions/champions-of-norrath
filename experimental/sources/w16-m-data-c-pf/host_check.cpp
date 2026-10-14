// Host check for pf. Not an EE plan input. It includes the definition file and supplies a
// recording vprintf, so the check does not need the libc output path.
#include <cstring>
#include "pf.cpp"

namespace {
int calls;
char format_seen[64];
int int_seen;
char string_seen[16];
}

extern "C" int vprintf(const char* format, __builtin_va_list arguments) {
    ++calls;
    std::strncpy(format_seen, format, sizeof format_seen - 1);
    if (std::strcmp(format, "n=%d s=%s") == 0) {
        int_seen = __builtin_va_arg(arguments, int);
        std::strncpy(string_seen, __builtin_va_arg(arguments, const char*), sizeof string_seen - 1);
    }
    return 0;
}

int main() {
    pf("n=%d s=%s", 7, "ab");
    if (calls != 1) return 1;
    if (std::strcmp(format_seen, "n=%d s=%s") != 0) return 2;
    if (int_seen != 7) return 3;
    if (std::strcmp(string_seen, "ab") != 0) return 4;

    pf("plain\n");
    if (calls != 2) return 5;
    if (std::strcmp(format_seen, "plain\n") != 0) return 6;
    return 0;
}
