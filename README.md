# CoCoSDC Commander

CoCoSDC Commander is Cross-Platform Tool (DECB, OS-9, FLEX) for Commanding
your CoCoSDC. The same program runs on all 3 major Operating Systems on the
Tandy/TRS-80 Color Computer.

 Download the latest version of the program from the
[Releases](https://github.com/n6il/cocosdc-commander/releases) page:

* Disk Extended Color Basic: `sdccmdr-decb.dsk`
* OS-9/NitrOS-9: `sdccmdr-os9.DSK`
* Color FLEX: `FLSDCCMDR.SDF`

# One Program? Three Operating Systems?

Isn't that Impossible?

Well, no it is not.  Actually it's quite simple and elegant.

## 1. It's Written in C

CoCoSDC Commander is written in K&R Style C.  We have compilers for all
three Operating Systems which can compile this "tradidtional" style of C:

* DECB: [CMOC](http://perso.b2b2c.ca/~sarrazip/dev/cmoc.html)
* OS-/NitrOS-9: [DCC (Deek's C Compiler)](https://github.com/Deek/CoCoC)
* FLEX: [McCosh "C" Compiler](http://swtpcemu.com/swtpc/Manuals/McCosh_C.pdf)

## 2. It's a "terminal" Application

When I say this is a "terminal" application, the base program code does not
make any assupmtions about the screen and keybaord.  Instead of writing
directly into the VDG memory the program simply sends "terminal control
codes" such as Clear Screen, Home Cursor, Move Cursor to Y,X.  This allows
the program to be easily customized for each Operating System.  The base
program code calls into a "screen" library, which is where this
customization is done.


## 3. Careful use of `#ifdef`

There are some differences in the C compilers between each Operating
System.  The way to handle that is to use `#ifdef` in C.  There are
`#defines` for each operating asystem: `DECB`, `FLEX` and `OS9` and code
that needs to be different for each OS is contained in an `#ifdef` block.
The main `sdccmdr.c` file is almost 100% platform independent and has very
few `#ifdef` blocks.  Most of the differences are different header files
required to compile in the different environments and of course the screen
modules are different for each OS as well.

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
