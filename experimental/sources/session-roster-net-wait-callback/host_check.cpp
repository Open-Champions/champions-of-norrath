#include "net_wait_callback.cpp"
int main() {
    unsigned char packet[4] = {1, 2, 3, 4};
    netWaitCallBack__FPUci(packet, 4);
    if (packet[0] != 1 || packet[3] != 4) return 1;
    netWaitCallBack__FPUci(0, 0);
    return 0;
}
