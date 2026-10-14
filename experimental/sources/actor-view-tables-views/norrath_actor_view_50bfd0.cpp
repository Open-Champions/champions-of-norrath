// Observed: the import name carries the address 0x50bfd0, but objectAddToSlowRunList
// forms its base with lui 0x50 and addiu 0xbfd0. The addiu sign-extends, so the
// loaded address is 0x4fbfd0. That is the .bss symbol slowRunList, recorded size 0.
// The next symbol, objectGrid, starts 240 bytes later, so this object is 240 bytes.
// Inferred: sixty 4-byte list heads. The caller wraps its index at 60 and indexes by
// index << 2. Each head is passed to listOnewayLink. Unknown: head layout beyond the
// first word, and why the earlier notes took 0x50bfd0 as a separate object.
// The reviewed -G0 profile places these zeros in .bss.
extern "C" {
unsigned char norrath_actor_view_50bfd0[240];
}
