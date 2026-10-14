// Host contract for hudMsgCallback. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "hud_msg.cpp"

#include <cstdint>
#include <cstring>

extern "C" unsigned char player[23984] = {};
extern "C" signed char recallOn = 0;
extern "C" char levelName[96] = {};
extern "C" int gameControlPaused = 0;
extern "C" int gamePause = 1;
extern "C" signed char particlesPaused = 1;

static int quick_a, quick_b, quick_c, quick_calls;
static int book_a, book_b, book_c, book_calls;
static long long book_rest;
static int prints;
static int print_player, print_busy, print_count;
static int blanks, blank_player, blank_a, blank_b;
static int dialog_active;
static int recalls, recall_value;
static int kicks;
static unsigned saved;
static int local_result;
static int damage_calls;
static unsigned char* damaged;
static int method_calls;
static unsigned char* method_object;
static unsigned method_flags;
static unsigned method_bits;
static int info_calls, info_duration, info_player, info_extra;
static unsigned short* info_text;
static int disconnects, disconnect_index;
static int clears;
static int status_result;
static int fades;
static signed char paused_at_status;

extern "C" void hudAssignQuickSpell__Fiii(int first, int second, int third) {
    ++quick_calls;
    quick_a = first;
    quick_b = second;
    quick_c = third;
}
extern "C" void hudAssignSpellBookSpell__Fiiil(int first, int second, int third, long long rest) {
    ++book_calls;
    book_a = first;
    book_b = second;
    book_c = third;
    book_rest = rest;
}
extern "C" void scePrintf(const char* text, int player_index, int busy, int countdown) {
    ++prints;
    const char* want = "MSG RECV:  Set player %d to busy=%d, countdown=%d\n";
    for (int i = 0;; ++i) {
        if (text[i] != want[i]) prints = -1;
        if (want[i] == 0) break;
    }
    print_player = player_index;
    print_busy = busy;
    print_count = countdown;
}
extern "C" void netBlankControllerButtons__Fiii(int player_index, int first, int second) {
    ++blanks;
    blank_player = player_index;
    blank_a = first;
    blank_b = second;
}
extern "C" int DLG_Active__Fv() { return dialog_active; }
extern "C" void debugRecall__Fi(int value) { ++recalls; recall_value = value; }
extern "C" void gameNetKick__Fi(int value) { ++kicks; if (value != 0) kicks = -1; }
extern "C" void saveActivate__9Savepoint(unsigned value) { saved = value; }
extern "C" int netPlayerIsLocal__Fi(int) { return local_result; }
extern "C" void playerDamageHandler__6Player(unsigned char* entity) { ++damage_calls; damaged = entity; }
extern "C" void ustrcpy__FPUsPc(unsigned short* dest, char* src) {
    unsigned i = 0;
    while (src[i] != 0) { dest[i] = static_cast<unsigned char>(src[i]); ++i; }
    dest[i] = 0;
}
extern "C" unsigned short* textGetString__FPc(char* key) {
    static unsigned short extra[2] = {0x21, 0};
    if (std::strcmp(key, "netMessagePlayerExit") != 0) extra[0] = 0;
    return extra;
}
extern "C" void ustrcat__FPUsT0(unsigned short* dest, unsigned short* src) {
    unsigned i = 0;
    while (dest[i] != 0) ++i;
    unsigned j = 0;
    while (src[j] != 0) dest[i++] = src[j++];
    dest[i] = 0;
}
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player_index, int extra) {
    ++info_calls;
    info_text = text;
    info_duration = duration;
    info_player = player_index;
    info_extra = extra;
}
extern "C" void netDisconnectPlayer__Fi(int player_index) { ++disconnects; disconnect_index = player_index; }
extern "C" char* strcpy(char* dest, const char* src) {
    char* out = dest;
    while ((*out++ = *src++) != 0) {}
    return dest;
}
extern "C" void debugClearStartMenu__Fv() { ++clears; }
extern "C" int soundGetStreamPlayStatus__Fi(int stream) {
    paused_at_status = particlesPaused;
    return stream == 0 ? status_result : 0;
}
extern "C" void soundFadeStream__Fi(int stream) { if (stream == 0) ++fades; }

static void store_ptr(unsigned char* bytes, void* value) {
    auto* in = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) bytes[i] = in[i];
}

static void on_method(unsigned char* object, int zero, unsigned flags, float amount) {
    ++method_calls;
    method_object = object;
    method_flags = flags;
    std::memcpy(&method_bits, &amount, 4);
    if (zero != 0) method_calls = -1;
}

int main() {
    unsigned char message[24] = {};
    message[0] = 9;
    hudMsgCallback__FPUci(message, 1);
    if (quick_calls != 0) return 1;

    message[0] = 0;
    message[1] = 0x80;
    message[2] = 2;
    message[3] = 3;
    hudMsgCallback__FPUci(message, 4);
    if (quick_calls != 1 || quick_a != static_cast<signed char>(0x80) || quick_b != 2 || quick_c != 3) return 2;

    message[0] = 1;
    message[1] = 4;
    message[2] = 5;
    message[4] = 0x78;
    message[5] = 0x56;
    message[6] = 0x34;
    message[7] = 0x12;
    message[8] = 1;
    message[9] = 2;
    message[10] = 3;
    message[11] = 4;
    message[12] = 5;
    message[13] = 6;
    message[14] = 7;
    message[15] = 8;
    hudMsgCallback__FPUci(message, 16);
    if (book_calls != 1 || book_a != 4 || book_b != 5 || book_c != static_cast<int>(0x12345678u)) return 3;
    if (book_rest != 0x0807060504030201ll) return 4;

    std::memset(player, 0, sizeof player);
    unsigned char entity[96] = {};
    store_ptr(player + 21872, entity);
    player[23968] = 4;
    entity[12] = 0xff;
    entity[14] = 0xc0;
    message[0] = 2;
    message[1] = 0;
    message[2] = 0;
    hudMsgCallback__FPUci(message, 3);
    if (prints != 1 || print_player != 0 || print_busy != 0 || print_count != 4) return 5;
    if (blanks != 1 || blank_player != 0 || blank_a != 60 || blank_b != 240) return 6;
    if (player[23968] != 10 || (entity[14] & 0x80) != 0) return 7;

    player[23968] = 4;
    message[2] = 1;
    hudMsgCallback__FPUci(message, 3);
    if (player[23968] != 0 || (entity[14] & 0x40) == 0 || (entity[14] & 0x80) != 0) return 8;

    message[2] = 3;
    hudMsgCallback__FPUci(message, 3);
    if ((entity[14] & 0xc0) != 0xc0) return 9;

    store_ptr(player + 21872, nullptr);
    prints = 0;
    message[2] = 1;
    hudMsgCallback__FPUci(message, 3);
    if (prints != 1 || blanks != 1) return 10;

    message[0] = 3;
    message[1] = 0xfe;
    dialog_active = 1;
    hudMsgCallback__FPUci(message, 2);
    if (recalls != 0) return 11;
    dialog_active = 0;
    recallOn = 0;
    hudMsgCallback__FPUci(message, 2);
    if (recalls != 1 || recall_value != static_cast<signed char>(0xfe) || kicks != 0) return 12;
    recallOn = 1;
    hudMsgCallback__FPUci(message, 2);
    if (kicks != 1) return 13;

    message[0] = 4;
    message[4] = 0x11;
    message[5] = 0x22;
    message[6] = 0;
    message[7] = 0;
    hudMsgCallback__FPUci(message, 8);
    if (saved != 0x2211u) return 14;

    message[0] = 5;
    message[1] = 0;
    local_result = 1;
    hudMsgCallback__FPUci(message, 2);
    if (disconnects != 0) return 15;
    local_result = 0;
    player[0] = 'A';
    player[1] = 0;
    float zero = 0;
    std::memcpy(player + 172, &zero, 4);
    store_ptr(player + 21872, entity);
    entity[12] = 0xff;
    entity[13] = 0xff;
    entity[14] = 0xff;
    entity[15] = 0xff;
    hudMsgCallback__FPUci(message, 2);
    if (method_calls != 0 || damage_calls != 0 || (entity[14] & 0x40) != 0) return 16;
    if (disconnects != 1 || disconnect_index != 0 || info_duration != 300 || info_player != -1 || info_extra != 0) return 17;
    if (info_text[0] != 'A' || info_text[1] != 0x21) return 18;

    float one = 1.0f;
    std::memcpy(player + 172, &one, 4);
    unsigned char table[32] = {};
    table[16] = 8;
    table[17] = 0;
    store_ptr(table + 20, reinterpret_cast<void*>(on_method));
    store_ptr(entity + 72, table);
    entity[14] = 0xff;
    hudMsgCallback__FPUci(message, 2);
    if (method_calls != 1 || method_object != entity + 8 || method_flags != 0x20000040u || method_bits != 0x4cbec20u) return 19;
    if (damage_calls != 1 || damaged != entity) return 20;

    message[0] = 6;
    message[1] = 'l';
    message[2] = 'a';
    message[3] = 0;
    status_result = 0x40;
    fades = 0;
    clears = 0;
    gamePause = 7;
    particlesPaused = 3;
    hudMsgCallback__FPUci(message, 4);
    if (std::strcmp(levelName, "la") != 0 || static_cast<unsigned char>(levelName[80]) != 0xff) return 21;
    if (clears != 1 || gameControlPaused != 60 || gamePause != 0 || paused_at_status != 0 || fades != 1) return 22;
    return 0;
}
