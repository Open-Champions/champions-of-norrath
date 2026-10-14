// Observed: Collide__6VIZoneR8VIEnvRayR7VIVect3T2 at 0x01119770 is 112 bytes. Arguments: zone, ray
// and two vector references. It calls Collide__6VIZoneiR8VIEnvRayR7VIVect3T3(zone, node, ray, v1, v2)
// once, with node 0 when the word at zone offset 68 is nonzero and 0x40000000 when it is zero, and
// returns 1 when that result is nonzero, otherwise 0.
// Inferred: the same node-selection as the VISceneCollRayVars overload, minus the pre-translation
// step. Unknown: what the two vectors are (they are passed through untouched).
extern "C" {
int Collide__6VIZoneiR8VIEnvRayR7VIVect3T3(unsigned char* zone, int node, void* ray, void* v1, void* v2);

int Collide__6VIZoneR8VIEnvRayR7VIVect3T2(unsigned char* zone, void* ray, void* v1, void* v2) {
    int node = *reinterpret_cast<int*>(zone + 68) != 0 ? 0 : 0x40000000;
    return Collide__6VIZoneiR8VIEnvRayR7VIVect3T3(zone, node, ray, v1, v2) != 0 ? 1 : 0;
}
}
