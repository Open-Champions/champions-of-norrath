// Observed SetNoTargetUpdates__6Camerai at 0x0026fc28, 8 bytes. Stores the low byte of
// the integer argument at camera+364. Nothing is read or returned. The byte's meaning
// beyond its name is not established here (doJumpEffect passes 5). Nothing here was
// executed on the EE.
extern "C" void SetNoTargetUpdates__6Camerai(unsigned char* camera, int value) {
    camera[364] = static_cast<unsigned char>(value);
}
