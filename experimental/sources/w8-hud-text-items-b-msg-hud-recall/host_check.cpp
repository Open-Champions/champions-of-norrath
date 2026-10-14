static int sends, handle, size;
static unsigned char bytes[2];

extern "C" {
int hudMsgHandle;
void netSendMessage__FiPUci(int h, unsigned char* b, int n) {
    sends += 1;
    handle = h;
    size = n;
    bytes[0] = b[0];
    bytes[1] = b[1];
}
void msgHudRecall__Fc(char recall);
}

int main() {
    hudMsgHandle = 21;
    msgHudRecall__Fc(1);
    if (sends != 1 || handle != 21 || size != 2) return 1;
    if (bytes[0] != 3 || bytes[1] != 1) return 2;
    msgHudRecall__Fc(static_cast<char>(0xfe));
    if (sends != 2 || bytes[0] != 3 || bytes[1] != 0xfe) return 3;
    return 0;
}
