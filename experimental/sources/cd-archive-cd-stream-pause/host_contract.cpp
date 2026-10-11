#include <cstdio>
#include <cstring>
extern "C" int SCE_CD_debug = 0;
extern "C" int stm_status = 4;
extern "C" unsigned char dum_mode[4] = {};
static int prints;
static int command;
static void* mode_pointer;
static const char* printed;
extern "C" int norrath_sce_printf(const char* format, ...) {
    ++prints;
    printed = format;
    return 0;
}
extern "C" int sceCdStream(int argument, int count, void* buffer, int stream_command, void* mode) {
    if (argument != 0 || count != 0 || buffer != nullptr) return -1;
    command = stream_command;
    mode_pointer = mode;
    return 33;
}
extern "C" int norrath_archive_cd_stream_pause();
int main() {
    if (norrath_archive_cd_stream_pause() != 33 || stm_status != 0 || prints != 0) return 1;
    if (command != 7 || mode_pointer != dum_mode) return 2;
    SCE_CD_debug = 2;
    stm_status = 1;
    if (norrath_archive_cd_stream_pause() != 33 || prints != 1 || stm_status != 0) return 3;
    if (std::strcmp(printed, "sceCdStPause call\n") != 0) return 4;
    std::puts("stream pause contract passed");
    return 0;
}
