// osdsrc: zeroed storage for an unresolved import. Closes the link name and extent only.
// Observed (source level):
// - Reference image: a local initialised-data symbol of size 0x7a8 (1960 bytes) at 0x00444a38 (reviewed EE nm).
// - The caller init_exec_ps2.cpp in runtime-initsys copies 1960 bytes from osdsrc to 0x80074000.
// Unknown:
// - The image bytes. This unit supplies zeros, so that copy writes zeros until the owner supplies real contents.
//   Do not execute the startup path with this storage.
// This unit does not establish any field value.
extern "C" {
unsigned char osdsrc[1960] = {0};
}
static_assert(sizeof osdsrc == 1960u, "osdsrc extent");
