// Observed loadDirectory__FP9directoryii at 0x00164c70, 488 bytes, a file-local
// symbol in the image. Arguments: a directory table, a first sector and a number
// of sectors. The table is rows of 32 bytes with a row count word at offset 24576
// (see the archive directory views); the count is set to zero first.
// For each sector, 0 to blocks - 1: sceCdRead(sector + index, 1, buffer, CDmode)
// into a 256-byte aligned stack buffer, then sceCdSync(0). A first byte of zero
// ends the sector's use. Otherwise it is a sequence of variable-length records,
// each starting with its length byte (a zero length ends the sector). In a record
// the name length is the byte at offset 32 and the name starts at offset 33; the
// 32-bit little-endian words at offsets 2 and 10 are the extent and the size.
// Each record becomes the next row: the name is copied to a local 64-byte
// buffer, ended with a zero byte, and strcpy'd to row offset 0; the word at row
// offset 16 receives the extent and the word at row offset 20 the size; the
// count increases by one.
// Inferred: the record layout is the ISO 9660 directory record, which these
// offsets agree with. The sector reads are one sector at a time into a 2048-byte
// buffer.
// Unknown: whether names of 64 bytes or more can occur; the image does not
// bound the copy.
extern "C" {
extern unsigned char CDmode[4];
int sceCdRead(unsigned sector, unsigned count, void* buffer, void* mode);
int sceCdSync(int mode);
char* strcpy(char* destination, const char* source);

static unsigned little_word(const unsigned char* bytes) {
    return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}

void loadDirectory__FP9directoryii(unsigned char* directory, int sector, int blocks) {
    alignas(256) unsigned char buffer[2048];
    char name[64];
    unsigned* const count = reinterpret_cast<unsigned*>(directory + 24576);
    *count = 0;
    for (int index = 0; index < blocks; ++index) {
        sceCdRead(sector + index, 1, buffer, CDmode);
        sceCdSync(0);
        if (buffer[0] == 0) continue;
        int position = 0;
        const unsigned char* record = buffer;
        do {
            const int length = record[32];
            int copied = 0;
            for (int at = position + 33; at <= position + 32 + length; ++at) name[copied++] = static_cast<char>(buffer[at]);
            name[copied] = 0;
            const unsigned extent = little_word(buffer + position + 2);
            const unsigned size = little_word(buffer + position + 10);
            strcpy(reinterpret_cast<char*>(directory + *count * 32), name);
            *reinterpret_cast<unsigned*>(directory + *count * 32 + 16) = extent;
            *reinterpret_cast<unsigned*>(directory + 4 + *count * 32 + 16) = size;
            *count = *count + 1;
            position += record[0];
            record = buffer + position;
        } while (record[0] != 0);
    }
}
}
