// Host check for msgHudSavePoint__FP9Savepoint. Not an EE plan input.
// The pointer slot is sizeof(void*) wide on the host and 4 bytes on the target.
#include "msg_hud_save_point.cpp"
extern "C" int hudMsgHandle = 0;
#include <cstring>
static int calls, seen_handle, seen_size;
static unsigned char seen[16];
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size) {
    ++calls;
    seen_handle = handle;
    seen_size = size;
    std::memcpy(seen, bytes, 4 + sizeof(void*));
}
int main() {
    int marker;
    hudMsgHandle = 41;
    msgHudSavePoint__FP9Savepoint(&marker);
    if (calls != 1 || seen_handle != 41 || seen_size != 8) return 1;
    for (int i = 0; i < 4; ++i) if (seen[i] != 4) return 2;
    void* stored;
    std::memcpy(&stored, seen + 4, sizeof stored);
    if (stored != &marker) return 3;
    msgHudSavePoint__FP9Savepoint(nullptr);
    std::memcpy(&stored, seen + 4, sizeof stored);
    if (calls != 2 || stored != nullptr) return 4;
    return 0;
}
