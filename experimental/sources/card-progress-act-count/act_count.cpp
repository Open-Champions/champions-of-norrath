// Observed: actCount is the 4-byte value 5. MC_Init loads it as the number
// of act tables. The image binding is local; authored callers import the name.
// Inferred: none.
// Unknown: whether a later store replaces the 5.
extern "C" int actCount = 5;
