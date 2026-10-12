// Observed sceSifRebootIop at 0x002dd530, 272 bytes. A path whose length is
// 70 or more prints too long parameter '%s' and returns 0. The length is the
// inline scan plus 11, compared with 81. A shorter path, including an empty
// one, calls sceSifInitRpc(0) and sceSifExitRpc, copies rom0:UDNL  from
// 0x0047b8f0, appends the path, and returns sceSifResetIop of that text with
// a second argument of 0. The prefix has no symbol. The stack frame is 128
// bytes; the text uses the first 80.
extern "C" int scePrintf(const char* text, ...);
extern "C" void sceSifInitRpc(int mode);
extern "C" void sceSifExitRpc();
extern "C" int sceSifResetIop(const char* command, int argument);

extern "C" int sceSifRebootIop(const char* image) {
    const char* end = image;
    if (*end != 0) {
        do {
            ++end;
        } while (*end != 0);
    }
    const unsigned span = static_cast<unsigned>(end - image) + 11u;
    if (span >= 81u) {
        scePrintf("too long parameter '%s'\n", image);
        return 0;
    }
    sceSifInitRpc(0);
    sceSifExitRpc();
    char command[80];
    const char prefix[] = "rom0:UDNL ";
    int used = 0;
    for (int index = 0; prefix[index] != 0; ++index)
        command[used++] = prefix[index];
    for (int index = 0;; ++index) {
        const char byte = image[index];
        command[used++] = byte;
        if (byte == 0)
            break;
    }
    return sceSifResetIop(command, 0);
}
