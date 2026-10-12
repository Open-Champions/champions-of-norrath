// The authored file declares libc strcpy/strlen; rename them for the host so
// they do not collide with the host headers.
#define strcpy hostcheck_strcpy
#define strlen hostcheck_strlen
#include "hud_net_level_warp.cpp"
#undef strcpy
#undef strlen
#include <cstring>
int hudMsgHandle;
static int calls, seenHandle, seenSize;
static unsigned char seenBytes[80];
extern "C" char* hostcheck_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
extern "C" unsigned int hostcheck_strlen(const char* t) { return std::strlen(t); }
extern "C" int netSendMessage__FiPUci(int handle, unsigned char* data, int size) {
    ++calls;
    seenHandle = handle;
    seenSize = size;
    std::memcpy(seenBytes, data, size);
    return 0;
}
int main() {
    hudMsgHandle = 5;
    char name[] = "tunnel";
    hudNetLevelWarp__FPc(name);
    if (calls != 1 || seenHandle != 5) return 1;
    if (seenSize != 8) return 2;
    if (seenBytes[0] != 6 || std::memcmp(seenBytes + 1, "tunnel", 7) != 0) return 3;
    return 0;
}
