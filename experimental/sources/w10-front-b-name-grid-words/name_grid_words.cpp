// Observed: nameGridW (0x00486d14) and nameGridCount (0x00486d18) are adjacent local zero-initialised
// words in .bss; initNamePad stores to both with word stores (see the init-name-pad notes) and the
// neighbours namePos (0x00486d10) and requestToExit (0x00486d1c) are other symbols, so each is 4 bytes.
// Unknown: the readers of either word.
extern "C" {
int nameGridW;
int nameGridCount;
}
