/* soundStartStream__FiPciiiiii at 0x0024a660 is 380 bytes. Arguments after
 * the stream number, in order: file name, flags, offset, length, and three
 * more words (the last two registers and two stack words in the original).
 *
 * `stream` (0x007dd290) holds 452-byte records. Record `index` has a count
 * at offset 448 and 48-byte queue blocks from offset 64. With a count of 8
 * or more nothing happens. Otherwise cdDirectoryLookup__FPcPiT1i is called
 * with the name, two output words, and 0; if it returns zero nothing
 * happens. The second output word (size), rounded up to a multiple of 16,
 * is the default length.
 *
 * Block `count` is then filled (offsets relative to the block start):
 *
 *   +0   name, copied with strcpy
 *   +16  offset argument      +20  length
 *   +24  offset argument      +28  length
 *   +32  sixth argument       +36  seventh argument
 *   +40  eighth argument      +44  flags argument
 *
 * The length is the length argument when it is not negative, otherwise the
 * rounded size. The count is re-read and incremented, and
 * soundPrintStream__Fi is called with the stream number. Meanings of the
 * paired offset and length words, and of the last three arguments, are not
 * established by this function. The first output word of the lookup is
 * ignored.
 */
extern "C" {
extern unsigned char stream[];
int cdDirectoryLookup__FPcPiT1i(char* name, int* first, int* second, int flag);
char* strcpy(char* destination, const char* source);
void soundPrintStream__Fi(int index);

void soundStartStream__FiPciiiiii(int index, char* name, int flags, int offset, int length,
                                  int sixth, int seventh, int eighth) {
    unsigned char* record = stream + index * 452;
    volatile int* count = reinterpret_cast<volatile int*>(record + 448);
    const int queued = *count;
    int found_first;
    int found_size;
    if (queued >= 8) return;
    if (!cdDirectoryLookup__FPcPiT1i(name, &found_first, &found_size, 0)) return;
    found_size = (found_size + 15) & -16;
    unsigned char* block = record + 64 + queued * 48;
    strcpy(reinterpret_cast<char*>(block), name);
    const int used = length >= 0 ? length : found_size;
    int* words = reinterpret_cast<int*>(block);
    words[4] = offset;
    words[5] = used;
    words[6] = offset;
    words[7] = used;
    words[8] = sixth;
    words[9] = seventh;
    words[10] = eighth;
    words[11] = flags;
    *count = *count + 1;
    soundPrintStream__Fi(index);
}
}
