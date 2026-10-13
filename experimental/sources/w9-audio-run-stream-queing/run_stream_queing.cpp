/* runStreamQueing__Fv at 0x0024a7e0 is 1216 bytes (a local function of the
 * sound unit). `stream` (0x007dd290) holds three 452-byte records; the
 * queue blocks of a record are 48 bytes from offset 64, their count is the
 * word at 448, and the flag word of block n is at 108 + 48 * n. The three
 * RPC buffers are the authored norrath_sound_rpc_*_view objects.
 *
 * Observed, in order:
 *
 *   1. sceSifCallRpc command 80, mode 0, no send bytes, 64 receive bytes,
 *      null end function and parameter. The first 48 bytes of the receive
 *      buffer are copied to a local table of three 4-word entries, one per
 *      stream; the calls below read it.
 *   2. For stream 0, 1, 2 in turn:
 *      - A record whose count is zero is skipped.
 *      - The record is skipped when flag word 0 has bit 0x8, or has bit 0x4
 *        while the record's word at +20 is nonzero.
 *      - If flag word 0 has bit 0x2: with the table entry's third word
 *        nonzero, command 112 is sent (send words: stream number, 0) and
 *        the record is done; with it zero, command 96 is sent (send word:
 *        stream number) and the record goes on.
 *      - The first queue block is copied aside; the remaining blocks move
 *        down one place (the count is re-read from memory on every pass of
 *        the move, so a count above seven reaches the count word itself);
 *        the count is reduced by one (re-read after the move); strcpy
 *        copies the saved block's start onto the record's first bytes;
 *        then record words +16, +20, +24, +28, +32, +52, +56, +60 take the
 *        saved block's words +16, +20, +24, +28, +44, +32, +36, +40 and
 *        record word +40 takes the table entry's first word.
 *      - Command 112 is sent with send words (stream number, 0x3fff).
 *      - If the record's word at +36 is nonzero, command 64 is sent with
 *        send words (stream number, the +36 word, the +52 word, the
 *        halfword at +56, and the halfword 1), and the +36 word is cleared.
 *      - The result becomes 1.
 *   3. The result (0 unless a record reached the queue advance) is returned.
 *
 * Not reproduced: every send copies 16 bytes of a stack local of which only
 * the words listed above are written; the others come from stale stack. This
 * source writes only the listed send words and leaves the rest of the send
 * buffer as it was, as the other authored sound RPC callers do. The meaning
 * of the commands (80, 96, 112, 64) and of the table words is not
 * established here.
 */
extern "C" {
extern unsigned char stream[];
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_rpc_send_view[];
extern unsigned char norrath_sound_rpc_receive_view[];
char* strcpy(char* destination, const char* source);
int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int send_bytes,
                           void* receive, int receive_bytes, void* end_function,
                           void* end_parameter);

static int load_int(const unsigned char* slot) {
    int value;
    __builtin_memcpy(&value, slot, sizeof value);
    return value;
}

static void store_int(unsigned char* slot, int value) {
    __builtin_memcpy(slot, &value, sizeof value);
}

static void send_command(int command, int receive_bytes_unused) {
    (void)receive_bytes_unused;
    norrath_sound_call_rpc(norrath_sound_rpc_client_view, command, 0, norrath_sound_rpc_send_view, 16,
                           norrath_sound_rpc_receive_view, 64, nullptr, nullptr);
}

int runStreamQueing__Fv() {
    norrath_sound_call_rpc(norrath_sound_rpc_client_view, 80, 0, norrath_sound_rpc_send_view, 0,
                           norrath_sound_rpc_receive_view, 64, nullptr, nullptr);
    int table[3][4];
    for (int entry = 0; entry < 3; ++entry)
        for (int word = 0; word < 4; ++word)
            table[entry][word] = load_int(norrath_sound_rpc_receive_view + entry * 16 + word * 4);

    int advanced = 0;
    for (int number = 0; number < 3; ++number) {
        unsigned char* record = stream + number * 452;
        if (load_int(record + 448) == 0) continue;
        const int flags = load_int(record + 108);
        if ((flags & 0x4) && load_int(record + 20) != 0) continue;
        if (flags & 0x8) continue;

        int* send = reinterpret_cast<int*>(norrath_sound_rpc_send_view);
        if (flags & 0x2) {
            if (table[number][2] != 0) {
                send[0] = number;
                send[1] = 0;
                send_command(112, 0);
                continue;
            }
            send[0] = number;
            send_command(96, 0);
        }

        unsigned char first[48];
        __builtin_memcpy(first, record + 64, sizeof first);
        if (load_int(record + 448) - 1 > 0) {
            int moved = 0;
            do {
                __builtin_memcpy(record + 64 + moved * 48, record + 64 + (moved + 1) * 48, 48);
                ++moved;
            } while (moved < *reinterpret_cast<volatile int*>(record + 448) - 1);
        }
        store_int(record + 448, load_int(record + 448) - 1);
        strcpy(reinterpret_cast<char*>(record), reinterpret_cast<const char*>(first));

        send[0] = number;
        send[1] = 0x3fff;
        store_int(record + 32, load_int(first + 44));
        store_int(record + 16, load_int(first + 16));
        store_int(record + 24, load_int(first + 24));
        store_int(record + 28, load_int(first + 28));
        store_int(record + 20, load_int(first + 20));
        store_int(record + 52, load_int(first + 32));
        store_int(record + 60, load_int(first + 40));
        store_int(record + 56, load_int(first + 36));
        store_int(record + 40, table[number][0]);
        send_command(112, 0);

        const int pending = load_int(record + 36);
        if (pending != 0) {
            unsigned short half;
            __builtin_memcpy(&half, record + 56, sizeof half);
            send[0] = number;
            send[1] = pending;
            send[2] = load_int(record + 52);
            const unsigned short one = 1;
            __builtin_memcpy(norrath_sound_rpc_send_view + 12, &half, sizeof half);
            __builtin_memcpy(norrath_sound_rpc_send_view + 14, &one, sizeof one);
            send_command(64, 0);
            store_int(record + 36, 0);
        }
        advanced = 1;
    }
    return advanced;
}
}
