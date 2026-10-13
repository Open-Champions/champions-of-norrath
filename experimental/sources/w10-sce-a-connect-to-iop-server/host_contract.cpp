// Host check only; not an EE build input. -fno-builtin keeps memset a real call.
// Build: c++ -std=c++17 -fno-builtin -Wall -Wextra -o /tmp/c host_contract.cpp connect_to_iop_server.cpp
#include <csignal>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
extern "C" {
unsigned char GLibData[0x668];
static int init_calls, init_arg = -1, binds, bind_result, bound_after, order[8], order_count;
static unsigned bind_service; static void* bind_client; static unsigned bind_mode; static unsigned char client_at_bind[40];
void sceSifInitRpc(int mode) { ++init_calls; init_arg = mode; order[order_count++] = 1; }
void* memset(void* destination, int value, __SIZE_TYPE__ count) {
    order[order_count++] = 2;
    unsigned char* p = static_cast<unsigned char*>(destination);
    for (__SIZE_TYPE__ i = 0; i < count; ++i) p[i] = static_cast<unsigned char>(value);
    return destination;
}
int sceSifBindRpc(void* client, unsigned service, unsigned mode) {
    ++binds; order[order_count++] = 3;
    bind_client = client; bind_service = service; bind_mode = mode;
    std::memcpy(client_at_bind, client, 40);
    if (binds >= bound_after) { unsigned one = 1; std::memcpy(GLibData + 36, &one, 4); }
    return bind_result;
}
int connect_to_iop_server();
}
int main() {
    std::memset(GLibData, 0x5a, sizeof GLibData);
    order_count = 0;
    bound_after = 1; bind_result = 0;
    if (connect_to_iop_server() != 0 || init_calls != 1 || init_arg != 0 || binds != 1) return 1;
    if (order_count != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 2;   // init, clear, bind
    if (bind_client != GLibData || bind_service != 0x61766973u || bind_mode != 0) return 3;
    for (int i = 0; i < 40; ++i) if (client_at_bind[i] != 0) return 4;                    // 40 bytes were cleared first
    if (GLibData[40] != 0x5a || GLibData[0x667] != 0x5a) return 5;                        // and no more
    // retries until the bound flag appears, without clearing the client again
    init_calls = binds = order_count = 0; bound_after = 3;
    std::memset(GLibData, 0x5a, sizeof GLibData);
    order_count = 0;
    if (connect_to_iop_server() != 0 || binds != 3 || init_calls != 1) return 6;
    if (order_count != 5) return 7;                                                       // init, clear, bind, bind, bind
    // a failing bind never returns: run it in a child and expect it to still be alive
    const pid_t child = fork();
    if (child == 0) { alarm(30); bind_result = -1; connect_to_iop_server(); _exit(77); }
    usleep(300000);
    int status = 0;
    if (waitpid(child, &status, WNOHANG) != 0) return 8;
    kill(child, SIGKILL); waitpid(child, &status, 0);
    return 0;
}
