#include <cstdio>
#include <cstdlib>
extern "C" unsigned char gCd[4] = {};
extern "C" unsigned char sbuff[64] = {};
extern "C" unsigned char sbuffin[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
extern "C" unsigned char stream[452 * 2] = {};
static int clears = 0;
static unsigned char* first_clear = nullptr;
extern "C" void* memset(void* destination, int value, unsigned long size) {
    if (value != 0 || size != 48) std::abort();
    if (clears == 0) first_clear = static_cast<unsigned char*>(destination);
    clears += 1;
    auto* bytes = static_cast<unsigned char*>(destination);
    for (unsigned long index = 0; index < size; ++index) bytes[index] = 0;
    return destination;
}
static int calls = 0;
extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                             int receive_size, void*, void*) {
    calls += 1;
    if (client != gCd || command != 96u || mode != 0u || send != sbuffin || send_size != 16) return 1;
    if (receive != sbuff || receive_size != 64) return 1;
    return 0;
}
extern "C" void soundResetStream__Fi(int);
static unsigned word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}
int main() {
    for (unsigned index = 0; index < sizeof stream; ++index) stream[index] = 0xab;
    soundResetStream__Fi(1);
    unsigned char* record = stream + 452;
    if (record[0] != 0 || word(record + 20) != 0 || word(record + 28) != 0) return 1;
    if (word(record + 32) != 0 || word(record + 36) != 0 || word(record + 448) != 0) return 2;
    if (clears != 8 || first_clear != record + 64 || record[64] != 0 || record[447] != 0) return 3;
    if (stream[0] != 0xab || calls != 1 || word(sbuffin) != 1u || word(sbuffin + 4) != 0) return 4;
    std::puts("reset stream contract passed");
    return 0;
}
