// Observed words that only runFrontEnd touches.
// endTimer.1104 at 0x00484c18 is a local four-byte word that starts at zero.
// The byte at 0x00484ba8 has no symbol; it sits after the local menu pointer
// and starts at zero. The image does not show whether anything else writes it.
// The name below is a placeholder tied to that address, not an original name.
extern "C" {
int norrath_frontend_end_timer __asm__("endTimer.1104") = 0;
unsigned char norrath_frontend_byte_484ba8 = 0;
}
