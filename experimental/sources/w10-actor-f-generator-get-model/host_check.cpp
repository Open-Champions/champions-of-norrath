// Host contract for getModel__9Generator. Includes the source; not an EE input.
#include "get_model.cpp"
int main() {
    unsigned char object[32] = {1, 2, 3};
    return getModel__9Generator(object) == 0 && getModel__9Generator(0) == 0 && object[2] == 3 ? 0 : 1;
}
