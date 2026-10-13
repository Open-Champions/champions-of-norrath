// Observed _$_8VIRaster at 0x01109c68, 212 bytes. It calls Close__8VIRaster
// with the object, then runs these destructors, each with flag 2, in order,
// on members at the given byte offsets: VIPool<VIRFont*> 19508,
// VIPool<VIRasterMaterialLRU> 19452, VIPool<VIMaterialPal*> 19428,
// VIList<VIMaterial> 19416, VIList<VISurface*> 19404, VIPool<VISurface*>
// 19380, VIPool<VIColorBuffer*> 19356, VIList<VIPrimBuffer*> 19344,
// VIPool<VIPrimBuffer*> 19320, then VIMatrix44 at 17648 and 17584. If bit 0
// of the second argument is set it finally tail-calls __builtin_delete with
// the object. It returns nothing. The Close result is ignored.
extern "C" {
struct VIRaster;
struct Member;

int Close__8VIRaster(VIRaster* raster);
void _$_t6VIPool1ZP7VIRFont(Member* member, int flags);
void _$_t6VIPool1Z19VIRasterMaterialLRU(Member* member, int flags);
void _$_t6VIPool1ZP13VIMaterialPal(Member* member, int flags);
void _$_t6VIList1Z10VIMaterial(Member* member, int flags);
void _$_t6VIList1ZP9VISurface(Member* member, int flags);
void _$_t6VIPool1ZP9VISurface(Member* member, int flags);
void _$_t6VIPool1ZP13VIColorBuffer(Member* member, int flags);
void _$_t6VIList1ZP12VIPrimBuffer(Member* member, int flags);
void _$_t6VIPool1ZP12VIPrimBuffer(Member* member, int flags);
void _$_10VIMatrix44(Member* member, int flags);
void __builtin_delete(void* pointer);

void _$_8VIRaster(VIRaster* raster, int flags) {
    char* base = reinterpret_cast<char*>(raster);
    Close__8VIRaster(raster);
    _$_t6VIPool1ZP7VIRFont(reinterpret_cast<Member*>(base + 19508), 2);
    _$_t6VIPool1Z19VIRasterMaterialLRU(reinterpret_cast<Member*>(base + 19452), 2);
    _$_t6VIPool1ZP13VIMaterialPal(reinterpret_cast<Member*>(base + 19428), 2);
    _$_t6VIList1Z10VIMaterial(reinterpret_cast<Member*>(base + 19416), 2);
    _$_t6VIList1ZP9VISurface(reinterpret_cast<Member*>(base + 19404), 2);
    _$_t6VIPool1ZP9VISurface(reinterpret_cast<Member*>(base + 19380), 2);
    _$_t6VIPool1ZP13VIColorBuffer(reinterpret_cast<Member*>(base + 19356), 2);
    _$_t6VIList1ZP12VIPrimBuffer(reinterpret_cast<Member*>(base + 19344), 2);
    _$_t6VIPool1ZP12VIPrimBuffer(reinterpret_cast<Member*>(base + 19320), 2);
    _$_10VIMatrix44(reinterpret_cast<Member*>(base + 17648), 2);
    _$_10VIMatrix44(reinterpret_cast<Member*>(base + 17584), 2);
    if (flags & 1) __builtin_delete(raster);
}
}
