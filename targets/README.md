# Establishing a baseline

No baseline manifest is populated yet. A future target must independently record:

- Disc region, release/version and executable filename from the contributor's copy.
- Executable SHA-256, ELF machine, entry point, section bounds and load layout.
- Reproducible function inventory and its limits; validate the first candidate's boundary manually.
- Compiler, ABI, relocation and linker evidence, including unresolved questions.
- Private input paths and acquisition steps in local notes, never published original files.
- A sanitized verification receipt for the first complete source-derived function.

Keep edition-specific targets separate. Do not put placeholder zeros or the Return to
Arms executable hash into a manifest to make a verifier run. A missing baseline means
progress is unmeasured; it does not mean 0 of Return to Arms' 7,732 functions.

The [Return to Arms workflow](https://github.com/Open-Champions/champions-return-to-arms/blob/main/docs/REBUILD.md)
is a method reference. Its addresses, formats and compiler assumptions must be verified
independently for this game before reuse.
