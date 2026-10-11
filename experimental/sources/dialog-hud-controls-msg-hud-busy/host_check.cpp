extern "C" {
unsigned char netNetworkMode;
int hudMsgHandle;
void msgHudSetPlayerBusy__Fcc(int player, int busy);
}

static int prints;
static int print_player;
static int print_busy;
static const char* print_text;
static int sends;
static int send_handle;
static int send_size;
static unsigned char send_bytes[3];

extern "C" void scePrintf(const char* text, int player, int busy) {
    prints += 1;
    print_text = text;
    print_player = player;
    print_busy = busy;
}

extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size) {
    sends += 1;
    send_handle = handle;
    send_size = size;
    for (int i = 0; i < 3 && i < size; ++i) send_bytes[i] = bytes[i];
}

int main() {
    netNetworkMode = 0;
    hudMsgHandle = 42;
    msgHudSetPlayerBusy__Fcc(1, 1);
    if (prints != 0 || sends != 0) return 1;

    netNetworkMode = 0xff;
    msgHudSetPlayerBusy__Fcc(0x180, 0x1ff);
    if (prints != 1 || sends != 1) return 2;
    if (print_player != -128 || print_busy != -1) return 3;
    if (send_handle != 42 || send_size != 3) return 4;
    if (send_bytes[0] != 2 || send_bytes[1] != 0x80 || send_bytes[2] != 0xff) return 5;
    const char* want = "MSG SEND:  Set player %d to busy=%d\n";
    for (int i = 0;; ++i) {
        if (print_text[i] != want[i]) return 6;
        if (want[i] == 0) break;
    }
    return 0;
}
