// Observed msg_run__8HateHole at 0x00226318, 28 bytes. Calls msg_run__8AutoProp with the same
// object argument and returns; nothing else is read or written, and no result is used.
// Inferred: HateHole inherits AutoProp's per-frame message unchanged. Nothing here ran on the EE.
extern "C" {
void msg_run__8AutoProp(void* self);
}

extern "C" void msg_run__8HateHole(void* self) {
    msg_run__8AutoProp(self);
}
