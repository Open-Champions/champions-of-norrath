// Observed: __20VIWndGenericRenderer at 0x010b9110 is 232 bytes. With the object as this it, in
// order: calls __5VIWnd(this); stores the address of _vt$20VIWndGenericRenderer (0x012094a8) at
// this+32; runs __t6VIList1ZP9VIGUIPage on this+40, __19VIFlatFileStructure on this+1364 and
// __9VIWndEdit on this+1424; calls LoadSRSServer__Fv() and passes its result as the third
// argument of __16VIRealmInterfaceP20VIWndGenericRendererPCc(this+1880, this, result); stores 0
// at this+36, 52, 56, 60, 1348, 1352, 1360, 1400, 1404, 1408, 1412 and 1416 and -1 at this+1356;
// stores 0 at this+1088 and clears 64 bytes at each of this+1220, 1284, 1092 and 1156 and
// 1024 bytes at this+64 with memset (the last as a tail call). Nothing is returned.
// Inferred: LoadSRSServer__Fv returns a C string (the type of the third constructor argument in
// the symbol name). Unknown: the meaning of the cleared fields and buffers.
extern "C" {
void __5VIWnd(void* self);
void __t6VIList1ZP9VIGUIPage(void* self);
void __19VIFlatFileStructure(void* self);
void __9VIWndEdit(void* self);
const char* LoadSRSServer__Fv();
void __16VIRealmInterfaceP20VIWndGenericRendererPCc(void* self, void* renderer, const char* server);
void* memset(void* destination, int value, unsigned count);
extern unsigned char _vt$20VIWndGenericRenderer[];

void __20VIWndGenericRenderer(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$20VIWndGenericRenderer));
    __t6VIList1ZP9VIGUIPage(bytes + 40);
    __19VIFlatFileStructure(bytes + 1364);
    __9VIWndEdit(bytes + 1424);
    __16VIRealmInterfaceP20VIWndGenericRendererPCc(bytes + 1880, self, LoadSRSServer__Fv());
    *reinterpret_cast<unsigned*>(bytes + 36) = 0;
    *reinterpret_cast<unsigned*>(bytes + 52) = 0;
    *reinterpret_cast<int*>(bytes + 1356) = -1;
    *reinterpret_cast<unsigned*>(bytes + 60) = 0;
    *reinterpret_cast<unsigned*>(bytes + 56) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1360) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1400) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1404) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1408) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1348) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1352) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1412) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1416) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1088) = 0;
    memset(bytes + 1220, 0, 64);
    memset(bytes + 1284, 0, 64);
    memset(bytes + 1092, 0, 64);
    memset(bytes + 1156, 0, 64);
    memset(bytes + 64, 0, 1024);
}
}
