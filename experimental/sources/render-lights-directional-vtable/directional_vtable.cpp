// Observed _vt$21charDirectional_Light at 0x44d1c8, 144 bytes in .data.
// Eighteen 8-byte slots. Each slot's first word is zero and its second word
// is a function address, except the last slot, whose second word is also zero.
// The factory stores this object's address at object+72. Shared GameObject
// slots keep the names used by that vtable. These bodies are not in this unit.

extern "C" {
void norrath_vt_tf_21charDirectional_Light();
void norrath_vt_dtor_21charDirectional_Light();
void norrath_vt_msg_hurt_10GameObject();
void norrath_vt_msg_trigger_10GameObject();
void norrath_vt_msg_run_10GameObject();
void norrath_vt_msg_draw_10GameObject();
void norrath_vt_msg_collision_10GameObject();
void norrath_vt_msg_route_10GameObject();
void norrath_vt_msg_load_10GameObject();
void norrath_vt_msg_save_10GameObject();
void norrath_vt_msg_collision_world_10GameObject();
void norrath_vt_msg_move_collision_10GameObject();
void norrath_vt_msg_alert_10GameObject();
void norrath_vt_msg_get_poke_name_10GameObject();
void norrath_vt_msg_poke_10GameObject();
void norrath_vt_clone_21charDirectional_Light();
void norrath_vt_get_model_10GameObject();
}

struct VtableSlot {
    unsigned leading_zero;
    void (*function)();
};

extern "C" {
VtableSlot norrath_factory_directional_vtable[18] = {
    {0, norrath_vt_tf_21charDirectional_Light},
    {0, norrath_vt_dtor_21charDirectional_Light},
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
    {0, norrath_vt_clone_21charDirectional_Light},
    {0, norrath_vt_get_model_10GameObject},
    {0, nullptr},
};
}
