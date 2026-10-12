/* soundFlushCommandBuffer__Fv at 0x00249fb0 is 364 bytes.
 *
 * kons (0x00486bc0) and koffs (0x00486bc8) are two words each. First every
 * kons word has the bits of the matching koffs word removed. Then, for the
 * kons words 0 and 1 in order and then the koffs words 0 and 1 in order, a
 * nonzero word is appended to batchBuffer (0x007dc280, 8-byte entries) when
 * nmBatchCommands (0x00485954) is below 512, then set to zero in either
 * case. The entry is:
 *
 *   halfword 2, halfword 0x1500 | n for kons or 0x1600 | n for koffs, word
 *
 * If the count is then nonzero: FlushCache(0); sceSdTransToIOP(batchBuffer,
 * batchIopBuffer, count * 8, 1); sceSdRemote(1, 0x80b0, batchIopBuffer, 0,
 * count) with the count read again; the count is set to zero. The IOP
 * address word is batchIopBuffer (0x00485950); the original stores it in
 * .sdata. The meanings of the command 2 and of 0x80b0 are not established.
 */
extern "C" {
extern unsigned char norrath_sound_key_on_words_view[];
extern unsigned char norrath_sound_key_off_words_view[];
extern unsigned char norrath_sound_batch_iop_buffer_word_view[];
extern unsigned char batchBuffer[];
extern int nmBatchCommands;
void FlushCache(int mode);
int sceSdTransToIOP(void* source, int destination, int size, int mode);
int sceSdRemote(int one, int command, int argument, int extra, int count);

static void append(unsigned tag, unsigned value) {
    if (nmBatchCommands < 512) {
        unsigned char* entry = batchBuffer + nmBatchCommands * 8;
        *reinterpret_cast<unsigned short*>(entry + 2) = static_cast<unsigned short>(tag);
        nmBatchCommands = nmBatchCommands + 1;
        *reinterpret_cast<unsigned short*>(entry) = 2;
        *reinterpret_cast<unsigned*>(entry + 4) = value;
    }
}

void soundFlushCommandBuffer__Fv() {
    unsigned* on = reinterpret_cast<unsigned*>(norrath_sound_key_on_words_view);
    unsigned* off = reinterpret_cast<unsigned*>(norrath_sound_key_off_words_view);
    const unsigned off_second = off[1];
    const unsigned off_first = off[0];
    const unsigned on_first = on[0];
    const unsigned on_second = on[1];
    on[0] = on_first & ~off_first;
    on[1] = on_second & ~off_second;
    for (unsigned n = 0; n < 2; ++n) {
        const unsigned word = on[n];
        if (word != 0) {
            append(n | 0x1500u, word);
            on[n] = 0;
        }
    }
    for (unsigned n = 0; n < 2; ++n) {
        const unsigned word = off[n];
        if (word != 0) {
            append(n | 0x1600u, word);
            off[n] = 0;
        }
    }
    if (nmBatchCommands == 0) return;
    FlushCache(0);
    const int iop = *reinterpret_cast<int*>(norrath_sound_batch_iop_buffer_word_view);
    sceSdTransToIOP(batchBuffer, iop, nmBatchCommands * 8, 1);
    sceSdRemote(1, 0x80b0, *reinterpret_cast<int*>(norrath_sound_batch_iop_buffer_word_view), 0,
                nmBatchCommands);
    nmBatchCommands = 0;
}
}
