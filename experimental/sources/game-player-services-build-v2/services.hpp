#pragma once
#include "player_inventory.hpp"
#include "creature_animation.hpp"
extern "C" {
unsigned norrath_factory_strlen(const char* text); // Canonical genuine compiled leaf, no response.
void norrath_animation_update_component(void* component);
extern const unsigned char norrath_player_services_view_48068c[];
char* norrath_factory_strchr(const char*,int);
}
