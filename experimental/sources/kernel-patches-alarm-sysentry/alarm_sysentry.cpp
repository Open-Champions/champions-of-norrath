// Observed: the local SysEntry at 0x00445968, 64 bytes of .data: the words
// 0x5a, the address of the local kCopy at 0x002ddd80, 0x5b, 0x80076000, then
// the pairs (0xfc, 0), (0xfe, 0), (0xfd, 0), (0xff, 0), (0x12c, 0), (8, 0).
// InitAlarm passes the first two pairs to its setup service, then walks the
// remaining six: for each, it queries the entry address for the first word
// and passes both back to setup.
// Inferred: pairs of a service number and a handler or buffer address.
// Unknown: what the setup service does with a pair.
extern "C" {
void kernel_alarm_pattern_kcopy();
unsigned kernel_alarm_sysentry[16] = {
    0x5au, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(kernel_alarm_pattern_kcopy)),
    0x5bu, 0x80076000u,
    0xfcu, 0u,
    0xfeu, 0u,
    0xfdu, 0u,
    0xffu, 0u,
    0x12cu, 0u,
    0x8u, 0u,
};
}
