// Observed: FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F at 0x0102f340 is 184 bytes. With (this, rect, color), where
// rect points at four ints (x0, y0, x1, y1) and the raster is the word at +72 of this, it calls in order:
// SetSolidFillUIMaterial__8VIRaster(raster); BeginTriFan__8VIRaster(raster); Color__8VIRasterRC10VIColor32F(raster,
// color); Vertex__8VIRasterRC8VIVect2I(raster, p) for the four corners (x0,y0), (x1,y0), (x1,y1), (x0,y1) in that
// order, the first and third taken straight from rect and rect+8, the other two built in two temporary pairs; and
// EndTriFan__8VIRaster(raster). Nothing in memory outside the stack is written.
// Inferred: a solid-colour screen-space rectangle drawn as one triangle fan.
// Unknown: whether rect holds (x0,y0,x1,y1) or (position, size) is settled only by the vertex order, which treats the
// last two ints as the opposite corner; the return type (void here, no result is produced).
extern "C" {
void SetSolidFillUIMaterial__8VIRaster(void* raster);
void BeginTriFan__8VIRaster(void* raster);
void Color__8VIRasterRC10VIColor32F(void* raster, const void* color);
void Vertex__8VIRasterRC8VIVect2I(void* raster, const int* point);
void EndTriFan__8VIRaster(void* raster);

void FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(void* self, const int* rect, const void* color) {
    void* raster = reinterpret_cast<void*>(static_cast<unsigned long>(static_cast<unsigned*>(self)[18]));
    SetSolidFillUIMaterial__8VIRaster(raster);
    int top_right[2];
    top_right[0] = rect[2];
    top_right[1] = rect[1];
    int bottom_left[2];
    bottom_left[0] = rect[0];
    bottom_left[1] = rect[3];
    BeginTriFan__8VIRaster(raster);
    Color__8VIRasterRC10VIColor32F(raster, color);
    Vertex__8VIRasterRC8VIVect2I(raster, rect);
    Vertex__8VIRasterRC8VIVect2I(raster, top_right);
    Vertex__8VIRasterRC8VIVect2I(raster, rect + 2);
    Vertex__8VIRasterRC8VIVect2I(raster, bottom_left);
    EndTriFan__8VIRaster(raster);
}
}
