// Observed: _vt$16CosmeticPropAnim is 144 bytes. Each slot is a zero word
// followed by a pointer. The final slot's pointer is zero.
// Inferred: nothing about a C++ language declaration. The slot width is the
// repeated 8-byte pattern, and 144 / 8 is 18 slots.
// Unknown: a caller that reads these slots. Current factory code only stores
// the address of this table. The targets below are unresolved object methods.

extern "C" {
extern const unsigned char __tf16CosmeticPropAnim[];
extern const unsigned char _$_16CosmeticPropAnim[];
extern const unsigned char msg_hurt__10GameObjectfP10GameObjecti[];
extern const unsigned char msg_trigger__10GameObjecti[];
extern const unsigned char msg_run__16CosmeticPropAnim[];
extern const unsigned char msg_draw__16CosmeticPropAnim[];
extern const unsigned char msg_collision__10GameObjectP10GameObject[];
extern const unsigned char msg_route__10GameObjecti[];
extern const unsigned char msg_load__16CosmeticPropAnimPc[];
extern const unsigned char msg_save__16CosmeticPropAnimPc[];
extern const unsigned char msg_collisionWorld__10GameObjectG6Point3[];
extern const unsigned char msg_moveCollision__10GameObjectPP10GameObject[];
extern const unsigned char msg_alert__10GameObjectifG6Point3P10GameObject[];
extern const unsigned char msg_getPokeName__10GameObjectPUsi[];
extern const unsigned char msg_poke__10GameObjecti[];
extern const unsigned char clone__16CosmeticPropAnim[];
extern const unsigned char getModel__10GameObject[];
}

struct NorrathAnimationVtableSlot {
    unsigned leading_zero;
    const unsigned char* target;
};

extern "C" {
NorrathAnimationVtableSlot norrath_factory_animation_vtable[18] = {
    {0u, __tf16CosmeticPropAnim},
    {0u, _$_16CosmeticPropAnim},
    {0u, msg_hurt__10GameObjectfP10GameObjecti},
    {0u, msg_trigger__10GameObjecti},
    {0u, msg_run__16CosmeticPropAnim},
    {0u, msg_draw__16CosmeticPropAnim},
    {0u, msg_collision__10GameObjectP10GameObject},
    {0u, msg_route__10GameObjecti},
    {0u, msg_load__16CosmeticPropAnimPc},
    {0u, msg_save__16CosmeticPropAnimPc},
    {0u, msg_collisionWorld__10GameObjectG6Point3},
    {0u, msg_moveCollision__10GameObjectPP10GameObject},
    {0u, msg_alert__10GameObjectifG6Point3P10GameObject},
    {0u, msg_getPokeName__10GameObjectPUsi},
    {0u, msg_poke__10GameObjecti},
    {0u, clone__16CosmeticPropAnim},
    {0u, getModel__10GameObject},
    {0u, nullptr},
};
}
static_assert(sizeof(norrath_factory_animation_vtable) == 144, "eighteen 8-byte slots");
