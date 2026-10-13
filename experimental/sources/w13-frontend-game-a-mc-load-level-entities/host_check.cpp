// Host check entry for MC_LoadLevelEntities__FPcT0. Not an EE plan input.
// The authored source to plan is w11-front-a-mc-load-level-entities/mc_load_level_entities.cpp
// (the w10 copy of the same function reads 32-bit pointer words as host pointers and cannot be
// checked on a 64-bit host). Its committed check needs -DW11_FRONT_A_HOST; this unit sets that
// switch itself so a plain build works:
//   c++ -std=c++17 -Wall -Wextra -Werror -Wno-deprecated-declarations -fno-builtin host_check.cpp
// A repeated paired run of the w11 source against the original image shows no differences.
#define W11_FRONT_A_HOST 1
#include "../w11-front-a-mc-load-level-entities/host_check.cpp"
