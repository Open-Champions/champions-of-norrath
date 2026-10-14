// Observed picAdd__4itemPc at 0x00100948, 216 bytes, 48-byte frame. _4item$picList is an array of 64
// records of 192 bytes; a record is free when its first byte is zero.
//   Loop: scan the 64 records in order for the first free one.
//   - None free: call picKickOneOut__4itemv(). A zero result returns immediately with nothing
//     added; a nonzero result starts the scan again from record 0.
//   - Free record found: strcpy(record, name) (no length limit), then the halfword at record + 64 is
//     set to -1, the word at record + 124 is set to drawFrameNm, and the byte _4item$picsToLoad is
//     set to 1.
// Nothing is returned (v0 is left over from the last operation).
// Inferred: queues a picture name for loading; a -1 halfword marks "still loading" (getPic reads it
// that way), the word at +124 is the last-used frame stamp, and the flag asks a loader to run.
// Unknown: whether picKickOneOut's result is a count or a flag (only zero/nonzero is used), and the
// bytes after the name inside the record.
extern "C" {
extern unsigned char _4item$picList[];
extern signed char _4item$picsToLoad;
extern int drawFrameNm;
int picKickOneOut__4itemv();
char* strcpy(char* destination, const char* source);

void picAdd__4itemPc(char* name) {
    for (;;) {
        int index = 0;
        while (index < 64 && _4item$picList[index * 192] != 0) ++index;
        if (index != 64) {
            unsigned char* record = _4item$picList + index * 192;
            strcpy(reinterpret_cast<char*>(record), name);
            *reinterpret_cast<short*>(record + 64) = -1;
            *reinterpret_cast<int*>(record + 124) = drawFrameNm;
            _4item$picsToLoad = 1;
            return;
        }
        if (picKickOneOut__4itemv() == 0) return;
    }
}
}
