// Observed: GetActorTransform__7VISceneiR10VIMatrix44 at 0x011456b8, 44 bytes. The scene
// pointer is not used. The int argument is used as an address (it is added to directly,
// without scaling). The function calls the matrix's Transform with the 3-vector at
// address + 80, the float at address + 92, and the 3-vector at address + 68, then returns 0.
// Inferred: the int is the address of an actor-like record holding a position and an
// orientation; the output matrix is built from them.
// Unknown: the record type, which of the two vectors is position and which orientation,
// and why the parameter is declared as an int.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_address(int value);
#define NORRATH_ADDRESS(value) norrath_host_address(value)
#else
#define NORRATH_ADDRESS(value) reinterpret_cast<unsigned char*>(static_cast<long>(value))
#endif

extern "C" {
void Transform__10VIMatrix44RC7VIVect3fT1(void* matrix, const void* a, float angle, const void* b);

int GetActorTransform__7VISceneiR10VIMatrix44(void* scene, int actor, void* matrix) {
    (void)scene;
    unsigned char* record = NORRATH_ADDRESS(actor);
    Transform__10VIMatrix44RC7VIVect3fT1(matrix, record + 80, *reinterpret_cast<float*>(record + 92), record + 68);
    return 0;
}
}
