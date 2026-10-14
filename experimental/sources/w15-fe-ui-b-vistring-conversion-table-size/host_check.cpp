#include "conversion_table_size.cpp"

int main() {
    if (norrath_conversion_table_size != 32) return 1;
    if (sizeof(norrath_conversion_table_size) != 4) return 2;
    return 0;
}
