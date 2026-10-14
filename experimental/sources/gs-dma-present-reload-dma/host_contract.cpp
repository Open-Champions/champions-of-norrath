#define NORRATH_HOST_CONTRACT
#include "reload_dma.cpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

extern "C" unsigned char drawHead[128] = {};
extern "C" int endDrawSemaphore = 0x42;

static ReloadTexture tex_pool[8];

static unsigned tid(unsigned i) { return 0x1000u * (i + 1u); }

extern "C" ReloadTexture* norrath_resolve_texture(unsigned id) {
    if (id == 0u) return nullptr;
    const unsigned i = id / 0x1000u - 1u;
    assert(i < 8u);
    return &tex_pool[i];
}

static unsigned id_of(ReloadTexture* p) {
    if (p == nullptr) return 0u;
    assert(p >= tex_pool && p < tex_pool + 8);
    return static_cast<unsigned>(p - tex_pool + 1u) * 0x1000u;
}

struct Store {
    unsigned address;
    unsigned long long value;
    unsigned size;
};
static std::map<unsigned, unsigned long long> ee_mem;
static std::vector<Store> stores;
static std::vector<unsigned> texture_calls;
static std::vector<unsigned> sema_calls;
static int sync_calls;
static int ei_calls;
static int intr_calls;
enum { CALL_LOCK = 1, CALL_UNLOCK = 2, CALL_CACHEIN = 3 };

static unsigned long long mem_read(unsigned address, unsigned size) {
    unsigned long long out = 0;
    for (unsigned i = 0; i < size; ++i) {
        const unsigned a = address + i;
        const auto it = ee_mem.find(a & ~7u);
        const unsigned long long w = (it == ee_mem.end()) ? 0ull : it->second;
        out |= ((w >> ((a & 7u) * 8)) & 0xffull) << (i * 8);
    }
    return out;
}

extern "C" unsigned norrath_ee_load32(unsigned address) {
    return static_cast<unsigned>(mem_read(address, 4));
}

extern "C" unsigned long long norrath_ee_load64(unsigned address) {
    return mem_read(address, 8);
}

static void mem_write(unsigned address, unsigned long long value, unsigned size) {
    for (unsigned i = 0; i < size; ++i) {
        const unsigned a = address + i;
        const unsigned key = a & ~7u;
        const unsigned shift = (a & 7u) * 8;
        ee_mem[key] =
            (ee_mem[key] & ~(0xffull << shift)) | (((value >> (i * 8)) & 0xffull) << shift);
    }
    stores.push_back({address, value, size});
}

extern "C" void norrath_ee_store32(unsigned address, unsigned value) {
    mem_write(address, value, 4);
}
extern "C" void norrath_ee_store64(unsigned address, unsigned long long value) {
    mem_write(address, value, 8);
}
extern "C" void norrath_ee_store16(unsigned address, unsigned value) {
    mem_write(address, value & 0xffffull, 2);
}
extern "C" void norrath_host_pipeline_sync() { ++sync_calls; }
extern "C" void norrath_host_enable_interrupts() { ++ei_calls; }

extern "C" void textureLock__FP8_texture(ReloadTexture* texture) {
    texture_calls.push_back(CALL_LOCK * 0x10000u + id_of(texture));
}
extern "C" void textureUnlock__FP8_texture(ReloadTexture* texture) {
    texture_calls.push_back(CALL_UNLOCK * 0x10000u + id_of(texture));
}
extern "C" void textureCacheIn__FP8_texture(ReloadTexture* texture) {
    texture_calls.push_back(CALL_CACHEIN * 0x10000u + id_of(texture));
}
extern "C" int EIntr() {
    ++intr_calls;
    return 0;
}
extern "C" int DIntr() {
    ++intr_calls;
    return 0;
}
extern "C" void SignalSema(int identifier) { sema_calls.push_back(0x10000u + identifier); }
extern "C" void iSignalSema(int identifier) { sema_calls.push_back(0x20000u + identifier); }

static void set_draw_word(unsigned byte_offset, unsigned value) {
    assert(byte_offset + 4u <= sizeof(drawHead));
    drawHead[byte_offset] = static_cast<unsigned char>(value);
    drawHead[byte_offset + 1] = static_cast<unsigned char>(value >> 8);
    drawHead[byte_offset + 2] = static_cast<unsigned char>(value >> 16);
    drawHead[byte_offset + 3] = static_cast<unsigned char>(value >> 24);
}

static void set_tex(unsigned i, unsigned w0, unsigned w4, unsigned p8, unsigned next,
                    unsigned w16, unsigned w20, unsigned w24) {
    tex_pool[i].w[0] = w0;
    tex_pool[i].w[1] = w4;
    tex_pool[i].w[2] = p8;
    tex_pool[i].w[3] = next;
    tex_pool[i].w[4] = w16;
    tex_pool[i].w[5] = w20;
    tex_pool[i].w[6] = w24;
}

static void set_half(unsigned i, unsigned offset, unsigned value) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(tex_pool[i].w) + offset;
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
}

static void reset_state() {
    ee_mem.clear();
    stores.clear();
    texture_calls.clear();
    sema_calls.clear();
    sync_calls = 0;
    ei_calls = 0;
    intr_calls = 0;
    std::memset(tex_pool, 0, sizeof(tex_pool));
    std::memset(drawHead, 0, sizeof(drawHead));
    initialized_744 = 0;
    readBuffer_745 = 0;
    channelBusy_746 = 0;
    channelDone_747 = 0;
    drTexLoad_748 = 0;
    drTexLoaded_749 = 0;
    drDraw_750 = 0;
    drDone_751 = 0;
    sentDone_752 = 0;
    donesGot_753 = 0;
    donesNeeded_754 = 0;
    flushing_755 = 0;
    preTex_756 = 0;
    preVif_757 = 0;
    texTypes_758 = 0;
    drawSizeOverEnable = 1;
    bucket = 0;
    texDmaListFlip = 0;
    vifDmaListFlip = 0;
    texDmaListPos = 0;
    vifDmaListPos = 0;
}

static bool have_store(unsigned address, unsigned long long value, unsigned size) {
    for (const Store& s : stores)
        if (s.address == address && s.value == value && s.size == size) return true;
    return false;
}

int main() {
    // T1: uninitialized engine with a non--1 cause returns early.
    reset_state();
    assert(reloadDMA__Fi(0) == 1);
    assert(sync_calls == 1 && ei_calls == 1);
    assert(stores.empty() && texture_calls.empty() && sema_calls.empty());
    assert(initialized_744 == 0);

    // T2: cause -1 reframes an empty head (readBuffer flips to slot 1),
    // scans out the bucket, and completes the frame at the semaphore tail.
    reset_state();
    set_draw_word(32, 0x3000u);
    mem_write(0x3000u + 12u, 0u, 4);
    stores.clear();
    assert(reloadDMA__Fi(-1) == 1);
    assert(readBuffer_745 == 1 && bucket == 8);
    assert(drTexLoad_748 == 0u && drTexLoaded_749 == 0u && drDraw_750 == 0u &&
           drDone_751 == 0u);
    assert(channelBusy_746 == 0u && channelDone_747 == 2u);
    assert(donesGot_753 == 0 && donesNeeded_754 == 0);
    assert(sentDone_752 == 1 && initialized_744 == 0);
    assert(intr_calls == 2 && sema_calls.size() == 1u &&
           sema_calls[0] == 0x10000u + 0x42u);

    // T3: cause -2 bumps donesGot with bit 1 set, else clears flushing.
    reset_state();
    initialized_744 = 1;
    channelDone_747 = 2;
    channelBusy_746 = 0;
    bucket = 8;
    sentDone_752 = 1;
    assert(reloadDMA__Fi(-2) == 1);
    assert(donesGot_753 == 1);
    assert(have_store(0x12001000u, 2ull, 8));
    reset_state();
    initialized_744 = 1;
    channelDone_747 = 0;
    channelBusy_746 = 0;
    flushing_755 = 1;
    bucket = 8;
    sentDone_752 = 1;
    assert(reloadDMA__Fi(-2) == 1);
    assert(flushing_755 == 0 && donesGot_753 == 0);

    // T4: tex-side drain walks drTexLoaded forward while preTex trails.
    // The on-demand vif kick fires afterwards (bit 1 was clear).
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 4;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    texTypes_758 = 1;
    set_tex(0, 0, 0, 0, tid(1), 0, 0, 0);
    set_tex(1, 0, 0, 0, tid(2), 0, 0, 0);
    set_tex(2, 0, 0, 0, 0u, 0, 0, 0);
    drTexLoaded_749 = tid(0);
    drDraw_750 = tid(2);
    // count 2 from 0x1000a040: ((value - 0x800 - 0x6142c0) >> 4) - 1.
    mem_write(0x1000a040u, 0x6142c0u + 0x800u + 3u * 16u, 4);
    assert(reloadDMA__Fi(3) == 1);
    assert(drTexLoaded_749 == tid(2));
    assert(preTex_756 == 2);
    assert(have_store(0x10009030u, 0x614240ull, 4));
    assert(have_store(0x10009000u, 325ull, 4));
    assert((channelBusy_746 & 6u) == 6u && (channelDone_747 & 2u) != 0u);

    // T5: vif-side drain unlocks toward drDraw, then the tail stops it.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 2;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    set_tex(0, 0, 0, tid(3), tid(1), 0, 0, 0);
    set_tex(1, 0, 0, 0, tid(2), 0, 0, 0);
    set_tex(2, 0, 0, 0, tid(2), 0, 0, 0);
    set_tex(3, 0, 0, 0, 0, 0, 0, 0);
    drDone_751 = tid(0);
    drDraw_750 = tid(2);
    // count 6 from 0x10009040 with preVif 1.
    mem_write(0x10009040u, 0x6152c0u + 0x800u + 7u * 16u, 4);
    preVif_757 = 1;
    assert(reloadDMA__Fi(0) == 1);
    assert(texture_calls.size() == 1u &&
           texture_calls[0] == CALL_UNLOCK * 0x10000u + tid(3));
    assert(drDone_751 == tid(2));
    assert(preVif_757 == 6);

    // T6: cause 1 sweeps drDone to drDraw unlocking every waiter.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    set_tex(0, 0, 0, tid(3), tid(1), 0, 0, 0);
    set_tex(1, 0, 0, 0, tid(2), 0, 0, 0);
    set_tex(2, 0, 0, tid(4), tid(5), 0, 0, 0);
    set_tex(3, 0, 0, 0, 0, 0, 0, 0);
    set_tex(4, 0, 0, 0, 0, 0, 0, 0);
    set_tex(5, 0, 0, 0, 0u, 0, 0, 0);
    drDone_751 = tid(0);
    drDraw_750 = tid(5);
    assert(reloadDMA__Fi(1) == 1);
    assert(texture_calls.size() == 2u &&
           texture_calls[0] == CALL_UNLOCK * 0x10000u + tid(3) &&
           texture_calls[1] == CALL_UNLOCK * 0x10000u + tid(4));
    assert(drDone_751 == tid(5));

    // T7: cause 2 repoints drTexLoaded, then flushing returns early.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    flushing_755 = 1;
    drTexLoad_748 = tid(5);
    drTexLoaded_749 = tid(6);
    assert(reloadDMA__Fi(2) == 1);
    assert(drTexLoaded_749 == tid(5));
    assert(sema_calls.empty());

    // T8: a full bucket with idle channels reaches the semaphore tail
    // through iSignalSema for non--1 causes (done word preset, no reframe).
    // (The cause -1 semaphore path is already covered by T2.)
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 2;
    bucket = 7;
    sentDone_752 = 0;
    assert(reloadDMA__Fi(5) == 1);
    assert(bucket == 8 && sentDone_752 == 1 && sema_calls.size() == 1u &&
           sema_calls[0] == 0x20000u + 0x42u);

    // T9: tex upload chain for a cachein texture with a backing store.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    set_tex(0, 0, 0, tid(1), 0u, 0, 0, 0);
    set_tex(1, 0x80u, 0, 0, 0, 0x90u, 0x70u, 0);
    set_half(1, 24, 0);
    set_half(1, 4, 3);
    drTexLoad_748 = tid(0);
    drDraw_750 = tid(0);
    drTexLoaded_749 = tid(0);
    drDone_751 = tid(0);
    assert(reloadDMA__Fi(7) == 1);
    assert(texture_calls.size() == 1u &&
           texture_calls[0] == CALL_CACHEIN * 0x10000u + tid(1));
    const unsigned list = (((0u + 0u) + tid(0)) << 3) + 0x6142c0u | 0x30000000u;
    assert(have_store(list, 0x50000070ull, 8));
    assert(have_store(list + 8u, 0ull, 8));
    assert(have_store((0x70u | 0x20000000u) + 36u, 0u, 2));
    assert(have_store((0x70u | 0x20000000u) + 1156u, 4u, 2));
    assert(have_store(list + 16u, 0x70000000ull, 8));
    assert(have_store(0x1000a030u, (list & 0x0fffffffu), 4));
    assert(have_store(0x1000a000u, 261ull, 4));
    assert((channelBusy_746 & 4u) != 0u && texDmaListFlip == 1u && preTex_756 == 0);
    assert(texTypes_758 == 1u);
    assert(drTexLoad_748 == 0u && drDraw_750 == tid(0));

    // T10: tex upload small path (no backing store) with the mode halves.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    set_tex(0, 0, 0, tid(1), 0u, 0, 0, 0);
    set_tex(1, 0x80u, 0, 0, 0, 0x90u, 0u, 0);
    set_half(1, 24, 0);
    set_half(1, 8, 4);
    set_half(1, 6, 7);
    set_half(1, 4, 3);
    drTexLoad_748 = tid(0);
    drDraw_750 = tid(0);
    drTexLoaded_749 = tid(0);
    drDone_751 = tid(0);
    assert(reloadDMA__Fi(7) == 1);
    assert(texture_calls.size() == 1u &&
           texture_calls[0] == CALL_CACHEIN * 0x10000u + tid(1));
    const unsigned list2 = (((0u + 0u) + tid(0)) << 3) + 0x6142c0u | 0x30000000u;
    assert(have_store((0x90u | 0x20000000u) + 20u, 0u, 2));
    assert(have_store((0x90u | 0x20000000u) + 1140u, 4u, 2));
    assert(have_store((0x90u | 0x20000000u) + 180u, 4u, 2));
    assert(have_store(list2, 0x30000097ull, 8));
    assert(have_store(list2 + 16u, 0x70000000ull, 8));
    assert(texTypes_758 == 2u);
    assert(drTexLoad_748 == 0u && drDraw_750 == tid(0));

    // T11: s3 budget trip consumes the head without a second texture.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    set_tex(0, 0, 0, tid(1), tid(2), 0, 0, 0);
    set_tex(1, 0x80u, 0, 0, 0, 0x90u, 0x70u, 0);
    set_tex(2, 0, 0, 0, 0u, 0, 0, 0);
    set_half(1, 24, 0);
    set_half(1, 4, 2000);
    drTexLoad_748 = tid(0);
    drDraw_750 = tid(0);
    drTexLoaded_749 = tid(0);
    drDone_751 = tid(0);
    assert(reloadDMA__Fi(7) == 1);
    assert(texture_calls.size() == 1u &&
           texture_calls[0] == CALL_CACHEIN * 0x10000u + tid(1));
    assert(drTexLoad_748 == tid(2));
    const unsigned list3 = (((0u + 0u) + tid(0)) << 3) + 0x6142c0u | 0x30000000u;
    assert(have_store(list3 + 16u, 0x70000000ull, 8));

    // T12: vif send chain forwards one small node then closes the packet.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 0;
    bucket = 8;
    sentDone_752 = 1;
    set_tex(0, 0x41u, 0, tid(1), tid(2), 0, 0, 0);
    set_tex(1, 0, 0, 0, 0x55u, 0, 0, 0);
    set_tex(2, 0, 0, 0, 0, 0, 0, 0);
    drDraw_750 = tid(0);
    drTexLoaded_749 = tid(2);
    drDone_751 = tid(2);
    assert(reloadDMA__Fi(9) == 1);
    const unsigned vlist = (((0u << 8) + 0u) << 3) + 0x6152c0u | 0x30000000u;
    assert(have_store(vlist, 0x50000055ull, 8));
    assert(have_store(vlist + 8u, 0ull, 8));
    assert(have_store(vlist + 16u, 0x70000000ull, 8));
    assert(have_store(0x10009030u, (vlist & 0x0fffffffu), 4));
    assert(have_store(0x1000a000u, 325ull, 4));
    assert((channelBusy_746 & 2u) != 0u && vifDmaListFlip == 1u && preVif_757 == 0);
    assert(drDraw_750 == tid(2));

    // T13: bucket refill locks a fresh head, then the queues settle.
    reset_state();
    initialized_744 = 1;
    channelBusy_746 = 0;
    channelDone_747 = 2;
    bucket = 0;
    sentDone_752 = 1;
    set_draw_word(4, 0x4000u);
    mem_write(0x4000u + 12u, tid(0), 4);
    set_tex(0, 0, 0, tid(5), 0u, 0, 0, 0);
    set_tex(5, 0, 0, 0, 0, 0, 0, 0);
    set_half(5, 24, 9);
    assert(reloadDMA__Fi(7) == 1);
    assert(texture_calls.size() == 1u &&
           texture_calls[0] == CALL_LOCK * 0x10000u + tid(5));
    assert(bucket == 1 && channelDone_747 == 0u);
    assert(drDraw_750 == 0u && drDone_751 == tid(0));
    assert(drTexLoad_748 == 0u && drTexLoaded_749 == 0u);

    std::puts("reload dma contract passed");
    return 0;
}
