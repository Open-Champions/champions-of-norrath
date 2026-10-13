// Observed bossMeterName at 0x0034b988 (.data), 128 bytes. nm size 128 in .data; the image bytes are all zero. The authored creature-every-frame routine copies a name text into it with strcpy.
// Inferred: A text buffer for the boss health meter name.
// Unknown: Who reads it; the text limit.
extern "C" {
char bossMeterName[128];
}
