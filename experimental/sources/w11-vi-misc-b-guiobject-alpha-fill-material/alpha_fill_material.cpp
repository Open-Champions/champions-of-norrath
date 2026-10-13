// Observed: _11VIGUIObject$m_alphaFillMaterial at 0x0118e818 is a four-byte object that holds
// 0xffffffff in the pinned SLUS-20565 image. FillRectangle__11VIGUIObject loads it, and when it
// equals -1 it asks the raster for a new material (CreateMaterial__8VIRasteri) and stores the
// non-negative result back; CleanUp__11VIGUIObject stores -1 again.
// Inferred: a signed 32-bit material index shared by all GUI objects, with -1 meaning "not yet
// created". Unknown: the original declaration (the mangled name suggests a static class member).
extern "C" {
int _11VIGUIObject$m_alphaFillMaterial = -1;
}
