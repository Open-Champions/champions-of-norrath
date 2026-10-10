# Contributing

This target begins with evidence collection. Read [the baseline checklist](targets/README.md)
and coordinate the intended edition and area before starting parallel work.

Never commit original executables, disc/BIOS images, extracted game assets, proprietary
SDK/compiler files, raw disassembly, raw decompiler output or game dialogue. Use a
private copy for analysis. Independently authored notes, metadata and readable C++
with documented provenance are welcome.

Every finding should state what was observed, inferred or remains unknown, with its
edition and address evidence. A proposed name is not proof of behavior. A byte match
requires freshly compiled complete-function comparison against the pinned baseline.
Runtime fixes require recorded execution; source inspection alone is insufficient.

Stage specific reviewed files, retain other contributors' work, and keep changes small.
The native client for both games lives in the [Godot repository](https://github.com/Open-Champions/open-champions-godot).
