#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_libmtap_port_open_client_view[40] = {};
unsigned char norrath_libmtap_rpc_buffer_view[128] = {};
}
static int rpc_result = 0, rpc_calls = 0, dprints = 0;
static char dtext[64];
extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                             int receive_size, void* end, void* end_param) {
    rpc_calls += 1;
    if (client != norrath_libmtap_port_open_client_view || command != 1 || mode != 0) return -1;
    if (send != norrath_libmtap_rpc_buffer_view || receive != send || send_size != 128 || receive_size != 128) return -1;
    if (end != nullptr || end_param != nullptr) return -1;
    norrath_libmtap_rpc_buffer_view[4] = 9;
    return rpc_result;
}
extern "C" void DPRINT(const char* format, ...) { dprints += 1; std::snprintf(dtext, sizeof dtext, "%s", format); }
extern "C" int sceMtapPortOpen(int port);
int main() {
    if (sceMtapPortOpen(1) != 9 || rpc_calls != 1 || norrath_libmtap_rpc_buffer_view[0] != 1 || dprints != 0) return 1;
    rpc_result = -1;
    if (sceMtapPortOpen(0) != 0 || norrath_libmtap_rpc_buffer_view[0] != 0 || dprints != 1) return 2;
    if (std::strcmp(dtext, "sceMtapPortOpen: rpc error\n") != 0) return 3;
    std::puts("mtap port open contract passed");
    return 0;
}
