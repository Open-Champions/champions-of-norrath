// Observed: Clear__9VIHSprite at 0x0105f9f0, 160 bytes. It calls, in this order, the Clear routine of six embedded containers and returns
// -1 as soon as one of them returns a negative value: Clear__t8VIVector1Z13VIHSpriteNode(this+64), Clear__t8VIVector1Z10VIMatrix44(this+76),
// Clear__t7VIArray1Z16VIHSpriteTrigger(this+88), Clear__t6VIPool1Z19VIHSpriteAttachment(this+96), Clear__t6VIPool1Z13VIHSpritePlay(this+120)
// and Clear__t6VIPool1Z17VIHSpritePlayNode(this+144). When all succeed it stores -1 at this+388 and this+384 and the float 1.0 at this+208,
// and returns zero. Nothing else is written; an early return leaves the later containers and the three fields untouched.
// Inferred: the sprite holds these containers by value at the stated offsets, and the three fields are reset values (two indices that
// mean "none" and a scale of one); their names are not known.
// Unknown: the purpose of the fields at +384, +388 and +208.
extern "C" {
int Clear__t8VIVector1Z13VIHSpriteNode(void* container);
int Clear__t8VIVector1Z10VIMatrix44(void* container);
int Clear__t7VIArray1Z16VIHSpriteTrigger(void* container);
int Clear__t6VIPool1Z19VIHSpriteAttachment(void* container);
int Clear__t6VIPool1Z13VIHSpritePlay(void* container);
int Clear__t6VIPool1Z17VIHSpritePlayNode(void* container);

int Clear__9VIHSprite(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (Clear__t8VIVector1Z13VIHSpriteNode(bytes + 64) < 0) return -1;
    if (Clear__t8VIVector1Z10VIMatrix44(bytes + 76) < 0) return -1;
    if (Clear__t7VIArray1Z16VIHSpriteTrigger(bytes + 88) < 0) return -1;
    if (Clear__t6VIPool1Z19VIHSpriteAttachment(bytes + 96) < 0) return -1;
    if (Clear__t6VIPool1Z13VIHSpritePlay(bytes + 120) < 0) return -1;
    if (Clear__t6VIPool1Z17VIHSpritePlayNode(bytes + 144) < 0) return -1;
    *reinterpret_cast<int*>(bytes + 388) = -1;
    *reinterpret_cast<int*>(bytes + 384) = -1;
    *reinterpret_cast<float*>(bytes + 208) = 1.0f;
    return 0;
}
}
