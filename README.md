# CoCoSDC Commander

CoCoSDC Commander is Cross-Platform Tool (DECB, OS-9, FLEX) for Commanding
your CoCoSDC. The same program runs on all 3 major Operating Systems on the
Tandy/TRS-80 Color Computer.

 Download the latest version of the program from the
[Releases](https://github.com/n6il/cocosdc-commander/releases) page:

* Disk Extended Color Basic: `sdccmdr-decb.dsk`
* OS-9/NitrOS-9: `sdccmdr-os9.DSK`
* Color FLEX: `FLSDCCMDR.SDF`

# Building for DECB

    make DECB=1

This produces `sdccmdr.dsk` containing `SDCCMDR.BIN`

# Building for OS9

    make sdccmdr-os9.DSK
    make update-os9

This copies the source code into a disk image `sdccmdr-os9.DSK`.
Attach this disk to a NitrOS-9 system and type `make` to build
the binary.

# Building for FLEX

The `SDCFLEX.DSK` disk image contains the FLEX source and can be built as
follows:

    ASN W=<DRIVE>
    CC SDCCMDR.C +DFLEX=1 +R
    CC LIBSDC.C +DFLEX=1 +R
    CC COMMSDC.C +DFLEX=1 +R
    CC STRING.C +DFLEX=1 +R
    CC SCREEN.C +DFLEX=1 +R
    CC SDCCMDR.R LIBSDC.R COMMSDC.R STRING.R SCREEN.R +DFLEX=1 +F=<DRIVE>.SDCCMDR.CMD
