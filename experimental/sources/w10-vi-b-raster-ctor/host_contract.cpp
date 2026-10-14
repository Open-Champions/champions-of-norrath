#include "raster_ctor.cpp"

#include <cstring>
#include <string>
#include <vector>

static unsigned char* object_bytes;
static std::vector<std::string> trace;

#define MEMBER(sym)                                                                         \
    extern "C" void sym(Member* m) {                                                        \
        trace.push_back(std::string(#sym) + "@" +                                           \
                        std::to_string(reinterpret_cast<unsigned char*>(m) - object_bytes)); \
    }
MEMBER(__t6VIPool1ZP12VIPrimBuffer)
MEMBER(__t6VIList1ZP12VIPrimBuffer)
MEMBER(__t6VIPool1ZP13VIColorBuffer)
MEMBER(__t6VIPool1ZP9VISurface)
MEMBER(__t6VIList1ZP9VISurface)
MEMBER(__t6VIList1Z10VIMaterial)
MEMBER(__t6VIPool1ZP13VIMaterialPal)
MEMBER(__t6VIPool1Z19VIRasterMaterialLRU)
MEMBER(__t6VIPool1ZP7VIRFont)

int main() {
    const int size = 19600;
    alignas(4) static unsigned char object[size];
    std::memset(object, 0xa5, size);
    object_bytes = object;
    __8VIRaster(reinterpret_cast<VIRaster*>(object));

    std::vector<std::string> members = {
        "__t6VIPool1ZP12VIPrimBuffer@19320", "__t6VIList1ZP12VIPrimBuffer@19344",
        "__t6VIPool1ZP13VIColorBuffer@19356", "__t6VIPool1ZP9VISurface@19380",
        "__t6VIList1ZP9VISurface@19404", "__t6VIList1Z10VIMaterial@19416",
        "__t6VIPool1ZP13VIMaterialPal@19428", "__t6VIPool1Z19VIRasterMaterialLRU@19452",
        "__t6VIPool1ZP7VIRFont@19508"};
    if (trace != members) return 1;

    static unsigned char want[size];
    std::memset(want, 0xa5, size);
    auto zero = [&](int at, int n) { std::memset(want + at, 0, n); };
    auto set32 = [&](int at, int v) { std::memcpy(want + at, &v, 4); };
    zero(12, 8);
    zero(20, 12);
    zero(32, 16);
    for (int i = 0; i < 32; ++i) zero(64 + 24 * i, 16);
    zero(948, 49 * 48);
    zero(3300, 7 * 48);
    zero(3636, 7 * 48);
    zero(4488, 272 * 48);
    zero(17560, 16);
    zero(17584, 64);
    zero(17648, 64);
    zero(17788, 16);
    zero(18924, 12);
    zero(18936, 32 * 12);
    for (int at : {0, 4, 8, 52, 56, 19476, 19484, 19488, 19492, 19496, 19532}) set32(at, 0);
    set32(19500, 1);
    set32(17848, -1);
    for (int i = 0; i < 6; ++i) set32(17824 + 4 * i, -1);
    for (int i = 0; i < size; ++i)
        if (object[i] != want[i]) return 2;
    return 0;
}
