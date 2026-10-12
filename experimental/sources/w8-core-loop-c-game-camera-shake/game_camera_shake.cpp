// gameCameraShake at 0x00484f64, 4 bytes, in SLUS_205.65.
// Observed: zero-initial word in .data accessed with float loads and stores (gameSetCameraShake compares it with its float argument).
extern "C" {
float gameCameraShake = 0.0f;
}
