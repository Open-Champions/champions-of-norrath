// Observed msg_run__8HateHole at 0x226318, 28 bytes: the object is passed
// unchanged to msg_run__8AutoProp and nothing else happens. The AutoProp body is
// defined by another source in this package.
extern "C" void msg_run__8AutoProp(void* self);
extern "C" void msg_run__8HateHole(void* self) {
    msg_run__8AutoProp(self);
}
