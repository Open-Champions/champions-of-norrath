// engineDisable30fps at 0x0048441c, 4 bytes, in SLUS_205.65.
// Observed: zero-initial word in .data. Authored code in another package stores to it. Unknown: the consumer.
extern "C" {
int engineDisable30fps = 0;
}
