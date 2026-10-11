// Observed DLG_StartDialog__FPcP9DlgPacketii at 0x001d2578, 364 bytes.
// Update runs on gameCam first. DLG_StartWithSwoopDone is then stored as 0.
// A zero result from netPlayerIsLocal(activePlayer) returns. A nonzero
// curDlg word returns. Otherwise the stack message is: a zero byte, the low
// byte of strlen, the name bytes without the terminator, then 0xff bytes
// until the cursor is four-byte aligned. The packet pointer follows as four
// little-endian bytes, then the low byte of activePlayer, the low bytes of
// the two integers, a zero byte, the 12 bytes at worldCameraPos, and the
// low halfwords from GetYaw and GetPitch. netSendMessage receives
// dialogMsgHandle, that message, and its length. Those symbols are not
// defined here.
extern "C" unsigned char* gameCam;
extern "C" void Update__6Camera(unsigned char* camera);
extern "C" int DLG_StartWithSwoopDone;
extern "C" int activePlayer;
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" unsigned curDlg;
extern "C" unsigned int strlen(const char* text);
extern "C" unsigned char worldCameraPos[12];
extern "C" int GetYaw__6Camera(unsigned char* camera);
extern "C" int GetPitch__6Camera(unsigned char* camera);
extern "C" int dialogMsgHandle;
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int length);

static void store_word(unsigned char*& cursor, unsigned value) {
    cursor[0] = static_cast<unsigned char>(value);
    cursor[1] = static_cast<unsigned char>(value >> 8);
    cursor[2] = static_cast<unsigned char>(value >> 16);
    cursor[3] = static_cast<unsigned char>(value >> 24);
    cursor += 4;
}

static void store_half(unsigned char*& cursor, unsigned value) {
    cursor[0] = static_cast<unsigned char>(value);
    cursor[1] = static_cast<unsigned char>(value >> 8);
    cursor += 2;
}

extern "C" void DLG_StartDialog__FPcP9DlgPacketii(char* name, void* packet, int first, int second) {
    Update__6Camera(gameCam);
    DLG_StartWithSwoopDone = 0;
    if (netPlayerIsLocal__Fi(activePlayer) == 0) return;
    if (curDlg != 0u) return;
    alignas(4) unsigned char message[256];
    unsigned char* cursor = message;
    *cursor++ = 0;
    *cursor++ = static_cast<unsigned char>(strlen(name));
    unsigned index = 0;
    while (name[index] != 0) {
        *cursor++ = static_cast<unsigned char>(name[index]);
        ++index;
    }
    while (((cursor - message) & 3) != 0) *cursor++ = 0xff;
    store_word(cursor, static_cast<unsigned>(reinterpret_cast<unsigned long>(packet)));
    *cursor++ = static_cast<unsigned char>(activePlayer);
    *cursor++ = static_cast<unsigned char>(first);
    *cursor++ = static_cast<unsigned char>(second);
    *cursor++ = 0;
    for (int i = 0; i < 12; ++i) *cursor++ = worldCameraPos[i];
    store_half(cursor, static_cast<unsigned>(GetYaw__6Camera(gameCam)));
    store_half(cursor, static_cast<unsigned>(GetPitch__6Camera(gameCam)));
    netSendMessage__FiPUci(dialogMsgHandle, message, static_cast<int>(cursor - message));
}
