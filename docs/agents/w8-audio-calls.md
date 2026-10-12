# Sound and music calls (w8-audio-calls)

Addresses, sizes, and instruction behavior below were read from the pinned
SLUS-20565 image on this machine. Observed means read from those bytes.
Inferred means deduced from callers. Unknown means not established. Nothing
here ran on the EE. Compiled bodies are not claimed as original-byte matches.

Compile check used the reviewed `authored_cpp` flags with the toolchain
at `work/toolchains/ee`. Data units are verified against the executable with
`docs/agents/w8-audio-calls-data-check.py`.

## Sound state data

Unit `sources/w8-audio-calls-sound-state/sound_state.cpp`. Defines
`streamVolume`, `streaming`, `streamOutput`, `streamSizeLeft`,
`musicPlaying`, `musicStart`, `emergencyMusicWaitAbort`, `musicDuckAmount`,
`soundFrameNm`, `sequenceInProgress`, `sequenceVolume`, `musicTimer`,
`disableSoundPlaying`, `sequence`, `delaySoundList`, `stream`. Sizes and
initial bytes of all sixteen match the image (checker output: sixteen `ok`).
The same storage also has `norrath_sound_*_view` names in the audio package;
both sets cannot be linked into one image without choosing one name per object.
