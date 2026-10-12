// Observed __8ListHeadRC8ListHead at 0x194498, extent 8. The copy constructor
// stores zero in the first word of the destination and never reads the source.
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    (void)source;
    *static_cast<unsigned*>(destination) = 0u;
}
