// Observed: globalShadowBrightness at 0x00484090, four bytes, a float with file bytes 00 00 80 3e
// (0.25f); and halos at 0x00484084, four bytes, a pointer whose file value is the address
// 0x0034c018, which is the object defaultHalos (16 bytes: two eight-byte records). The
// authored callers (model reset, model draw) read both; the model-reset routine copies the first
// to shadowBrightness and resets halos to defaultHalos.
// Inferred: halos is the current halo-definition table pointer, defaultHalos its default.
// Unknown: whether the original's first reader is guaranteed to see these initial values.
// defaultHalos is defined by another package (or remains an unresolved import).
extern "C" {
extern char defaultHalos[];
float globalShadowBrightness = 0.25f;
const void* halos = defaultHalos;
}
