/* Observed _sceFs_Rcv_Intr at 0x002d8af0, 948 bytes (a local routine; sceFsInit
 * registers it as the handler for command 0x80000011 with _sif_FsRcv_Data as the
 * argument). It is the receive side of the file-system server's replies.
 *
 * The first argument is a command packet. When _fs_rcv_bufdbl (0x004449f8) is
 * nonzero its word 3 (offset 12) selects one of two reply buffers, otherwise
 * buffer 0 is used. The selector is stored in ee_retbuf_sel (0x004449f0). The reply
 * buffer is the 0x440-byte slot of _rcv_data_cmd (0x008e1140) at that index, read
 * through the uncached alias (address | 0x20000000).
 *
 * The first 16 bytes of the reply are copied to the stack as four words: a status
 * word S, a command word C, a destination D and a length L. In order:
 *   1. When S is not negative, L bytes are copied from reply+16 to D.
 *   2. C minus 2, as an unsigned number, selects a case (the jump table at
 *      0x0047b870 has 25 entries; other values do nothing):
 *      C=2: reply words at 20 and 24 are two byte counts. For the first, when it is
 *           positive, bytes are copied from reply+36 to the address in the word at
 *           reply+28, stored both through that address with the uncached bit set
 *           and through the plain address, one byte at a time; the count is read
 *           again after every byte. The second count (reply+24) does the same with
 *           the destination word at reply+32 and the source at reply+100.
 *      C=11: when the word at address D is positive, 324 bytes are copied from
 *           reply+24 to the address in the word at reply+20.
 *      C=12: 64 bytes are copied from reply+24 to the address in the word at
 *           reply+20.
 *      C=23, 25 or 26: the word at reply+24 is a length, limited to 1024; that many
 *           bytes are copied from reply+28 to the address in the word at reply+20.
 *   3. When S is not negative, iSignalSema(S) is called. When S is negative, -S is
 *      searched for in the 32 words of _sceFs_q (0x00444970), in order, and the
 *      first word that equals it is set to -1; only the first match changes.
 * Nothing is returned. The second argument is not used.
 * Inferred: S is a semaphore to wake for a normal reply, and a negative S names a
 * queue entry the request used, which is then released.
 * Unknown: the meaning of each command number and of each copied block, whether the
 * server can send other numbers, and the reply buffer layout beyond the offsets
 * used here. Compiled output is not claimed to match the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
// Host: 32-bit addresses are offsets into an arena and the uncached alias bit is dropped.
extern "C" unsigned char* norrath_host_arena;
extern "C" unsigned norrath_host_reply_offset;
#define NORRATH_REPLY_BASE() (norrath_host_reply_offset)
#define NORRATH_ADDRESS_POINTER(address) (norrath_host_arena + ((address) & 0x1fffffffu))
#else
#define NORRATH_REPLY_BASE() (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_rcv_data_cmd)))
#define NORRATH_ADDRESS_POINTER(address) \
    (reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(static_cast<int>(address))))
#endif
extern "C" {
extern int _fs_rcv_bufdbl;
extern int ee_retbuf_sel;
extern int _sceFs_q[32];
extern unsigned char _rcv_data_cmd[];
int iSignalSema(int semaphore);
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

static inline unsigned load_word(const volatile unsigned char* p) {
    return static_cast<unsigned>(p[0]) | (static_cast<unsigned>(p[1]) << 8) |
           (static_cast<unsigned>(p[2]) << 16) | (static_cast<unsigned>(p[3]) << 24);
}
static inline unsigned char* as_pointer(unsigned address) { return NORRATH_ADDRESS_POINTER(address); }

// Copies count bytes one at a time to the address and its uncached alias, reading the
// count word again after every byte.
static void copy_through_both(const volatile unsigned char* source, unsigned destination,
                              const volatile int* count) {
    if (*count <= 0) return;
    unsigned char* const plain = as_pointer(destination);
    volatile unsigned char* const uncached = as_pointer(destination | 0x20000000u);
    int index = 0;
    do {
        const unsigned char value = source[index];
        uncached[index] = value;
        plain[index] = value;
        ++index;
    } while (index < *count);
}

void _sceFs_Rcv_Intr(const unsigned* packet, void* unused) {
    int selector = 0;
    if (_fs_rcv_bufdbl != 0) selector = static_cast<int>(packet[3]);
    ee_retbuf_sel = selector;
    const volatile unsigned char* const reply = as_pointer(
        NORRATH_REPLY_BASE() + selector * 0x440u + 0x20000000u);
    const int status = static_cast<int>(load_word(reply));
    const unsigned command = load_word(reply + 4);
    const unsigned destination = load_word(reply + 8);
    const unsigned length = load_word(reply + 12);
    if (status >= 0) memcpy(as_pointer(destination), const_cast<const unsigned char*>(reply + 16), length);
    switch (command) {
    case 2:
        copy_through_both(reply + 36, load_word(reply + 28), reinterpret_cast<const volatile int*>(reply + 20));
        copy_through_both(reply + 100, load_word(reply + 32), reinterpret_cast<const volatile int*>(reply + 24));
        break;
    case 11:
        if (static_cast<int>(load_word(as_pointer(destination))) > 0)
            memcpy(as_pointer(load_word(reply + 20)), const_cast<const unsigned char*>(reply + 24), 324);
        break;
    case 12:
        memcpy(as_pointer(load_word(reply + 20)), const_cast<const unsigned char*>(reply + 24), 64);
        break;
    case 23:
    case 25:
    case 26: {
        unsigned bytes = load_word(reply + 24);
        if (bytes > 1024u) bytes = 1024u;
        memcpy(as_pointer(load_word(reply + 20)), const_cast<const unsigned char*>(reply + 28), bytes);
        break;
    }
    default:
        break;
    }
    if (status >= 0) {
        iSignalSema(status);
        return;
    }
    const int wanted = -status;
    for (int index = 0; index < 32; ++index) {
        if (_sceFs_q[index] == wanted) {
            _sceFs_q[index] = -1;
            break;
        }
    }
}
}
