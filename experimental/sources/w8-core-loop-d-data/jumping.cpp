// Observed jumping at 0x00486030, 4 bytes (a local symbol). Every file byte is zero, so the definition is
// zero-initialized storage. jumpGateJumping__Fv returns it. Unknown: the values other than zero.
extern "C" {
int jumping = 0;
}
