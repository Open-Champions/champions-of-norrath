// Host check for invMsgCallback__FPUci. Every handler is a stub that records
// its name and arguments; packets are built field by field at the offsets
// listed in msg_callback.cpp.
#define NORRATH_INVENTORY_HOST 1
#include "msg_callback.cpp"
#include <cstdint>
#include <cstring>

extern "C" unsigned char player[23984 * 2] = {};

static const char* called;
static long long args[7];
static int calls;
static void note(const char* name, long long a = 0, long long b = 0, long long c = 0, long long d = 0, long long e = 0, long long f = 0, long long g = 0) {
    called = name;
    ++calls;
    args[0] = a; args[1] = b; args[2] = c; args[3] = d; args[4] = e; args[5] = f; args[6] = g;
}
static long long addr(const void* p) { return static_cast<long long>(reinterpret_cast<std::uintptr_t>(p)); }
extern "C" {
void inventoryGridDropItem__Fiiciiii(int a, int b, char c, int d, int e, int f, int g) { note("drop", a, b, c, d, e, f, g); }
void equipItem__FiPQ24item11descriptioni(int a, void* b, int c) { note("equip", a, addr(b), c); }
void unequipItem__FiPQ24item11description(int a, void* b) { note("unequip", a, addr(b)); }
void inventoryExit__Fiii(int a, int b, int c) { note("exit", a, b, c); }
void resetPlayerAnimations__Fi(int a) { note("reset", a); }
void inventoryAdjustAbility__Fii(int a, int b) { note("ability", a, b); }
void inventoryAdjustSkill__Ficx(int a, char b, long long c) { note("skill", a, b, c); }
void shopItemSetAttributes__FiiPcPQ24item11description(int a, int b, char* c, void* d) { note("attributes", a, b, addr(c), addr(d)); }
void drinkPotion__6Playeriiii(void* self, int a, int b, int c, int d) { note("potion", addr(self), a, b, c, d); }
}

template <class T> static void put(unsigned char* p, int offset, T value) { std::memcpy(p + offset, &value, sizeof value); }
static unsigned char packet[32];
static void start(int type) { std::memset(packet, 0x5a, sizeof packet); called = nullptr; calls = 0; packet[0] = static_cast<unsigned char>(type); }
static void run() { invMsgCallback__FPUci(packet, 99); }
#define CHECK(c, code) do { if (!(c)) return code; } while (0)
static bool is(const char* name) { return called != nullptr && std::strcmp(called, name) == 0; }

int main() {
    start(0);
    packet[1] = 0xfe;                      // player -2
    put<short>(packet, 2, -300);
    put<int>(packet, 4, 123456);
    put<short>(packet, 8, 700);
    packet[10] = 0x80; packet[11] = 0xff; packet[12] = 3;
    run();
    CHECK(is("drop") && calls == 1, 1);
    CHECK(args[0] == -2 && args[1] == 123456 && args[2] == -128 && args[3] == -300 && args[4] == 700 && args[5] == -1 && args[6] == 3, 2);

    start(1);
    packet[1] = 7;
    put<short>(packet, 2, -5);
    put<unsigned>(packet, 4, 0x00123456u);
    run();
    CHECK(is("equip") && args[0] == -5 && args[1] == 0x123456 && args[2] == 7, 3);

    start(2);
    put<short>(packet, 2, 9);
    put<unsigned>(packet, 4, 0x777u);
    run();
    CHECK(is("unequip") && args[0] == 9 && args[1] == 0x777, 4);

    start(3);
    run();
    CHECK(calls == 0, 5);

    start(4);
    packet[1] = 1; packet[2] = 0xfd; packet[3] = 4;
    run();
    CHECK(is("exit") && args[0] == 1 && args[1] == -3 && args[2] == 4, 6);

    start(5);
    packet[1] = 1; packet[2] = 0x41;
    run();
    CHECK(is("reset") && args[0] == 1 && player[23984 + 23582] == 0x41, 7);

    start(6);
    packet[1] = 0; put<short>(packet, 2, -2);
    run();
    CHECK(is("ability") && args[0] == 0 && args[1] == -2, 8);

    start(7);
    packet[1] = 1; packet[2] = 0xff;
    put<int>(packet, 4, -70000);
    run();
    CHECK(is("skill") && args[0] == 1 && args[1] == -1 && args[2] == -70000, 9);

    start(8);
    packet[1] = 1;
    put<int>(packet, 4, 11);
    put<unsigned>(packet, 8, 0x2000u);
    put<unsigned>(packet, 12, 0x3000u);
    run();
    CHECK(is("attributes") && args[0] == 1 && args[1] == 11 && args[2] == 0x2000 && args[3] == 0x3000, 10);

    start(9);
    packet[1] = 1;
    put<short>(packet, 2, 0x1234);
    run();
    CHECK(calls == 0 && player[23984 + 88] == 0x34, 11);

    start(10);
    packet[1] = 1;
    put<short>(packet, 2, 1); put<short>(packet, 4, -2); put<short>(packet, 6, 3); put<short>(packet, 8, -4);
    put<unsigned>(player + 23984, 21872, 0x4400u);
    run();
    CHECK(is("potion") && args[0] == 0x4400 && args[1] == 1 && args[2] == -2 && args[3] == 3 && args[4] == -4, 12);

    start(11);
    run();
    CHECK(calls == 0, 13);
    start(0x80);
    run();
    CHECK(calls == 0, 14);
    start(0xff);
    run();
    CHECK(calls == 0, 15);
    return 0;
}
