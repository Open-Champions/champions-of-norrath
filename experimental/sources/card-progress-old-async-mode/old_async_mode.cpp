// Observed: oldAsyncMode is the next 4-byte word after asyncMode. newMode
// stores the previous asyncMode there when its second argument is zero.
// The image bytes are zero. No word load of this address was found.
// Inferred: none.
// Unknown: which routine reads the saved mode.
extern "C" int oldAsyncMode = 0;
