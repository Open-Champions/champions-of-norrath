// Observed: Close__9VICollide at 0x01096e30 is 44 bytes. It calls
// Clear__9VICollide(this) and returns -1 if the result is negative, otherwise 0.
extern "C" {
int Clear__9VICollide(void* object);

int Close__9VICollide(void* object) {
    return Clear__9VICollide(object) < 0 ? -1 : 0;
}
}
