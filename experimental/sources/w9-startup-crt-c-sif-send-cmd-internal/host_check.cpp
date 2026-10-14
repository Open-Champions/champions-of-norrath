#include "sif_send_cmd_internal.cpp"

#include <cstring>
#include <cstdint>

void* norrath_sound_command_dma_destination_word_view[1];

static int wb_calls, set_calls, iset_calls;
static void* wb_addr[4];
static int wb_bytes[4];
static SifDmaDescriptor seen[2];
static int seen_count;
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    wb_addr[wb_calls] = address;
    wb_bytes[wb_calls] = bytes;
    ++wb_calls;
}
extern "C" int sceSifSetDma(void* descriptors, int count) {
    ++set_calls;
    seen_count = count;
    std::memcpy(seen, descriptors, sizeof(SifDmaDescriptor) * count);
    return 100 + count;
}
extern "C" int isceSifSetDma(void* descriptors, int count) {
    ++iset_calls;
    seen_count = count;
    std::memcpy(seen, descriptors, sizeof(SifDmaDescriptor) * count);
    return 200 + count;
}

int main() {
    norrath_sound_command_dma_destination_word_view[0] = reinterpret_cast<void*>(0x1234);
    alignas(16) unsigned char packet[128];
    unsigned char extra[32];
    // Sizes outside 16..112 do nothing and return 0.
    static const int bad_sizes[] = {-1, 0, 15, 113, 128, 0x7fffffff};
    for (int size : bad_sizes) {
        std::memset(packet, 0xaa, sizeof packet);
        if (_sceSifSendCmd(7, 0, packet, size, extra, extra, 8) != 0) return 1;
        if (wb_calls || set_calls || iset_calls || packet[0] != 0xaa || packet[8] != 0xaa) return 2;
    }
    // No extra data, plain queue.
    std::memset(packet, 0xaa, sizeof packet);
    int result = _sceSifSendCmd(0x80000008u, 0, packet, 64, extra, extra, 0);
    if (result != 101 || set_calls != 1 || iset_calls != 0 || seen_count != 1) return 3;
    unsigned w[3];
    std::memcpy(w, packet, 12);
    if (packet[0] != 64 || packet[1] || packet[2] || packet[3]) return 4;   // low byte replaced by the size
    if (w[1] != 0 || w[2] != 0x80000008u) return 5;
    if (seen[0].source != packet || seen[0].destination != reinterpret_cast<void*>(0x1234) ||
        seen[0].size != 64 || seen[0].attributes != 0x44) return 6;
    if (wb_calls != 1 || wb_addr[0] != packet || wb_bytes[0] != 64) return 7;
    // Extra data with the interrupt flag and the write-back flag.
    wb_calls = 0;
    std::memset(packet, 0xaa, sizeof packet);
    packet[0] = 0x5a;  // the low byte is kept in word 0 before the size overwrites it
    result = _sceSifSendCmd(9, 5, packet, 16, extra, packet + 100, 32);
    if (result != 202 || iset_calls != 1 || seen_count != 2) return 8;
    std::memcpy(w, packet, 12);
    if (packet[0] != 16 || packet[1] != 32 || packet[2] || packet[3]) return 9;
    if (w[1] != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(packet + 100)) && sizeof(void*) == 4) return 10;
    if (w[2] != 9) return 11;
    if (seen[0].source != extra || seen[0].destination != packet + 100 || seen[0].size != 32 || seen[0].attributes != 0) return 12;
    if (seen[1].source != packet || seen[1].size != 16 || seen[1].attributes != 0x44) return 13;
    if (wb_calls != 2 || wb_addr[0] != extra || wb_bytes[0] != 32 || wb_addr[1] != packet || wb_bytes[1] != 16) return 14;
    // Extra data without bit 2: no write-back of the extra block.
    wb_calls = 0;
    result = _sceSifSendCmd(9, 0, packet, 112, extra, extra, 1);
    if (result != 102 || wb_calls != 1 || wb_addr[0] != packet || wb_bytes[0] != 112) return 15;
    // A negative extra size counts as no extra data even with the write-back flag.
    wb_calls = 0;
    result = _sceSifSendCmd(9, 4, packet, 16, extra, extra, -4);
    if (result != 101 || wb_calls != 1) return 16;
    return 0;
}
