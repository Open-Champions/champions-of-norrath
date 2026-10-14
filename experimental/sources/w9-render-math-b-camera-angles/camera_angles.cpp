// Observed in the initialized data region of the executable:
//   myPitch (0x0048431e): 2 bytes, file value 0x2710 (10000), accessed with
//     signed halfword loads (lh) and halfword stores.
//   myRoll  (0x00484320): 2 bytes, file value 0, signed halfword loads and stores.
//   myFOV   (0x00484324): 4 bytes, file value 0x42480000 (50.0f as a float).
// Inferred: the camera pitch, roll and field of view in a view setup; the 16-bit
// values are likely the engine's integer angle unit. Unknown: units of pitch and
// roll (the 0x2710 default is only observed), and who last writes the values.
extern "C" {
short myPitch = 10000;
short myRoll = 0;
float myFOV = 50.0f;
}
