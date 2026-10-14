// Observed haloTexture5 at 0x484f74, a 4-byte .sdata zero. haloTexture6 begins at 0x484f78
// main and launchSRS store a lump resource pointer here. This address is not
// norrath_factory_halo_tag_bytes at 0x485470 and is not any other object
// already authored on this branch. The reviewed -G0 profile places the zero
// pointer in BSS rather than file-backed .sdata.

extern "C" {
char* haloTexture5;
}
