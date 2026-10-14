// tlbsrc: zeroed storage for an unresolved import. Closes the link name and extent only.
// Observed (source level):
// - Reference image: a local initialised-data symbol of size 0x330 (816 bytes) at 0x004459d0 (reviewed EE nm).
// - The caller init_tlb.cpp in runtime-initsys copies 816 bytes from tlbsrc to 0x80075000.
// Unknown:
// - The image bytes. This unit supplies zeros, so that copy writes zeros until the owner supplies real contents.
//   Do not execute the startup path with this storage.
// This unit does not establish any field value.
extern "C" {
unsigned char tlbsrc[816] = {0};
}
static_assert(sizeof tlbsrc == 816u, "tlbsrc extent");
