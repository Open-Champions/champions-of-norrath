/* Observed mceStorePwd at 0x002e1a00, 140 bytes: the completion function of
 * sceMcChdir. The argument is the caller's result pointer (null allowed).
 * With a non-null pointer, the text in currentDir (0x008e7100), read through
 * its uncached alias (address OR 0x20000000), is copied to the pointer: its
 * length is measured with strlen, a length of 1024 or more is replaced by
 * 1023, and the characters are copied with memcpy followed by a terminating
 * zero byte at that offset. strlen runs a second time when the first length is
 * below 1024 (the original evaluates it in both arms of one conditional).
 * In every case iSignalSema is then called with semaid_timerfunc
 * (0x00446078) as a tail call and its result is returned.
 */
extern "C" {
extern int semaid_timerfunc;
extern unsigned char currentDir[];

__SIZE_TYPE__ strlen(const char* text);
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);
int iSignalSema(int semaphore);

#if defined(NORRATH_HOST_CONTRACT)
static const char* uncached_current_dir() { return reinterpret_cast<const char*>(currentDir); }
#else
static const char* uncached_current_dir() {
    return reinterpret_cast<const char*>(
        static_cast<__UINTPTR_TYPE__>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(currentDir)) | 0x20000000u));
}
#endif

int mceStorePwd(void* destination) {
    if (destination != nullptr) {
        __SIZE_TYPE__ length;
        if (strlen(uncached_current_dir()) < 1024)
            length = strlen(uncached_current_dir());
        else
            length = 1023;
        memcpy(destination, uncached_current_dir(), length);
        static_cast<char*>(destination)[length] = 0;
    }
    return iSignalSema(semaid_timerfunc);
}
}
