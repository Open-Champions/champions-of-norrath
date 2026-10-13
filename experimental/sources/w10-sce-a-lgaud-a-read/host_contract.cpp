// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -fno-builtin -o /tmp/c host_contract.cpp lgaud_a_read.cpp
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" {
int norrath_lgaud_semaphore_view = 5;
int norrath_lgaud_size_limit_view = 4096;
unsigned char* norrath_lgaud_request_buffer_view;
unsigned char norrath_lgaud_rpc_client_view[40];
unsigned norrath_lgaud_async_record_view[4];
static char printed[128]; static int prints, rpc_calls, rpc_result, order[16], order_n, wait_id = -9, isig_id = -9, isig_calls, stat_result, stat_calls;
static unsigned rpc_cmd, rpc_mode; static int rpc_ss, rpc_rs; static void *rpc_client, *rpc_send, *rpc_recv; static void (*rpc_end)(void*); static void* rpc_param;
static unsigned char at_call[128];
static void* mapped_pointer[4]; static unsigned mapped_word[4]; static int mapped_n;
void* norrath_host_word_pointer(unsigned word) {
    for (int i = 0; i < mapped_n; ++i) if (mapped_word[i] == word) return mapped_pointer[i];
    return nullptr;
}
static void map(void* p) { mapped_pointer[mapped_n] = p; mapped_word[mapped_n++] = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
int printf(const char* format, ...) {
    va_list a; va_start(a, format); std::vsnprintf(printed, 128, format, a); va_end(a); ++prints; order[order_n++] = 1; return 0;
}
int WaitSema(int s) { wait_id = s; order[order_n++] = 2; return 0; }
int iSignalSema(int s) { isig_id = s; ++isig_calls; order[order_n++] = 5; return 0; }
int sceSifCheckStatRpc(void* client) { (void)client; ++stat_calls; order[order_n++] = 6; return stat_result; }
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int ss, void* recv, int rs, void* end, void* param) {
    ++rpc_calls; order[order_n++] = 4;
    rpc_client = client; rpc_cmd = command; rpc_mode = mode; rpc_send = send; rpc_ss = ss; rpc_recv = recv; rpc_rs = rs;
    rpc_end = reinterpret_cast<void (*)(void*)>(end); rpc_param = param;
    std::memcpy(at_call, send, 128);
    return rpc_result;
}
unsigned lgAudARead(unsigned handle, unsigned char value, void* destination, int* count);
}
static unsigned word(const unsigned char* p, int i) { unsigned v; std::memcpy(&v, p + i * 4, 4); return v; }
int main() {
    alignas(16) static unsigned char buffer[512]; static unsigned char dest[512]; int count = 100; int other = 0;
    norrath_lgaud_request_buffer_view = buffer;
    map(dest); map(&count);
    // not initialised, bad arguments, busy
    norrath_lgaud_semaphore_view = -1;
    if (lgAudARead(1, 2, dest, &count) != 0x8000000au || prints != 0 || stat_calls != 0) return 1;
    norrath_lgaud_semaphore_view = 5;
    if (lgAudARead(1, 2, nullptr, &count) != 0x80000004u || std::strcmp(printed, "lgAudARead: invalid parameter (dest)\n") != 0) return 2;
    if (lgAudARead(1, 2, dest, nullptr) != 0x80000004u || std::strcmp(printed, "lgAudARead: invalid parameter (*size too big)\n") != 0) return 3;
    count = 4097;
    if (lgAudARead(1, 2, dest, &count) != 0x80000004u || stat_calls != 0) return 4;
    count = 4096; stat_result = 1;
    if (lgAudARead(1, 2, dest, &count) != 0x80000008u || std::strcmp(printed, "lgAudARead: error: another async call is in progress...\n") != 0 || wait_id != -9) return 5;
    // a good start
    stat_result = 0; count = 100; std::memset(buffer, 0x5a, sizeof buffer); order_n = 0;
    if (lgAudARead(0x31, 0x7f, dest, &count) != 0) return 6;
    if (order_n != 3 || order[0] != 6 || order[1] != 2 || order[2] != 4 || wait_id != 5) return 7;
    if (rpc_client != norrath_lgaud_rpc_client_view || rpc_cmd != 8 || rpc_mode != 1 || rpc_send != buffer || rpc_recv != buffer ||
        rpc_ss != 48 || rpc_rs != 160 || rpc_end == nullptr || rpc_param != norrath_lgaud_async_record_view) return 8;       // (100 + 63) >> 4 << 4
    if (word(at_call, 3) != 0x31 || word(at_call, 8) != 100 || at_call[18] != 0x7f || at_call[17] != 0x5a || at_call[19] != 0x5a) return 9;
    if (norrath_lgaud_async_record_view[0] != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(dest)) ||
        norrath_lgaud_async_record_view[1] != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(&count))) return 10;
    if (isig_calls != 0) return 11;                                             // the semaphore stays taken until completion
    // completion routine: copies, stores the count and result word, signals
    for (int i = 0; i < 512; ++i) buffer[i] = static_cast<unsigned char>(i + 1);
    unsigned reply_count = 20; std::memcpy(buffer + 32, &reply_count, 4); unsigned result = 0x600d; std::memcpy(buffer, &result, 4);
    std::memset(dest, 0xee, sizeof dest); count = 77; norrath_lgaud_async_record_view[2] = 0x1111;
    rpc_end(rpc_param);
    if (std::memcmp(dest, buffer + 48, 20) != 0 || dest[20] != 0xee || count != 20 || norrath_lgaud_async_record_view[2] != 0x600d) return 12;
    if (isig_calls != 1 || isig_id != 5) return 13;
    // receive size rounding
    count = 1; stat_result = 0; lgAudARead(1, 1, dest, &count);
    if (rpc_rs != 64) return 14;
    count = 0; lgAudARead(1, 1, dest, &count);
    if (rpc_rs != 48) return 15;
    // call failure: count cleared, text, no semaphore release, 0x80000000
    count = 50; rpc_result = 1; isig_calls = 0; order_n = 0;
    if (lgAudARead(1, 1, dest, &count) != 0x80000000u || count != 0 || order_n != 4 || order[3] != 1 || isig_calls != 0) return 16;
    if (std::strcmp(printed, "lgAudARead: rpc call failed\n") != 0) return 17;
    (void)other;
    return 0;
}
