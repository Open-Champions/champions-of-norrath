#include "registration.hpp"
extern "C" void norrath_game_object_registration_init() {
    // Both registered targets are full genuine source functions. No original
    // code addresses or machine-pointer/aggregate signature casts are used.
    norrath_object_set_create_by_name_function(norrath_game_create_by_name);
    norrath_object_set_filter_function(norrath_game_never_save_always_load_from_lump);
}
