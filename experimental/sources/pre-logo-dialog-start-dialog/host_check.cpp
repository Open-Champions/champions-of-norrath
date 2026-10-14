// Host contract for DLG_StartDialog. Not an EE plan input.
#include "start_dialog.cpp"

extern "C" unsigned char camera_bytes[8] = {};
extern "C" unsigned char* gameCam = camera_bytes;
extern "C" int DLG_StartWithSwoopDone = 4;
extern "C" int activePlayer = 0;
extern "C" unsigned curDlg = 0;
extern "C" unsigned char worldCameraPos[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
extern "C" int dialogMsgHandle = 19;

static int updates;
static int local_calls;
static int local_player;
static int yaw_calls;
static int pitch_calls;
static int sends;
static int send_handle;
static int send_length;
static unsigned char send_bytes[64];
static int length_calls;

extern "C" void Update__6Camera(unsigned char* camera) {
    ++updates;
    if (camera != gameCam) updates = -1;
}

extern "C" int netPlayerIsLocal__Fi(int player) {
    ++local_calls;
    local_player = player;
    return player == 0x105 ? 1 : 0;
}

extern "C" unsigned int strlen(const char* text) {
    ++length_calls;
    unsigned count = 0;
    while (text[count] != 0) ++count;
    return count;
}

extern "C" int GetYaw__6Camera(unsigned char* camera) {
    ++yaw_calls;
    if (camera != gameCam) return 0;
    return 0x12345;
}

extern "C" int GetPitch__6Camera(unsigned char* camera) {
    ++pitch_calls;
    if (camera != gameCam) return 0;
    return -2;
}

extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int length) {
    ++sends;
    send_handle = handle;
    send_length = length;
    for (int i = 0; i < length && i < 64; ++i) send_bytes[i] = bytes[i];
}

static int expect_prefix(const unsigned char* want, int count) {
    for (int i = 0; i < count; ++i) if (send_bytes[i] != want[i]) return 0;
    return 1;
}

int main() {
    char hi[] = "Hi";
    DLG_StartWithSwoopDone = 4;
    updates = 0;
    sends = 0;
    activePlayer = 1;
    curDlg = 0;
    DLG_StartDialog__FPcP9DlgPacketii(hi, reinterpret_cast<void*>(static_cast<unsigned long>(0x11223344u)), 1, 2);
    if (updates != 1 || DLG_StartWithSwoopDone != 0 || local_player != 1 || sends != 0) return 1;

    activePlayer = 0x105;
    curDlg = 3;
    DLG_StartWithSwoopDone = 4;
    sends = 0;
    DLG_StartDialog__FPcP9DlgPacketii(hi, nullptr, 1, 2);
    if (sends != 0 || DLG_StartWithSwoopDone != 0) return 2;

    curDlg = 0;
    length_calls = 0;
    yaw_calls = 0;
    pitch_calls = 0;
    void* packet = reinterpret_cast<void*>(static_cast<unsigned long>(0x11223344u));
    DLG_StartDialog__FPcP9DlgPacketii(hi, packet, 0x201, -1);
    if (sends != 1 || send_handle != 19 || length_calls != 1 || yaw_calls != 1 || pitch_calls != 1) return 3;
    const unsigned char hi_head[] = {0, 2, 'H', 'i', 0x44, 0x33, 0x22, 0x11, 0x05, 0x01, 0xff, 0};
    if (send_length != 28 || !expect_prefix(hi_head, 12)) return 4;
    for (int i = 0; i < 12; ++i) if (send_bytes[12 + i] != static_cast<unsigned char>(i + 1)) return 5;
    if (send_bytes[24] != 0x45 || send_bytes[25] != 0x23 || send_bytes[26] != 0xfe || send_bytes[27] != 0xff) return 6;

    char empty[] = "";
    DLG_StartDialog__FPcP9DlgPacketii(empty, packet, 0, 0);
    const unsigned char empty_head[] = {0, 0, 0xff, 0xff, 0x44, 0x33, 0x22, 0x11};
    if (send_length != 28 || !expect_prefix(empty_head, 8)) return 7;

    char abc[] = "Abc";
    DLG_StartDialog__FPcP9DlgPacketii(abc, packet, 0, 0);
    const unsigned char abc_head[] = {0, 3, 'A', 'b', 'c', 0xff, 0xff, 0xff, 0x44, 0x33, 0x22, 0x11};
    if (send_length != 32 || !expect_prefix(abc_head, 12)) return 8;
    return 0;
}
