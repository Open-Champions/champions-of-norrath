#include "../w9-animation-object-c-common/mem.hpp"
// Observed process__16VIRealmInterface at 0x01104ae8, 7652 bytes (a 3456-byte frame). With (this); offsets are in this, "state" is the word at
// +3468 (an inference: it is compared against small constants and changed by the packet handlers), page = the word at (the word at this+12) + 52,
// dialog = the same +56, sock = this+3396 (the UDP socket object, whose first word the code sets to 3000 before Open). app is the result of
// GetAppPtr__5VIWnd(the word at this+12); its fields at +0x17a34 (a 24-byte text), +0x17a74 (a 24-byte text) and +0x17ab4 (an int) are used
// as a player's name, class and level (inferred from the "Player1Name"/"Player1Class"/"Player1Level" page keys they are shown under). `frame`
// is the file-local counter frame.3503 (0x011b0188), incremented once at the end. A 20-byte address is a VIInetAddress; every by-value use passes a
// pointer to a copy. The string-table text of id n is GetString__13VIStringTableii(the word _5VIWnd$mUI, n, the word at that pointer + 48).
//  1. now = GetTime__7VITimer() (64-bit). When now minus the 64-bit word at +9728 is at least 30001, ForceDisconnect__20VIWndGenericRenderer(
//     the word at this+12).
//  2. When strstr(GetName__9VIGUIPage(page), "GameServ") is nonzero, the roster is shown: sprintf("L%i", the int at app+0x17ab4); page values
//     Player1Name/Player1Class/Player1Level; then for players 2..4 (index i = 0..2; row base this+1144*i, fields at +6180 name, +6204 class,
//     +6228 level int): when i+1 is below the int at +3476 and the row's first name byte is nonzero those three values are set from the row
//     (level through "L%i"); when i+1 is below the count but the name is empty all three are "?"; otherwise ".........", "......" and "...".
//  3. When now minus the 64-bit global g_lastSessionTouch is at least 10001: g_lastSessionTouch = GetTime(); and when the word `connected` is
//     nonzero: printf("Touching session\n"), touchSession__Q212RealmService15RealmGatewayAPIPCUsPv(this, this+16, 0), and when the page name
//     contains "FindGame": getRealmStats__...Pv(this, 0), a MatchGameAttrs is constructed, matchGame__...(this, this+16, 2, 0, 0, -1, &attrs, 1,
//     100, 1024, 0) is called and the attrs destroyed with flag 2.
//  4. Then by state (the word at +3468):
//     1: open the socket if IsConnected is false (first word = 3000, Open); when the word at +2344 is zero, GetInetAddress(sock, this+2328, this+1300,
//        5000). When the socket is connected, +2344 is nonzero and frame % 30 == 0: when the word at +9688 is zero, GetLocalAddress(sock, this+9672)
//        and, if +9688 is then nonzero, memset(this+9692, 0, 24), GetStringAddress(sock, this+9672, this+9692), a printf, UpdateNumPlayers(this, the
//        count at +3476); a packet is built with packDiscoveryPacket__...(this, buffer, this+16) and sent with SendTo (the 6-argument form) to a copy
//        of this+2328 with port word +2324.
//     5: when frame % 30 == 0: printf("Doing client discovery\n") and the same socket/address/packet steps (without UpdateNumPlayers).
//     11 or 12: socket open (with a printf if closed), the server address from +3376 (text +2348) fetched when the word at +3392 is zero; when
//        connected, +3392 nonzero and frame % 30 == 0, a 108-byte packet is built (state, the text at +9692, g_localIp, g_localPort, then a 48-byte
//        name/class block and the level) and sent with the 9-argument SendTo (copy of +3376 with port +3372; copies of the 20 bytes at +0x947c and
//        +0x8b08; 3000; the packet; 108). Then, when now minus the 64-bit word at +9720 is at least 10001: GoBack, Show(string 1840), SetText(string
//        1858), state = 0, Reset.
//     2, 3 or 4: when frame % 30 == 0 and the count at +3476 minus one (others) is positive, each client row i (row base this+1144*i, fields:
//        text +5088, address +6116, port word +6112, flag word +6132, status word +6136, 64-bit time +6144, text +6152, word +6176, 52-byte
//        player block +6180) is visited: the address is resolved when +6132 is zero; when connected and +6132 is nonzero a ping (status
//        != 9) or start-game (status == 9) packet is built and sent with the SendTo form that takes two addresses and two texts; a row whose time word is
//        20001 or more old is removed by moving the following rows down 1144 bytes each, then UpdateNumPlayers(this, count - 1); others is
//        recomputed each pass and i always advances by one. Afterwards, state 3 with now minus the 64-bit word at +9664 at least 5001 copies the
//        roster into g_mode/g_numClients/g_ips/g_ports/g_localClientIps/g_localClientPorts, Finish, state = 0, Reset; state 4 (after a fresh
//        GetTime) with a count of 1, or 5001 elapsed, sets state = 0, Reset, GoBack and cancelGame(this, this+16, 0).
//  5. When the socket is connected, a datagram is read with RecvFrom (1024-byte buffer; from-address, port) into a buffer whose first word is the packet type.
//     app2 = GetAppPtr again. When app2's first word is 2 and state is 0, a 4-byte reply with the value 13 is sent back and the datagram is dropped.
//     Otherwise, with a positive length: printf of the port and the length, then by type:
//       13 (app2 word 1): every client row whose address (20 bytes) and port match the sender is removed (rows moved down, UpdateNumPlayers).
//       11 or 12 (app2 word 1): "ping from a client": rows are matched on (16 address bytes and port) or (text and the discovered port); the
//          match sets status 7 to 8, the time word, the text +6152, word +6176 = 3000, and (when the length is at least 51) the 52-byte block from the
//          packet; an unmatched ping updates the port of a row with the same 16 address bytes.
//       7 (app2 word 2): the server address (+3376, port +3372) is set from the sender, or from a same-NAT text; state = 12; ServerStatus text 1842.
//       8 (app2 word 2): state = 12; roster page values from the packet's player table; ServerStatus text 1843; NumPlayers text.
//       14 (app2 word 2, page name contains "GameClnt"): GoBack twice, state = 0, Reset, Show(string 1869), SetText(string 1871).
//       9 (state 12, app2 word 2): the packet is copied; when it came from the server address (+3376) or the +0x8b08 address, g_numClients,
//          g_ips, g_ports, g_localClientIps, g_localClientPorts, g_myIndex and g_mode = 2 are filled from it, Finish, state = 0, Reset.
//       10 (app2 word 2, state nonzero): GoBack twice, Show(string 1840), SetText(string 1841), state = 0, Reset.
//  6. frame = frame + 1; process__Q212RealmService15RealmGatewayAPI(this).
// Inferred: this is the per-frame pump of the online lobby session: it keeps the realm session alive, runs a small UDP handshake and
// roster exchange between a game host and its clients, and drives the page text. Packet type numbers 7..14 are as observed; their names are unknown.
// Unknown: the true meaning of each state value and packet type; the sizes of the packet records beyond the offsets used; the unused
// frame space; the reason the packet text "format" arguments come from address text; MatchGameAttrs' size (8 bytes are enough as seen here).
typedef long long __attribute__((may_alias)) i64_t;
template <unsigned N>
struct Bytes {
    unsigned char b[N];
} __attribute__((may_alias));
extern "C" {
extern int frame_counter __asm__("frame.3503");
extern int connected;
extern long long g_lastSessionTouch;
extern int g_mode, g_myIndex, g_numClients, g_localPort;
extern char g_ips[], g_localClientIps[], g_localIp[];
extern int g_ports[], g_localClientPorts[];
extern unsigned vi_wnd_mui __asm__("_5VIWnd$mUI");

long long GetTime__7VITimer();
void* GetAppPtr__5VIWnd(void* renderer);
void ForceDisconnect__20VIWndGenericRenderer(void* renderer);
const char* GetName__9VIGUIPage(void* page);
char* strstr_call(const char* hay, const char* needle) __asm__("strstr");
int sprintf(char* out, const char* format, ...);
int printf(const char* format, ...);
void* memset(void* dest, int value, unsigned long size);
int memcmp(const void* a, const void* b, unsigned long size);
int strcmp(const char* a, const char* b);
char* strcpy(char* dest, const char* source);
void SetStringValue__9VIGUIPagePCcT1b(void* page, const char* name, const char* value, int flag);
void SetStringValue__9VIGUIPagePCcPCUsb(void* page, const char* name, const unsigned short* value, int flag);
void touchSession__Q212RealmService15RealmGatewayAPIPCUsPv(void* api, const unsigned short* session, void* data);
void getRealmStats__Q212RealmService15RealmGatewayAPIPv(void* api, void* data);
void __Q212RealmService14MatchGameAttrs(void* attrs);
void _$_Q212RealmService14MatchGameAttrs(void* attrs, int flags);
void matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv(
    void* api, const unsigned short* session, short a, unsigned b, unsigned c, int d, const void* attrs, bool e, unsigned f, unsigned g, void* h);
void cancelGame__Q212RealmService15RealmGatewayAPIPCUsPv(void* api, const unsigned short* session, void* data);
int IsConnected__11VIUDPSocket(void* socket);
void Open__11VIUDPSocket(void* socket);
void GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(void* socket, void* address, const char* host, int port);
void GetLocalAddress__11VIUDPSocketP13VIInetAddress(void* socket, void* address);
void GetStringAddress__11VIUDPSocketP13VIInetAddressPc(void* socket, void* address, char* out);
void UpdateNumPlayers__16VIRealmInterfacei(void* self, int count);
int packDiscoveryPacket__Q212RealmService15RealmGatewayAPIPUcPCUs(void* api, unsigned char* out, const unsigned short* name);
int SendTo__11VIUDPSocketG13VIInetAddressiPCciPii(void* socket, const void* address, int port, const char* data, int length, int* a, int b);
int SendTo__11VIUDPSocketG13VIInetAddressiT1T1iPCciPii(void* socket, const void* address, int port, const void* second, const void* third, int a,
                                                       const char* data, int length, int* b, int c);
int SendTo__11VIUDPSocketG13VIInetAddressiT1PCciT4iPii(void* socket, const void* address, int port, const void* second, const char* text, int a,
                                                       const char* data, int length, int* b, int c);
int RecvFrom__11VIUDPSocketP13VIInetAddressPiPciT2i(void* socket, void* from, int* port, char* buffer, int length, int* a, int b);
void GoBack__20VIWndGenericRenderer(void* renderer);
void Finish__20VIWndGenericRenderer(void* renderer);
void Reset__16VIRealmInterface(void* self);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third);
void Show__11VIGUIDialogPCUsPCc(void* dialog, const unsigned short* text, const char* extra);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, int flag);
void process__Q212RealmService15RealmGatewayAPI(void* api);

}
static inline unsigned word_at(const void* base, int offset) {
    return *reinterpret_cast<const unsigned*>(static_cast<const unsigned char*>(base) + offset);
}
static inline void set_word(void* base, int offset, unsigned value) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(base) + offset) = value;
}
static inline long long time_at(const void* base, int offset) {
    return *reinterpret_cast<const i64_t*>(static_cast<const unsigned char*>(base) + offset);
}
template <unsigned N>
static inline void copy_bytes(void* dest, const void* source) {
    *static_cast<Bytes<N>*>(dest) = *static_cast<const Bytes<N>*>(source);
}
static inline void* renderer_of(void* self) {
    return norrath_aoc::load_pointer(static_cast<unsigned char*>(self) + 12);
}
static inline void* page_of(void* self) {
    return norrath_aoc::load_pointer(static_cast<unsigned char*>(renderer_of(self)) + 52);
}
static inline void* dialog_of(void* self) {
    return norrath_aoc::load_pointer(static_cast<unsigned char*>(renderer_of(self)) + 56);
}
static const unsigned short* realm_string(int id) {
    using namespace norrath_aoc;
    unsigned char* table = load_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui));
    return GetString__13VIStringTableii(table, id, static_cast<int>(load_word(table + 48)));
}
// Opens the socket when it is closed (the first word holds the port).
static inline void open_socket(void* sock) {
    if (!IsConnected__11VIUDPSocket(sock)) {
        set_word(sock, 0, 3000);
        Open__11VIUDPSocket(sock);
    }
}
// Moves rows [first+1, ...) down by one row for `count` rows (1144 bytes each, starting at this+5088+1144*first).
static inline void shift_rows(unsigned char* object, int first, int count) {
    for (int j = 0; j < count; ++j) copy_bytes<1144>(object + 5088 + 1144 * (first + j), object + 5088 + 1144 * (first + j + 1));
}
static inline void fill_player_block(unsigned char* block, const unsigned char* app) {
    strcpy(reinterpret_cast<char*>(block), reinterpret_cast<const char*>(app + 0x17a34));
    strcpy(reinterpret_cast<char*>(block + 24), reinterpret_cast<const char*>(app + 0x17a74));
}

extern "C" {
void process__16VIRealmInterface(void* self) {
    using namespace norrath_aoc;
    unsigned char* object = static_cast<unsigned char*>(self);
    void* sock = object + 3396;
    const long long now = GetTime__7VITimer();
    unsigned char* app = static_cast<unsigned char*>(GetAppPtr__5VIWnd(renderer_of(self)));
    if (!(now - time_at(object, 9728) < 30001)) ForceDisconnect__20VIWndGenericRenderer(renderer_of(self));
    if (strstr_call(GetName__9VIGUIPage(page_of(self)), "GameServ") != 0) {
        char level[32], nameKey[32], classKey[32], levelKey[32];
        sprintf(level, "L%i", static_cast<int>(word_at(app, 0x17ab4)));
        SetStringValue__9VIGUIPagePCcT1b(page_of(self), "Player1Name", reinterpret_cast<const char*>(app + 0x17a34), 0);
        SetStringValue__9VIGUIPagePCcT1b(page_of(self), "Player1Class", reinterpret_cast<const char*>(app + 0x17a74), 0);
        SetStringValue__9VIGUIPagePCcT1b(page_of(self), "Player1Level", level, 0);
        int index = 0;
        do {
            sprintf(nameKey, "Player%iName", index + 2);
            sprintf(classKey, "Player%iClass", index + 2);
            sprintf(levelKey, "Player%iLevel", index + 2);
            unsigned char* row = object + 1144 * index;
            const bool present = index + 1 < static_cast<int>(word_at(object, 3476));
            index += 1;
            if (!present) {
                SetStringValue__9VIGUIPagePCcT1b(page_of(self), nameKey, ".........", 0);
                SetStringValue__9VIGUIPagePCcT1b(page_of(self), classKey, "......", 0);
                SetStringValue__9VIGUIPagePCcT1b(page_of(self), levelKey, "...", 0);
            } else {
                sprintf(level, "L%i", static_cast<int>(word_at(row, 6228)));
                if (static_cast<signed char>(row[6180]) != 0) {
                    SetStringValue__9VIGUIPagePCcT1b(page_of(self), nameKey, reinterpret_cast<const char*>(row + 6180), 0);
                    SetStringValue__9VIGUIPagePCcT1b(page_of(self), classKey, reinterpret_cast<const char*>(row + 6204), 0);
                    SetStringValue__9VIGUIPagePCcT1b(page_of(self), levelKey, level, 0);
                } else {
                    SetStringValue__9VIGUIPagePCcT1b(page_of(self), nameKey, "?", 0);
                    SetStringValue__9VIGUIPagePCcT1b(page_of(self), classKey, "?", 0);
                    SetStringValue__9VIGUIPagePCcT1b(page_of(self), levelKey, "?", 0);
                }
            }
        } while (index < 3);
    }
    if (!(now - g_lastSessionTouch < 10001)) {
        g_lastSessionTouch = GetTime__7VITimer();
        if (connected != 0) {
            printf("Touching session\n");
            touchSession__Q212RealmService15RealmGatewayAPIPCUsPv(self, reinterpret_cast<const unsigned short*>(object + 16), 0);
            if (strstr_call(GetName__9VIGUIPage(page_of(self)), "FindGame") != 0) {
                getRealmStats__Q212RealmService15RealmGatewayAPIPv(self, 0);
                unsigned char attrs[16];
                __Q212RealmService14MatchGameAttrs(attrs);
                matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv(
                    self, reinterpret_cast<const unsigned short*>(object + 16), 2, 0, 0, -1, attrs, true, 100, 1024, 0);
                _$_Q212RealmService14MatchGameAttrs(attrs, 2);
            }
        }
    }
    unsigned state = word_at(object, 3468);
    if (state == 1 || state == 5) {
        unsigned char packet[256];
        bool run = true;
        if (state == 1) {
            open_socket(sock);
            if (word_at(object, 2344) == 0)
                GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(sock, object + 2328, reinterpret_cast<const char*>(object + 1300), 5000);
            run = IsConnected__11VIUDPSocket(sock) != 0 && word_at(object, 2344) != 0 && frame_counter % 30 == 0;
        } else if (frame_counter % 30 == 0) {
            printf("Doing client discovery\n");
            open_socket(sock);
            if (word_at(object, 2344) == 0)
                GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(sock, object + 2328, reinterpret_cast<const char*>(object + 1300), 5000);
            run = IsConnected__11VIUDPSocket(sock) != 0 && word_at(object, 2344) != 0;
        } else {
            run = false;
        }
        if (run) {
            if (word_at(object, 9688) == 0) {
                GetLocalAddress__11VIUDPSocketP13VIInetAddress(sock, object + 9672);
                if (word_at(object, 9688) != 0) {
                    memset(object + 9692, 0, 24);
                    GetStringAddress__11VIUDPSocketP13VIInetAddressPc(sock, object + 9672, reinterpret_cast<char*>(object + 9692));
                    printf("!!!!!!!!!!!!My address done equal (%s)\n", object + 9692);
                    if (state == 1) UpdateNumPlayers__16VIRealmInterfacei(self, static_cast<int>(word_at(object, 3476)));
                }
            }
            const int length = packDiscoveryPacket__Q212RealmService15RealmGatewayAPIPUcPCUs(self, packet, reinterpret_cast<const unsigned short*>(object + 16));
            unsigned char address[20];
            copy_bytes<20>(address, object + 2328);
            SendTo__11VIUDPSocketG13VIInetAddressiPCciPii(sock, address, static_cast<int>(word_at(object, 2324)), reinterpret_cast<const char*>(packet), length, 0, 0);
        }
    } else if (state - 11 < 2u) {
        if (!IsConnected__11VIUDPSocket(sock)) {
            printf("\tserver socket not connected\n");
            set_word(sock, 0, 3000);
            Open__11VIUDPSocket(sock);
        }
        if (word_at(object, 3392) == 0) {
            printf("\tserver address not initialized\n");
            GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(sock, object + 3376, reinterpret_cast<const char*>(object + 2348), 5000);
        }
        if (IsConnected__11VIUDPSocket(sock) != 0 && word_at(object, 3392) != 0 && frame_counter % 30 == 0) {
            char localText[24];
            unsigned char packet[108], block[52], first[20], second[20], third[20];
            const int stateWord = static_cast<int>(word_at(object, 3468));
            memset(localText, 0, 24);
            strcpy(localText, g_localIp);
            const int localPort = g_localPort;
            memset(packet, 0, 108);
            set_word(packet, 0, static_cast<unsigned>(stateWord));
            copy_bytes<24>(packet + 4, object + 9692);
            copy_bytes<24>(packet + 28, localText);
            memset(block, 0, 52);
            set_word(packet, 52, static_cast<unsigned>(localPort));
            fill_player_block(block, app);
            const unsigned level = word_at(app, 0x17ab4);
            copy_bytes<48>(packet + 56, block);
            set_word(packet, 104, level);
            copy_bytes<20>(first, object + 3376);
            copy_bytes<20>(second, object + 0x947c);
            copy_bytes<20>(third, object + 0x8b08);
            SendTo__11VIUDPSocketG13VIInetAddressiT1T1iPCciPii(sock, first, static_cast<int>(word_at(object, 3372)), second, third, 3000,
                                                               reinterpret_cast<const char*>(packet), 108, 0, 0);
        }
        if (!(now - time_at(object, 9720) < 10001)) {
            GoBack__20VIWndGenericRenderer(renderer_of(self));
            Show__11VIGUIDialogPCUsPCc(dialog_of(self), realm_string(1840), 0);
            SetText__11VIGUIDialogPCUsb(dialog_of(self), realm_string(1858), 0);
            set_word(object, 3468, 0);
            Reset__16VIRealmInterface(self);
        }
    } else if (state - 2 < 3u) {
        int others = static_cast<int>(word_at(object, 3476)) - 1;
        if (frame_counter % 30 == 0 && others > 0) {
            int i = 0;
            do {
                unsigned char* row = object + 1144 * i;
                if (word_at(row, 6132) == 0)
                    GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(sock, row + 6116, reinterpret_cast<const char*>(row + 5088), 5000);
                if (IsConnected__11VIUDPSocket(sock) != 0 && word_at(row, 6132) != 0) {
                    if (word_at(row, 6136) == 9) {
                        unsigned char packet[236], address[20], second[20];
                        memset(packet, 0, 236);
                        set_word(packet, 0, word_at(row, 6136));
                        set_word(packet, 4, word_at(object, 3476));
                        sprintf(reinterpret_cast<char*>(packet + 8), reinterpret_cast<const char*>(object + 2348));
                        set_word(packet, 104, word_at(object, 3372));
                        sprintf(reinterpret_cast<char*>(packet + 120), reinterpret_cast<const char*>(object + 9692));
                        set_word(packet, 216, 3000);
                        for (int k = 1; k < static_cast<int>(word_at(object, 3476)); ++k) {
                            unsigned char* peer = object + 1144 * k;
                            sprintf(reinterpret_cast<char*>(packet + 8 + 24 * k), reinterpret_cast<const char*>(peer + 3944));
                            set_word(packet, 104 + 4 * k, word_at(peer, 4968));
                            sprintf(reinterpret_cast<char*>(packet + 120 + 24 * k), reinterpret_cast<const char*>(peer + 5008));
                            set_word(packet, 216 + 4 * k, word_at(peer, 5032));
                        }
                        printf("Server sending start game packet to client\n");
                        set_word(packet, 232, static_cast<unsigned>(i + 1));
                        copy_bytes<20>(address, row + 6116);
                        copy_bytes<20>(second, object + 0x947c);
                        const int sent = SendTo__11VIUDPSocketG13VIInetAddressiT1PCciT4iPii(
                            sock, address, static_cast<int>(word_at(row, 6112)), second, reinterpret_cast<const char*>(row + 6152),
                            static_cast<int>(word_at(row, 6176)), reinterpret_cast<const char*>(packet), 236, 0, 0);
                        printf("Server sending start game packet to client, sent %i of %i bytes\n", sent, 236);
                    } else {
                        unsigned char packet[256], table[208], address[20], second[20];
                        memset(packet, 0, 256);
                        const unsigned status = word_at(row, 6136);
                        printf("Server sending ping packet to client, state = %i\n", static_cast<int>(status));
                        set_word(packet, 0, status);
                        if (word_at(row, 6136) == 7) copy_bytes<24>(packet + 4, object + 9692);
                        sprintf(reinterpret_cast<char*>(packet + 28), "%i/%i", static_cast<int>(word_at(object, 3476)), static_cast<int>(word_at(object, 3472)));
                        memset(table, 0, 208);
                        fill_player_block(table, app);
                        set_word(table, 48, word_at(app, 0x17ab4));
                        const int count = static_cast<int>(word_at(object, 3476));
                        for (int j = 0; j < 3; ++j) {
                            if (j + 1 < count) copy_bytes<52>(table + 52 * (j + 1), object + 1144 * j + 6180);
                        }
                        copy_bytes<208>(packet + 32, table);
                        copy_bytes<20>(address, row + 6116);
                        copy_bytes<20>(second, object + 0x947c);
                        SendTo__11VIUDPSocketG13VIInetAddressiT1PCciT4iPii(sock, address, static_cast<int>(word_at(row, 6112)), second,
                                                                           reinterpret_cast<const char*>(row + 6152), 3000,
                                                                           reinterpret_cast<const char*>(packet), 256, 0, 0);
                    }
                }
                const int next = i + 1;
                if (!(now - time_at(row, 6144) < 20001)) {
                    const int moves = others - i - 1;
                    if (moves > 0) shift_rows(object, i, moves);
                    UpdateNumPlayers__16VIRealmInterfacei(self, static_cast<int>(word_at(object, 3476)) - 1);
                }
                i = next;
                others = static_cast<int>(word_at(object, 3476)) - 1;
            } while (i < others);
        }
        if (word_at(object, 3468) == 3 && !(now - time_at(object, 9664) < 5001)) {
            g_mode = 1;
            g_numClients = static_cast<int>(word_at(object, 3476));
            sprintf(g_ips, reinterpret_cast<const char*>(object + 2348));
            g_ports[0] = static_cast<int>(word_at(object, 3372));
            sprintf(g_localClientIps, reinterpret_cast<const char*>(object + 9692));
            g_localClientPorts[0] = 3000;
            for (int k = 1; k < g_numClients; ++k) {
                unsigned char* peer = object + 1144 * k;
                sprintf(g_ips + 24 * k, reinterpret_cast<const char*>(peer + 3944));
                g_ports[k] = static_cast<int>(word_at(peer, 4968));
                sprintf(g_localClientIps + 24 * k, reinterpret_cast<const char*>(peer + 5008));
                g_localClientPorts[k] = static_cast<int>(word_at(peer, 5032));
            }
            Finish__20VIWndGenericRenderer(renderer_of(self));
            set_word(object, 3468, 0);
            Reset__16VIRealmInterface(self);
        }
        if (word_at(object, 3468) == 4) {
            const long long later = GetTime__7VITimer();
            bool cancel = false;
            if (word_at(object, 3476) == 1) {
                set_word(object, 3468, 0);
                cancel = true;
            } else if (!(later - time_at(object, 9664) < 5001)) {
                set_word(object, 3468, 0);
                cancel = true;
            }
            if (cancel) {
                Reset__16VIRealmInterface(self);
                GoBack__20VIWndGenericRenderer(renderer_of(self));
                cancelGame__Q212RealmService15RealmGatewayAPIPCUsPv(self, reinterpret_cast<const unsigned short*>(object + 16), 0);
            }
        }
    }
    // receive
    if (IsConnected__11VIUDPSocket(sock) != 0) {
        unsigned char from[20] __attribute__((aligned(8)));
        unsigned char buffer[1024] __attribute__((aligned(8)));
        int fromPort;
        memset(buffer, 0, 1024);
        int length = RecvFrom__11VIUDPSocketP13VIInetAddressPiPciT2i(sock, from, &fromPort, reinterpret_cast<char*>(buffer), 1024, 0, 0);
        const unsigned type = word_at(buffer, 0);
        unsigned char* app2 = static_cast<unsigned char*>(GetAppPtr__5VIWnd(renderer_of(self)));
        if (length > 0) {
            if (word_at(app2, 0) == 2 && word_at(object, 3468) == 0) {
                int reply = 13;
                SendTo__11VIUDPSocketG13VIInetAddressiPCciPii(sock, from, fromPort, reinterpret_cast<const char*>(&reply), 4, 0, 0);
                length = 0;
            }
            if (length > 0) {
                printf("client received packet from server, port = %i\n", fromPort);
                printf("ret returned %i\n", length);
                if (type == 13 && word_at(app2, 0) == 1) {
                    const int clients = static_cast<int>(word_at(object, 3476)) - 1;
                    for (int i = 0; i < clients; ++i) {
                        unsigned char* row = object + 1144 * i;
                        if (memcmp(row + 6116, from, 20) == 0 && static_cast<int>(word_at(row, 6112)) == fromPort) {
                            const int moves = clients - i - 1;
                            if (moves > 0) shift_rows(object, i, moves);
                            UpdateNumPlayers__16VIRealmInterfacei(self, static_cast<int>(word_at(object, 3476)) - 1);
                        }
                    }
                } else if (type - 11 < 2u && word_at(app2, 0) == 1) {
                    unsigned char discovered[24];
                    printf("!!!!received ping from local ip %s!!!!\n", buffer + 4);
                    copy_bytes<24>(discovered, buffer + 28);
                    bool found = false;
                    const int discoveredPort = static_cast<int>(word_at(buffer, 52));
                    printf("\tdiscoveredIp = %s\n", discovered);
                    printf("\tdiscoveredPort = %i\n", discoveredPort);
                    const bool small = length < 51;
                    for (int i = 0; i < static_cast<int>(word_at(object, 3476)) - 1; ++i) {
                        unsigned char* row = object + 1144 * i;
                        bool direct = false;
                        if (static_cast<signed char>(row[6152]) != 0) {
                            if (strcmp(reinterpret_cast<const char*>(buffer + 4), reinterpret_cast<const char*>(row + 6152)) != 0) continue;
                        }
                        if (memcmp(from, row + 6116, 16) == 0 && static_cast<int>(word_at(row, 6112)) == fromPort) {
                            direct = true;
                        } else {
                            if (strcmp(reinterpret_cast<const char*>(row + 5088), reinterpret_cast<const char*>(discovered)) != 0) continue;
                            if (static_cast<int>(word_at(row, 6112)) != discoveredPort) continue;
                        }
                        found = true;
                        if (word_at(row, 6136) == 7) set_word(row, 6136, 8);
                        *reinterpret_cast<i64_t*>(row + 6144) = GetTime__7VITimer();
                        strcpy(reinterpret_cast<char*>(row + 6152), reinterpret_cast<const char*>(buffer + 4));
                        set_word(row, 6176, 3000);
                        if (!direct) {
                            copy_bytes<20>(row + 6116, from);
                            set_word(row, 6112, static_cast<unsigned>(fromPort));
                        }
                        if (!small) copy_bytes<52>(row + 6180, buffer + 56);
                        break;
                    }
                    if (!found) {
                        for (int i = 0; i < static_cast<int>(word_at(object, 3476)) - 1; ++i) {
                            unsigned char* row = object + 1144 * i;
                            if (memcmp(from, row + 6116, 16) == 0 && fromPort != static_cast<int>(word_at(row, 6112))) {
                                set_word(row, 6112, static_cast<unsigned>(fromPort));
                                if (word_at(row, 6136) == 7) set_word(row, 6136, 8);
                                *reinterpret_cast<i64_t*>(row + 6144) = GetTime__7VITimer();
                                strcpy(reinterpret_cast<char*>(row + 6152), reinterpret_cast<const char*>(buffer + 4));
                                set_word(row, 6176, 3000);
                                if (!small) copy_bytes<52>(row + 6180, buffer + 56);
                            }
                        }
                    }
                } else if (type == 7 && word_at(app2, 0) == 2) {
                    *reinterpret_cast<i64_t*>(object + 9720) = GetTime__7VITimer();
                    char text[32];
                    sprintf(text, "%d.%d.%d.%d", from[7], from[6], from[5], from[4]);
                    if (strcmp(text, g_localIp) == 0) {
                        char same[32];
                        strcpy(same, reinterpret_cast<const char*>(buffer + 4));
                        printf("Received message from machine behind the same NAT, localIP = %s\n", same);
                        set_word(object, 3372, 3000);
                        GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(sock, object + 3376, same, 1000);
                    } else if (fromPort != 3000) {
                        copy_bytes<20>(object + 3376, from);
                        set_word(object, 3372, static_cast<unsigned>(fromPort));
                    }
                    printf("Received REALM_SERVER_CLIENT_INITIAL\n");
                    set_word(object, 3468, 12);
                    SetStringValue__9VIGUIPagePCcPCUsb(page_of(self), "ServerStatus", realm_string(1842), 1);
                } else if (type == 8 && word_at(app2, 0) == 2) {
                    *reinterpret_cast<i64_t*>(object + 9720) = GetTime__7VITimer();
                    set_word(object, 3468, 12);
                    char count[16];
                    unsigned char table[208];
                    *reinterpret_cast<i64_t*>(count) = 0;
                    sprintf(count, "%c / %c", static_cast<char>(buffer[28]), static_cast<char>(buffer[30]));
                    copy_bytes<208>(table, buffer + 32);
                    if (strstr_call(GetName__9VIGUIPage(page_of(self)), "GameClnt") != 0) {
                        char level[32], nameKey[32], classKey[32], levelKey[32];
                        int index = 0;
                        do {
                            const int number = index + 1;
                            sprintf(nameKey, "Player%iName", number);
                            sprintf(classKey, "Player%iClass", number);
                            sprintf(levelKey, "Player%iLevel", number);
                            unsigned char* entry = table + 52 * index;
                            index = number;
                            sprintf(level, "L%i", static_cast<int>(word_at(entry, 48)));
                            if (static_cast<signed char>(entry[0]) != 0) {
                                SetStringValue__9VIGUIPagePCcT1b(page_of(self), nameKey, reinterpret_cast<const char*>(entry), 0);
                                SetStringValue__9VIGUIPagePCcT1b(page_of(self), classKey, reinterpret_cast<const char*>(entry + 24), 0);
                                SetStringValue__9VIGUIPagePCcT1b(page_of(self), levelKey, level, 0);
                            } else {
                                SetStringValue__9VIGUIPagePCcT1b(page_of(self), nameKey, ".........", 0);
                                SetStringValue__9VIGUIPagePCcT1b(page_of(self), classKey, "......", 0);
                                SetStringValue__9VIGUIPagePCcT1b(page_of(self), levelKey, "...", 0);
                            }
                        } while (index < 4);
                    }
                    printf("Received REALM_SERVER_CLIENT_RECEIVED_PACKET\n");
                    SetStringValue__9VIGUIPagePCcPCUsb(page_of(self), "ServerStatus", realm_string(1843), 1);
                    SetStringValue__9VIGUIPagePCcT1b(page_of(self), "NumPlayers", count, 0);
                } else if (type == 14 && word_at(app2, 0) == 2) {
                    if (strstr_call(GetName__9VIGUIPage(page_of(self)), "GameClnt") != 0) {
                        GoBack__20VIWndGenericRenderer(renderer_of(self));
                        GoBack__20VIWndGenericRenderer(renderer_of(self));
                        set_word(object, 3468, 0);
                        Reset__16VIRealmInterface(self);
                        Show__11VIGUIDialogPCUsPCc(dialog_of(self), realm_string(1869), 0);
                        SetText__11VIGUIDialogPCUsb(dialog_of(self), realm_string(1871), 0);
                    }
                } else if (type == 9 && word_at(object, 3468) == 12 && word_at(app2, 0) == 2) {
                    unsigned char packet[236] __attribute__((aligned(8)));
                    *reinterpret_cast<i64_t*>(object + 9720) = GetTime__7VITimer();
                    copy_bytes<236>(packet, buffer);
                    if (memcmp(from, object + 3376, 20) == 0 || memcmp(from, object + 0x8b08, 20) == 0) {
                        g_numClients = static_cast<int>(word_at(packet, 4));
                        if (g_numClients > 0) {
                            for (int k = 0; k < g_numClients; ++k) {
                                sprintf(g_ips + 24 * k, reinterpret_cast<const char*>(packet + 8 + 24 * k));
                                g_ports[k] = static_cast<int>(word_at(packet, 104 + 4 * k));
                                sprintf(g_localClientIps + 24 * k, reinterpret_cast<const char*>(packet + 120 + 24 * k));
                                g_localClientPorts[k] = static_cast<int>(word_at(packet, 216 + 4 * k));
                            }
                        }
                        g_mode = 2;
                        printf("!!g_numClients = %i\n", g_numClients);
                        g_myIndex = static_cast<int>(word_at(packet, 232));
                        Finish__20VIWndGenericRenderer(renderer_of(self));
                        set_word(object, 3468, 0);
                        Reset__16VIRealmInterface(self);
                    }
                } else if (type == 10) {
                    if (word_at(app2, 0) == 2) {
                        *reinterpret_cast<i64_t*>(object + 9720) = GetTime__7VITimer();
                        if (word_at(object, 3468) != 0) {
                            GoBack__20VIWndGenericRenderer(renderer_of(self));
                            GoBack__20VIWndGenericRenderer(renderer_of(self));
                            Show__11VIGUIDialogPCUsPCc(dialog_of(self), realm_string(1840), 0);
                            SetText__11VIGUIDialogPCUsb(dialog_of(self), realm_string(1841), 0);
                            set_word(object, 3468, 0);
                            Reset__16VIRealmInterface(self);
                        }
                    }
                }
            }
        }
    }
    frame_counter = frame_counter + 1;
    process__Q212RealmService15RealmGatewayAPI(self);
}
}
