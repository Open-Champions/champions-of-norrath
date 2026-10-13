// Observed one float at 0x48068c in .lit4: bits 0x3ecccccc, one ulp below
// the nearest float to 0.4. The treasure ramp multiplies its scale by this
// coefficient. Bytes below are the image at that address, little-endian.
// No ELF symbol names this word.
// Inferred: it is norrath_player_services_view_48068c.
// Unknown: placement at the original address and section identity with
// .lit4 are not claimed by this relocatable object.
extern "C" const unsigned char norrath_player_services_view_48068c[4] = {
    0xcc, 0xcc, 0xcc, 0x3e,
};
