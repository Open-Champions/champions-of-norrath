#define NORRATH_HOST_CONTRACT 1
#define strlen norrath_host_strlen
#include "set_arg.cpp"
#undef strlen

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>

unsigned _kExecArg = 0;
unsigned kernel_setarg_sysentry[2] = {0x5au, 0xabcd0000u};
static char exec_buffer[1024];
static int setup_calls;
static unsigned setup_service, setup_handler;
static int copy_calls;
extern "C" char* norrath_host_exec_buffer() { return exec_buffer; }
extern "C" unsigned norrath_host_strlen(const char* s) { return static_cast<unsigned>(std::strlen(s)); }
extern "C" unsigned long kernel_setarg_setup(unsigned service, unsigned handler) {
    ++setup_calls; setup_service = service; setup_handler = handler;
    return 0;
}
extern "C" unsigned long kernel_setarg_copy(void* d, const void* s, unsigned n) {
    if (setup_calls == 0) return 1;           // the copy service is only installed by setup
    ++copy_calls;
    std::memcpy(d, s, n);
    return 0;
}

static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned word_at(int offset) { unsigned v; std::memcpy(&v, exec_buffer + offset, 4); return v; }

int main() {
    char a[] = "one", b[] = "second";
    char* argv[20];
    // Two arguments.
    argv[0] = a; argv[1] = b;
    std::memset(exec_buffer, 0xee, sizeof exec_buffer);
    if (SetArg("path", 2, argv) != low(exec_buffer + 64)) return 1;
    if (setup_calls != 1 || setup_service != 0x5au || setup_handler != 0xabcd0000u) return 2;
    if (std::strcmp(exec_buffer + 64, "path") != 0) return 3;
    if (std::strcmp(exec_buffer + 69, "one") != 0 || std::strcmp(exec_buffer + 73, "second") != 0) return 4;
    if (word_at(0) != low(exec_buffer + 64) || word_at(4) != low(exec_buffer + 69) || word_at(8) != low(exec_buffer + 73)) return 5;
    if (static_cast<unsigned char>(exec_buffer[12]) != 0xee) return 6;          // no count word
    if (copy_calls != 3 + 3) return 7;
    // No arguments, including a negative count.
    for (int count : {0, -4}) {
        std::memset(exec_buffer, 0xee, sizeof exec_buffer);
        copy_calls = 0;
        SetArg("x", count, argv);
        if (word_at(0) != low(exec_buffer + 64) || std::strcmp(exec_buffer + 64, "x") != 0) return 8;
        if (static_cast<unsigned char>(exec_buffer[4]) != 0xee || copy_calls != 2) return 9;
    }
    // Large counts are limited to 15.
    char names[20][2];
    for (int i = 0; i < 20; ++i) { names[i][0] = static_cast<char>('A' + i); names[i][1] = 0; argv[i] = names[i]; }
    std::memset(exec_buffer, 0xee, sizeof exec_buffer);
    copy_calls = 0;
    SetArg("p", 20, argv);
    if (copy_calls != 2 + 2 * 15) return 10;
    if (word_at(60) == 0xeeeeeeeeu || static_cast<unsigned char>(exec_buffer[64 + 2 + 2 * 15 + 1]) != 0xee) return 11;
    if (std::strcmp(exec_buffer + 66 + 2 * 14, "O") != 0) return 12;
    // 16 is limited too, 15 is kept.
    copy_calls = 0; SetArg("p", 16, argv); if (copy_calls != 32) return 13;
    copy_calls = 0; SetArg("p", 15, argv); if (copy_calls != 32) return 14;
    copy_calls = 0; SetArg("p", 14, argv); if (copy_calls != 30) return 15;
    std::puts("set arg contract passed");
    return 0;
}
