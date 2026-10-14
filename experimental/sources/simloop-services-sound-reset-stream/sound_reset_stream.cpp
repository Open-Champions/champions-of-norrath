// Observed soundResetStream__Fi at 0x0024af80, 272 bytes.
// The selected 452-byte stream record clears byte 0 and the words at +20,
// +28, +448, +32, and +36. Eight separate 48-byte clears then cover +64
// through +447. The first word written into sbuffin is the index. The other
// twelve send bytes are copied from uninitialized stack in the original, so
// this reconstruction clears them. sceSifCallRpc then uses gCd, command 96,
// mode 0, a 16-byte send from sbuffin, and a 64-byte receive into sbuff.
extern "C" {
extern unsigned char gCd[];
extern unsigned char sbuff[];
extern unsigned char sbuffin[];
extern unsigned char stream[];
void* memset(void* destination, int value, unsigned long size);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end, void* end_param);

static void store_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

void soundResetStream__Fi(int index) {
    unsigned char* const record = stream + index * 452;
    record[0] = 0;
    store_word(record + 20, 0);
    store_word(record + 28, 0);
    store_word(record + 448, 0);
    store_word(record + 32, 0);
    store_word(record + 36, 0);
    for (int part = 0; part < 8; ++part) memset(record + 64 + part * 48, 0, 48);
    store_word(sbuffin, static_cast<unsigned>(index));
    store_word(sbuffin + 4, 0);
    store_word(sbuffin + 8, 0);
    store_word(sbuffin + 12, 0);
    sceSifCallRpc(gCd, 96u, 0u, sbuffin, 16, sbuff, 64, nullptr, nullptr);
}
}
