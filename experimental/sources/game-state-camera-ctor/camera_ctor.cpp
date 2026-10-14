// Observed __6Camera at 0x0026f948, 100 bytes.
// The object pointer arrives in a0. Halfwords are stored before SetMode:
//   126 = 100, 128 = 4000, 130 = 6000, 132 = 8000, 134 = 9000, 136 = 10000.
// SetMode__6Camerai then receives that object and 1. Reset__6Cameraii is the
// tail call and receives the same object, 1, and 1. No other store is in this
// body. The object size is not established here. Nothing here was executed.
extern "C" void SetMode__6Camerai(unsigned char* camera, int mode);
extern "C" void Reset__6Cameraii(unsigned char* camera, int first, int second);

static void store_u16(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
}

extern "C" void __6Camera(unsigned char* camera) {
    store_u16(camera + 126, 100u);
    store_u16(camera + 128, 4000u);
    store_u16(camera + 130, 6000u);
    store_u16(camera + 132, 8000u);
    store_u16(camera + 134, 9000u);
    store_u16(camera + 136, 10000u);
    SetMode__6Camerai(camera, 1);
    Reset__6Cameraii(camera, 1, 1);
}
