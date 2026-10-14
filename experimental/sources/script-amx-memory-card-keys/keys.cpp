// Observed memory-card script entry names passed to scriptStart.
// preStart is the nine bytes at 0x00474ca8. start is the six bytes at 0x00485ac0.
// Both include the terminating zero. Neighboring section names are separate objects.
extern "C" {
unsigned char norrath_mc_pre_start_key_bytes[] = {
    'p', 'r', 'e', 'S', 't', 'a', 'r', 't', 0
};
unsigned char norrath_mc_start_key_bytes[] = {
    's', 't', 'a', 'r', 't', 0
};
}
