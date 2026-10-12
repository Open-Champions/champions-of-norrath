// Host contract for netSendMessage. Not an EE plan input.
#include "net_send.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned char netNetworkMode = 0;
extern "C" unsigned char partners[17632 * 2] = {};
extern "C" void (*msgHandler[8])(unsigned char* bytes, int length) = {};

static int local_calls;
static int handler_calls;
static unsigned char* handler_bytes;
static int handler_length;

extern "C" int netGetLocalPlayerNm__Fi(int index) {
    ++local_calls;
    if (index != 0) return 99;
    return 0;
}

static void capture(unsigned char* bytes, int length) {
    ++handler_calls;
    handler_bytes = bytes;
    handler_length = length;
}

static unsigned load_counter() {
    return static_cast<unsigned>(partners[17580]) |
           (static_cast<unsigned>(partners[17581]) << 8) |
           (static_cast<unsigned>(partners[17582]) << 16) |
           (static_cast<unsigned>(partners[17583]) << 24);
}

static void store_counter(unsigned value) {
    partners[17580] = static_cast<unsigned char>(value);
    partners[17581] = static_cast<unsigned char>(value >> 8);
    partners[17582] = static_cast<unsigned char>(value >> 16);
    partners[17583] = static_cast<unsigned char>(value >> 24);
}

static unsigned char ring_at(int delta) {
    return partners[12 + 9376 + delta];
}

int main() {
    std::memset(partners, 0, sizeof partners);
    msgHandler[3] = capture;
    unsigned char payload[4] = {10, 20, 30, 40};
    netNetworkMode = 0;
    local_calls = 0;
    handler_calls = 0;
    netSendMessage__FiPUci(3, payload, 4);
    if (handler_calls != 1 || handler_bytes != payload || handler_length != 4 || local_calls != 0) return 1;
    if (load_counter() != 0 || ring_at(0) != 0) return 2;

    netNetworkMode = 1;
    store_counter(10);
    netSendMessage__FiPUci(0x123, payload, 3);
    if (local_calls != 1) return 3;
    if (ring_at(10) != 0x23 || ring_at(11) != 3) return 4;
    if (ring_at(12) != 10 || ring_at(13) != 20 || ring_at(14) != 30) return 5;
    if (load_counter() != 15) return 6;

    store_counter(0xfffffffeu);
    unsigned char one = 7;
    netSendMessage__FiPUci(1, &one, 1);
    if (ring_at(-2) != 1 || ring_at(-1) != 1) return 7;
    if (ring_at(0) != 7) return 8;
    if (load_counter() != 1) return 9;

    store_counter(0);
    netSendMessage__FiPUci(9, payload, 0);
    if (ring_at(0) != 9 || ring_at(1) != 0 || load_counter() != 2) return 10;

    store_counter(4);
    netSendMessage__FiPUci(0x105, payload, -3);
    if (ring_at(4) != 5 || ring_at(5) != static_cast<unsigned char>(-3)) return 11;
    if (load_counter() != ((4u + static_cast<unsigned>(-3) + 2u) & 0x1fffu)) return 12;

    netNetworkMode = 0x80;
    handler_calls = 0;
    store_counter(0);
    netSendMessage__FiPUci(2, payload, 1);
    if (handler_calls != 0 || ring_at(0) != 2 || ring_at(1) != 1 || ring_at(2) != 10) return 13;
    if (load_counter() != 3) return 14;
    if (partners[17632] != 0) return 15;
    return 0;
}
