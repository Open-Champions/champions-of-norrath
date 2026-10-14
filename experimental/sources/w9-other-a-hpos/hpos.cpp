// Observed hpos at 0x008d5a08 (.bss). The image bytes are zero. The span to the next symbol is 144 bytes; the authored callers index it as twelve three-float records.
// Inferred: only the access width. Unknown: the meaning of the value and its writers.
struct Point3 { float x, y, z; };
extern "C" Point3 hpos[12] = {};
