// Observed lifeTimes at 0x0037f720, 16 bytes in .data (symbol table: local data
// object). Four 32-bit words, in file order 11, 30, 60 and 90. updatePS__FP14ParticleSystem
// reads word n for the n-th entry of each four-entry group. They are the values
// added to a particle's stamp before it is compared with P_FrameNum. The name is
// the symbol table's; what the four tiers mean is not established here.
extern "C" {
unsigned lifeTimes[4] = {11u, 30u, 60u, 90u};
}
static_assert(sizeof(lifeTimes) == 16u, "lifeTimes extent");
