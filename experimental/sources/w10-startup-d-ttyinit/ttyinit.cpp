// Observed ttyinit at 0x00444950: one 4-byte word in initialized .data holding zero in the
// image. write() sets it to 1 once the tty has been initialized and sceResetttyinit clears
// it. Nothing else about its meaning is known.
extern "C" {
int ttyinit __attribute__((section(".data"))) = 0;
}
