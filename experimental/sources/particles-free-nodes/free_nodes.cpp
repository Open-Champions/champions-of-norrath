// freeNodes at 0x00486a6c, four .sbss bytes, next eqGroup.
// The image value is zero. Under -mlong32 this pointer is one word.
// object-infra publishes the same address as norrath_constructor_list_pool_view.
// This is the original name, so the link has two objects. No list has run.
extern "C" {
unsigned char* freeNodes;
}
