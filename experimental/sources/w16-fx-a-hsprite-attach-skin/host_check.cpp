// Host check only; not a build input.
#include "attach_skin.cpp"
static int shareRet, addRet, shareCalls, addCalls;
static int shareId;
static void* scenePtr;
static const void* poolPtr;
static unsigned rec0, rec1;
extern "C" int ShareSprite__7VIScenei(void* scene, int id) { ++shareCalls; scenePtr = scene; shareId = id; return shareRet; }
extern "C" int Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment(void* pool, const void* r) {
    ++addCalls; poolPtr = pool; rec0 = static_cast<const unsigned*>(r)[0]; rec1 = static_cast<const unsigned*>(r)[1]; return addRet;
}
int main() {
    alignas(8) static unsigned char object[128], scene[8];
    shareRet = 0; addRet = 17;
    if (AttachSkin__9VIHSpriteiR7VIScene(object, 42, scene) != 17) return 1;
    if (shareCalls != 1 || addCalls != 1 || scenePtr != scene || shareId != 42 || poolPtr != object + 96) return 2;
    if (rec0 != 42 || rec1 != 0xffffffffu) return 3;
    shareRet = -1; addCalls = 0;
    if (AttachSkin__9VIHSpriteiR7VIScene(object, 1, scene) != -1 || addCalls != 0) return 4;
    return 0;
}
