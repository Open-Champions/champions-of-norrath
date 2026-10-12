#include <cstdio>
#include <cstring>
extern "C" unsigned char gCd[4] = {};
extern "C" unsigned char sbuff[64] = {};
extern "C" unsigned char sbuffin[16] = {};
extern "C" unsigned char stream[452 * 3] = {};
static int calls = 0;
static void* seen_client = nullptr;
static int seen_send = -1;
static int seen_recv = -1;
extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                             int receive_size, void*, void*) {
    calls += 1;
    seen_client = client;
    seen_send = send_size;
    seen_recv = receive_size;
    if (command != 80u || mode != 0u || send != sbuffin || receive != sbuff) return 1;
    return 0;
}
extern "C" int soundGetStreamPlayStatus__Fi(int);
static void put(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    put(sbuff, 1);
    put(sbuff + 4, 2);
    put(stream + 32, 4);
    if (soundGetStreamPlayStatus__Fi(0) != 5 || calls != 1 || seen_client != gCd || seen_send != 0 || seen_recv != 64)
        return 1;
    put(sbuff, 7);
    put(sbuff + 4, 7);
    put(stream + 20, 1);
    if (soundGetStreamPlayStatus__Fi(0) != 5) return 2;
    put(stream + 20, 0);
    put(stream + 448, 0);
    if (soundGetStreamPlayStatus__Fi(0) != 0) return 3;
    put(stream + 448, 1);
    put(stream + 108, 8);
    if (soundGetStreamPlayStatus__Fi(0) != 0) return 4;
    put(stream + 108, 16);
    if (soundGetStreamPlayStatus__Fi(0) != 17) return 5;
    std::puts("stream status contract passed");
    return 0;
}
