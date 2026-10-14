// Observed: AttachSkin__9VIHSpriteiR7VIScene at 0x01063788, 92 bytes. With (this, id, scene) it calls ShareSprite__7VIScenei(scene, id).
// A negative result returns -1 and nothing else happens. Otherwise it builds a record on the stack whose first word is id and whose
// second word is -1 (the ShareSprite result is not stored) and returns Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment
// (this+96, record) unchanged. The third and fourth words of the 16-byte record are not written here.
// Inferred: the pool at this+96 holds the attachments (the 16-byte record size is the copy size in the pool copy-initialiser), the second word
// is an attach-point number where -1 means "none / skin", and the sprite is shared so the scene keeps it alive while it is attached.
// Unknown: what ShareSprite counts and the meaning of the two unwritten record words.
extern "C" {
int ShareSprite__7VIScenei(void* scene, int id);
int Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment(void* pool, const void* record);

int AttachSkin__9VIHSpriteiR7VIScene(unsigned char* self, int id, void* scene) {
    if (ShareSprite__7VIScenei(scene, id) < 0) return -1;
    unsigned record[4];
    record[0] = id;
    record[1] = static_cast<unsigned>(-1);
    return Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment(self + 96, record);
}
}
