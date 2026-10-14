// Observed: CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster at 0x01063280 is 316 bytes in the pinned SLUS-20565 image.
// Arguments: the sprite (this), a pick segment and a raster. It calls GetModelView__C8VIRaster(raster) three times and forms a
// difference vector d = (view[12] - segment[0], view[13] - segment[1], view[14] - segment[2]) where view and segment are read as
// float arrays (the view's words 12..14 are bytes 48..56); each difference is formed right after its own GetModelView call. The
// squared length s = (dx*dx + dy*dy) + dz*dz is then compared with the squares of the floats stored at sprite +172, one per level, the
// number of levels being the int at sprite +168. It starts at level -1 and, for each level in order, stops at the first one whose
// squared threshold is greater than s (strict less-than, so a NaN never stops), otherwise adds one; the result is the level reached,
// which is -1 when s is below the first threshold or the count is not positive, and count - 1 when s reaches every threshold.
// Inferred: the sprite keeps a table of level-of-detail switch distances, the view matrix word group 12..14 is the eye or
// translation, and the pick segment's first three floats are a point, so this picks a detail level from a distance.
// Unknown: the sprite and segment layouts beyond those words, the size of the threshold table, and GetModelView's matrix layout.
// Nothing here ran on the EE.
extern "C" {
const float* GetModelView__C8VIRaster(const unsigned char* raster);
int CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster(unsigned char* self, const unsigned char* segment, const unsigned char* raster) {
    const float* point = reinterpret_cast<const float*>(segment);
    float d[3] = {0.0f, 0.0f, 0.0f};
    d[0] = GetModelView__C8VIRaster(raster)[12] - point[0];
    d[1] = GetModelView__C8VIRaster(raster)[13] - point[1];
    d[2] = GetModelView__C8VIRaster(raster)[14] - point[2];
    float squared = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    int level = -1;
    int count = *reinterpret_cast<int*>(self + 168);
    const float* threshold = reinterpret_cast<const float*>(self + 172);
    for (int i = 0; i < count; ++i) {
        if (squared < threshold[i] * threshold[i]) break;
        ++level;
    }
    return level;
}
}
