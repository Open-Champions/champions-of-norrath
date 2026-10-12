// Host contract for engineLoadEnvTex. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstring>
#include "engine_load_env_tex.cpp"

static char log[16];
static int logCount = 0;
static void note(char c) { log[logCount++] = c; }

static unsigned storedAddress = 0, storedValue = 0;
static void* dmaChannelSeen = nullptr;
static unsigned long packetSeen = 0;
static unsigned countSeen = 0;
static int chanArg = -1, syncA = -1, syncB = -1;
static char fakeChannel;

extern "C" {
int engineEnvTexAddress = 0;
void norrath_host_store16(unsigned address, unsigned value) {
    note('S');
    storedAddress = address;
    storedValue = value & 0xffffu;
}
void norrath_host_sync() { note('Y'); }
void* sceDmaGetChan(int channel) {
    note('G');
    chanArg = channel;
    return &fakeChannel;
}
void sceDmaSendN(void* channel, void* packet, unsigned count) {
    note('N');
    dmaChannelSeen = channel;
    packetSeen = reinterpret_cast<unsigned long>(packet);
    countSeen = count;
}
void sceGsSyncPath(int mode, int timeout) {
    note('P');
    syncA = mode;
    syncB = timeout;
}
}

static int run(int address, unsigned packet, unsigned short count) {
    unsigned char texture[32] __attribute__((aligned(8)));
    std::memset(texture, 0xAA, sizeof texture);
    texture[16] = packet & 0xff;
    texture[17] = (packet >> 8) & 0xff;
    texture[18] = (packet >> 16) & 0xff;
    texture[19] = (packet >> 24) & 0xff;
    texture[6] = count & 0xff;
    texture[7] = count >> 8;
    engineEnvTexAddress = address;
    logCount = 0;
    engineLoadEnvTex__FP8_texture(reinterpret_cast<EnvTextureRecord*>(texture));
    log[logCount] = 0;
    return 0;
}

static_assert(sizeof(EnvTextureRecord) == 20, "record view");
static_assert(__builtin_offsetof(EnvTextureRecord, quadwordCount) == 6, "count offset");
static_assert(__builtin_offsetof(EnvTextureRecord, packetAddress) == 16, "packet offset");

int main() {
    run(0x00348000, 0x01234560u, 0xfff0);
    if (std::strcmp(log, "GSYNP") != 0) return 1;
    if (chanArg != 2 || dmaChannelSeen != &fakeChannel) return 2;
    if (storedAddress != (0x01234560u | 0x20000000u) + 20u) return 3;
    if (storedValue != 0x3480u) return 4;
    if (packetSeen != 0x01234560ul || countSeen != 0xfff0u) return 5;
    if (syncA != 0 || syncB != 0) return 6;
    // Negative words shift arithmetically; only the low 16 bits are stored.
    run(-0x10000, 0x00000100u, 7);
    if (storedValue != 0xff00u || countSeen != 7u) return 7;
    return 0;
}
