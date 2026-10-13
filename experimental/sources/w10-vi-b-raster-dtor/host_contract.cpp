#include "raster_dtor.cpp"

#include <string>

static std::string trace;
static unsigned char* object_bytes;

static std::string off(const char* tag, Member* m) {
    return std::string(tag) + std::to_string(reinterpret_cast<unsigned char*>(m) - object_bytes) + ",";
}
extern "C" int Close__8VIRaster(VIRaster* r) {
    trace += reinterpret_cast<unsigned char*>(r) == object_bytes ? "close;" : "close?;";
    return -1;
}
#define MEMBER(sym, tag) \
    extern "C" void sym(Member* m, int flags) { trace += off(tag, m) + std::to_string(flags) + ";"; }
MEMBER(_$_t6VIPool1ZP7VIRFont, "font")
MEMBER(_$_t6VIPool1Z19VIRasterMaterialLRU, "lru")
MEMBER(_$_t6VIPool1ZP13VIMaterialPal, "pal")
MEMBER(_$_t6VIList1Z10VIMaterial, "mat")
MEMBER(_$_t6VIList1ZP9VISurface, "surfl")
MEMBER(_$_t6VIPool1ZP9VISurface, "surfp")
MEMBER(_$_t6VIPool1ZP13VIColorBuffer, "color")
MEMBER(_$_t6VIList1ZP12VIPrimBuffer, "primbl")
MEMBER(_$_t6VIPool1ZP12VIPrimBuffer, "primbp")
MEMBER(_$_10VIMatrix44, "m44")
extern "C" void __builtin_delete(void* p) { trace += p == object_bytes ? "delete;" : "delete?;"; }

int main() {
    static unsigned char object[20000];
    object_bytes = object;
    const std::string members =
        "close;font19508,2;lru19452,2;pal19428,2;mat19416,2;surfl19404,2;surfp19380,2;color19356,2;"
        "primbl19344,2;primbp19320,2;m4417648,2;m4417584,2;";
    _$_8VIRaster(reinterpret_cast<VIRaster*>(object), 0);
    if (trace != members) return 1;
    trace.clear();
    _$_8VIRaster(reinterpret_cast<VIRaster*>(object), 1);
    if (trace != members + "delete;") return 2;
    trace.clear();
    _$_8VIRaster(reinterpret_cast<VIRaster*>(object), 2);
    if (trace != members) return 3;
    return 0;
}
