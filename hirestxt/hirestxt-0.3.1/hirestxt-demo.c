/*  hirestxt.c - Example program that uses hirestxt.h.

    By Pierre Sarrazin <http://sarrazip.com/>
    This file is in the public domain.
*/


#include "hirestxt.h"


#define USE_51x24
#define USE_42x24


enum { BREAK = 3 };


static BOOL demoContents(byte cols)
{
    char title[] = "51x24 software text screen";
    if (cols != 51)
        title[0] = '0' + cols / 10, title[1] = '0' + cols % 10;
    writeCenteredLine(1, title);
    writeCenteredLine(3, "ISO-8859-1 (Latin-1) character set ");

    // Display character table (32..127, 160..255):
    //
    for (byte i = 2; i < 16; ++i)
    {
        moveCursor(cols == 42 ? 3 : 8, 3 + i);
        printf("%X: ", i);
        if (i == 8 || i == 9)
            continue;
        for (byte j = 0; j < 16; ++j)
        {
            writeChar(i << 4 | j);
            writeChar(' ');
        }
    }
    
    // Fill the leftmost and rightmode columns:
    //
    for (byte y = 0; y < 24; ++y)
    {
        writeCharAt(0, y, '*');
        writeCharAt(hiResWidth - 1, y, '*');
    }

    // Show a prompt two lines below the table:
    //
    writeCenteredLine(textPosY + 2, "Press a key to continue: ");

    if (waitKeyBlinkingCursor() == BREAK)
        return FALSE;

    // Demonstrate printf() and automatic scrolling:
    //
    clrscr();
    for (byte i = 0; i < 26; ++i)
        printf("%c\n", 'A' + i);

    printf("\n""The screen scrolls automatically\n"
               "when the cursor reaches the bottom.\n\n");

    writeString("Press a key to continue: ");
    if (waitKeyBlinkingCursor() == BREAK)
        return FALSE;

    // VT52 demo via printf() (writeString() does not go
    // through processVT52()). Here, we clear the screen
    // without clrscr().
    //
    printf("\x1BH");  // cursor to home
    printf("\x1BJ");  // erase to end of screen
    printf("The screen has just been cleared with the\n"
           "VT52 sequence <ESC> H <ESC> J.");

    byte line = 7, col = 10;  // line and column numbers start at 1
    moveCursor(col, line);
    printf("This line starts at column %u of line %u.\n", col, line);

    line = 20;
    const char *prompt0 = "The cursor can blink over text.";
    const char *prompt1 = "Press a key to quit.";
    col = (hiResWidth - (byte) strlen(prompt0)) / 2;
    moveCursor(col, line);
    writeString(prompt0);
    moveCursor(col, line + 1);
    writeString(prompt1);
    moveCursor(col + 21, line);  // move to 'o' of "over"
    if (waitKeyBlinkingCursor() == BREAK)
        return FALSE;

    return TRUE;
}


// Returns TRUE if the user wants to continue to the next demo.
// Returns FALSE if the user pressed the Break key.
//
static BOOL demo(byte cols)
{
    // initHiResTextScreen() must be called first.
    // Assumes 4 graphics pages reserved at $0E00.
    // TRUE requests that printf() be redirected to the VT52 interpreter,
    // which writes to the 51x24 screen.
    //
    struct HiResTextScreenInit init =
        {
            cols,
            #if defined(USE_51x24) && defined(USE_42x24)
            cols == 42 ? writeCharAt_42cols : writeCharAt_51cols,
            #elif defined(USE_51x24)
            writeCharAt_51cols,
            #else
            writeCharAt_42cols,
            #endif
            (byte) (0x0E00 / 512),
            TRUE
        };
    initHiResTextScreen(&init);
    BOOL cont = demoContents(cols);
    closeHiResTextScreen();  // this unhooks printf() from VT52
    return cont;
}


int main()
{
    #if defined(USE_51x24) && defined(USE_42x24)
    if (demo(51))
        demo(42);
    #elif defined(USE_51x24)
    demo(51);
    #else
    demo(42);
    #endif

    cls(255);  // go back to hardware text mode
    printf("THE END\n\n");  // printf() now writes to normal console
    return 0;
}
