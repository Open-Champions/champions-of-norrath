#include "cut_msg_handler_function.cpp"
int netNmReady = 0;
int main() {
    unsigned char message[4] = {1, 2, 3, 4};
    cutMsgHandlerFunction__FPUci(message, 4);
    if (netNmReady != 1) return 1;
    cutMsgHandlerFunction__FPUci(nullptr, -1);
    if (netNmReady != 2) return 2;
    netNmReady = 0x7fffffff;
    cutMsgHandlerFunction__FPUci(message, 0);
    return (unsigned)netNmReady == 0x80000000u ? 0 : 3;
}
