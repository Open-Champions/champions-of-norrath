// Host comparison only. Not an EE build input.
#include "typeinfo_eq.cpp"

#include <cstdio>
#include <cstring>

int main() {
    // Each fake typeinfo object is one word: a pointer to its name.
    const char prop[] = "4Prop";
    const char prop_copy[] = "4Prop";
    const char item[] = "8ItemProp";
    const char empty_a[] = "";
    const char empty_b[] = "";
    const char* t_prop = prop;
    const char* t_prop_copy = prop_copy;
    const char* t_item = item;
    const char* t_empty_a = empty_a;
    const char* t_empty_b = empty_b;
    // Identical pointers are equal without reading any name.
    if (__eq__C9type_infoRC9type_info(&t_prop, &t_prop) != 1) return 1;
    // Distinct objects with equal names are equal.
    if (__eq__C9type_infoRC9type_info(&t_prop, &t_prop_copy) != 1) return 2;
    // A shared prefix is not enough.
    if (__eq__C9type_infoRC9type_info(&t_prop, &t_item) != 0) return 3;
    // Empty names compare equal; empty against nonempty does not.
    if (__eq__C9type_infoRC9type_info(&t_empty_a, &t_empty_b) != 1) return 4;
    if (__eq__C9type_infoRC9type_info(&t_empty_a, &t_prop) != 0) return 5;
    if (__eq__C9type_infoRC9type_info(&t_item, &t_empty_a) != 0) return 6;
    return 0;
}
