// Observed SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi at 0x01145948, 92 bytes.
// Three calls in order, none of whose results is used:
//   SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend(scene, camera, blend)
//   SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(scene, blend, flags)
//   SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend(scene, blend)
// The result is zero. The last argument is an unsigned integer whose meaning is not established
// here; it is forwarded only to the fog step.
extern "C" {
struct VIScene;
struct VICamera;
struct VITerrainProfileBlend;
int SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend(VIScene* scene, VICamera* camera,
                                                                     VITerrainProfileBlend* blend);
int SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(VIScene* scene, VITerrainProfileBlend* blend,
                                                         unsigned flags);
int SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend(VIScene* scene, VITerrainProfileBlend* blend);

int SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi(VIScene* scene, VICamera* camera,
                                                                 VITerrainProfileBlend* blend,
                                                                 unsigned flags) {
    SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend(scene, camera, blend);
    SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(scene, blend, flags);
    SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend(scene, blend);
    return 0;
}
}
