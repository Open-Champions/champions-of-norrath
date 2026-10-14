// Observed: browserState is a 4-byte word. MC_Init stores zero. The image
// bytes are zero. Word loads and stores are the accesses at this address.
// Inferred: none.
// Unknown: the state numbers stored by the browser.
extern "C" int browserState = 0;
