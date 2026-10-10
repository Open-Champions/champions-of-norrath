// Independently authored from the hash-pinned Norrath ELF.
// Original symbol: GetYaw__6Camera; address 0x0026fc40; complete extent 8 bytes.
// Reads a signed 16-bit value at receiver offset 0x68 and sign-extends it.
// This is a comparison view, not a recovered Camera class definition.
// Requires a valid aligned live CameraYawView object. Compatibility with the
// original class type/lifetime, null inputs and misaligned inputs is unverified.
struct CameraYawView {
    unsigned char unknown[0x68];
    short yaw;
};

static_assert(sizeof(short) == 2, "The comparison requires a 16-bit short.");
static_assert(sizeof(int) == 4, "The comparison requires a 32-bit int.");

extern "C" int norrath_camera_get_yaw(const CameraYawView* camera) {
    return camera->yaw;
}
