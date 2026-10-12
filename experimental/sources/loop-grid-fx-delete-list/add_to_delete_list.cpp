// Observed objectAddToDeleteList__FP10GameObject at 0x1437e8, extent 68.
// Detaches the node word at object+4 when it is nonzero, then links the object
// itself (not object+4) onto the delete list head at 0x486880 (deleteList,
// 8 bytes, reached through gp). Both link value arguments are the object. The
// result is whatever listOnewayLink returned. listRemove and listOnewayLink
// stay imports.
extern "C" unsigned norrath_list_remove(unsigned char* node);
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);
extern "C" unsigned char norrath_restore_broadcast_head_view[];

extern "C" unsigned norrath_object_add_to_delete_list(void* object) {
    const unsigned char* bytes = static_cast<const unsigned char*>(object);
    const unsigned existing = unsigned(bytes[4]) | (unsigned(bytes[5]) << 8) |
                              (unsigned(bytes[6]) << 16) | (unsigned(bytes[7]) << 24);
    if (existing != 0u) (void)norrath_list_remove(reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(existing)));
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(
        norrath_list_oneway_link(norrath_restore_broadcast_head_view, object, object)));
}
