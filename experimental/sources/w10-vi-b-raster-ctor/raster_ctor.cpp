// Observed __8VIRaster at 0x011098e8, 896 bytes. Offsets are bytes from the
// object. The constructor has no arguments besides the object and returns
// nothing. In order it
//  - writes float zero to: words 12 and 16; words 20, 24 and 28; words 32 to
//    44 (four words); the first four words of each of 32 elements of 24
//    bytes starting at 64; 49 elements of 48 bytes (all twelve words) starting
//    at 948; 7 elements of 48 bytes at 3300; 7 at 3636; 272 at 4488; four
//    words at 17560; the 64 bytes at 17584 and the 64 bytes at 17648 (the
//    original writes each of those twice, the second time as four 16-byte
//    vector stores); four words at 17788; three words at 18924; and 32
//    elements of three words from 18936;
//  - runs the constructors of the members at 19320 (VIPool<VIPrimBuffer*>),
//    19344 (VIList<VIPrimBuffer*>), 19356 (VIPool<VIColorBuffer*>), 19380
//    (VIPool<VISurface*>), 19404 (VIList<VISurface*>), 19416
//    (VIList<VIMaterial>), 19428 (VIPool<VIMaterialPal*>), 19452
//    (VIPool<VIRasterMaterialLRU>) and 19508 (VIPool<VIRFont*>);
//  - stores zero at 0, 4, 8, 52, 56, 19476, 19484, 19488, 19492, 19496 and
//    19532; 1 at 19500; and -1 at 17848 and at the six words 17824..17844.
// The 24- and 48-byte elements are the strides of the original loops; what
// the elements are is not established here, and neither is the unwritten
// remainder of each 24-byte element.
extern "C" {
struct VIRaster;
struct Member;

void __t6VIPool1ZP12VIPrimBuffer(Member* member);
void __t6VIList1ZP12VIPrimBuffer(Member* member);
void __t6VIPool1ZP13VIColorBuffer(Member* member);
void __t6VIPool1ZP9VISurface(Member* member);
void __t6VIList1ZP9VISurface(Member* member);
void __t6VIList1Z10VIMaterial(Member* member);
void __t6VIPool1ZP13VIMaterialPal(Member* member);
void __t6VIPool1Z19VIRasterMaterialLRU(Member* member);
void __t6VIPool1ZP7VIRFont(Member* member);

static void zero_floats(char* at, int count) {
    float* words = reinterpret_cast<float*>(at);
    for (int i = 0; i < count; ++i) words[i] = 0.0f;
}

static void zero_elements(char* at, int elements, int stride, int words) {
    for (int i = 0; i < elements; ++i) zero_floats(at + stride * i, words);
}

void __8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    zero_floats(base + 12, 2);
    zero_floats(base + 20, 3);
    zero_floats(base + 32, 4);
    zero_elements(base + 64, 32, 24, 4);
    zero_elements(base + 948, 49, 48, 12);
    zero_elements(base + 3300, 7, 48, 12);
    zero_elements(base + 3636, 7, 48, 12);
    zero_elements(base + 4488, 272, 48, 12);
    zero_floats(base + 17560, 4);
    zero_floats(base + 17584, 16);
    zero_floats(base + 17648, 16);
    zero_floats(base + 17788, 4);
    zero_floats(base + 18924, 3);
    zero_elements(base + 18936, 32, 12, 3);
    __t6VIPool1ZP12VIPrimBuffer(reinterpret_cast<Member*>(base + 19320));
    __t6VIList1ZP12VIPrimBuffer(reinterpret_cast<Member*>(base + 19344));
    __t6VIPool1ZP13VIColorBuffer(reinterpret_cast<Member*>(base + 19356));
    __t6VIPool1ZP9VISurface(reinterpret_cast<Member*>(base + 19380));
    __t6VIList1ZP9VISurface(reinterpret_cast<Member*>(base + 19404));
    __t6VIList1Z10VIMaterial(reinterpret_cast<Member*>(base + 19416));
    __t6VIPool1ZP13VIMaterialPal(reinterpret_cast<Member*>(base + 19428));
    __t6VIPool1Z19VIRasterMaterialLRU(reinterpret_cast<Member*>(base + 19452));
    __t6VIPool1ZP7VIRFont(reinterpret_cast<Member*>(base + 19508));
    int* words = reinterpret_cast<int*>(base);
    words[0] = 0;
    *reinterpret_cast<int*>(base + 19500) = 1;
    *reinterpret_cast<int*>(base + 17848) = -1;
    *reinterpret_cast<int*>(base + 19532) = 0;
    words[1] = 0;
    words[2] = 0;
    words[13] = 0;
    words[14] = 0;
    *reinterpret_cast<int*>(base + 19476) = 0;
    *reinterpret_cast<int*>(base + 19484) = 0;
    *reinterpret_cast<int*>(base + 19488) = 0;
    *reinterpret_cast<int*>(base + 19492) = 0;
    *reinterpret_cast<int*>(base + 19496) = 0;
    for (int i = 0; i < 6; ++i) *reinterpret_cast<int*>(base + 17824 + 4 * i) = -1;
}
}
