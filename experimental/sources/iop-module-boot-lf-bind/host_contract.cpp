#define NORRATH_HOST_CONTRACT 1
#include "lf_bind.cpp"

#include <cstdio>

extern "C" int _bind_check = -1;
extern "C" unsigned char _lfversion[4] = {};
int norrath_lf_bind_wait_steps;

static int bind_results[4];
static int binds;
static unsigned servers[4];
static int plant;
static int rpc_result;
static int rpc_operation;
static void* rpc_client;
static void* rpc_send;
static void* rpc_receive;
static int rpc_receive_size;
static int rpcs;

extern "C" int sceSifBindRpc(void* client, unsigned server, int) {
    servers[binds] = server;
    auto* bytes = static_cast<unsigned char*>(client);
    unsigned value = 0x22u;
    if (plant == 2 && binds == 0) value = 0;
    bytes[36] = static_cast<unsigned char>(value);
    bytes[37] = 0;
    bytes[38] = 0;
    bytes[39] = 0;
    const int result = bind_results[binds];
    ++binds;
    return result;
}

extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument) {
    ++rpcs;
    rpc_client = client;
    rpc_operation = operation;
    rpc_send = send;
    rpc_receive = receive;
    rpc_receive_size = receive_size;
    if (mode != 0 || send_size != 0 || completion != nullptr || completion_argument != nullptr) return 8;
    auto* bytes = static_cast<unsigned char*>(receive);
    bytes[0] = '2';
    bytes[1] = '8';
    bytes[2] = '0';
    bytes[3] = '0';
    return rpc_result;
}

int main() {
    if (sizeof norrath_loadfile_client != 40 || sizeof _senddata != 512) return 1;
    _bind_check = 3;
    if (_lf_bind() != 0 || binds != 0 || rpcs != 0) return 2;

    _bind_check = -1;
    bind_results[0] = -2;
    if (_lf_bind() != -1 || binds != 1 || rpcs != 0 || _bind_check != -1 || norrath_lf_bind_wait_steps != 0) return 3;
    if (servers[0] != 0x80000006u) return 4;

    binds = 0;
    rpcs = 0;
    norrath_lf_bind_wait_steps = 0;
    _bind_check = -1;
    plant = 2;
    bind_results[0] = 1;
    bind_results[1] = 1;
    rpc_result = -4;
    if (_lf_bind() != static_cast<int>(0xfffeffffu) || binds != 2 || rpcs != 1) return 5;
    if (norrath_lf_bind_wait_steps != 0x100001 || _bind_check != 0) return 6;
    if (rpc_operation != 255 || rpc_client != norrath_loadfile_client || rpc_send != nullptr || rpc_receive != _senddata || rpc_receive_size != 4) return 7;

    binds = 0;
    rpcs = 0;
    norrath_lf_bind_wait_steps = 0;
    _bind_check = -1;
    plant = 1;
    _lfversion[0] = 0;
    rpc_result = 0;
    if (_lf_bind() != 0 || binds != 1 || rpcs != 1 || norrath_lf_bind_wait_steps != 0) return 8;
    if (_lfversion[0] != '2' || _lfversion[1] != '8' || _lfversion[2] != '0' || _lfversion[3] != '0') return 9;
    if (_bind_check != 0) return 10;
    std::puts("_lf_bind passed");
    return 0;
}
