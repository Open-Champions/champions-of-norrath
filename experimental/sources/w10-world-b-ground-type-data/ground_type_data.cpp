// Observed groundType at 0x00484ce8, a 4-byte local data word in SLUS_205.65, stored as
// zero in the image. The only access found in code is a 32-bit store; users in the loader
// and death-screen sources declare it as an int. Unknown: what the values mean and who reads
// it.
extern "C" {
int groundType = 0;
}
