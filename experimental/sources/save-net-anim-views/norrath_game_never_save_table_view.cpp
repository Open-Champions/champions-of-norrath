// Observed: neverSave is 76 bytes, eighteen name pointers and a null word.
// The two clearColor entries are the same address, not two copies.
// Inferred: nothing. 76 divided by 4 is the whole object.
// Unknown: whether a later write replaces any of these pointers.
// The name at the faytree slot has no separate import. The other names are
// the factory text objects already referenced by the object factory.

extern "C" {
extern const char norrath_factory_text_483b38[];
extern const char norrath_factory_text_483b40[];
extern const char norrath_factory_text_483b48[];
extern const char norrath_factory_text_461fc0[];
extern const char norrath_factory_text_483b50[];
extern const char norrath_factory_text_461fd0[];
extern const char norrath_factory_text_461fe0[];
extern const char norrath_factory_text_483b60[];
extern const char norrath_factory_text_483b68[];
extern const char norrath_factory_text_461ff0[];
extern const char norrath_factory_text_462000[];
extern const char norrath_factory_text_462010[];
extern const char norrath_factory_text_462028[];
extern const char norrath_factory_text_462040[];
extern const char norrath_factory_text_462058[];
extern const char norrath_factory_text_462068[];
}

extern "C" {
const char* norrath_game_never_save_table_view[19] = {
    norrath_factory_text_483b38,
    norrath_factory_text_483b40,
    norrath_factory_text_483b48,
    norrath_factory_text_461fc0,
    norrath_factory_text_483b50,
    norrath_factory_text_461fd0,
    norrath_factory_text_461fe0,
    "faytree",
    norrath_factory_text_483b60,
    norrath_factory_text_483b68,
    norrath_factory_text_461ff0,
    norrath_factory_text_462000,
    norrath_factory_text_462010,
    norrath_factory_text_462028,
    norrath_factory_text_462040,
    norrath_factory_text_462058,
    norrath_factory_text_462058,
    norrath_factory_text_462068,
    nullptr,
};
}
static_assert(sizeof(norrath_game_never_save_table_view) == 76,
              "eighteen pointers and one null word");
