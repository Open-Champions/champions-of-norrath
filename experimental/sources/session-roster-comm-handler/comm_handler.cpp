// Observed commHandler__FPUci at 0x002cd110, 32 bytes. The byte at packet+132
// is passed, with the packet pointer, to recievePacket__FiPUc. The length
// argument is not read. The callee's return value is not used here.
extern "C" void recievePacket__FiPUc(int source, unsigned char* packet);

extern "C" void commHandler__FPUci(unsigned char* packet, int length) {
    (void)length;
    recievePacket__FiPUc(packet[132], packet);
}
