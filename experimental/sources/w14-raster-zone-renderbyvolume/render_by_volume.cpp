// Observed: RenderByVolume__6VIZoneR15VISceneRendVarsi at 0x011192b8 is 120 bytes in the pinned SLUS-20565 image. With the zone as
// this and a render-variables record (a1) it stores the third argument at +48 of the record, then calls
// SetPretranslations__6VIZoneR8VIRasterR8VICamera(zone, record word at +16, record word at +12); a negative result returns -1.
// It then calls RenderByVolume__6VIZoneiR15VISceneRendVars(zone, flags, record) with flags = 0 when the zone's word at +68 is
// nonzero and flags = 0x40000000 when it is zero. A negative result returns -1; otherwise it returns 0.
// Inferred: record +16 is a raster reference and +12 a camera reference, matching the callee's mangled parameter order (raster, then
// camera); the flag selects a traversal mode.
// Unknown: the flag meanings, the zone's word at +68 (w12-vimath-a records the zone's tree there), and the record size. Nothing here
// ran on the EE.
extern "C" {
int SetPretranslations__6VIZoneR8VIRasterR8VICamera(void* zone, unsigned raster, unsigned camera);
int RenderByVolume__6VIZoneiR15VISceneRendVars(void* zone, int flags, void* record);

int RenderByVolume__6VIZoneR15VISceneRendVarsi(unsigned char* zone, unsigned char* record, int third) {
    *reinterpret_cast<int*>(record + 48) = third;
    if (SetPretranslations__6VIZoneR8VIRasterR8VICamera(zone, *reinterpret_cast<unsigned*>(record + 16),
                                                        *reinterpret_cast<unsigned*>(record + 12)) < 0) {
        return -1;
    }
    int flags = *reinterpret_cast<int*>(zone + 68) != 0 ? 0 : 0x40000000;
    if (RenderByVolume__6VIZoneiR15VISceneRendVars(zone, flags, record) < 0) return -1;
    return 0;
}
}
