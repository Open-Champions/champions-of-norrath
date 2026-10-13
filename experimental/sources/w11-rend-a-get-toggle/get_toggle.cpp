// Observed GetToggle__6Camera at 0x0026fc58, 8 bytes. Returns the 32-bit word at
// camera+84 unchanged. What the word selects is not established here; callers that
// compare it with 2 are in doJumpEffect. Nothing here was executed on the EE.
extern "C" int GetToggle__6Camera(unsigned char* camera) {
    return *reinterpret_cast<int*>(camera + 84);
}
