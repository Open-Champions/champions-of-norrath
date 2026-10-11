// Observed: player 0 status is the byte immediately after the local-player flag; records are 17632 bytes apart.
// Inferred: four records, from the same partners extent. Only that status byte belongs to this view.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char norrath_animation_dependency_view_4e6755[52897] = {};
}
static_assert(sizeof(norrath_animation_dependency_view_4e6755) == 52897, "complete object size");
