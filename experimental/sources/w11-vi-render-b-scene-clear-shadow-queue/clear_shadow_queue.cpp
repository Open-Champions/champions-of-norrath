// Observed: ClearShadowQueue__7VIScene at 0x01144ab8 is 8 bytes. It stores a zero word at
// offset 6792 of the scene and returns nothing.
// Inferred: the word at 6792 is the shadow queue's count or head; no reader is checked here.
// Unknown: the queue's element type and capacity.
extern "C" {
struct VIScene;

void ClearShadowQueue__7VIScene(VIScene* scene) {
    *reinterpret_cast<unsigned*>(reinterpret_cast<char*>(scene) + 6792) = 0;
}
}
