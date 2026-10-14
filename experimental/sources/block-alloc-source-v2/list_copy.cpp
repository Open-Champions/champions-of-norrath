#include "byte_view.hpp"
extern "C" void norrath_list_set_copy_object(void* object) {
    using namespace norrath_block_view;
    write_word(address(norrath_list_copy_object_word), address(object));
}
