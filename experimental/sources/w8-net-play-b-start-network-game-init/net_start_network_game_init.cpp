// Observed netStartNetworkGameInit at 0x0012cf60, 1788 bytes, returning
// nothing. It first stores 0 to nmPartners.
//
// Broadcast mode (netBroadCastStart nonzero): a 64-byte parameter block is
// cleared and its word 1 set to 3000; sceInetCreate(cd, g_net_buf, &block)
// gives a handle, which is stored in broadcastConnection; then
// sceInetOpen(cd, g_net_buf, handle, 1000) is called and its result is not
// kept (the image stores the handle in the branch delay slot of that call). Partner record 0 (17632-byte stride in `partners`) gets bytes 0..3
// from myaddr bytes 4..7 and byte 17620 = 1; nmPartners becomes 1; and the
// word at 17600 of each of the four records becomes 1.
//
// Launch mode (netBroadCastStart zero), with info = netGameLaunchInfo, whose
// byte 0 holds flag bits (2, 4, and 1 are tested):
//  1. If bit 2 is set, MC_BuildPackedPlayer(playerSelectInfo, 0).
//  2. If bit 2 or 4 is set, record 0 of the temporary player storage
//     (MC_GetTempPlayerStorage(0), 23984 bytes) is copied to the record at
//     index info[6] (signed byte) of the same storage, through a local copy.
//  3. lnet_buf = memAlloc(0x8000, "Net Launch buff"); a second cleared
//     64-byte block (word 1 = 3000) goes to sceInetCreate; its result is
//     stored in udpLaunchConnection and passed to sceInetOpen(cd, lnet_buf,
//     handle, 1000), whose result is not kept.
//     hostLaunchMask becomes -1; currentTime, lastPingTime, the three
//     launch/quit flags, hostStartedWithoutMe, netLaunchTimer and
//     netLaunchExpirationCount become 0.
//  4. For i below info[4] (signed byte, read again after each pass), the
//     252-byte entry launchPeers[i] is cleared and filled from info arrays
//     (see the code for the exact offsets). Entry bytes 92 and 93 depend on
//     how i compares with info[6] and info[5]. Bytes 0..15 are copied to
//     16..31, word 48 to word 56, word 64 is i, word 68 and byte 100 are 0.
//  5. With e = launchPeers[info[6]]: if bit 1 is set and bits 2 and 4 are
//     clear, temporary storage record 0 is overwritten with the RAM player
//     (MC_ByteCopy of 23984 bytes) and its word at 21872 is cleared.
//     MC_ConvertTempPlayerToReal(0) runs. Then the name of record 0 of
//     `player` is copied to e+104, and e+136..e+140, e+141..e+144, e+148,
//     e+152 and fourteen words from MC_GameConfig+52 to e+164 are copied from
//     that record. If bit 1 is set and bit 2 or 4 is set, e+156 is
//     gameDifficulty. e+220 is 512 with isModem set and 1300 otherwise.
//     compressSize is MC_NetPack() when bit 1 is set and bits 2 and 4 are
//     clear, else -1. e+160 is 41 if bit 2 is set, else 23984 if bit 4 is set,
//     else compressSize if bit 1 is set, else unchanged.
// Inferred: launchPeers entries describe each peer for a network launch; the
// flag bits select the host, client and local cases. The two local record
// copies in the image also run constructors for 64-entry lists whose stores
// are overwritten by the whole-record copy that follows; they are not
// reproduced because they leave no lasting effect.
// Unknown: the meaning of the entry fields and of info bit 1, and the layout
// of the launch info beyond the offsets read here.
extern "C" {
extern signed char netBroadCastStart;
extern int nmPartners;
extern int broadcastConnection;
extern unsigned char myaddr[];
extern unsigned char partners[];
extern unsigned char cd[];
extern unsigned char g_net_buf[];
extern unsigned char netGameLaunchInfo[];
extern unsigned char launchPeers[];
extern unsigned char playerSelectInfo[];
extern unsigned char player[];
extern unsigned char MC_GameConfig[];
extern void* lnet_buf;
extern int udpLaunchConnection;
extern signed char hostLaunchMask;
extern int currentTime;
extern int lastPingTime;
extern signed char netNewLaunchMaybeQuit;
extern signed char netNewLaunchMaybeLaunch;
extern signed char hostForcedLaunch;
extern signed char hostStartedWithoutMe;
extern int netLaunchTimer;
extern int netLaunchExpirationCount;
extern int compressSize;
extern int gameDifficulty;
extern signed char isModem;
void* memset(void* destination, int value, unsigned long length);
char* strcpy(char* destination, const char* source);
int sceInetCreate(void* object, void* buffer, void* parameters);
int sceInetOpen(void* object, void* buffer, int handle, int options);
void* memAlloc__FiPc(int size, char* tag);
void MC_BuildPackedPlayer__FP11playerEntryi(void* entry, int index);
void* MC_GetTempPlayerStorage__Fi(int index);
void* MC_GetRamPlayerPtr__Fi(int index);
void MC_ByteCopy__FPvT0i(void* destination, void* source, int length);
void MC_ConvertTempPlayerToReal__Fi(int index);
int MC_NetPack__Fv();
}

namespace {
struct PlayerRecord {
    unsigned char bytes[23984];
};

int word_at(const unsigned char* base, int offset) {
    return *reinterpret_cast<const int*>(base + offset);
}
void set_word(unsigned char* base, int offset, int value) {
    *reinterpret_cast<int*>(base + offset) = value;
}
}

extern "C" void netStartNetworkGameInit__Fv() {
    nmPartners = 0;
    if (netBroadCastStart != 0) {
        int parameters[16];
        memset(parameters, 0, 64);
        parameters[1] = 3000;
        parameters[0] = 0;
        int handle = sceInetCreate(cd, g_net_buf, parameters);
        broadcastConnection = handle;
        sceInetOpen(cd, g_net_buf, handle, 1000);
        partners[0] = myaddr[4];
        partners[1] = myaddr[5];
        partners[2] = myaddr[6];
        partners[3] = myaddr[7];
        partners[17620] = 1;
        nmPartners = 1;
        for (int i = 0; i < 4; ++i) set_word(partners, i * 17632 + 17600, 1);
        return;
    }
    unsigned char* info = netGameLaunchInfo;
    if ((info[0] & 2) != 0) MC_BuildPackedPlayer__FP11playerEntryi(playerSelectInfo, 0);
    if ((info[0] & 6) != 0) {
        PlayerRecord* storage = static_cast<PlayerRecord*>(MC_GetTempPlayerStorage__Fi(0));
        PlayerRecord copy = storage[0];
        storage[static_cast<signed char>(info[6])] = copy;
    }
    lnet_buf = memAlloc__FiPc(0x8000, const_cast<char*>("Net Launch buff"));
    {
        int parameters[16];
        memset(parameters, 0, 64);
        parameters[1] = 3000;
        parameters[0] = 0;
        int handle = sceInetCreate(cd, g_net_buf, parameters);
        udpLaunchConnection = handle;
        sceInetOpen(cd, lnet_buf, handle, 1000);
    }
    hostLaunchMask = -1;
    currentTime = 0;
    lastPingTime = 0;
    netNewLaunchMaybeQuit = 0;
    netNewLaunchMaybeLaunch = 0;
    hostForcedLaunch = 0;
    hostStartedWithoutMe = 0;
    netLaunchTimer = 0;
    netLaunchExpirationCount = 0;
    for (int i = 0; i < static_cast<signed char>(info[4]); ++i) {
        unsigned char* peer = launchPeers + i * 252;
        memset(peer, 0, 252);
        int local = static_cast<signed char>(info[6]);
        if (i != local) {
            int other = static_cast<signed char>(info[5]);
            bool mark = true;
            if (local != other) {
                if (!(i < local) || other == i) mark = false;
            }
            if (mark) peer[93] |= 1;
            peer[92] = 0;
        }
        peer[4] = info[266 + 4 * i];
        peer[5] = info[265 + 4 * i];
        peer[6] = info[264 + 4 * i];
        peer[7] = info[263 + 4 * i];
        peer[36] = info[282 + 4 * i];
        peer[37] = info[281 + 4 * i];
        peer[38] = info[280 + 4 * i];
        peer[39] = info[279 + 4 * i];
        set_word(peer, 48, word_at(info, 296 + 4 * i));
        set_word(peer, 52, word_at(info, 312 + 4 * i));
        __builtin_memcpy(peer + 16, peer, 16);
        set_word(peer, 56, word_at(peer, 48));
        set_word(peer, 64, i);
        set_word(peer, 68, 0);
        peer[100] = 0;
    }
    unsigned char* entry = launchPeers + static_cast<signed char>(info[6]) * 252;
    if ((info[0] & 1) != 0 && (info[0] & 6) == 0) {
        void* storage = MC_GetTempPlayerStorage__Fi(0);
        void* ram = MC_GetRamPlayerPtr__Fi(0);
        MC_ByteCopy__FPvT0i(storage, ram, 23984);
        set_word(static_cast<unsigned char*>(MC_GetTempPlayerStorage__Fi(0)), 21872, 0);
    }
    MC_ConvertTempPlayerToReal__Fi(0);
    PlayerRecord record = *reinterpret_cast<PlayerRecord*>(player);
    strcpy(reinterpret_cast<char*>(entry + 104), reinterpret_cast<const char*>(record.bytes));
    entry[136] = record.bytes[84];
    entry[137] = record.bytes[32];
    entry[138] = record.bytes[33];
    entry[139] = record.bytes[35];
    entry[140] = record.bytes[34];
    for (int k = 0; k < 4; ++k) entry[141 + k] = record.bytes[144 + 4 * k];
    set_word(entry, 152, static_cast<signed char>(record.bytes[89]));
    set_word(entry, 148, word_at(record.bytes, 232));
    for (int k = 0; k < 14; ++k) set_word(entry, 164 + 4 * k, word_at(MC_GameConfig, 52 + 4 * k));
    if ((info[0] & 1) != 0 && (info[0] & 6) != 0) set_word(entry, 156, gameDifficulty);
    set_word(entry, 220, isModem != 0 ? 512 : 1300);
    if ((info[0] & 1) != 0 && (info[0] & 6) == 0) compressSize = MC_NetPack__Fv();
    else compressSize = -1;
    if ((info[0] & 2) != 0) set_word(entry, 160, 41);
    else if ((info[0] & 4) != 0) set_word(entry, 160, 23984);
    else if ((info[0] & 1) != 0) set_word(entry, 160, compressSize);
}
