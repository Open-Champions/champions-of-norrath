// Observed worldToMap__3mapP6Point2PfT2 at 0x0015e0f0, 72 bytes (a local
// symbol of the map class in the original; here it has external linkage so
// that the separate revealMap unit can call it).
// Given a pair of floats `point` and two output floats: *out_x =
// _3map$mapOffset[0] (0x00483e80) + point[0] * 0.25 * k and *out_y =
// _3map$mapOffset[1] + point[1] * 0.25 * k, where k is a float literal held in
// a constant at 0x0047ff40 whose bits are 0x3d088888. In each product the
// multiplication by 0.25 is done first, then by k, then the offset is added.
// Nothing is returned. What k means is unknown. Nothing here was executed on
// the EE.
extern "C" float _3map$mapOffset[2];

extern "C" void worldToMap__3mapP6Point2PfT2(const float* point, float* out_x, float* out_y) {
    const float k = 0x1.11111p-5f;
    *out_x = _3map$mapOffset[0] + point[0] * 0.25f * k;
    *out_y = _3map$mapOffset[1] + point[1] * 0.25f * k;
}
