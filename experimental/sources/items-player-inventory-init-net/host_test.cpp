extern "C" int invMsgHandle;
static int calls;
static void (*seen)(unsigned char*, int);
extern "C" void invMsgCallback__FPUci(unsigned char*, int) {}
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*callback)(unsigned char*, int)) {
    ++calls;
    seen = callback;
    return 42;
}
extern "C" void inventoryInitNet__Fv();
int main() {
    invMsgHandle = 7;
    inventoryInitNet__Fv();
    if (calls != 1) return 1;
    if (seen != invMsgCallback__FPUci) return 2;
    if (invMsgHandle != 42) return 3;
    return 0;
}
