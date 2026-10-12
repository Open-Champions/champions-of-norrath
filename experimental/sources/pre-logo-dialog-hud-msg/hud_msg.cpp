// Observed hudMsgCallback__FPUci at 0x001c1cc8, 740 bytes.
// The length is not read. The first byte selects one of seven entries in
// the jump table at 0x0046a5e0. Any other byte returns.
// Byte 0 calls hudAssignQuickSpell with the next three signed bytes.
// Byte 1 calls hudAssignSpellBookSpell with the next two signed bytes, the
// word at offset 4, and the eight bytes at offset 8.
// Byte 2 prints the text at 0x0046a590. The player record stride is 23984.
// The countdown byte is at record+23968 and the entity word is at
// record+21872. A null entity returns after the print. Busy 0 calls
// netBlankControllerButtons(index, 60, 240), stores 10 in the countdown,
// and clears bit 0x00800000 of entity+12. Any other busy value sets bit
// 0x00400000 and clears the countdown. Busy 2 or greater also sets bit
// 0x00800000.
// Byte 3 returns when DLG_Active is nonzero. Otherwise it calls
// debugRecall with the next signed byte, then gameNetKick(0) when the
// recallOn byte is nonzero.
// Byte 4 calls saveActivate with the word at offset 4.
// Byte 5 returns when the next signed byte is a local player. Otherwise it
// clears bit 0x00400000 of the entity word at record+12 when that word is
// nonzero. When the float at record+172 is greater than zero it calls the
// function pointer at (entity+72)+20 with the entity plus the halfword at
// (entity+72)+16, integer 0, bits 0x20000040, and the float bits
// 0x4cbec20, then calls playerDamageHandler. It then copies the record
// bytes through ustrcpy, appends textGetString("netMessagePlayerExit"),
// calls hudSetInfoMessage with 300, -1, and 0, and calls
// netDisconnectPlayer. Those callees and player are not defined here.
// Byte 6 copies the bytes after the selector into levelName, stores 0xff
// at levelName+80, calls debugClearStartMenu, stores 60 in
// gameControlPaused, stores 0 in gamePause and particlesPaused, and fades
// stream 0 when soundGetStreamPlayStatus(0) has bit 0x40.
static void copy_bytes(void* dest, const void* src, unsigned size) {
    auto* out = static_cast<unsigned char*>(dest);
    auto* in = static_cast<const unsigned char*>(src);
    for (unsigned i = 0; i < size; ++i) out[i] = in[i];
}

extern "C" void hudAssignQuickSpell__Fiii(int first, int second, int third);
extern "C" void hudAssignSpellBookSpell__Fiiil(int first, int second, int third, long long rest);
extern "C" unsigned char player[];
extern "C" void scePrintf(const char* text, int player_index, int busy, int countdown);
extern "C" void netBlankControllerButtons__Fiii(int player_index, int first, int second);
extern "C" int DLG_Active__Fv();
extern "C" void debugRecall__Fi(int value);
extern "C" signed char recallOn;
extern "C" void gameNetKick__Fi(int value);
extern "C" void saveActivate__9Savepoint(unsigned value);
extern "C" int netPlayerIsLocal__Fi(int player_index);
extern "C" void playerDamageHandler__6Player(unsigned char* entity);
extern "C" void ustrcpy__FPUsPc(unsigned short* dest, char* src);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void ustrcat__FPUsT0(unsigned short* dest, unsigned short* src);
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player_index, int extra);
extern "C" void netDisconnectPlayer__Fi(int player_index);
extern "C" char levelName[];
extern "C" char* strcpy(char* dest, const char* src);
extern "C" void debugClearStartMenu__Fv();
extern "C" int gameControlPaused;
extern "C" int gamePause;
extern "C" signed char particlesPaused;
extern "C" int soundGetStreamPlayStatus__Fi(int stream);
extern "C" void soundFadeStream__Fi(int stream);

static unsigned load32(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) |
           (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) |
           (static_cast<unsigned>(bytes[3]) << 24);
}

static void store32(unsigned char* bytes, unsigned value) {
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
}

static void* load_pointer(const unsigned char* bytes) {
#if defined(NORRATH_HOST_CONTRACT)
    void* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
#else
    return reinterpret_cast<void*>(static_cast<unsigned long>(load32(bytes)));
#endif
}

static unsigned char* record_for(int index) {
    return player + index * 23984;
}

static void disconnect_player(int index) {
    unsigned short text[256];
    ustrcpy__FPUsPc(text, reinterpret_cast<char*>(record_for(index)));
    ustrcat__FPUsT0(text, textGetString__FPc(const_cast<char*>("netMessagePlayerExit")));
    hudSetInfoMessage__FPUsiic(text, 300, -1, 0);
    netDisconnectPlayer__Fi(index);
}

extern "C" void hudMsgCallback__FPUci(unsigned char* message, int length) {
    (void)length;
    if (message[0] > 6) return;
    switch (message[0]) {
    case 0:
        hudAssignQuickSpell__Fiii(static_cast<signed char>(message[1]),
                                  static_cast<signed char>(message[2]),
                                  static_cast<signed char>(message[3]));
        return;
    case 1: {
        long long rest = 0;
        copy_bytes(&rest, message + 8, sizeof rest);
        hudAssignSpellBookSpell__Fiiil(static_cast<signed char>(message[1]),
                                       static_cast<signed char>(message[2]),
                                       static_cast<int>(load32(message + 4)),
                                       rest);
        return;
    }
    case 2: {
        const signed char index = static_cast<signed char>(message[1]);
        const signed char busy = static_cast<signed char>(message[2]);
        unsigned char* record = record_for(index);
        const unsigned countdown = record[23968];
        scePrintf("MSG RECV:  Set player %d to busy=%d, countdown=%d\n", index, busy,
                  static_cast<int>(countdown));
        auto* entity = static_cast<unsigned char*>(load_pointer(record + 21872));
        if (entity == nullptr) return;
        unsigned flags = load32(entity + 12);
        if (busy == 0) {
            netBlankControllerButtons__Fiii(index, 60, 240);
            record[23968] = 10;
            store32(entity + 12, flags & 0xff7fffffu);
            return;
        }
        flags |= 0x00400000u;
        record[23968] = 0;
        if (busy >= 2) flags |= 0x00800000u;
        store32(entity + 12, flags);
        return;
    }
    case 3:
        if (DLG_Active__Fv() != 0) return;
        debugRecall__Fi(static_cast<signed char>(message[1]));
        if (recallOn == 0) return;
        gameNetKick__Fi(0);
        return;
    case 4:
        saveActivate__9Savepoint(load32(message + 4));
        return;
    case 5: {
        const signed char index = static_cast<signed char>(message[1]);
        if (netPlayerIsLocal__Fi(index) != 0) return;
        unsigned char* record = record_for(index);
        auto* entity = static_cast<unsigned char*>(load_pointer(record + 21872));
        if (entity != nullptr) {
            store32(entity + 12, load32(entity + 12) & 0xffbfffffu);
            float amount = 0;
            copy_bytes(&amount, record + 172, sizeof amount);
            if (0.0f < amount) {
                auto* table = static_cast<unsigned char*>(load_pointer(entity + 72));
                const short offset = static_cast<short>(static_cast<unsigned>(table[16]) |
                                                        (static_cast<unsigned>(table[17]) << 8));
                auto* method = reinterpret_cast<void (*)(unsigned char*, int, unsigned, float)>(
                    load_pointer(table + 20));
                unsigned bits = 0x4cbec20u;
                float passed = 0;
                copy_bytes(&passed, &bits, sizeof passed);
                method(entity + offset, 0, 0x20000040u, passed);
                playerDamageHandler__6Player(entity);
            }
        }
        disconnect_player(index);
        return;
    }
    case 6:
        strcpy(levelName, reinterpret_cast<char*>(message + 1));
        levelName[80] = static_cast<char>(0xff);
        debugClearStartMenu__Fv();
        gameControlPaused = 60;
        gamePause = 0;
        particlesPaused = 0;
        if ((soundGetStreamPlayStatus__Fi(0) & 0x40) != 0) soundFadeStream__Fi(0);
        return;
    }
}
