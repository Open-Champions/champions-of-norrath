// Observed Push__6Camera at 0x0026f9b0, 108 bytes.
// Saves the current camera state into a one-deep slot. The halfword result of
// GetYaw__6Camera goes to camera+354 and that of GetPitch__6Camera to
// camera+356. The three floats of worldCameraPos (0x0037ee80, 12 bytes) are
// copied to camera+368, +372 and +376. GetZoomDistance__6Camera's float result
// goes to camera+360. Then SetMode__6Camerai is called with mode 0. Nothing is
// returned. Whether the slot is ever popped as an unchanged restore is not
// established here. Nothing here was executed on the EE.
extern "C" {
short GetYaw__6Camera(void* camera);
short GetPitch__6Camera(void* camera);
float GetZoomDistance__6Camera(void* camera);
void SetMode__6Camerai(void* camera, int mode);
extern unsigned char worldCameraPos[12];
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" void Push__6Camera(unsigned char* camera) {
    *reinterpret_cast<AliasedShort*>(camera + 354) = GetYaw__6Camera(camera);
    *reinterpret_cast<AliasedShort*>(camera + 356) = GetPitch__6Camera(camera);
    const AliasedFloat* pos = reinterpret_cast<AliasedFloat*>(worldCameraPos);
    *reinterpret_cast<AliasedFloat*>(camera + 368) = pos[0];
    *reinterpret_cast<AliasedFloat*>(camera + 372) = pos[1];
    *reinterpret_cast<AliasedFloat*>(camera + 376) = pos[2];
    *reinterpret_cast<AliasedFloat*>(camera + 360) = GetZoomDistance__6Camera(camera);
    SetMode__6Camerai(camera, 0);
}
