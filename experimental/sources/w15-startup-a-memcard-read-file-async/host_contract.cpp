#include "read_file_async.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

int sMemCardInitialized, sAsyncFd, sAsyncMode;
unsigned char sIconDefinition[16], sListIcon[16];
char sProductTitle[] = "title";
char sProductId[] = "PID";
static int titled, iconed, icon_size, opens, syncs, reads, sync_result, order[8], norder;
static int read_fd, read_size, mode_at_open;
static void* read_buffer;
static char opened_path[64];

void SetTitle__16VIIconDefinitionPCc(void*, const char*) { ++titled; order[norder++] = 1; }
void SetIcon__16VIIconDefinitionPvi(void*, void* icon, int size) { ++iconed; icon_size = size; (void)icon; }
int sprintf(char* buffer, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    const char* a = __builtin_va_arg(ap, const char*);
    const char* b = __builtin_va_arg(ap, const char*);
    __builtin_va_end(ap);
    if (std::strcmp(format, "/%s/%s") != 0) std::abort();
    return std::snprintf(buffer, 256, "/%s/%s", a, b);
}
int sceMcOpen(int port, int slot, const char* path, int mode) {
    ++opens;
    mode_at_open = sAsyncMode;
    std::strcpy(opened_path, path);
    return port + slot + mode;
}
int sceMcSync(int mode, int*, int* result) { ++syncs; (void)mode; *result = sync_result; return 1; }
int sceMcRead(int fd, void* buffer, int size) { ++reads; read_fd = fd; read_buffer = buffer; read_size = size; return 0; }

int main() {
    char buf[8];
    sAsyncMode = 5;
    sync_result = 3;
    if (VIMemCardReadFileAsync__FPCcPvi("save.dat", buf, 40) != 0) return 1;
    if (sAsyncMode != 0 || titled != 1 || iconed != 1 || icon_size != 0x1cdd8 || sMemCardInitialized != 1) return 2;
    if (sAsyncFd != 3 || reads != 1 || read_fd != 3 || read_buffer != buf || read_size != 40) return 3;
    if (std::strcmp(opened_path, "/PID/save.dat") != 0 || opens != 1 || syncs != 1) return 4;
    sAsyncFd = 99;
    sync_result = -4;
    if (VIMemCardReadFileAsync__FPCcPvi("x", buf, 1) != -2 || reads != 1 || sAsyncFd != 99 || titled != 1) return 5;
    sync_result = -1;
    if (VIMemCardReadFileAsync__FPCcPvi("x", buf, 1) != -1 || reads != 1 || sAsyncFd != 99) return 6;
    sync_result = -5;
    if (VIMemCardReadFileAsync__FPCcPvi("x", buf, 1) != -1) return 7;
    sync_result = 0;
    sAsyncMode = 4;
    if (VIMemCardReadFileAsync__FPCcPvi("y", buf, 2) != 0 || sAsyncFd != 0 || reads != 2 || mode_at_open != 0) return 8;
    std::puts("VIMemCardReadFileAsync contract passed");
    return 0;
}
