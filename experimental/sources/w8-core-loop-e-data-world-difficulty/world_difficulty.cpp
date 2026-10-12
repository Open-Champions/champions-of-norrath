// Observed worldDifficulty at 0x00484cac, four bytes in .sdata. The file word
// is 0x3f800000, the float 1.0. Its meaning beyond the name is unknown.
extern "C" {
float worldDifficulty = 1.0f;
}
