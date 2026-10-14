// Observed: Pick__14VISimpleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x011670b8, 136 bytes. Arguments: the sprite (S), the pick segment (G), the scene filter, the
// scene (V) and the raster (R). Calls GetNextColorBuffer__7VIScene(V) (the filter is not read). If the result N is
// negative it tail-calls PickPrimBuffer__8VIRasterR13VIPickSegmentii(R, G, word at S+64, word at S+72); otherwise it
// tail-calls PickPrimBuffer__8VIRasterR13VIPickSegmentiii(R, G, word at S+64, N, word at S+72). Either result is returned.
// Inferred: N is a colour-buffer index and the three-integer overload takes it as the extra selector. Unknown: the
// names of the S words.
extern "C" {
int GetNextColorBuffer__7VIScene(unsigned char* scene);
int PickPrimBuffer__8VIRasterR13VIPickSegmentii(unsigned char* raster, unsigned char* segment, int first, int second);
int PickPrimBuffer__8VIRasterR13VIPickSegmentiii(unsigned char* raster, unsigned char* segment, int first, int buffer, int second);

int Pick__14VISimpleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char* self, unsigned char* segment, unsigned char*, unsigned char* scene, unsigned char* raster) {
    int buffer = GetNextColorBuffer__7VIScene(scene);
    if (buffer < 0) return PickPrimBuffer__8VIRasterR13VIPickSegmentii(raster, segment, *reinterpret_cast<int*>(self + 64), *reinterpret_cast<int*>(self + 72));
    return PickPrimBuffer__8VIRasterR13VIPickSegmentiii(raster, segment, *reinterpret_cast<int*>(self + 64), buffer, *reinterpret_cast<int*>(self + 72));
}
}
