// _4item$undEnable at 0x00483220, 4 bytes, a local .data symbol of item (static member undEnable
// by its mangled form). The file image holds 0. Its meaning is not established here.
// Observed: one 32-bit word, initial value 0. The reviewed -G0 profile places a zero object in BSS.
extern "C" {
int _4item$undEnable;
}
static_assert(sizeof(_4item$undEnable) == 4u, "item undEnable extent");
