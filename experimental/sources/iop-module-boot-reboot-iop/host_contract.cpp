#include "reboot_iop.cpp"

#include <cstdio>
#include <cstring>

#include <cstdarg>

static int prints;
static char printed[160];
static const char* printed_arg;
static int inits;
static int init_mode;
static int exits;
static int resets;
static char command[96];
static int reset_argument;
static int reset_result = 15;

extern "C" int scePrintf(const char* text, ...) {
    ++prints;
    std::snprintf(printed, sizeof printed, "%s", text);
    va_list args;
    va_start(args, text);
    printed_arg = va_arg(args, const char*);
    va_end(args);
    return 1;
}
extern "C" void sceSifInitRpc(int mode) {
    ++inits;
    init_mode = mode;
}
extern "C" void sceSifExitRpc() { ++exits; }
extern "C" int sceSifResetIop(const char* text, int argument) {
    ++resets;
    std::snprintf(command, sizeof command, "%s", text);
    reset_argument = argument;
    return reset_result;
}

int main() {
    if (sceSifRebootIop("abc") != 15 || inits != 1 || init_mode != 0 || exits != 1 || resets != 1) return 1;
    if (std::strcmp(command, "rom0:UDNL abc") != 0 || reset_argument != 0 || prints != 0) return 2;
    if (sceSifRebootIop("") != 15 || std::strcmp(command, "rom0:UDNL ") != 0) return 3;
    char long_name[80];
    for (int index = 0; index < 69; ++index) long_name[index] = 'a';
    long_name[69] = 0;
    resets = 0;
    if (sceSifRebootIop(long_name) != 15 || resets != 1) return 4;
    if (std::strncmp(command, "rom0:UDNL ", 10) != 0 || std::strlen(command) != 79) return 5;
    long_name[69] = 'b';
    long_name[70] = 0;
    inits = 0;
    exits = 0;
    resets = 0;
    if (sceSifRebootIop(long_name) != 0 || inits != 0 || exits != 0 || resets != 0 || prints != 1) return 6;
    if (std::strcmp(printed, "too long parameter '%s'\n") != 0 || printed_arg != long_name) return 7;
    std::puts("sceSifRebootIop passed");
    return 0;
}
