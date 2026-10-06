#include "../sclide.cpp"
#include <cmath>

int main() {
    // ═══════════════════════════════════════════════════════════════════
    //  sclide — MCU Opening Sequence, Final Cut
    //  Fifteen wipes. Fifteen palettes. Fifteen unique trail alphabets.
    //  All at row 1. Slow open, then accelerate hard.
    //
    //  And then — the title card. "SCLIDE" rendered with a vertical
    //  density gradient: solid blocks at the top, thinning to light
    //  shade at the bottom, so the word reads as if it's catching
    //  light from above. The hue cycles forever. It never leaves.
    // ═══════════════════════════════════════════════════════════════════

    // ── Terminal probe ─────────────────────────────────────────────────
    struct winsize probe;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &probe);

    const int TERM_W = probe.ws_col;
    const int TERM_H = probe.ws_row;

    //  Every wipe lives at row 1. Always. No exceptions.
    const int ROW = 1;

    // ── Palette — fifteen color pairs, one per wipe ────────────────────
    struct ColorPair {
        const char* line;
        const char* degrade;
    };

    const ColorPair palette[15] = {
        { "#FF0000", "#220000" },  //  1 — crimson into shadow
        { "#FF8800", "#221100" },  //  2 — amber into coal
        { "#FFDD00", "#222000" },  //  3 — gold into olive-black
        { "#AAFF00", "#112200" },  //  4 — lime into forest black
        { "#00FF44", "#002211" },  //  5 — emerald into deep green
        { "#00FFCC", "#002222" },  //  6 — teal into dark teal
        { "#00DDFF", "#001133" },  //  7 — cyan into navy
        { "#0088FF", "#000822" },  //  8 — azure into midnight
        { "#3344FF", "#000033" },  //  9 — royal blue into void
        { "#8800FF", "#110022" },  // 10 — violet into plum black
        { "#DD00FF", "#220022" },  // 11 — magenta into wine
        { "#FF00AA", "#220011" },  // 12 — hot pink into maroon
        { "#FF4488", "#220011" },  // 13 — rose into burgundy
        { "#FF6666", "#220000" },  // 14 — salmon into brick
        { "#FFFFFF", "#000000" },  // 15 — white into true black (finale)
    };

    // ── Speed curve — held open, then a hard drop, then a decay ────────
    const double tempo[15] = {
        0.020,  //  1 — slow open, the audience settles
        0.015,  //  2 — still held back, one more breath
        0.008,  //  3 — the cut: tempo drops by nearly half
        0.0065, //  4 — pick up from here
        0.0055, //  5
        0.0045, //  6
        0.0038, //  7
        0.0032, //  8
        0.0027, //  9
        0.0023, // 10
        0.0020, // 11
        0.0017, // 12
        0.0014, // 13
        0.0012, // 14
        0.0010, // 15 — the finale, a blur
    };

    // ── Trail alphabets — fifteen signatures, one per wipe ─────────────
    const std::vector<std::vector<std::string>> trails = {
        { "O",  "0",  "o",  "."  },  //  1 — circles, fading into a dot
        { "@",  "#",  "+",  "-"  },  //  2 — hash-family, techy
        { "*",  "+",  "x",  "."  },  //  3 — starlight dissolving
        { "%",  "&",  "$",  "."  },  //  4 — currency symbols
        { "=",  "-",  "~",  "."  },  //  5 — a straight line melting
        { "^",  "/",  "|",  "."  },  //  6 — angles collapsing
        { "8",  "0",  "o",  "."  },  //  7 — clock faces, running out
        { "X",  "x",  "+",  "."  },  //  8 — construction marks
        { "#",  "=",  "-",  "."  },  //  9 — grid decaying
        { "◆",  "◇",  "·",  "."  },  // 10 — diamonds into dust
        { "★",  "☆",  "·",  "."  },  // 11 — stars into silence
        { "▲",  "△",  "·",  "."  },  // 12 — triangles into nothing
        { "●",  "○",  "·",  "."  },  // 13 — filled fading to hollow
        { "█",  "▓",  "▒",  "░"  },  // 14 — the original block family
        { "@",  "@",  "@",  "@"  },  // 15 — pure density, finale
    };

    // ── Curtain up — hide the cursor for the whole performance ─────────
    std::cout << "\033[?25l" << std::flush;

    // ── The sequence — fifteen wipes, one after another, all at row 1 ──
    for (int i = 0; i < 15; ++i) {

        sclide wipe(ROW);

        wipe.setSclideColor(palette[i].line);
        wipe.setShadeDegradingColor(palette[i].degrade);
        wipe.setSpeed(tempo[i]);

        wipe.changeShades(trails[i]);

        if (i == 14) {
            wipe.setSclideColor(255, 255, 255);
            wipe.setShadeDegradingColor(0, 0, 0);
        }

        wipe.clean();
    }

    // ═══════════════════════════════════════════════════════════════════
    //  THE TITLE CARD
    //  Clear the whole stage, then paint "SCLIDE" as a vertical
    //  density gradient: solid blocks at the top of the letters,
    //  thinning through the shade families down to a light stub.
    //  The hue cycles forever. The card never leaves.
    // ═══════════════════════════════════════════════════════════════════

    // ── Wipe the whole stage clean ─────────────────────────────────────
    {
        std::string blankRow(TERM_W, ' ');
        std::string buffer;
        for (int r = 1; r <= TERM_H; ++r) {
            buffer += "\033[" + std::to_string(r) + ";1H" + blankRow;
        }
        buffer += "\033[0m";
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
    }

    // ── Bitmap font for "SCLIDE" ───────────────────────────────────────
    //  Same 5x7 grids as before. The change is in how we PAINT them.

    const std::vector<std::string> glyph_S = {
        "11111",
        "10000",
        "10000",
        "11111",
        "00001",
        "00001",
        "11111",
    };
    const std::vector<std::string> glyph_C = {
        "01111",
        "10000",
        "10000",
        "10000",
        "10000",
        "10000",
        "01111",
    };
    const std::vector<std::string> glyph_L = {
        "10000",
        "10000",
        "10000",
        "10000",
        "10000",
        "10000",
        "11111",
    };
    const std::vector<std::string> glyph_I = {
        "11111",
        "00100",
        "00100",
        "00100",
        "00100",
        "00100",
        "11111",
    };
    const std::vector<std::string> glyph_D = {
        "11110",
        "10001",
        "10001",
        "10001",
        "10001",
        "10001",
        "11110",
    };
    const std::vector<std::string> glyph_E = {
        "11111",
        "10000",
        "10000",
        "11110",
        "10000",
        "10000",
        "11111",
    };

    const std::vector<std::vector<std::string>> letters = {
        glyph_S, glyph_C, glyph_L, glyph_I, glyph_D, glyph_E
    };

    const int LETTER_W = 5;
    const int LETTER_H = 7;
    const int LETTER_GAP = 1;
    const int TITLE_W = (int)letters.size() * LETTER_W
                      + ((int)letters.size() - 1) * LETTER_GAP;
    const int TITLE_H = LETTER_H;

    const int TITLE_TOP = std::max(1, (TERM_H - TITLE_H) / 2 + 1);
    const int TITLE_LEFT = std::max(1, (TERM_W - TITLE_W) / 2 + 1);

    // ── Density gradient — one glyph per row of the title ─────────────
    //  Seven rows, seven densities. Top of the letters is solid; the
    //  bottom thins out. This is the "light from above" effect.
    //
    //  The progression is intentional:
    //    Row 0: █ — the ceiling, full weight
    //    Row 1: █ — still solid, the letter's spine
    //    Row 2: ▓ — dark shade begins
    //    Row 3: ▒ — medium shade, the visual equator
    //    Row 4: ▒ — still medium, one beat of stability
    //    Row 5: ░ — light shade, almost gone
    //    Row 6: ░ — the base, holding on
    //
    //  The result reads as if the letters are lit from above and
    //  shadow falls toward the bottom of each glyph.
    const std::string density[7] = { "█", "█", "▓", "▒", "▒", "░", "░" };

    //  A blank cell — used where the glyph bitmap has a '0'.
    const std::string SPACE = " ";

    // ── Render the title and hold ──────────────────────────────────────
    uint64_t frame = 0;
    while (true) {
        //  Slowly rotating hue, same sin-trio as before.
        double hue = frame * 0.01;

        int r = (int)((std::sin(hue)               * 0.5 + 0.5) * 175.0) + 80;
        int g = (int)((std::sin(hue + 2.0943951)   * 0.5 + 0.5) * 175.0) + 80;
        int b = (int)((std::sin(hue + 4.1887902)   * 0.5 + 0.5) * 175.0) + 80;

        std::string color = "\033[38;2;" + std::to_string(r) + ";"
                                            + std::to_string(g) + ";"
                                            + std::to_string(b) + "m";

        std::string buffer;
        buffer += "\033[?25l";

        for (int row = 0; row < TITLE_H; ++row) {
            buffer += "\033[" + std::to_string(TITLE_TOP + row) + ";"
                             + std::to_string(TITLE_LEFT) + "H";
            buffer += color;

            //  The glyph for this row is decided ONCE, not per cell.
            //  That's what makes the gradient read as bands of density
            //  rather than as noise.
            const std::string& glyph = density[row];

            for (size_t li = 0; li < letters.size(); ++li) {
                const auto& bitmap = letters[li];
                for (int col = 0; col < LETTER_W; ++col) {
                    buffer += (bitmap[row][col] == '1') ? glyph : SPACE;
                }
                if (li + 1 < letters.size()) {
                    buffer += SPACE;
                }
            }

            buffer += "\033[0m";
        }

        write(STDOUT_FILENO, buffer.c_str(), buffer.size());

        ++frame;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0;
}