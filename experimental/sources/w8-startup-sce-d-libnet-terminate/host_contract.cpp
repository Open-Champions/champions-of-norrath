#include "libnet_terminate.cpp"

#include <cstdio>

static int script[8];
static int script_len, calls;
static void* last_client;
static unsigned last_mode;

extern "C" int sceSifMUnBindRpc(void* client, unsigned mode) {
    last_client = client;
    last_mode = mode;
    return script[calls++ < script_len ? calls - 1 : script_len - 1];
}

int main() {
    char client;
    script[0] = 1; script_len = 1;
    if (sceLibnetTerminate(&client) != 0 || calls != 1 || last_client != &client || last_mode != 0) return 1;
    calls = 0; script[0] = -1;
    if (sceLibnetTerminate(&client) != -540 || calls != 1) return 2;
    calls = 0; script[0] = 0; script[1] = 2; script[2] = 1; script_len = 3;
    if (sceLibnetTerminate(&client) != 0 || calls != 3) return 3;
    calls = 0; script[0] = 0; script[1] = 5; script[2] = -3; script_len = 3;
    if (sceLibnetTerminate(&client) != -540 || calls != 3) return 4;
    std::puts("libnet terminate contract passed");
    return 0;
}
