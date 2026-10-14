// Observed: BufferFlush__6VIFile at 0x010cb0a8 is 200 bytes. File object fields used: the
// descriptor int at +0, the 64-bit position at +24, three 64-bit values at +136 (buffer base
// offset), +144 (dirty start) and +152 (dirty end), a 64-bit value at +176 and the 32-bit
// buffer address at +188. Steps:
//   1. If the 64-bit value at +176 is zero, or the dirty start equals the dirty end, return 0.
//   2. length = (int)(end - start); target = base + start (64-bit). If target differs from the
//      position at +24 it calls sceLseek(fd, (int)target, 0), stores the (sign-extended)
//      result in the position at +24, and returns -1 if the result differs from target.
//   3. It calls sceWrite(fd, buffer + (int)start, length), where buffer is the word at +188 and
//      (int)start is the low 32 bits of the dirty start. It adds the written count (sign-
//      extended) to the position at +24 even when it differs from length, and returns -1 then.
//   4. On a complete write it stores zero in the dirty end (+152) and the dirty start (+144)
//      and returns 0.
// Inferred: +176 is a "write-enabled / buffer dirty" flag, +144..+152 the dirty byte range
// inside the file buffer, +136 the file offset of the buffer's first byte. Flushing writes
// the dirty range at the right file offset.
// Unknown: the true field names, and whether +176 is a flag or a count.
extern "C" {
int sceLseek(int fd, int offset, int whence);
int sceWrite(int fd, const void* data, int length);

#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

int BufferFlush__6VIFile(unsigned char* file) {
    if (*reinterpret_cast<long long*>(file + 176) == 0) {
        return 0;
    }
    long long start = *reinterpret_cast<long long*>(file + 144);
    long long end = *reinterpret_cast<long long*>(file + 152);
    if (start == end) {
        return 0;
    }
    int length = static_cast<int>(end - start);
    long long target = *reinterpret_cast<long long*>(file + 136) + start;
    if (target != *reinterpret_cast<long long*>(file + 24)) {
        long long where = sceLseek(*reinterpret_cast<int*>(file), static_cast<int>(target), 0);
        *reinterpret_cast<long long*>(file + 24) = where;
        if (target != where) {
            return -1;
        }
    }
    int written = sceWrite(*reinterpret_cast<int*>(file),
                           NORRATH_WORD_ADDRESS(*reinterpret_cast<int*>(file + 188) + *reinterpret_cast<int*>(file + 144)),
                           length);
    *reinterpret_cast<long long*>(file + 24) += written;
    if (written != length) {
        return -1;
    }
    *reinterpret_cast<long long*>(file + 152) = 0;
    *reinterpret_cast<long long*>(file + 144) = 0;
    return 0;
}
}
