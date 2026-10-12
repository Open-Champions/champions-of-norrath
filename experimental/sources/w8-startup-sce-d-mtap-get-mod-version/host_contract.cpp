#include "mtap_get_mod_version.cpp"

#include <cstdio>
#include <cstring>

unsigned char norrath_libmtap_module_version_client_view[40];
int norrath_libmtap_rpc_buffer_view[32];

static int rpc_result, calls, prints;
static void *g_client, *g_send, *g_recv, *g_done, *g_arg;
static unsigned g_cmd, g_mode;
static int g_ssize, g_rsize, first_word_seen;
static char text[64];

extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* done, void* done_arg) {
    g_client = client; g_cmd = command; g_mode = mode; g_send = send; g_ssize = send_size;
    g_recv = receive; g_rsize = receive_size; g_done = done; g_arg = done_arg;
    first_word_seen = norrath_libmtap_rpc_buffer_view[0];
    calls += 1;
    return rpc_result;
}
extern "C" void DPRINT(const char* format, ...) {
    std::snprintf(text, sizeof text, "%s", format);
    prints += 1;
}

int main() {
    norrath_libmtap_rpc_buffer_view[0] = 0x0305;
    rpc_result = 0;
    if (sceMtapGetModVersion() != 0x0305) return 1;
    if (calls != 1 || prints != 0) return 2;
    if (g_client != norrath_libmtap_module_version_client_view || g_cmd != 1 || g_mode != 0) return 3;
    if (g_send != norrath_libmtap_rpc_buffer_view || g_recv != g_send || g_ssize != 128 || g_rsize != 128) return 4;
    if (g_done || g_arg || first_word_seen != 0x0305) return 5; /* buffer is not written before the call */
    rpc_result = -1;
    norrath_libmtap_rpc_buffer_view[0] = 77;
    if (sceMtapGetModVersion() != 0 || prints != 1) return 6;
    if (std::strcmp(text, "sceMtapGetModVersion: rpc error\n") != 0) return 7;
    rpc_result = 1; /* positive results succeed */
    if (sceMtapGetModVersion() != 77 || prints != 1) return 8;
    std::puts("mtap get mod version contract passed");
    return 0;
}
