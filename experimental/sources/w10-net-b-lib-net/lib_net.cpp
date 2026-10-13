// Observed words at 0x0118e6b0 (net_buf) and 0x0118e6b4 (sLibNetInitialized),
// 4 bytes each per the ELF symbol table. The addresses lie beyond the loaded
// file image, so no initial bytes exist in the file; they start as zero storage.
// The authored unet_shutdown reads sLibNetInitialized and frees the pointer in
// net_buf. Unknown: which routine allocates net_buf and sets the flag.
extern "C" {
void* net_buf;
int sLibNetInitialized;
}
