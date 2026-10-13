// Observed msg_run__8GothTrap at 0x00226290, 8 bytes. The function returns immediately: it reads
// nothing, writes nothing and calls nothing. Inferred: GothTrap's per-frame message does no work of
// its own. Nothing here ran on the EE.
extern "C" void msg_run__8GothTrap(void* self) {
    (void)self;
}
