// Observed msg_trigger__11MissileTrapi at 0x1e5c78, 44 bytes. A nonzero argument
// tail-calls objectAddToRunList with the object; zero calls
// objectRemoveFromRunList with it. The result is whatever the called routine
// returns in V0. Inferred: those routines are the already defined
// norrath_actor_add_run_list and norrath_actor_remove_from_run_list.
extern "C" unsigned norrath_actor_add_run_list(void* object);
extern "C" unsigned norrath_actor_remove_from_run_list(void* object);
extern "C" unsigned msg_trigger__11MissileTrapi(void* self, int on) {
    if (on != 0) return norrath_actor_add_run_list(self);
    return norrath_actor_remove_from_run_list(self);
}
