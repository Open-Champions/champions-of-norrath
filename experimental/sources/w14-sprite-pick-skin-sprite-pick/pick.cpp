// Observed: Pick__12VISkinSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x0116d548, 36 bytes. Arguments: the sprite (S), the pick segment (G), the scene filter, the
// scene and the raster (R). Tail-calls PickPrimBuffer__8VIRasterR13VIPickSegmentii(R, G, word at S+68, word at S+72) and
// returns its result; the filter and scene arguments are not read.
// Inferred: the two words are the arguments the raster needs to pick this sprite's primitive buffer. Unknown: their names.
extern "C" {
int PickPrimBuffer__8VIRasterR13VIPickSegmentii(unsigned char* raster, unsigned char* segment, int first, int second);

int Pick__12VISkinSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char* self, unsigned char* segment, unsigned char*, unsigned char*, unsigned char* raster) {
    return PickPrimBuffer__8VIRasterR13VIPickSegmentii(raster, segment, *reinterpret_cast<int*>(self + 68), *reinterpret_cast<int*>(self + 72));
}
}
