// Observed initialise_event_list at 0x002e8dd0, 12 bytes. Clears the halfwords at record + 36 and
// record + 38 and returns; it touches nothing else. Its only observed caller is lgKbdRead, which
// passes its `record` pointer just before releasing the device semaphore. The meaning of the two
// halfwords is not established; "event list" is only the symbol name. Nothing was executed on the EE.
extern "C" {
typedef short __attribute__((may_alias)) AliasedShort;

void initialise_event_list(void* record) {
    unsigned char* const bytes = static_cast<unsigned char*>(record);
    *reinterpret_cast<AliasedShort*>(bytes + 36) = 0;
    *reinterpret_cast<AliasedShort*>(bytes + 38) = 0;
}
}
