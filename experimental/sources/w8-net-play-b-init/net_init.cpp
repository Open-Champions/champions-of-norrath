// Observed netInit at 0x00131170, 1628 bytes. It does nothing when the byte
// netInitState is nonzero. Otherwise, in order:
//  - trueNetFrameNm, localNetFrameNm and localNetSentFrameNm become 1,
//    netInitState becomes 1, totallyDisconnected and netTrouble become 0,
//    netKickedOut becomes 0, and the 8-byte object netCameraYaw, the 48-byte
//    padBlank and the 32-byte netLocalPadState are cleared with memset;
//    localPredictedPlayer and logChecksum become 0.
//  - Four 80-byte stack templates are built, one per compression object
//    (compImmediateCompressor, compImmediateDecompressor, compLocalCompressor,
//    compLocalDecompressor, in that order). Each template is five 16-byte
//    memset calls on consecutive chunks, then the words at byte 8, 24, 40 and
//    56 are set to the float 128.0 and the word at byte 72 to 0. The first 76
//    bytes of the template are then copied to the object (four-byte aligned
//    copy).
//  - kickState (8 bytes) is cleared, msgBufferSent becomes 0,
//    localPlayerPadState (8 bytes) is cleared, derivedFrameNm becomes -10,
//    sentStatePacketFrameNm and noAdvanceTime become 0, currentTime and
//    lastPingTime become 0, sceSifMInitRpc(0) is called, isModem becomes 0.
//  - sceLibnetInitialize(cd, 2048, 8192, 32) is called. A stack word starts
//    at -1000. While it is still -1000, sceInetGetInterfaceList(cd, g_net_buf,
//    &word, 1) runs; a result other than 1 sets the word to -100. Unless the
//    word is -100 (it is read again after the call), a six-argument
//    sceInetInterfaceControl(cd, g_net_buf, word, 9, myaddr, 16) is made.
//  - sceSifSearchModuleByName("INET_SPEED_UART_driver") sets isModem to 1 when
//    the result is not negative.
// Inferred: the 128.0 floats and the four-object layout are an initialised
// default for the four compression objects; names are the image's own.
// Unknown: the layout of each compression object, whether the image's
// interface-control call really takes six arguments (it passes six words),
// what the sceInet calls do on the target, and the fields of netCameraYaw,
// padBlank and netLocalPadState beyond their sizes.
struct Slot {
    unsigned first;
    unsigned second;
    float value;
    unsigned fourth;
};
struct Compression {
    Slot slot[4];
    unsigned tail[3];
};
struct StackTemplate {
    Compression object;
    unsigned spare;
};

#define BUILD_COMPRESSION(target) \
    do { \
        StackTemplate stack; \
        memset(&stack.object.slot[0], 0, 16); \
        memset(&stack.object.slot[1], 0, 16); \
        memset(&stack.object.slot[2], 0, 16); \
        memset(&stack.object.slot[3], 0, 16); \
        memset(&stack.object.tail, 0, 16); \
        stack.object.slot[0].value = 128.0f; \
        stack.object.slot[1].value = 128.0f; \
        stack.object.slot[2].value = 128.0f; \
        stack.object.slot[3].value = 128.0f; \
        stack.object.tail[2] = 0; \
        target = stack.object; \
    } while (0)

extern "C" {
extern unsigned char netInitState;
extern int trueNetFrameNm;
extern int localNetFrameNm;
extern int localNetSentFrameNm;
extern unsigned char totallyDisconnected;
extern int netTrouble;
extern unsigned char netKickedOut;
extern unsigned char netCameraYaw[8];
extern unsigned char padBlank[48];
extern unsigned char netLocalPadState[32];
extern int localPredictedPlayer;
extern int logChecksum;
extern Compression compImmediateCompressor;
extern Compression compImmediateDecompressor;
extern Compression compLocalCompressor;
extern Compression compLocalDecompressor;
extern unsigned kickState[2];
extern int msgBufferSent;
extern unsigned char localPlayerPadState[8];
extern int derivedFrameNm;
extern int sentStatePacketFrameNm;
extern int noAdvanceTime;
extern int currentTime;
extern int lastPingTime;
extern unsigned char isModem;
extern unsigned char cd[];
extern unsigned char g_net_buf[];
extern unsigned char myaddr[];
void* memset(void* destination, int value, unsigned long length);
void sceSifMInitRpc(int mode);
int sceLibnetInitialize(void* object, int first, int second, int third);
int sceInetGetInterfaceList(void* object, void* buffer, int* list, int count);
int sceInetInterfaceControl(void* object, void* buffer, int identifier, int code, void* option, int length);
int sceSifSearchModuleByName(char* name);
}


extern "C" void netInit__Fv() {
    if (netInitState != 0) return;
    trueNetFrameNm = 1;
    localNetFrameNm = 1;
    localNetSentFrameNm = 1;
    netInitState = 1;
    totallyDisconnected = 0;
    netTrouble = 0;
    netKickedOut = 0;
    memset(netCameraYaw, 0, 8);
    memset(padBlank, 0, 48);
    memset(netLocalPadState, 0, 32);
    localPredictedPlayer = 0;
    logChecksum = 0;
    BUILD_COMPRESSION(compImmediateCompressor);
    BUILD_COMPRESSION(compImmediateDecompressor);
    BUILD_COMPRESSION(compLocalCompressor);
    BUILD_COMPRESSION(compLocalDecompressor);
    memset(kickState, 0, 8);
    msgBufferSent = 0;
    memset(localPlayerPadState, 0, 8);
    derivedFrameNm = -10;
    sentStatePacketFrameNm = 0;
    noAdvanceTime = 0;
    int word = -1000;
    currentTime = 0;
    lastPingTime = 0;
    sceSifMInitRpc(0);
    isModem = 0;
    sceLibnetInitialize(cd, 2048, 8192, 32);
    if (word == -1000) {
        if (sceInetGetInterfaceList(cd, g_net_buf, &word, 1) != 1) word = -100;
    }
    if (word != -100)
        sceInetInterfaceControl(cd, g_net_buf, word, 9, myaddr, 16);
    if (sceSifSearchModuleByName(const_cast<char*>("INET_SPEED_UART_driver")) >= 0) isModem = 1;
}
