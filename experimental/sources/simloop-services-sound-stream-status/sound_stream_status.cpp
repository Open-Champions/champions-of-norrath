// Observed soundGetStreamPlayStatus__Fi at 0x0024adb8, 248 bytes.
// It always calls sceSifCallRpc on gCd, command 80, mode 0, send sbuffin with
// length 0, and receive sbuff for 64 bytes. The two words at sbuff+index*16
// decide the first branch. stream records are 452 bytes. Unequal words, or a
// positive signed word at record+20, return the word at record+32 with bit 0
// set. Otherwise a non-positive word at record+448 returns 0. A word at
// record+108 whose bit 3 is clear returns that word with bit 0 set; bit 3
// set returns 0.
extern "C" {
extern unsigned char gCd[];
extern unsigned char sbuff[];
extern unsigned char sbuffin[];
extern unsigned char stream[];
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end, void* end_param);

static unsigned load_word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}

int soundGetStreamPlayStatus__Fi(int index) {
    sceSifCallRpc(gCd, 80u, 0u, sbuffin, 0, sbuff, 64, nullptr, nullptr);
    const unsigned first = load_word(sbuff + index * 16);
    const unsigned second = load_word(sbuff + index * 16 + 4);
    unsigned char* const record = stream + index * 452;
    if (first != second || static_cast<int>(load_word(record + 20)) > 0) return static_cast<int>(load_word(record + 32) | 1u);
    if (static_cast<int>(load_word(record + 448)) <= 0) return 0;
    const unsigned marker = load_word(record + 108);
    if ((marker & 8u) == 0u) return static_cast<int>(marker | 1u);
    return 0;
}
}
