#pragma once
// These declarations match the existing level-restoration caller exactly.
extern "C" {
void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask);
void norrath_list_safe_iterator_next(unsigned char* iterator);
void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags);
// Existing genuine __builtin_delete provider; no new binding or body.
void norrath_animation_state_deallocate(void* allocation);
}
