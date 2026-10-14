// Observed: Attach__9VIHSpriteiiR7VIScene at 0x010637e8, 100 bytes. With (this, id, point, scene) it calls ShareSprite__7VIScenei(scene, id).
// A negative result returns -1 and nothing else happens. Otherwise it builds a record on the stack whose first word is id and whose
// second word is point, and returns Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment(this+96, record) unchanged. The third and
// fourth words of the 16-byte record are not written here.
// Inferred: same pool and sharing as AttachSkin__9VIHSpriteiR7VIScene, which differs only in storing -1 for the second word.
// Unknown: what the second word selects (a node or attach-point number is a guess) and the meaning of the two unwritten record words.
extern "C" {
int ShareSprite__7VIScenei(void* scene, int id);
int Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment(void* pool, const void* record);

int Attach__9VIHSpriteiiR7VIScene(unsigned char* self, int id, int point, void* scene) {
    if (ShareSprite__7VIScenei(scene, id) < 0) return -1;
    unsigned record[4];
    record[0] = id;
    record[1] = point;
    return Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment(self + 96, record);
}
}
