// Observed: Init__9VIHSprite at 0x01063480, 44 bytes. It calls Clear__9VIHSprite(this); a negative result returns -1, any other result
// returns 0.
// Inferred: initialisation is a plain reset; no other state is touched here.
extern "C" {
int Clear__9VIHSprite(void* object);

int Init__9VIHSprite(void* object) {
    return Clear__9VIHSprite(object) < 0 ? -1 : 0;
}
}
