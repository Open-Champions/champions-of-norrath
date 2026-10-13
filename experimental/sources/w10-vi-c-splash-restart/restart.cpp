// Observed: RestartConnect__11VIWndSplash at 0x010d6f88 is 64 bytes. With the object as this it
// calls EndConnect__12VIWndConnect(this+416), then stores 0 at (this+416)+28, this+2436 and
// this+4804. Nothing is returned.
// Inferred: this+2436 and this+4804 are the same +28 word of the Options and Message children
// (this+2408 and this+4776); that is arithmetic on the offsets seen in Init__11VIWndSplashP7VIScene.
extern "C" {
void EndConnect__12VIWndConnect(void* self);

void RestartConnect__11VIWndSplash(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* connect = bytes + 416;
    EndConnect__12VIWndConnect(connect);
    *reinterpret_cast<unsigned*>(connect + 28) = 0;
    *reinterpret_cast<unsigned*>(bytes + 2436) = 0;
    *reinterpret_cast<unsigned*>(bytes + 4804) = 0;
}
}
