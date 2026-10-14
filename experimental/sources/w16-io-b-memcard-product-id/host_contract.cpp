#include "product_id.cpp"

#include <cstdio>
#include <cstring>

extern "C" char sProductId[32] = "BASLUS-20470EQONLINE";

int main() {
    if (VIMemCardProductId__Fv() != sProductId) return 1;
    if (std::strcmp(VIMemCardProductId__Fv(), "BASLUS-20470EQONLINE") != 0) return 2;
    std::puts("memcard product-id passed");
    return 0;
}
