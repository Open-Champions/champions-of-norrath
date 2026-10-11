// Observed _vt$10GameObject at 0x44d498, 144 bytes in .data.
// Eighteen 8-byte slots. Each slot's first word is zero and its second word is
// a function address, except the last slot, whose second word is also zero.
// The constructor stores this object's address at object+72. These names are
// the observed symbols; their bodies are not in this package.

extern "C" {
void norrath_vt_tf_10GameObject();                 // __tf10GameObject
void norrath_vt_dtor_10GameObject();               // _$_10GameObject
void norrath_vt_msg_hurt_10GameObject();           // msg_hurt__10GameObjectfP10GameObjecti
void norrath_vt_msg_trigger_10GameObject();        // msg_trigger__10GameObjecti
void norrath_vt_msg_run_10GameObject();            // msg_run__10GameObject
void norrath_vt_msg_draw_10GameObject();           // msg_draw__10GameObject
void norrath_vt_msg_collision_10GameObject();      // msg_collision__10GameObjectP10GameObject
void norrath_vt_msg_route_10GameObject();          // msg_route__10GameObjecti
void norrath_vt_msg_load_10GameObject();           // msg_load__10GameObjectPc
void norrath_vt_msg_save_10GameObject();           // msg_save__10GameObjectPc
void norrath_vt_msg_collision_world_10GameObject(); // msg_collisionWorld__10GameObjectG6Point3
void norrath_vt_msg_move_collision_10GameObject(); // msg_moveCollision__10GameObjectPP10GameObject
void norrath_vt_msg_alert_10GameObject();          // msg_alert__10GameObjectifG6Point3P10GameObject
void norrath_vt_msg_get_poke_name_10GameObject();  // msg_getPokeName__10GameObjectPUsi
void norrath_vt_msg_poke_10GameObject();           // msg_poke__10GameObjecti
void norrath_vt_clone_10GameObject();              // clone__10GameObject
void norrath_vt_get_model_10GameObject();          // getModel__10GameObject
}

struct VtableSlot {
    unsigned leading_zero;
    void (*function)();
};

extern "C" {
VtableSlot norrath_game_object_vtable_view[18] = {
    {0, norrath_vt_tf_10GameObject},
    {0, norrath_vt_dtor_10GameObject},
    {0, norrath_vt_msg_hurt_10GameObject},
    {0, norrath_vt_msg_trigger_10GameObject},
    {0, norrath_vt_msg_run_10GameObject},
    {0, norrath_vt_msg_draw_10GameObject},
    {0, norrath_vt_msg_collision_10GameObject},
    {0, norrath_vt_msg_route_10GameObject},
    {0, norrath_vt_msg_load_10GameObject},
    {0, norrath_vt_msg_save_10GameObject},
    {0, norrath_vt_msg_collision_world_10GameObject},
    {0, norrath_vt_msg_move_collision_10GameObject},
    {0, norrath_vt_msg_alert_10GameObject},
    {0, norrath_vt_msg_get_poke_name_10GameObject},
    {0, norrath_vt_msg_poke_10GameObject},
    {0, norrath_vt_clone_10GameObject},
    {0, norrath_vt_get_model_10GameObject},
    {0, nullptr},
};
}
