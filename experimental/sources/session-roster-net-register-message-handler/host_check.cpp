#include "register_message_handler.cpp"
// One flat block: 16 message slots then the word that follows them.
extern "C" NetHandler msgHandler[17] = {};
#define block msgHandler
static void a(unsigned char*, int) {}
static void b(unsigned char*, int) {}
static void c(unsigned char*, int) {}
int main() {
    if (netRegisterMessageHandler__FPFPUci_v(a) != 0 || block[0] != a) return 1;
    if (netRegisterMessageHandler__FPFPUci_v(b) != 1 || block[1] != b) return 2;
    if (netRegisterMessageHandler__FPFPUci_v(a) != 0) return 3;  // repeat: same slot
    if (netRegisterMessageHandler__FPFPUci_v(b) != 1) return 4;
    block[2] = 0;
    // A hole earlier than a used slot is filled first.
    block[0] = 0;
    if (netRegisterMessageHandler__FPFPUci_v(c) != 0 || block[0] != c) return 5;
    // Fill everything, then a new callback lands one past the table.
    for (int i = 0; i < 16; ++i) block[i] = (i & 1) ? b : a;
    block[16] = 0;
    if (netRegisterMessageHandler__FPFPUci_v(c) != 16 || block[16] != c) return 6;
    // The 16th slot being the only empty one returns 15 without overflow.
    for (int i = 0; i < 16; ++i) block[i] = (i & 1) ? b : a;
    block[15] = 0;
    block[16] = 0;
    if (netRegisterMessageHandler__FPFPUci_v(c) != 15 || block[15] != c || block[16] != 0) return 7;
    // A match in the last slot is found before any insertion.
    if (netRegisterMessageHandler__FPFPUci_v(c) != 15) return 8;
    return 0;
}
