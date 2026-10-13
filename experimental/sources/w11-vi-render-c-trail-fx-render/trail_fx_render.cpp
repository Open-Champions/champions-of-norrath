// Observed VITrailFxRender__FlR8VIRasterR8VICamera at 0x01027148, 240 bytes. Nothing is returned.
// The first argument (a time) is passed to Update only. The lists are the image's local lists
// published as `VITrailFx$sActiveTrailFx` (0x011e51f8) and `VITrailFx$sInactiveTrailFx`
// (0x011e5208); the first word of a list or node is its next link and a node whose next link is zero
// ends the walk (the list's own word 0 is the first node).
//   Active list: for each node n with a nonzero next word:
//       if Update__9VITrailFxl(n, time) > 0 then Render__9VITrailFxR8VIRasterR8VICamera(n, raster, camera).
//       The next node is read after those calls.
//   Inactive list: for each node n with a nonzero next word, the next link is read first, then
//       Update(n, time); if the result is positive Render(n, raster, camera); otherwise
//       Remove__7VILNode(n) and AddTail__7VILListP7VILNode(free list, n), the free list being
//       `VITrailFx$sFreeTrailFx` (0x011e51e8).
// Inferred from the shape: a nonpositive Update result means the trail has expired. Not established.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIRaster;
struct VICamera;

extern unsigned norrath_trail_free[4] __asm__("VITrailFx$sFreeTrailFx");
extern unsigned norrath_trail_active[4] __asm__("VITrailFx$sActiveTrailFx");
extern unsigned norrath_trail_inactive[4] __asm__("VITrailFx$sInactiveTrailFx");

int Update__9VITrailFxl(void* trail, long time);
void Render__9VITrailFxR8VIRasterR8VICamera(void* trail, VIRaster* raster, VICamera* camera);
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);

static unsigned next_of(unsigned node) { return *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(node)); }

void VITrailFxRender__FlR8VIRasterR8VICamera(long time, VIRaster* raster, VICamera* camera) {
    unsigned node = norrath_trail_active[0];
    while (next_of(node) != 0) {
        void* trail = NORRATH_TRANSLATE(node);
        if (Update__9VITrailFxl(trail, time) > 0) Render__9VITrailFxR8VIRasterR8VICamera(trail, raster, camera);
        node = next_of(node);
    }
    node = norrath_trail_inactive[0];
    unsigned next = next_of(node);
    while (next != 0) {
        void* trail = NORRATH_TRANSLATE(node);
        if (Update__9VITrailFxl(trail, time) > 0) {
            Render__9VITrailFxR8VIRasterR8VICamera(trail, raster, camera);
        } else {
            Remove__7VILNode(trail);
            AddTail__7VILListP7VILNode(norrath_trail_free, trail);
        }
        node = next;
        next = next_of(node);
    }
}
}
