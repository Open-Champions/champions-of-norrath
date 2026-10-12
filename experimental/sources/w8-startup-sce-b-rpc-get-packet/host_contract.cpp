#include "rpc_get_packet.cpp"

#include <cstdio>

static int disables;
static int enables;
static int order_bad;

extern "C" int DIntr() {
    if (disables != enables) order_bad = 1;
    return ++disables;
}
extern "C" int EIntr() {
    if (disables != enables + 1) order_bad = 1;
    return ++enables;
}

int main() {
    RpcPacket packets[4] = {};
    RpcPacketPool pool = {0, packets, 4};

    RpcPacket* got = _sceRpcGetPacket(&pool);
    if (got != &packets[0] || packets[0].words[4] != 5u || packets[0].words[6] != 1u) return 1;
    if (packets[0].words[5] != static_cast<unsigned>(reinterpret_cast<unsigned long>(got))) return 2;
    if (pool.next_id != 2u) return 3;
    if (disables != 1 || enables != 1 || order_bad) return 4;

    got = _sceRpcGetPacket(&pool);
    if (got != &packets[1] || packets[1].words[4] != 0x10005u || packets[1].words[6] != 3u) return 5;
    if (pool.next_id != 3u) return 6;

    // A taken packet in the middle is skipped; bit 0 alone decides.
    packets[2].words[4] = 0xfffeu;
    packets[3].words[4] = 0;
    got = _sceRpcGetPacket(&pool);
    if (got != &packets[2] || packets[2].words[4] != 0x20005u) return 7;
    packets[2].words[4] = 0x20005u;
    got = _sceRpcGetPacket(&pool);
    if (got != &packets[3] || packets[3].words[4] != 0x30005u || packets[3].words[6] != 5u) return 8;

    // Pool exhausted: null, id untouched, interrupts restored.
    unsigned id = pool.next_id;
    int before = enables;
    if (_sceRpcGetPacket(&pool) != 0 || pool.next_id != id || enables != before + 1) return 9;

    // Empty pool.
    RpcPacketPool empty = {7, packets, 0};
    if (_sceRpcGetPacket(&empty) != 0 || empty.next_id != 7u) return 10;

    // Wrap: id 0xffffffff increments to 0, which is stored as is.
    packets[0].words[4] = 0;
    pool.next_id = 0xffffffffu;
    got = _sceRpcGetPacket(&pool);
    if (got != &packets[0] || got->words[6] != 0u || pool.next_id != 0u) return 11;
    if (disables != enables || order_bad) return 12;
    std::puts("_sceRpcGetPacket passed");
    return 0;
}
