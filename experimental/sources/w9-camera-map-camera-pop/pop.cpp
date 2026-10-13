// Observed Pop__6Camera at 0x0026fa20, 104 bytes.
// Restores the slot written by Push__6Camera, in this order: SetMode__6Camerai
// with mode 1; SetYaw__6Camerai with the signed halfword at camera+354;
// SetPitch__6Camerai with the signed halfword at camera+356;
// BuildMatrix__6CamerafffP6Point3 with the floats at camera+368, +372 and +376
// and a null point; SetZoomDistance__6Cameraf with the float at camera+360.
// Nothing is returned and the slot is not cleared. Nothing here was executed on
// the EE.
extern "C" {
void SetMode__6Camerai(void* camera, int mode);
void SetYaw__6Camerai(void* camera, int yaw);
void SetPitch__6Camerai(void* camera, int pitch);
void BuildMatrix__6CamerafffP6Point3(void* camera, float a, float b, float c, void* point);
void SetZoomDistance__6Cameraf(void* camera, float distance);
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" void Pop__6Camera(unsigned char* camera) {
    SetMode__6Camerai(camera, 1);
    SetYaw__6Camerai(camera, *reinterpret_cast<AliasedShort*>(camera + 354));
    SetPitch__6Camerai(camera, *reinterpret_cast<AliasedShort*>(camera + 356));
    BuildMatrix__6CamerafffP6Point3(camera, *reinterpret_cast<AliasedFloat*>(camera + 368),
                                    *reinterpret_cast<AliasedFloat*>(camera + 372),
                                    *reinterpret_cast<AliasedFloat*>(camera + 376), nullptr);
    SetZoomDistance__6Cameraf(camera, *reinterpret_cast<AliasedFloat*>(camera + 360));
}
