// Observed putBlock__FP8_textureP6PacketPPP9uncompMapPPii at 0x0016f5e8, 720 bytes. Arguments:
// a texture, a packet record whose first word is a scratchpad cursor, a pointer to a word that
// holds the address of a word to receive a map-entry address (the chain slot), a pointer to a
// word holding the output cursor, and an int flag. No result.
// Names and sizes of the shared tables come from the caller's authored unit and the image's
// symbol table: uncompMap is 1280 entries of 8 bytes {owner texture, next entry}, uncompCache
// holds 4096-byte blocks indexed by uncompSpin, drawFrameNm is the frame counter.
//   1. entry = uncompMap[uncompSpin]. The texture's word at +28 is set to drawFrameNm. When
//      the entry has an owner whose +28 is greater than drawFrameNm - 3 (used within the last
//      three frames), uncompSpin advances (wrapping from 1279 to 0, each step stored) until
//      an entry is free or its owner's +28 is not greater than that limit.
//   2. A claimed entry with an owner has the owner's word at +20 cleared and the entry named by
//      ((owner[+20] - uncompCache) >> 12) and every entry reached through the "next" words
//      starting there get their owner word cleared.
//   3. DMA: 0x1000e010 is written 0x100, then 0x1000d010 gets uncompCache + (uncompSpin << 12) +
//      16, then 0x1000d080 gets 0x70000000 and 0x1000d020 gets (n + 15) >> 4 where n is the
//      packet cursor minus 0x70000000 limited to 4080. The entry's owner becomes the texture,
//      0x1000d000 gets 0x100, and the code waits until bit 0x100 of 0x1000e010 is set. Each
//      register is read back after being written, as in the original. Whether these are the
//      scratchpad-to-memory DMA registers is inferred from the addresses, not asserted.
//   4. The leftover (cursor - n - 0x70000000) bytes, if positive, are copied down from
//      0x70000000 + n to 0x70000000 with memcpy, and the packet cursor becomes cursor - n.
//   5. If the chain slot word is nonzero, the address of the claimed entry is stored where it
//      points. The output cursor word points at a word that receives the block address
//      (uncompCache + (uncompSpin << 12)); the chain slot is set to the address of the entry's
//      "next" word (entry + 4). The block starts with a word (n >> 4) | 0x20000000, or
//      0x60000000 instead of 0x20000000 when the int flag is nonzero and nothing was left over;
//      it is written through the address with 0x20000000 OR-ed in and the output cursor
//      advances by 4 from that address.
//   6. uncompSpin advances by one, wrapping at 1280.
// Unknown: the meaning of the DMA tag words and of the packet record beyond its first word.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_ptr(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
extern "C" volatile unsigned* norrath_host_reg(unsigned address);
extern "C" void norrath_host_sync();
#define NORRATH_PTR(a) norrath_host_ptr(a)
#define NORRATH_ADDR(p) norrath_host_address(p)
#define NORRATH_REG(a) (*norrath_host_reg(a))
#define NORRATH_SYNC() norrath_host_sync()
#else
#define NORRATH_PTR(a) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a))
#define NORRATH_ADDR(p) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p))
#define NORRATH_REG(a) (*reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(a)))
#define NORRATH_SYNC() asm volatile("sync" ::: "memory")
#endif

struct Packet {
    int cursor;
};
struct _texture;

extern "C" {
extern int uncompSpin;
extern int uncompCache;
extern int drawFrameNm;
extern unsigned char uncompMap[];
void* memcpy(void* destination, const void* source, unsigned long count);
}

namespace {
inline unsigned& word(unsigned char* base, unsigned offset) {
    return *reinterpret_cast<unsigned*>(base + offset);
}
inline unsigned char* entry_at(int index) {
    return uncompMap + index * 8;
}
}  // namespace

extern "C" void putBlock__FP8_textureP6PacketPPP9uncompMapPPii(_texture* texture, Packet* packet, unsigned* chain,
                                                               unsigned* output, int flag) {
    unsigned char* tex = reinterpret_cast<unsigned char*>(texture);
    int spin = uncompSpin;
    int frame = drawFrameNm;
    unsigned owner = word(entry_at(spin), 0);
    word(tex, 28) = static_cast<unsigned>(frame);
    if (owner != 0) {
        int limit = frame - 3;
        if (limit < static_cast<int>(word(NORRATH_PTR(owner), 28))) {
            for (;;) {
                spin = spin + 1;
                if (!(spin < 1280)) spin = 0;
                uncompSpin = spin;
                unsigned next = word(entry_at(spin), 0);
                if (next == 0) break;
                if (!(limit < static_cast<int>(word(NORRATH_PTR(next), 28)))) break;
            }
        }
    }
    unsigned claimed = word(entry_at(spin), 0);
    if (claimed != 0) {
        unsigned char* old = NORRATH_PTR(claimed);
        unsigned char* cleared = entry_at((static_cast<int>(word(old, 20)) - uncompCache) >> 12);
        word(old, 20) = 0;
        if (cleared != 0) {
            word(cleared, 0) = 0;
            unsigned next = word(cleared, 4);
            while (next != 0) {
                word(NORRATH_PTR(next), 0) = 0;
                next = word(NORRATH_PTR(next), 4);
            }
        }
        spin = uncompSpin;
    }
    int cache = uncompCache;
    int cursor = packet->cursor;
    NORRATH_REG(0x1000e010u) = 256u;
    (void)NORRATH_REG(0x1000e010u);
    NORRATH_REG(0x1000d010u) = static_cast<unsigned>(cache + (spin << 12) + 16);
    int used = static_cast<int>(static_cast<unsigned>(cursor) + 0x90000000u);
    if (used >= 4081) used = 4080;
    (void)NORRATH_REG(0x1000d010u);
    NORRATH_REG(0x1000d080u) = 0x70000000u;
    (void)NORRATH_REG(0x1000d080u);
    NORRATH_REG(0x1000d020u) = static_cast<unsigned>((used + 15) >> 4);
    (void)NORRATH_REG(0x1000d020u);
    word(entry_at(spin), 0) = NORRATH_ADDR(tex);
    NORRATH_SYNC();
    NORRATH_REG(0x1000d000u) = 256u;
    (void)NORRATH_REG(0x1000d000u);
    NORRATH_SYNC();
    while ((NORRATH_REG(0x1000e010u) & 0x100u) == 0) {
    }
    int remaining = static_cast<int>(static_cast<unsigned>(cursor) - static_cast<unsigned>(used));
    int left = static_cast<int>(static_cast<unsigned>(remaining) + 0x90000000u);
    if (left > 0) memcpy(NORRATH_PTR(0x70000000u), NORRATH_PTR(0x70000000u + static_cast<unsigned>(used)), left);
    unsigned slot = *chain;
    packet->cursor = remaining;
    if (slot != 0) word(NORRATH_PTR(slot), 0) = NORRATH_ADDR(entry_at(uncompSpin));
    int block = uncompCache + (uncompSpin << 12);
    word(NORRATH_PTR(*output), 0) = static_cast<unsigned>(block);
    *chain = NORRATH_ADDR(entry_at(uncompSpin) + 4);
    unsigned char* uncached = NORRATH_PTR(static_cast<unsigned>(uncompCache + (uncompSpin << 12)) | 0x20000000u);
    unsigned id = (flag != 0 && left == 0) ? 0x60000000u : 0x20000000u;
    word(uncached, 0) = static_cast<unsigned>(used >> 4) | id;
    *output = NORRATH_ADDR(uncached) + 4;
    int next = uncompSpin + 1;
    if (!(next < 1280)) next = 0;
    uncompSpin = next;
}
