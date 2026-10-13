// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp cd_st_start.cpp
extern "C" {
int stm_status = 4;
static int calls, g_arg, g_count, g_cmd, g_result;
static void *g_buffer, *g_mode;
int sceCdStream(int argument, int count, void* buffer, int command, void* mode) {
    ++calls; g_arg = argument; g_count = count; g_buffer = buffer; g_cmd = command; g_mode = mode;
    g_buffer = buffer;
    return g_result;
}
int sceCdStStart(int sector, void* mode);
}
int main() {
    static char mode[4];
    g_result = 33;
    if (sceCdStStart(0x1234, mode) != 33) return 1;
    if (calls != 1 || g_arg != 0x1234 || g_count != 0 || g_buffer != nullptr || g_cmd != 1 || g_mode != mode) return 2;
    if (stm_status != 1) return 3;
    stm_status = 0; g_result = 0;
    if (sceCdStStart(-7, nullptr) != 0 || g_arg != -7 || g_mode != nullptr || stm_status != 1) return 4;
    g_result = -1;
    if (sceCdStStart(5, mode) != -1) return 5;
    return 0;
}
