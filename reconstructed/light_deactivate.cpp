// Observed intrusive links only, not the full Light object layout.
struct LightNode {
    LightNode* next;
    LightNode** previous_next_slot;
};
typedef char pointer_width[sizeof(void*) == 4 ? 1 : -1];
typedef char view_width[sizeof(LightNode) == 8 ? 1 : -1];

// node is a nonnull live writable view. A nonnull previous_next_slot addresses
// a live writable LightNode* slot, either a predecessor's next field or an
// independent list head. A nonnull next addresses a live writable node view.
// Node and standalone-slot storage do not overlap. Node aliases, self-links and
// shared neighbors are permitted; concurrent mutation is outside this contract.
extern "C" void norrath_light_deactivate(LightNode* node)
{
    if (node->previous_next_slot)
        *node->previous_next_slot = node->next;
    if (node->next)
        node->next->previous_next_slot = node->previous_next_slot;
    node->next = 0;
    node->previous_next_slot = 0;
}
