// Observed: Collide__6VIZoneR18VISceneCollRayVars at 0x01115dc8 is 164 bytes. If the word at
// zone offset 84 is positive it first calls SetPreTranslations__9VICollideiP7VIVect3 with the
// collide object (word at vars offset 16), that count and the word at zone offset 88; a negative
// result returns -1. Then, if the word at zone offset 68 is nonzero, it calls
// Collide__6VIZoneiR18VISceneCollRayVars(zone, 0, vars) and returns 1 when that is nonzero, else 0;
// if the word at offset 68 is zero it makes the same call with 0x40000000 as the second argument
// and returns whether the result is nonzero (1 or 0).
// Inferred: offsets 84/88 are a count and array of pre-translations; the second argument of the
// inner call is a tree node index/flag (0 for the root of one tree, 0x40000000 for the leaf flag
// seen in the plane tree of VIWorld). Unknown: the meaning of the offset-68 word and the exact type
// of the inner result beyond zero/nonzero.
extern "C" {
int SetPreTranslations__9VICollideiP7VIVect3(unsigned collide, int count, unsigned translations);
int Collide__6VIZoneiR18VISceneCollRayVars(unsigned char* zone, int node, unsigned char* vars);

int Collide__6VIZoneR18VISceneCollRayVars(unsigned char* zone, unsigned char* vars) {
    int count = *reinterpret_cast<int*>(zone + 84);
    if (count > 0) {
        if (SetPreTranslations__9VICollideiP7VIVect3(*reinterpret_cast<unsigned*>(vars + 16), count,
                                                     *reinterpret_cast<unsigned*>(zone + 88)) < 0)
            return -1;
    }
    if (*reinterpret_cast<int*>(zone + 68) != 0)
        return Collide__6VIZoneiR18VISceneCollRayVars(zone, 0, vars) != 0 ? 1 : 0;
    return Collide__6VIZoneiR18VISceneCollRayVars(zone, 0x40000000, vars) != 0 ? 1 : 0;
}
}
