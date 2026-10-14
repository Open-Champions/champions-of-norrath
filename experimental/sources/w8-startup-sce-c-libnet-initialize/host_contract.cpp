#include <cstdio>
#include "libnet_initialize.cpp"
static int bind_result, calls, set_after, a1, a2, a3; static unsigned seen_id, seen_mode; static void* seen_client; static int* flag;
extern "C" int sceSifMBindRpcParam(void* client, unsigned id, unsigned mode, int first, int second, int third) {
    ++calls; seen_client = client; seen_id = id; seen_mode = mode; a1 = first; a2 = second; a3 = third;
    if (calls == set_after) *flag = 5;
    return bind_result;
}
int main() {
    int client[10] = {};
    flag = &client[9];
    bind_result = 0; set_after = 1;
    if (sceLibnetInitialize(client, 2048, 8192, 32) != 0 || calls != 1) return 1;
    if (seen_client != client || seen_id != 0x80001201u || seen_mode != 0 || a1 != 2048 || a2 != 8192 || a3 != 32) return 2;
    // Bind repeated until the word at +36 becomes nonzero.
    calls = 0; client[9] = 0; set_after = 3;
    if (sceLibnetInitialize(client, 256, 1, 2) != 0 || calls != 3) return 3;
    // Size checks do not call the bind.
    calls = 0;
    if (sceLibnetInitialize(client, 255, 1, 2) != -543 || sceLibnetInitialize(client, 320 + 8, 1, 2) != -543) return 4;
    if (sceLibnetInitialize(client, 0, 1, 2) != -543 || sceLibnetInitialize(client, -64, 1, 2) != -543 || calls != 0) return 5;
    // Negative bind result.
    client[9] = 5; bind_result = -1;
    if (sceLibnetInitialize(client, 512, 1, 2) != -540 || calls != 1) return 6;
    // Word already nonzero: success after the first bind.
    bind_result = 0; calls = 0; set_after = -1;
    if (sceLibnetInitialize(client, 512, 1, 2) != 0 || calls != 1) return 7;
    std::puts("sceLibnetInitialize contract passed");
    return 0;
}
