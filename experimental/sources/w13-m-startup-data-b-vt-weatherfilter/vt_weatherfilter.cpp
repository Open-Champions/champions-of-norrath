// Observed _vt$13WeatherFilter at 0x01204180, 24 bytes in .srs (weak object in the image).
// 3 slots of 8 bytes: a zero adjustment word and a function word. The last slot is null.
// The 2 function words, in slot order, are the observed symbol names listed below; each
// name is the image symbol at that address. The bodies are not in this unit.
// Inferred: a constructor of this class stores this table address in the object (another unit).
// Unknown: dispatch order beyond the table itself, and the role of the null last slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_weatherfilter_fn_0() asm("__tf13WeatherFilter");
void vt_weatherfilter_fn_1() asm("FilterActor__13WeatherFilterR7VIScenei");
VtSlot vt_weatherfilter[3] asm("_vt$13WeatherFilter") = {
    {0x0u, vt_weatherfilter_fn_0},
    {0x0u, vt_weatherfilter_fn_1},
    {0x0u, nullptr},
};
}
