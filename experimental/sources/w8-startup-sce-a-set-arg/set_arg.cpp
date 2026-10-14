/* Observed SetArg at 0x002ddb10, 280 bytes. _kExecArg (0x00445d48) holds the
 * address of a buffer. The function returns that address plus 64, and uses a
 * running cursor that starts there. The pair in SysEntry (0x004451f8) goes to
 * the local setup service (0x002ddb00, service 116), which installs the
 * byte-copy handler for service 90. The incoming argument count is limited to
 * 15 (a count below 16 is kept; a negative count runs no loop). The cursor
 * word is then copied to buffer offset 0 through the local Copy
 * (0x002ddac0, service 90) and the path is copied, with its terminator, to the
 * cursor, which advances by its length plus one. Each of the counted argument
 * strings gets the same treatment: the cursor word is copied to the next
 * buffer word (offsets 4, 8, ...), the string is copied to the cursor, and the
 * cursor advances. The count itself is not stored in the buffer. The original
 * names for the three local helpers are Copy, setup and SysEntry; they are
 * published as kernel_setarg_* here. What the services do is unknown. Compiled
 * output is not claimed to match the original.
 */
extern "C" {
extern unsigned _kExecArg;
extern unsigned kernel_setarg_sysentry[2];
unsigned long kernel_setarg_setup(unsigned service, unsigned handler);
unsigned long kernel_setarg_copy(void* destination, const void* source, unsigned count);
unsigned strlen(const char* text);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_exec_buffer();
#define NORRATH_EXEC_BUFFER() norrath_host_exec_buffer()
#else
#define NORRATH_EXEC_BUFFER() reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(_kExecArg))
#endif

extern "C" unsigned SetArg(const char* path, int argc, char** argv) {
    char* buffer = NORRATH_EXEC_BUFFER();
    char* const filename = buffer + 64;
    char* cursor = filename;
    kernel_setarg_setup(kernel_setarg_sysentry[0], kernel_setarg_sysentry[1]);
    if (argc >= 16) argc = 15;
    kernel_setarg_copy(buffer, &cursor, 4);
    buffer += 4;
    unsigned length = strlen(path) + 1;
    kernel_setarg_copy(cursor, path, length);
    cursor += length;
    for (int index = 0; index < argc; ++index) {
        kernel_setarg_copy(buffer, &cursor, 4);
        buffer += 4;
        length = strlen(argv[index]) + 1;
        kernel_setarg_copy(cursor, argv[index], length);
        cursor += length;
    }
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(filename));
}
