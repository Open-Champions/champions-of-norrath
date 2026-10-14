// Observed: ProcessHierarchy__9VIHSprite at 0x01064598, 164 bytes. It makes a 64-byte VIMatrix44 on the stack with all sixteen floats zero,
// calls Identity__10VIMatrix44 on it, then calls ProcessHierarchy__9VIHSpriteiR10VIMatrix44(this, 0, matrix). It then calls the
// VIMatrix44 destructor _$_10VIMatrix44(matrix, 2). It returns -1 when the process call returned a negative value, otherwise 0.
// Inferred: this is the root entry that starts the recursive hierarchy walk at node 0 with an identity parent transform; the process
// result is reduced to success or failure.
// Unknown: the meaning of the destructor's second argument value 2, and what the walk does with the matrix.
extern "C" {
void Identity__10VIMatrix44(void* matrix);
int ProcessHierarchy__9VIHSpriteiR10VIMatrix44(void* self, int node, void* matrix);
void _$_10VIMatrix44(void* matrix, int flags);

int ProcessHierarchy__9VIHSprite(void* self) {
    float matrix[16];
    for (int i = 0; i < 16; ++i) matrix[i] = 0.0f;
    Identity__10VIMatrix44(matrix);
    int result = ProcessHierarchy__9VIHSpriteiR10VIMatrix44(self, 0, matrix);
    _$_10VIMatrix44(matrix, 2);
    return result < 0 ? -1 : 0;
}
}
