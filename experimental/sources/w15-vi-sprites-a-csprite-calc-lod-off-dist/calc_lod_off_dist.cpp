// Observed: CalcLODOffDist__9VICSpriteR7VIScene at 0x010b2f90, 128 bytes. Arguments (this, scene). Let id be the signed word at this+3856.
// When id is not -1 it calls GetSpriteType__7VIScenei(scene, id); if that is 7, or (after re-reading this+3856 and calling GetSpriteType
// again) is 3, it calls Sprite__7VIScenei(scene, this+3856's value) and copies the float at +48 of the returned record to this+48. It
// returns zero in every case and writes nothing else.
// Inferred: this+3856 is the index of a related scene sprite (-1 for none), and +48 holds the LOD switch-off distance that is taken over
// from it for those two sprite kinds. The type numbers 7 and 3 are as read; their names are not known.
// Unknown: what kinds 3 and 7 are, and what the returned record type is beyond the float at +48.
extern "C" {
int GetSpriteType__7VIScenei(void* scene, int id);
unsigned char* Sprite__7VIScenei(void* scene, int id);

int CalcLODOffDist__9VICSpriteR7VIScene(void* object, void* scene) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    int id = *reinterpret_cast<int*>(bytes + 3856);
    if (id != -1) {
        if (GetSpriteType__7VIScenei(scene, id) == 7 || GetSpriteType__7VIScenei(scene, *reinterpret_cast<int*>(bytes + 3856)) == 3)
            *reinterpret_cast<float*>(bytes + 48) = *reinterpret_cast<float*>(Sprite__7VIScenei(scene, *reinterpret_cast<int*>(bytes + 3856)) + 48);
    }
    return 0;
}
}
