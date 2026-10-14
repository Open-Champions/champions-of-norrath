static int sends, handle, size;
static unsigned char bytes[2];
static int local_mask;
static int queries;
static int shrink_after = -1;

extern "C" {
signed char netNetworkMode;
int nmPlayers;
int hudMsgHandle;
int netPlayerIsLocal__Fi(int player) {
    queries += 1;
    if (queries == shrink_after) nmPlayers = 1;
    return (local_mask >> player) & 1;
}
void netSendMessage__FiPUci(int h, unsigned char* b, int n) {
    sends += 1;
    handle = h;
    size = n;
    bytes[0] = b[0];
    bytes[1] = b[1];
}
void msgHudPlayerExit__Fv();
}

static void reset(int mode, int players, int mask) {
    netNetworkMode = static_cast<signed char>(mode);
    nmPlayers = players;
    local_mask = mask;
    hudMsgHandle = 77;
    sends = 0;
    queries = 0;
    shrink_after = -1;
}

int main() {
    reset(0, 4, 0xf);
    msgHudPlayerExit__Fv();
    if (sends != 0 || queries != 0) return 1;
    reset(1, 1, 0xf);
    msgHudPlayerExit__Fv();
    if (sends != 0 || queries != 0) return 2;
    reset(-1, 4, 0x4);
    msgHudPlayerExit__Fv();
    if (sends != 1 || queries != 3 || size != 2 || handle != 77) return 3;
    if (bytes[0] != 5 || bytes[1] != 2) return 4;
    reset(1, 3, 0);
    msgHudPlayerExit__Fv();
    if (sends != 1 || queries != 3 || bytes[1] != 3) return 5;
    reset(1, 2, 0x1);
    msgHudPlayerExit__Fv();
    if (queries != 1 || bytes[1] != 0) return 6;
    // nmPlayers is read again on every pass.
    reset(1, 4, 0);
    shrink_after = 2;
    msgHudPlayerExit__Fv();
    if (queries != 2 || bytes[1] != 2) return 7;
    return 0;
}
