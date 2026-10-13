// Observed: CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44 at
// 0x010b2c98, 232 bytes, returns zero. Arguments after this: a node id, an actor number,
// the scene and the output matrix. A 64-byte local matrix is zero-filled (16-byte aligned).
// The slot word is the int at this + 3860 + id * 4. If the slot reads -1,
// GetActorTransform__7VISceneiR10VIMatrix44(scene, actor, output matrix) is called and the
// local matrix is unused. Otherwise GetActorTransform(scene, actor, local matrix) is called
// followed by CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(this, slot, local
// matrix, scene, output matrix). In both cases the local matrix destructor
// _$_10VIMatrix44(local, 2) runs last.
// Inferred: the slot word is a node index of the sprite (-1 meaning the slot has no node,
// so the actor transform is the world transform); the local matrix is the actor transform
// which CalcNodeTransform then composes. Unknown: member names and the enum of node ids.
extern "C" {
int GetActorTransform__7VISceneiR10VIMatrix44(void* scene, int actor, void* matrix);
int CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(void* sprite, int node, void* matrix, void* scene,
                                                           void* output);
void _$_10VIMatrix44(void* matrix, int flags);

int CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(void* sprite, int id, int actor,
                                                                               void* scene, void* output) {
    alignas(16) float local[16];
    for (int i = 0; i < 16; ++i) local[i] = 0.0f;
    int slot = reinterpret_cast<const int*>(static_cast<unsigned char*>(sprite) + 3860)[id];
    if (slot == -1) {
        GetActorTransform__7VISceneiR10VIMatrix44(scene, actor, output);
    } else {
        GetActorTransform__7VISceneiR10VIMatrix44(scene, actor, local);
        CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, slot, local, scene, output);
    }
    _$_10VIMatrix44(local, 2);
    return 0;
}
}
