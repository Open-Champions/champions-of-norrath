// Observed: AddRoot__9VIHSprite at 0x01064390, 108 bytes. Let V be this+64 (a vector object) and M this+76 (another). If the first word of
// V is non-zero it returns -1 without calling anything. Otherwise it calls Add__t8VIVector1Z13VIHSpriteNode(V); a negative result N is
// returned at once. Then it calls Add__t8VIVector1Z10VIMatrix44(M). A negative result from that returns -1; otherwise it returns N.
// Inferred: the sprite keeps a node vector at +64 and a matrix vector at +76 that stay the same length; a root is only added to an empty
// node vector, and the returned index is the new root node's index.
// Unknown: what the first word of the vector is (a count or a storage pointer) and whether a failed second add leaves the node added.
extern "C" {
int Add__t8VIVector1Z13VIHSpriteNode(void* vector);
int Add__t8VIVector1Z10VIMatrix44(void* vector);

int AddRoot__9VIHSprite(unsigned char* self) {
    if (*reinterpret_cast<unsigned*>(self + 64) != 0) return -1;
    int node = Add__t8VIVector1Z13VIHSpriteNode(self + 64);
    if (node < 0) return -1;
    if (Add__t8VIVector1Z10VIMatrix44(self + 76) < 0) return -1;
    return node;
}
}
