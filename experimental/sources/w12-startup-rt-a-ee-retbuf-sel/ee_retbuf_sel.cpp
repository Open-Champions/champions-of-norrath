// Observed ee_retbuf_sel at 0x004449f0, pinned SLUS_205.65: a four-byte word in the initialised data
// section (a file-local symbol of the EE file-service library). The image holds zero. _sceFs_Rcv_Intr
// stores the reply-buffer index it selected into it each time a reply arrives.
// The object is placed in .data explicitly because a zero initialiser alone would go to .bss.
// Unknown: whether anything reads the word; no authored routine does.
extern "C" {
#if defined(__mips__)
__attribute__((section(".data")))
#endif
int ee_retbuf_sel = 0;
}
