// Observed dialogMsgHandle at 0x00486b90 and hudMsgHandle at 0x00486aec.
// Both ELF sizes are 0 and both are .sbss. curScriptPtr begins four bytes
// after dialogMsgHandle. attemptLoadGame begins four bytes after
// hudMsgHandle. Each slot is therefore four bytes and starts at zero.
extern "C" int dialogMsgHandle = 0;
extern "C" int hudMsgHandle = 0;
