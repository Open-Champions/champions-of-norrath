// Observed: errorCondition is a 4-byte word. stopCardQuery passes its
// address to MCA_Sync and then stores zero. The image bytes are zero.
// Inferred: none.
// Unknown: which values MCA_Sync writes before that store.
extern "C" int errorCondition = 0;
