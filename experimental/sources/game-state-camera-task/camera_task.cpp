// Observed gameCameraTask__Fv at 0x001d0b88, 48 bytes.
// gameCam is the 4-byte pointer at 0x00484f50. A zero pointer returns.
// netNetworkMode is the byte at 0x00483829. A nonzero byte returns without
// Update__6Camera. Otherwise Update__6Camera receives the pointer loaded
// before the network test. Nothing here was executed.
extern "C" unsigned char* gameCam;
extern "C" unsigned char netNetworkMode;
extern "C" void Update__6Camera(unsigned char* camera);

extern "C" void gameCameraTask__Fv() {
    unsigned char* camera = gameCam;
    if (camera == 0) return;
    if (netNetworkMode != 0) return;
    Update__6Camera(camera);
}
