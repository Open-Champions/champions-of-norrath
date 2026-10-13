// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp cd_st_resume.cpp
#include <cstring>
extern "C" {
int SCE_CD_debug = 0;
int stm_status = 4;
unsigned char dum_mode[4];
static int prints, order, print_at, store_seen_at_print = -1;
static const char* printed;
int norrath_sce_printf(const char* format, ...) {
    ++prints; printed = format; print_at = ++order; store_seen_at_print = stm_status;
    return 0;
}
static int g_arg, g_count, g_cmd, calls;
static void *g_buffer, *g_mode;
int sceCdStream(int argument, int count, void* buffer, int command, void* mode) {
    ++calls; ++order; g_arg = argument; g_count = count; g_buffer = buffer; g_cmd = command; g_mode = mode;
    return 41;
}
int sceCdStResume();
}
int main() {
    if (sceCdStResume() != 41 || prints != 0 || stm_status != 1) return 1;
    if (g_arg != 0 || g_count != 0 || g_buffer != nullptr || g_cmd != 8 || g_mode != dum_mode) return 2;
    SCE_CD_debug = 0; stm_status = 0;
    if (sceCdStResume() != 41 || prints != 0 || stm_status != 1) return 3;          // zero does not print
    SCE_CD_debug = -3; stm_status = 0;
    if (sceCdStResume() != 41 || prints != 0) return 4;                              // signed compare
    SCE_CD_debug = 1; stm_status = 0; order = 0;
    if (sceCdStResume() != 41 || prints != 1 || print_at != 1 || order != 2) return 5;  // print before the stream call
    if (store_seen_at_print != 1) return 6;                                          // status stored before the print
    if (std::strcmp(printed, "sceCdStResume call\n") != 0) return 7;
    return 0;
}
