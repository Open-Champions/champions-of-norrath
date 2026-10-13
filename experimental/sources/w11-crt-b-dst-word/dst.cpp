// Observed: the data word `dst` at 0x00486d04, 4 bytes of zero-initialised small data (a local
// symbol; its neighbour `src` is the word at 0x00486d00). The only writer seen is initPosition__Fii,
// which stores its second argument in it (the first goes to `src`). No reader has been identified.
// Unknown: what consumes the two words. The definition is global so initPosition__Fii resolves it.
// Placement differs from the original: a definition here lands wherever the link puts it.
int dst = 0;
