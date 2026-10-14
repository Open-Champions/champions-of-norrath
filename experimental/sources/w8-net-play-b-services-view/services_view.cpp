// Observed four constant bytes at 0x0048068c in the pinned SLUS_205.65 image:
// cc cc cc 3e, the float bits 0x3ecccccc (one unit below 0.4f). The authored
// treasure-ramp routine reads them as a float coefficient. They are a literal
// pool slot, not a symbol of their own in the image.
// Unknown: whether the compiler's pool entry is shared with other uses.
extern "C" const unsigned char norrath_player_services_view_48068c[4];
extern "C" const unsigned char norrath_player_services_view_48068c[4] = {0xcc, 0xcc, 0xcc, 0x3e};
