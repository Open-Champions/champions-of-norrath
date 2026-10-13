/* Observed layout of the three zero-initialised tty objects of the original
 * (local symbols tinfo at 0x008de610, wbuf at 0x008de640 and rbuf at
 * 0x008de780; the sizes below are the gaps to the next symbol, 48, 320 and not
 * established for rbuf). Field meanings are inferred from sceTtyInit,
 * sceTtyWrite and sceTtyRead; the field names are descriptive, not original.
 * The original reaches wbuf, rbuf through addresses OR-ed with 0x20000000.
 */
#ifndef W9_STARTUP_CRT_F_TTY_STATE_HPP
#define W9_STARTUP_CRT_F_TTY_STATE_HPP

struct TtyQueue {          // the 16-byte header of the local queue q.12 (0x008de500)
    int size;              // +0
    volatile int count;    // +4
    unsigned char* readPtr;  // +8
    unsigned char* writePtr; // +12
};

struct TtyInfo {           // 48 bytes
    int socket;            // +0  result of sceDeci2Open
    int length;            // +4  bytes still to send
    int received;          // +8  bytes received so far
    volatile int busy;     // +12 set while a send is in flight
    unsigned char* sendCursor; // +16
    unsigned char* receiveBuffer; // +20
    TtyQueue* queue;       // +24
    int unknown[5];        // +28..+47, not touched by the three functions
};

struct TtyPacketHeader {   // the first 12 bytes of wbuf
    unsigned short length; // +0
    unsigned short zero;   // +2
    unsigned short protocol; // +4
    unsigned char byte6;   // +6
    unsigned char byte7;   // +7
    unsigned int word8;    // +8
};

// The original reaches wbuf and rbuf through the uncached alias (address OR 0x20000000).
// A host check defines NORRATH_HOST_CONTRACT and gets the plain pointer instead.
#if defined(NORRATH_HOST_CONTRACT)
inline unsigned char* ttyUncached(unsigned char* pointer) { return pointer; }
#else
inline unsigned char* ttyUncached(unsigned char* pointer) {
    return reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned>(pointer) | 0x20000000u);
}
#endif

extern TtyInfo tinfo;
extern unsigned char wbuf[320];
extern unsigned char rbuf[];

#endif
