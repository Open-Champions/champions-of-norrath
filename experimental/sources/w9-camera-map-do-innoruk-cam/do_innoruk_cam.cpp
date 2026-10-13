// Observed doInnorukCam__6Camera at 0x0026f7a0, 424 bytes.
// Looks up an object by name with objectFindObject__FPcPP10GameObjecti
// ("InnoruukB", a local pointer that starts unset, count 10). When no object
// is found nothing else happens. Otherwise, with obj the found object (its
// position floats are at +16, +20, +24), pos the three floats of worldCameraPos
// (0x0037ee80), m the floats of worldCameraMatrix (0x0037ef00), and frame the
// word frameNm (0x004844a4):
//   1. angle = (frame * 80) % 65536 (C remainder); pitch float =
//      icos__Fi(angle) * 1447.0 + 8725.0.
//   2. The camera halfword at +104 gets 33 added (wrapping to 16 bits) and the
//      halfword at +106 is set to the pitch float truncated to an int.
//   3. BuildMatrix__6CamerafffP6Point3(camera, pos.x + (obj.x - pos.x),
//      pos.y + (obj.y - pos.y), pos.z + ((obj.z + 131.0) - pos.z), null).
//   4. angle2 = (frame * 81) % 65536; d = isin__Fi(angle2) * 94.0 + 279.0.
//   5. BuildMatrix__6CamerafffP6Point3(camera, x - d * m[2], y - d * m[5],
//      z - d * m[8], null), where x, y and z are the three values passed in
//      step 3. m is read after the first BuildMatrix call.
// The float constants are the file words 0x44b4e000, 0x46085400, 0x438b8000,
// 0x43030000 and 0x42bc0000 (1447, 8725, 279, 131 and 94). The meaning of the fields is not established. Nothing here was
// executed on the EE.
extern "C" {
void objectFindObject__FPcPP10GameObjecti(const char* name, void** out, int count);
float icos__Fi(int angle);
float isin__Fi(int angle);
void BuildMatrix__6CamerafffP6Point3(void* camera, float a, float b, float c, void* point);
extern float worldCameraPos[3];
extern float worldCameraMatrix[12];
extern int frameNm;
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" void doInnorukCam__6Camera(unsigned char* camera) {
    void* found = nullptr;
    objectFindObject__FPcPP10GameObjecti("InnoruukB", &found, 10);
    if (found == nullptr) return;
    const unsigned char* obj = static_cast<const unsigned char*>(found);

    const float pitch = icos__Fi((frameNm * 80) % 65536) * 1447.0f + 8725.0f;
    const float px = worldCameraPos[0];
    const float py = worldCameraPos[1];
    const float pz = worldCameraPos[2];
    const float dx = *reinterpret_cast<const AliasedFloat*>(obj + 16) - px;
    *reinterpret_cast<AliasedShort*>(camera + 104) =
        static_cast<short>(*reinterpret_cast<AliasedShort*>(camera + 104) + 33);
    *reinterpret_cast<AliasedShort*>(camera + 106) = static_cast<short>(static_cast<int>(pitch));
    float x = px + dx;
    const float dy = *reinterpret_cast<const AliasedFloat*>(obj + 20) - py;
    float y = py + dy;
    const float dz = (*reinterpret_cast<const AliasedFloat*>(obj + 24) + 131.0f) - pz;
    float z = pz + dz;
    BuildMatrix__6CamerafffP6Point3(camera, x, y, z, nullptr);

    const float d = isin__Fi((frameNm * 81) % 65536) * 94.0f + 279.0f;
    z = z - d * worldCameraMatrix[8];
    x = x - d * worldCameraMatrix[2];
    y = y - d * worldCameraMatrix[5];
    BuildMatrix__6CamerafffP6Point3(camera, x, y, z, nullptr);
}
