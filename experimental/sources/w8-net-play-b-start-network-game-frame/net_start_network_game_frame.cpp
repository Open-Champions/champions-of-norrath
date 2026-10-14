// Observed netStartNetworkGameFrame at 0x0012c8f8, 1636 bytes, returning an
// int. With netBroadCastStart zero it returns 0 at once. Otherwise:
//  1. Broadcast. When (drawFrameNm & 63) == 0 it clears broadcastConnectionFlags,
//     builds a 16-byte address block with bytes 4..7 = ff 01 a8 c0, sets a
//     stack word to 0xbd3d and calls sceInetSendTo with (cd, g_net_buf,
//     broadcastConnection, &word, 4, 0, &block, 3000, 1000). When nmPartners is
//     at least 2, or bit 0x40 of the word at byte 12 of `pad` is set, it
//     builds a second block the same way, sets the word to 0xbd3e and sends
//     the same call eight times.
//  2. Receive. broadcastConnectionFlags is cleared and sceInetRecvFrom(cd,
//     g_net_buf, broadcastConnection, buffer, 512, &broadcastConnectionFlags,
//     &from, &word, 0) is called with a 512-byte buffer and a 16-byte `from`
//     block. A negative result, or a first buffer word other than 0xbd3d or
//     0xbd3e, goes to step 4.
//  3. Partner table. The four bytes at from+4 are looked up in the first four
//     bytes of each of the first nmPartners 17632-byte partner records. If
//     absent and the index equals nmPartners and is below 4, a new record is
//     added: the four bytes, then the stack word at byte 4, and nmPartners
//     grows by one. Then, for j from the new index down to 1, records j-1 and
//     j are swapped (whole 17632 bytes) when byte 0 of record j-1 is below
//     byte 0 of record j. If the buffer word is 0xbd3e it sets nmPlayers to
//     nmPartners, calls playerInit(i, 1, name_i, -1, 0, -1, 0, 0) for each i
//     below nmPlayers (reloading nmPlayers each time) with the four names
//     "Toonces", "Edna", "Otis" and "Gaylord", copies "vtest2" to levelName
//     and returns 1.
//  4. Otherwise it draws: fontStart(defaultFont, 6, 0x80808080, -1), and for
//     each partner prints "%d.%d.%d.%d" of bytes 3, 2, 1, 0 centered at x 320,
//     y 50 + 20 * row, limit 10000, then fontEnd(), and returns 0.
// Inferred: this is the lobby discovery screen: 0xbd3d announces, 0xbd3e starts.
// The sort key is only the first octet.
// Unknown: the stack word read after a receive when no send ran in the same
// call (it is read uninitialised in the image; here it is zero), the argument
// meanings of the two sceInet calls beyond the observed words, and what the
// pad bit 0x40 is.
extern "C" {
extern signed char netBroadCastStart;
extern unsigned drawFrameNm;
extern int broadcastConnection;
extern int broadcastConnectionFlags;
extern int nmPartners;
extern int nmPlayers;
extern unsigned char pad[];
extern unsigned char partners[];
extern unsigned char cd[];
extern unsigned char g_net_buf[];
extern char levelName[];
extern char* defaultFont;
void* memset(void* destination, int value, unsigned long length);
int sprintf(char* buffer, const char* format, ...);
int sceInetSendTo(void* object, void* buffer, int connection, void* data, int length, int flags,
                  void* address, int first_limit, int second_limit);
int sceInetRecvFrom(void* object, void* buffer, int connection, void* data, int length, int* flags,
                    void* address, void* word, int options);
void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char first_byte,
                                 signed char second_byte, signed char third_byte,
                                 signed char fourth_byte, char* tint);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
void fontPrintCentered__FiiPci(int x, int y, char* text, int limit);
void fontEnd__Fv();
}

namespace {
struct PartnerRecord {
    unsigned char bytes[17632];
};

const char* const kPlayerNames[4] = {"Toonces", "Edna", "Otis", "Gaylord"};
}

extern "C" int netStartNetworkGameFrame__Fv() {
    if (netBroadCastStart == 0) return 0;
    int word = 0;
    unsigned char first_block[16];
    unsigned char second_block[16];
    if ((drawFrameNm & 0x3f) == 0) {
        broadcastConnectionFlags = 0;
        memset(first_block, 0, 16);
        word = 0xbd3d;
        first_block[7] = static_cast<unsigned char>(-64);
        first_block[6] = static_cast<unsigned char>(-88);
        first_block[5] = 1;
        first_block[4] = static_cast<unsigned char>(-1);
        sceInetSendTo(cd, g_net_buf, broadcastConnection, &word, 4, 0, first_block, 3000, 1000);
    }
    if (nmPartners >= 2 || (*reinterpret_cast<unsigned*>(pad + 12) & 0x40) != 0) {
        memset(second_block, 0, 16);
        broadcastConnectionFlags = 0;
        word = 0xbd3e;
        second_block[7] = static_cast<unsigned char>(-64);
        second_block[6] = static_cast<unsigned char>(-88);
        second_block[5] = 1;
        second_block[4] = static_cast<unsigned char>(-1);
        for (int attempt = 7; attempt >= 0; --attempt)
            sceInetSendTo(cd, g_net_buf, broadcastConnection, &word, 4, 0, second_block, 3000, 1000);
    }
    unsigned char buffer[512];
    unsigned char from[16];
    broadcastConnectionFlags = 0;
    int received = sceInetRecvFrom(cd, g_net_buf, broadcastConnection, buffer, 512,
                                   &broadcastConnectionFlags, from, &word, 0);
    unsigned header = 0;
    bool known = false;
    if (received >= 0) {
        header = *reinterpret_cast<unsigned*>(buffer);
        known = header - 0xbd3du < 2u;
    }
    if (known) {
        int count = nmPartners;
        int index = 0;
        PartnerRecord* records = reinterpret_cast<PartnerRecord*>(partners);
        for (; index < count; ++index) {
            const unsigned char* bytes = records[index].bytes;
            if (bytes[0] == from[4] && bytes[1] == from[5] && bytes[2] == from[6] && bytes[3] == from[7]) break;
        }
        if (index == count && index < 4) {
            PartnerRecord* added = &records[index];
            added->bytes[0] = from[4];
            added->bytes[1] = from[5];
            added->bytes[2] = from[6];
            added->bytes[3] = from[7];
            *reinterpret_cast<int*>(added->bytes + 4) = word;
            nmPartners = index + 1;
            for (int j = index; j > 0; --j) {
                if (records[j - 1].bytes[0] < records[j].bytes[0]) {
                    PartnerRecord swap = records[j - 1];
                    records[j - 1] = records[j];
                    records[j] = swap;
                }
            }
            header = *reinterpret_cast<unsigned*>(buffer);
        }
        if (header == 0xbd3e) {
            int players = nmPartners;
            nmPlayers = players;
            if (players > 0) {
                int i = 0;
                do {
                    playerInit__FiiPcScScScScT2(i, 1, const_cast<char*>(kPlayerNames[i]), -1, 0, -1, 0, 0);
                    ++i;
                } while (i < nmPlayers);
            }
            levelName[0] = 'v'; levelName[1] = 't'; levelName[2] = 'e'; levelName[3] = 's';
            levelName[4] = 't'; levelName[5] = '2'; levelName[6] = 0;
            return 1;
        }
    }
    fontStart__FP4FontiUii(defaultFont, 6, 0x80808080u, -1);
    if (nmPartners > 0) {
        char text[64];
        int y = 50;
        const PartnerRecord* records = reinterpret_cast<const PartnerRecord*>(partners);
        int row = 0;
        do {
            const unsigned char* bytes = records[row].bytes;
            sprintf(text, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
            ++row;
            fontPrintCentered__FiiPci(320, y, text, 10000);
            y += 20;
        } while (row < nmPartners);
    }
    fontEnd__Fv();
    return 0;
}
