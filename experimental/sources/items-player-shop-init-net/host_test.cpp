extern "C" int shopMsgHandle;
static int calls;
static void (*seen)(unsigned char*, int);
extern "C" void shopMsgCallback__FPUci(unsigned char*, int) {}
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*callback)(unsigned char*, int)) {
    ++calls;
    seen = callback;
    return 77;
}
extern "C" void shopInitNet__Fv();
int main() {
    shopMsgHandle = 7;
    shopInitNet__Fv();
    if (calls != 1) return 1;
    if (seen != shopMsgCallback__FPUci) return 2;
    if (shopMsgHandle != 77) return 3;
    return 0;
}
