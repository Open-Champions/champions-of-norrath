// Observed netDoNewLaunch at 0x0012d790, 8064 bytes, returning an int. It is
// one step of the network-launch lobby: it handles the on-screen confirmation
// prompts, receives one datagram, drives a per-peer state machine, and, when
// every peer is ready or the host forces the launch, publishes the peers and
// starts the game. Names below are the image's own symbols; the offsets are
// bytes into launchPeers (four 252-byte entries) and netGameLaunchInfo (called
// info: byte 0 flags, byte 4 peer count, byte 5 host index, byte 6 local index).
// Entry fields used: +4..7 bytes, +16/+32 address blocks of 16 bytes, +20..23,
// +36..39 bytes, +52 and +56 limit words, +60 bytes still to receive, +64
// index, +72 and +76 words, +80 counter, +88 float time, +92 state (0..8), +93
// flag bits (1 host, 2 packed, 4 temp storage, 8 ready), +96 data pointer, +100
// flag, +104 a 136-byte block copied into most packets, +240 halfword sequence,
// +244 and +248 words. Packets are sent with sceInetSendTo(cd, lnet_buf,
// udpLaunchConnection, data, 512, &zero, address, limit, 100); a header of
// three words (the local entry's index, the type, a parameter) is followed by
// an optional 136-byte block taken from the local entry at +104. Types sent:
// 0 and 1 (join request/answer), 2 (size), 3 (size answer), 5 (data ack), 6
// and 7 (ready query/answer), 8 (launch with mask).
// Sequence:
//  1. With host index equal to the local index, "anyReady" is whether some
//     other entry has state 8. A pending cancel prompt (netNewLaunchMaybeQuit)
//     runs hudRunYesNo: answer 1 clears it, answer 2 gives result -1. A pending
//     launch prompt (netNewLaunchMaybeLaunch): answer 1 clears it and returns
//     0; answer 2 builds hostLaunchMask from state-8 entries, sets
//     hostForcedLaunch, shows the status and returns 0.
//  2. netLaunchExpirationCount above 2700, or hostStartedWithoutMe, gives result
//     0. Otherwise, unless hostForcedLaunch is set, currentTime advances by the
//     float 0x3c888888, one datagram is received (1300 bytes), and a datagram
//     of at least 16 bytes from an index other than the local one updates the
//     sender's address data and is handled by its type 0..8 as described in the
//     code, with the 16-bit checksum and 12-byte header for type 4 data.
//  3. The expiration count resets when some entry is not in state 8 (state
//     nonzero) and grows when an entry is still in state 0.
//  4. Each entry other than the local one resends according to its state
//     (0 join, 1 size, 2 join answer, 3 data chunk through peerSendDataMsg, 4
//     wait, 7 to 8, 8 ready query) with time gates of 0.5 or 1.0 second.
//  5. Result is 0 unless every other entry is in state 8 with flag 8; then the
//     launch timer counts to 120 and the result becomes nonzero. A forced host
//     sends type 8 to each peer.
//  6. A cancel (negative result) clears the four player records and frees the
//     buffer. A positive result copies the retained entries down, rebuilds the
//     partner table, the per-player button tables and the broadcast connection.
// Observed: the type 4 checksum loop adds the first payload byte on every step
// (the image reloads byte 0, not byte k). Inferred: the wire layout, from the
// stores and loads above.
// Unknown: field meanings beyond their use here, how many bytes of each 512-byte
// datagram are meaningful, and the exact payload semantics.
typedef int __attribute__((may_alias)) Int;
typedef short __attribute__((may_alias)) Short;
typedef float __attribute__((may_alias)) Single;

extern "C" {
extern unsigned char netGameLaunchInfo[];
extern unsigned char launchPeers[];
extern unsigned char playerSpecStorage[];
extern unsigned char playerSelectInfo[];
extern unsigned char cd[];
extern unsigned char g_net_buf[];
extern unsigned char pad[];
extern unsigned char player[];
extern unsigned char partners[];
extern void* lnet_buf;
extern int udpLaunchConnection;
extern int broadcastConnection;
extern signed char netNewLaunchMaybeQuit;
extern signed char netNewLaunchMaybeLaunch;
extern signed char hostForcedLaunch;
extern signed char hostLaunchMask;
extern signed char hostStartedWithoutMe;
extern int netLaunchTimer;
extern int netLaunchExpirationCount;
extern float currentTime;
extern float lastPingTime;
extern int compressSize;
extern int gameDifficulty;
extern int nmPartners;
extern signed char MC_ProtectRamSlot;
extern unsigned attackButton[4];
extern unsigned spellButton1[4];
extern unsigned spellButton2[4];
extern unsigned actionButton[4];
extern unsigned blockButton[4];
extern unsigned healthPotionButton[4];
extern unsigned energyPotionButton[4];
extern unsigned automapButton[4];
extern unsigned weaponToggleButton[4];

void* memset(void* destination, int value, unsigned long length);
void* memcpy(void* destination, const void* source, unsigned long length);
int sceInetRecvFrom(void* object, void* buffer, int connection, void* data, int length, int* flags,
                    void* address, void* length_out, int options);
int sceInetSendTo(void* object, void* buffer, int connection, void* data, int length, int* flags,
                  void* address, int first_limit, int second_limit);
int sceInetClose(void* object, void* buffer, int connection, int timeout);
int sceInetCreate(void* object, void* buffer, void* parameters);
int sceInetOpen(void* object, void* buffer, int handle, int options);
unsigned short* textGetString__FPc(char* key);
int hudRunYesNo__FPUsiiii(unsigned short* text, int first, int second, int third, int fourth);
void showNetPlayerStatus__Fv();
int peerSendDataMsg__FPciiP14sceInetAddressi(char* data, int size, int sequence, void* address, int limit);
void* MC_GetTempPlayerStorage__Fi(int index);
void* MC_GetRamSlot__FPi(int* size);
void* MC_GetRamPlayerPtr__Fi(int index);
void MC_BuildPackedPlayer__FP11playerEntryi(void* entry, int index);
void MC_ConvertTempPlayerToReal__Fi(int index);
void MC_ByteCopy__FPvT0i(void* destination, void* source, int length);
void MC_NetUnPack__Fv();
void MC_SetRamNmPlayers__Fi(int count);
void MC_ReplicateFirstPlayersImportantThings__Fv();
void dumpMem__FPvi(void* memory, int length);
void memFree__FPv(void* memory);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_launch_host_base;
#endif

namespace {
const int kPeer = 252;

inline Int& word(unsigned char* base, int offset) { return *reinterpret_cast<Int*>(base + offset); }
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<Single*>(base + offset); }
inline unsigned char* peer_at(int index) { return launchPeers + index * kPeer; }
inline int local_index() { return static_cast<signed char>(netGameLaunchInfo[6]); }

inline unsigned word_from_pointer(const void* pointer) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - norrath_launch_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
inline unsigned char* pointer_from_word(unsigned value) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_launch_host_base + value;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}

struct Block {
    unsigned char bytes[136];
};

// Sends one launch datagram: three header words and, when with_block is set,
// the 136-byte block of the local entry at +104.
inline __attribute__((always_inline)) void send_launch(unsigned char* peer, int address_offset, int limit_offset,
                                                      int type, int parameter, bool with_block) {
    alignas(16) unsigned char packet[512];
    unsigned char* local = peer_at(local_index());
    int flags = 0;
    int limit = word(peer, limit_offset);
    word(packet, 0) = word(local, 64);
    word(packet, 4) = type;
    word(packet, 8) = parameter;
    if (with_block) *reinterpret_cast<Block*>(packet + 12) = *reinterpret_cast<Block*>(local + 104);
    sceInetSendTo(cd, lnet_buf, udpLaunchConnection, packet, 512, &flags, peer + address_offset, limit, 100);
}
}

extern "C" int netDoNewLaunch__Fv() {
    int result = 1;
    int ready_any = 0;
    unsigned char* info = netGameLaunchInfo;
    int flags_word = 0;
    alignas(16) unsigned char packet[1300];
    alignas(16) unsigned char from[16];
    int from_length = 0;
    int received = 0;
    int s4 = 0;

    if (info[5] == info[6]) {
        int count = static_cast<signed char>(info[4]);
        int ready = 0;
        if (count > 1) {
            for (int j = 1; j < count; ++j)
                if (static_cast<signed char>(peer_at(j)[92]) == 8) ++ready;
        }
        ready_any = ready != 0;
    }

    if (netNewLaunchMaybeQuit != 0) {
        unsigned short* text = textGetString__FPc(const_cast<char*>("netLaunchCancelConfirm"));
        int answer = hudRunYesNo__FPUsiiii(text, 0, -1, 0, 0);
        if (answer == 1) {
            netNewLaunchMaybeQuit = 0;
        } else if (answer == 2) {
            result = -1;
            goto conclude;
        }
    }
    if (netNewLaunchMaybeLaunch != 0) {
        unsigned short* text = textGetString__FPc(const_cast<char*>("netLaunchHostWarn"));
        int answer = hudRunYesNo__FPUsiiii(text, 0, -1, 0, 0);
        if (answer == 1) {
            netNewLaunchMaybeLaunch = 0;
            return 0;
        }
        if (answer == 2) {
            int count = static_cast<signed char>(info[4]);
            hostLaunchMask = 1;
            netNewLaunchMaybeLaunch = 0;
            hostForcedLaunch = 1;
            if (count > 1) {
                int mask = 1;
                for (int j = 1; j < count; ++j)
                    if (static_cast<signed char>(peer_at(j)[92]) == 8) mask |= 1 << j;
                hostLaunchMask = static_cast<signed char>(mask);
            }
            showNetPlayerStatus__Fv();
            return 0;
        }
    }
    if (!(netLaunchExpirationCount < 2701)) {
        result = 0;
        goto timer_stage;
    }
    if (hostStartedWithoutMe != 0) {
        result = 0;
        goto timer_stage;
    }
    if (hostForcedLaunch != 0) goto decide;

    // receive one datagram
    {
        float step;
        const unsigned kStepBits = 0x3c888888u;
        memcpy(&step, &kStepBits, 4);
        currentTime = currentTime + step;
    }
    received = sceInetRecvFrom(cd, lnet_buf, udpLaunchConnection, packet, 1300, &flags_word, from,
                               &from_length, 0);
    if (received >= 16) {
        int sender = word(packet, 0);
        bool found = false;
        if (static_cast<signed char>(info[4]) > 0) {
            if (sender != local_index() && sender == word(peer_at(0), 64)) {
                found = true;
            } else {
                for (;;) {
                    ++s4;
                    if (!(s4 < static_cast<signed char>(info[4]))) break;
                    if (sender == local_index()) continue;
                    if (sender == word(peer_at(s4), 64)) {
                        found = true;
                        break;
                    }
                }
            }
        }
        if (found && s4 != static_cast<signed char>(info[4])) {
            unsigned char* peer = peer_at(s4);
            int type = word(packet, 4);
            int parameter = word(packet, 8);
            if (static_cast<signed char>(peer[100]) == 0) {
                if (from_length == word(peer, 56) && word(from, 4) == word(peer, 20)) {
                    // same sender as recorded: nothing to update
                } else if (from[4] == netGameLaunchInfo[7 + 259 + 4 * local_index()] &&
                           from[5] == netGameLaunchInfo[7 + 258 + 4 * local_index()] &&
                           from[6] == netGameLaunchInfo[7 + 257 + 4 * local_index()] &&
                           from[7] == netGameLaunchInfo[7 + 256 + 4 * local_index()]) {
                    peer[20] = netGameLaunchInfo[7 + 275 + 4 * s4];
                    peer[21] = netGameLaunchInfo[7 + 274 + 4 * s4];
                    peer[22] = netGameLaunchInfo[7 + 273 + 4 * s4];
                    peer[23] = netGameLaunchInfo[7 + 272 + 4 * s4];
                    word(peer, 56) = 3000;
                } else {
                    memcpy(peer + 16, from, 16);
                    word(peer, 56) = from_length;
                }
            }
            word(peer, 80) = 0;
            if (type >= 0 && type < 9) {
                switch (type) {
                case 0:
                case 1:
                    if (static_cast<signed char>(peer[92]) == 0) {
                        *reinterpret_cast<Block*>(peer + 104) = *reinterpret_cast<Block*>(packet + 12);
                        unsigned flags = peer[93];
                        peer[92] = (flags & 1) ? 1 : 5;
                        peer[100] = 1;
                        *reinterpret_cast<Short*>(peer + 240) = 0;
                        word(peer, 244) = 0;
                        real(peer, 88) = currentTime;
                        peer[93] = static_cast<unsigned char>(flags & 0xf7);
                        word(peer, 248) = word(peer, 160) + word(peer_at(local_index()), 160);
                    }
                    if (type == 0) send_launch(peer, 16, 56, 1, parameter, true);
                    break;
                case 2: {
                    int state = static_cast<signed char>(peer[92]);
                    if (state == 6) {
                        send_launch(peer, 16, 56, 3, parameter, false);
                    } else if (state == 5) {
                        unsigned flags = peer[93] & 0xf9;
                        word(peer, 60) = parameter;
                        peer[93] = static_cast<unsigned char>(flags);
                        if (parameter == 41) {
                            peer[93] = static_cast<unsigned char>(flags | 2);
                            word(peer, 96) = word_from_pointer(playerSpecStorage + 41 * s4);
                        } else if (parameter == 23984) {
                            word(peer, 96) = word_from_pointer(MC_GetTempPlayerStorage__Fi(s4));
                            peer[93] = static_cast<unsigned char>(peer[93] | 4);
                        } else {
                            word(peer, 96) = word_from_pointer(MC_GetRamSlot__FPi(0));
                        }
                        send_launch(peer, 16, 56, 3, parameter, false);
                        real(peer, 88) = currentTime;
                        peer[92] = 6;
                    }
                    break;
                }
                case 3:
                    if (static_cast<signed char>(peer[92]) == 2) peer[92] = 3;
                    break;
                case 4: {
                    int state = static_cast<signed char>(peer[92]);
                    int length = received - 12;
                    unsigned flags = peer[93];
                    if (state == 6 || state == 8 || (flags & 1) == 0) {
                        bool ack_only = static_cast<int>(*reinterpret_cast<Short*>(peer + 240)) !=
                                            static_cast<int>(parameter & 0xffff) ||
                                        (state != 6 && ((flags & 1) == 0 || state == 8));
                        if (ack_only) {
                            send_launch(peer, 16, 56, 5, parameter, false);
                        } else {
                            int sum = 535;
                            const signed char* data = reinterpret_cast<const signed char*>(packet + 12);
                            for (int k = 0; k < length; ++k) {
                                if (k & 1) sum += (data[k] * 3) ^ 0x30e15f;
                                else sum -= (data[k] & 0x148f37) * (k & 7);
                                sum += data[0];
                            }
                            if (static_cast<int>(sum & 0xffff) ==
                                static_cast<int>(static_cast<unsigned>(parameter) >> 16)) {
                                memcpy(pointer_from_word(word(peer, 96)), packet + 12, length);
                                int remaining = word(peer, 60) - length;
                                word(peer, 96) = word(peer, 96) + length;
                                word(peer, 244) = word(peer, 244) + length;
                                *reinterpret_cast<Short*>(peer + 240) =
                                    static_cast<short>(*reinterpret_cast<Short*>(peer + 240) + 1);
                                word(peer, 60) = remaining;
                                if (remaining == 0) {
                                    peer[92] = (peer[93] & 1) ? 7 : 1;
                                    real(peer, 88) = currentTime;
                                    *reinterpret_cast<Short*>(peer + 240) = 0;
                                }
                                send_launch(peer, 16, 56, 5, parameter, false);
                            }
                        }
                        real(peer, 88) = currentTime;
                    }
                    break;
                }
                case 5:
                    if (static_cast<signed char>(peer[92]) == 4) {
                        peer[92] = 3;
                        if (static_cast<int>(*reinterpret_cast<Short*>(peer + 240)) ==
                            static_cast<int>(parameter & 0xffff)) {
                            unsigned short sequence = *reinterpret_cast<unsigned short*>(peer + 240);
                            if (parameter == word(peer, 72)) {
                                int chunk = word(peer, 76);
                                int remaining = word(peer, 60) - chunk;
                                word(peer, 96) = word(peer, 96) + chunk;
                                word(peer, 244) = word(peer, 244) + chunk;
                                *reinterpret_cast<unsigned short*>(peer + 240) = static_cast<unsigned short>(sequence + 1);
                                word(peer, 60) = remaining;
                                if (remaining == 0) {
                                    peer[92] = (peer[93] & 1) ? 5 : 7;
                                    real(peer, 88) = currentTime;
                                    *reinterpret_cast<Short*>(peer + 240) = 0;
                                }
                            }
                        }
                    }
                    real(peer, 88) = currentTime;
                    break;
                case 6: {
                    int ready = 0;
                    int count = static_cast<signed char>(info[4]);
                    for (int j = 0; j < count; ++j) {
                        if (j == local_index()) continue;
                        if (static_cast<signed char>(peer_at(j)[92]) == 8) ++ready;
                    }
                    send_launch(peer, 16, 56, 7, ready, true);
                    break;
                }
                case 7:
                    if (parameter == static_cast<signed char>(info[4]) - 1) peer[93] = static_cast<unsigned char>(peer[93] | 8);
                    break;
                default:
                    if (((parameter >> local_index()) & 1) != 0) {
                        hostForcedLaunch = 1;
                        hostLaunchMask = static_cast<signed char>(parameter & 0xff);
                    } else {
                        result = 0;
                        hostStartedWithoutMe = 1;
                        goto timer_stage;
                    }
                    break;
                }
            }
        }
    }

    // expiration bookkeeping
    {
        int any_busy = 0;
        int any_idle = 0;
        int count = static_cast<signed char>(info[4]);
        int local = local_index();
        for (int j = 0; j < count; ++j) {
            if (j == local) continue;
            int state = static_cast<signed char>(peer_at(j)[92]);
            if (state != 0) {
                if (state != 8) any_busy = 1;
            } else {
                any_idle = 1;
            }
        }
        if (any_busy != 0) netLaunchExpirationCount = 0;
        else if (any_idle != 0) netLaunchExpirationCount = netLaunchExpirationCount + 1;
    }

    // per-entry resends
    s4 = 0;
    if (static_cast<signed char>(info[4]) > 0) {
        do {
            if (s4 != local_index()) {
                unsigned char* peer = peer_at(s4);
                int state = static_cast<signed char>(peer[92]);
                switch (state >= 0 && state < 9 ? state : 99) {
                case 0:
                    if (0.5f < currentTime - lastPingTime) {
                        word(peer, 80) = word(peer, 80) + 1;
                        send_launch(peer, 16, 56, 0, 0, true);
                        unsigned char* local = peer_at(local_index());
                        if (static_cast<signed char>(peer[4]) == static_cast<signed char>(local[4]) &&
                            static_cast<signed char>(peer[5]) == static_cast<signed char>(local[5]) &&
                            static_cast<signed char>(peer[6]) == static_cast<signed char>(local[6]) &&
                            static_cast<signed char>(peer[7]) == static_cast<signed char>(local[7]))
                            send_launch(peer, 32, 52, 0, 1, true);
                    }
                    break;
                case 1: {
                    unsigned flags = netGameLaunchInfo[0];
                    if (flags & 2) {
                        word(peer, 60) = 41;
                        word(peer, 96) = word_from_pointer(playerSelectInfo);
                    } else if (flags & 4) {
                        word(peer, 96) = word_from_pointer(MC_GetTempPlayerStorage__Fi(static_cast<signed char>(info[6])));
                        word(peer, 60) = 23984;
                    } else {
                        word(peer, 60) = compressSize;
                        word(peer, 96) = word_from_pointer(MC_GetRamSlot__FPi(0));
                    }
                    send_launch(peer, 16, 56, 2, word(peer, 60), false);
                    peer[92] = 2;
                    real(peer, 88) = currentTime;
                    word(peer_at(local_index()), 224 + 4 * s4) = 0;
                    break;
                }
                case 2:
                    if (1.0f < currentTime - real(peer, 88)) {
                        send_launch(peer, 16, 56, 1, 0, true);
                        peer[92] = 1;
                        real(peer, 88) = currentTime;
                    }
                    break;
                case 3: {
                    unsigned room = static_cast<unsigned>(word(peer, 220)) - 12u;
                    float room_value = static_cast<float>(room);
                    float left_value = static_cast<float>(word(peer, 60));
                    float smaller = room_value < left_value ? room_value : left_value;
                    int chunk = static_cast<int>(smaller);
                    int sequence = *reinterpret_cast<Short*>(peer + 240);
                    word(peer, 76) = chunk;
                    int returned = peerSendDataMsg__FPciiP14sceInetAddressi(
                        reinterpret_cast<char*>(pointer_from_word(word(peer, 96))), chunk, sequence, peer + 16,
                        word(peer, 56));
                    peer[92] = 4;
                    word(peer, 72) = returned;
                    real(peer, 88) = currentTime;
                    break;
                }
                case 4:
                    if (1.0f < currentTime - real(peer, 88)) {
                        word(peer_at(local_index()), 224 + 4 * s4) = word(peer_at(local_index()), 224 + 4 * s4) + 1;
                        peer[92] = 3;
                    }
                    break;
                case 7:
                    peer[92] = 8;
                    break;
                case 8:
                    if (0.5f < currentTime - lastPingTime) {
                        int counter = word(peer, 80);
                        if (!(counter < 21)) {
                            peer[92] = 0;
                        } else {
                            word(peer, 80) = counter + 1;
                            send_launch(peer, 16, 56, 6, 8, true);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            ++s4;
        } while (s4 < static_cast<signed char>(info[4]));
    }
    if (0.5f < currentTime - lastPingTime) lastPingTime = currentTime;
    {
        int count = static_cast<signed char>(info[4]);
        int local = local_index();
        for (int j = 0; j < count; ++j) {
            if (j == local) continue;
            unsigned char* peer = peer_at(j);
            if (static_cast<signed char>(peer[92]) != 8) result = 0;
            else if ((peer[93] & 8) == 0) result = 0;
        }
    }

decide:
    {
        unsigned forced = static_cast<unsigned char>(hostForcedLaunch);
        if (result != 1 && forced == 0) goto pad_stage;
        int before = netLaunchTimer;
        netLaunchTimer = before + 1;
        result = before >= 120;
        if (forced == 0) goto timer_stage;
        if (info[5] != info[6]) goto pad_stage;
        int count = static_cast<signed char>(info[4]);
        if (!(1 < count)) goto pad_stage;
        for (int j = 1; j < static_cast<signed char>(info[4]); ++j) {
            alignas(16) unsigned char go[512];
            unsigned char* local = peer_at(local_index());
            unsigned char* peer = peer_at(j);
            int zero = 0;
            int limit = word(peer, 56);
            word(go, 0) = word(local, 64);
            word(go, 4) = 8;
            word(go, 8) = hostLaunchMask;
            sceInetSendTo(cd, lnet_buf, udpLaunchConnection, go, 512, &zero, peer + 16, limit, 100);
        }
    }
timer_stage:
pad_stage:
    if (netLaunchTimer == 0 && hostForcedLaunch == 0) {
        int buttons = *reinterpret_cast<Int*>(pad + 12);
        if ((buttons & 0x20) != 0) netNewLaunchMaybeQuit = 1;
        if (ready_any != 0 && (buttons & 0x40) != 0) netNewLaunchMaybeLaunch = 1;
    }
    showNetPlayerStatus__Fv();
conclude:
    if (result == 0) return result;
    sceInetClose(cd, lnet_buf, udpLaunchConnection, 10000);
    if (result <= 0) {
        for (unsigned char* record = player; record < player + 4 * 23984; record += 23984) memset(record, 0, 23984);
        memFree__FPv(lnet_buf);
        return result;
    }
    // launch: publish the entries
    {
        int count = static_cast<signed char>(info[4]);
        for (int i = 0; i < count; ++i) {
            unsigned char* peer = peer_at(i);
            info[263 + 4 * i] = peer[23];
            info[264 + 4 * i] = peer[22];
            info[265 + 4 * i] = peer[21];
            info[266 + 4 * i] = peer[20];
            word(info, 296 + 4 * i) = word(peer, 56);
        }
        int kept = 1;
        for (int source = 1; source < static_cast<signed char>(info[4]); ++source) {
            if (((hostLaunchMask >> source) & 1) == 0) continue;
            memcpy(playerSpecStorage + 41 * kept, playerSpecStorage + 41 * source, 41);
            memcpy(peer_at(kept), peer_at(source), kPeer);
            word(peer_at(kept), 64) = kept;
            for (int k = 0; k < 4; ++k) info[263 + 4 * kept + k] = info[263 + 4 * source + k];
            word(info, 296 + 4 * kept) = word(info, 296 + 4 * source);
            if (local_index() == source) info[6] = static_cast<unsigned char>(kept);
            void* destination = MC_GetTempPlayerStorage__Fi(kept);
            ++kept;
            void* origin = MC_GetTempPlayerStorage__Fi(source);
            MC_ByteCopy__FPvT0i(destination, origin, 23984);
        }
        info[4] = static_cast<unsigned char>(kept);
        bool from_packed = false;
        unsigned flags = info[0];
        if (flags & 1) {
            if (flags & 6) from_packed = true;
        } else {
            from_packed = (peer_at(static_cast<signed char>(info[5]))[93] & 6) != 0;
        }
        if (from_packed) {
            int count_now = static_cast<signed char>(info[4]);
            for (int j = 0; j < count_now; ++j) {
                if (peer_at(j)[93] & 2) MC_BuildPackedPlayer__FP11playerEntryi(playerSpecStorage, j);
                MC_ConvertTempPlayerToReal__Fi(j);
            }
            result = 2;
            gameDifficulty = word(peer_at(0), 156);
        } else {
            MC_NetUnPack__Fv();
            for (int j = 1; j < static_cast<signed char>(info[4]); ++j) {
                if (peer_at(j)[93] & 2) MC_BuildPackedPlayer__FP11playerEntryi(playerSpecStorage, j);
                unsigned char* ram = static_cast<unsigned char*>(MC_GetRamPlayerPtr__Fi(j));
                unsigned char* temp = static_cast<unsigned char*>(MC_GetTempPlayerStorage__Fi(j));
                memcpy(ram, temp, 23984);
            }
            MC_SetRamNmPlayers__Fi(static_cast<signed char>(info[4]));
            MC_ReplicateFirstPlayersImportantThings__Fv();
        }
        MC_ProtectRamSlot = 0;
        dumpMem__FPvi(MC_GetRamPlayerPtr__Fi(0), 256);
        count = static_cast<signed char>(info[4]);
        for (int i = 0; i < count; ++i) {
            unsigned char* entry = partners + i * 17632;
            entry[3] = info[263 + 4 * i];
            entry[2] = info[264 + 4 * i];
            entry[1] = info[265 + 4 * i];
            entry[0] = info[266 + 4 * i];
            word(entry, 4) = word(info, 296 + 4 * i);
            entry[17620] = i == static_cast<signed char>(info[6]) ? 1 : 0;
        }
        nmPartners = static_cast<signed char>(info[4]);
        for (int i = 3; i >= 0; --i) word(partners + i * 17632, 17600) = 1;
        count = static_cast<signed char>(info[4]);
        for (int i = 0; i < count; ++i) {
            unsigned char* peer = peer_at(i);
            attackButton[i] = word(peer, 164);
            spellButton1[i] = word(peer, 168);
            spellButton2[i] = word(peer, 172);
            actionButton[i] = word(peer, 176);
            blockButton[i] = word(peer, 180);
            healthPotionButton[i] = word(peer, 184);
            energyPotionButton[i] = word(peer, 188);
            automapButton[i] = word(peer, 192);
            weaponToggleButton[i] = word(peer, 196);
        }
        int parameters[16];
        memset(parameters, 0, 64);
        parameters[1] = 3000;
        parameters[0] = 0;
        int handle = sceInetCreate(cd, g_net_buf, parameters);
        broadcastConnection = handle;
        sceInetOpen(cd, g_net_buf, handle, 1000);
    }
    memFree__FPv(lnet_buf);
    return result;
}
