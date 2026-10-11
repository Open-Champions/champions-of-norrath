extern "C" {
extern unsigned char skillWizardBeam[104];
extern unsigned char skillRegeneration[104];
extern unsigned char skillSlashing[104];
extern unsigned char* norrath_player_skill_catalog_view[63];
}
int main() {
    if (norrath_player_skill_catalog_view[0] != nullptr) return 1;
    if (norrath_player_skill_catalog_view[1] != skillWizardBeam) return 2;
    if (norrath_player_skill_catalog_view[19] != skillRegeneration) return 3;
    if (norrath_player_skill_catalog_view[61] != skillSlashing) return 4;
    if (norrath_player_skill_catalog_view[62] != nullptr) return 5;
    if (sizeof skillRegeneration != 104u || skillRegeneration[0] != 0 || skillRegeneration[103] != 0) return 6;
    return 0;
}
