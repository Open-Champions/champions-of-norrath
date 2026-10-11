#include "dispatch.hpp"
extern "C" void norrath_object_set_create_by_name_function(NorrathCreateByName function) {
    const unsigned word = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(function));
    norrath_create_by_name_function_word[0] = static_cast<unsigned char>(word);
    norrath_create_by_name_function_word[1] = static_cast<unsigned char>(word >> 8);
    norrath_create_by_name_function_word[2] = static_cast<unsigned char>(word >> 16);
    norrath_create_by_name_function_word[3] = static_cast<unsigned char>(word >> 24);
}
