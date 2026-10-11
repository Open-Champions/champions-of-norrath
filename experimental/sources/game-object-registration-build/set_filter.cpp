#include "registration.hpp"
extern "C" void norrath_object_set_filter_function(NorrathNameFilter function) {
    const unsigned word = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(function));
    norrath_registration_detail::write_word(
        norrath_registration_detail::address(norrath_user_filter_function_word), word);
}
