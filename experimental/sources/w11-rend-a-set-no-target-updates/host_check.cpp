extern "C" void SetNoTargetUpdates__6Camerai(unsigned char* camera, int value);
int main() {
    unsigned char cam[512] = {};
    cam[363] = 0x55; cam[365] = 0x66;
    SetNoTargetUpdates__6Camerai(cam, 5);
    if (cam[364] != 5 || cam[363] != 0x55 || cam[365] != 0x66) return 1;
    SetNoTargetUpdates__6Camerai(cam, 0x1234);
    if (cam[364] != 0x34) return 2;
    SetNoTargetUpdates__6Camerai(cam, -1);
    if (cam[364] != 0xff) return 3;
    return 0;
}
