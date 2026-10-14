// Observed: Render__9VITrailFxR8VIRasterR8VICamera at 0x01026c40, 736 bytes.
// The result is zero. With the object as this, the embedded list at this + 28
// (first-link at +0; a node whose first link is zero is the end marker):
//   If Length__C7VILList(this + 28) is below 2, return at once.
//   Otherwise SetModelView__8VIRasterRC10VIMatrix44(raster, camera + 48),
//   SetMaterial__8VIRasteri(raster, sMaterialHandle) and
//   BeginTriStrip__8VIRaster(raster), with a vertex count n = 0.
//   Pass 1 walks the list from the first node to the end marker. For each
//   node: alpha = min(max(1.0 - age / lifetime, 0.0), 1.0), age being the float
//   at node + 72 and lifetime the float at this + 24 (read again for every
//   node); alpha is stored at node + 68 and node + 52. Then
//   Color__8VIRasterRC10VIColor32F(raster, node + 40),
//   Vertex__8VIRasterRC7VIVect3(raster, node + 16),
//   Color(raster, node + 56), Vertex(raster, node + 28); n += 2. When n
//   reaches 24: EndTriStrip__8VIRaster(raster), n = 0, and, if the next node
//   is not the end marker, BeginTriStrip(raster), n = 2 and the same four
//   calls are made again for the same node (restarting the strip on its last
//   pair).
//   Between the passes, if n is non-zero EndTriStrip(raster) is called; then
//   BeginTriStrip(raster) and n = 0.
//   Pass 2 is pass 1 with the two pairs swapped: Color(raster, node + 56),
//   Vertex(raster, node + 28), Color(raster, node + 40),
//   Vertex(raster, node + 16), including the alpha stores and the restart.
//   At the end, if n is non-zero, EndTriStrip(raster).
// Inferred: segments are 24-byte-spaced vertex pairs with colours at +40 and
// +56 (four floats each, alpha last); two strips are drawn so both winding
// orders are covered (back-face handling is not shown in this function).
// Unknown: member names, and why the image recomputes the alpha in both passes.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
extern int norrath_trail_material __asm__("VITrailFx$sMaterialHandle");

int Length__C7VILList(void* list);
void SetModelView__8VIRasterRC10VIMatrix44(void* raster, void* matrix);
void SetMaterial__8VIRasteri(void* raster, int handle);
void BeginTriStrip__8VIRaster(void* raster);
void EndTriStrip__8VIRaster(void* raster);
void Color__8VIRasterRC10VIColor32F(void* raster, void* color);
void Vertex__8VIRasterRC7VIVect3(void* raster, void* vertex);

static void emitPair(void* raster, char* node, bool forward) {
    char* colorA = node + (forward ? 40 : 56);
    char* vertexA = node + (forward ? 16 : 28);
    char* colorB = node + (forward ? 56 : 40);
    char* vertexB = node + (forward ? 28 : 16);
    Color__8VIRasterRC10VIColor32F(raster, colorA);
    Vertex__8VIRasterRC7VIVect3(raster, vertexA);
    Color__8VIRasterRC10VIColor32F(raster, colorB);
    Vertex__8VIRasterRC7VIVect3(raster, vertexB);
}

static int pass(void* raster, char* trail, bool forward, int count) {
    char* node = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(trail + 28));
    while (*reinterpret_cast<unsigned*>(node) != 0) {
        float alpha = 1.0f - *reinterpret_cast<float*>(node + 72) / *reinterpret_cast<float*>(trail + 24);
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;
        *reinterpret_cast<float*>(node + 68) = alpha;
        *reinterpret_cast<float*>(node + 52) = alpha;
        emitPair(raster, node, forward);
        count += 2;
        char* next = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(node));
        if (count == 24) {
            EndTriStrip__8VIRaster(raster);
            count = 0;
            if (*reinterpret_cast<unsigned*>(next) != 0) {
                BeginTriStrip__8VIRaster(raster);
                count = 2;
                emitPair(raster, node, forward);
            }
        }
        node = next;
    }
    return count;
}

int Render__9VITrailFxR8VIRasterR8VICamera(void* self, void* raster, void* camera) {
    char* trail = static_cast<char*>(self);
    if (Length__C7VILList(trail + 28) < 2) return 0;
    SetModelView__8VIRasterRC10VIMatrix44(raster, static_cast<char*>(camera) + 48);
    SetMaterial__8VIRasteri(raster, norrath_trail_material);
    BeginTriStrip__8VIRaster(raster);
    int count = pass(raster, trail, true, 0);
    if (count != 0) EndTriStrip__8VIRaster(raster);
    BeginTriStrip__8VIRaster(raster);
    count = pass(raster, trail, false, 0);
    if (count != 0) EndTriStrip__8VIRaster(raster);
    return 0;
}
}
