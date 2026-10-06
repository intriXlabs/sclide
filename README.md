# sclide v.1.0.0

<div align="center">

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-11%2B-orange.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20WSL-lightgrey.svg)
![Dependencies](https://img.shields.io/badge/dependencies-0-brightgreen.svg)
![Colors](https://img.shields.io/badge/color-24--bit%20true--color-blueviolet.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

**A line that arrives like a wave — and erases like one.**

*A single-row C++ wipe effect for the terminal, with a four-stage shade trail and a full-screen clean sweep, all in true color.*


</div>

---

# Intro

<div align="center">
    <image src="demoGifs/intro.gif" alt="sclide demo" width="600"/>
</div>

# demos

<div align="center">
    <image src="demoGifs/demo1.gif" alt="sclide demos" width="300"/>
    <image src="demoGifs/demos.gif" alt="sclide demos" width="300"/>
    <image src="demoGifs/demo2.gif" alt="sclide clean" width="300"/>
</div>

---


## Important Note

`sclide` is designed for **POSIX terminals** (Linux, macOS, BSD, WSL) and requires **ANSI escape codes** and **24-bit true-color support**.

### Zero-Dependency by Design

The library ships with **no external dependencies**. The only POSIX-specific calls are terminal-size detection (`ioctl` + `TIOCGWINSZ`) and direct writes (`write`). Everything else is standard C++.

The only requirements are:

* ANSI escape code support
* 24-bit true-color support
* A terminal whose width and height can be queried

If your platform exposes terminal size and honors ANSI escapes, `sclide` works there.

**sclide handles the wave. Where it lands is up to you.**

---

## 📖 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [The Wipe Model](#-the-wipe-model)
- [The Four Shades](#-the-four-shades)
- [Architecture](#-architecture)
- [Quick Start](#-quick-start)
- [Use Cases](#-use-cases)
- [Why sclide](#-why-sclide)
- [Comparison](#-comparison)
- [Design Philosophy](#-design-philosophy)
- [API Reference](#-api-reference)
- [License](#-license)

---

## 🌟 Overview

`sclide` is a **single-row wipe effect** for the terminal. It draws a horizontal line at a row of your choosing, pulls it across the screen with a fading trail behind it, and then — when you ask — sweeps the whole screen clean in the same motion, top to bottom.

The name is a play on **"slide"** and **"ASCII"** — but with a nod to *sclide*, the wipe itself. Not a slide. Not a scroll. A moving bar of light that clears the way in front of it and erases what's behind.

### Why a Wipe?

Terminals are full of *moments*. A loading screen, a banner fading in, a screen clearing before the next thing. Most of the time, those moments are instant — the text appears, the text vanishes, no in-between.

`sclide` gives that in-between a body. The wipe **arrives** with a gradient of shades behind it, then **erases** with the same wave, top to bottom, leaving nothing behind but the cursor blinking on a clean screen.

- **One line of motion.** That's the whole idea.
- **A shade trail that sells the speed.** Four characters, four colors, trailing the head of the wipe.
- **Full true color.** The line's color and the shade's fading color are both fully yours.
- **A clean sweep on demand.** Call `clean()` and the whole terminal is wiped in the same language.

`sclide` doesn't want to draw your UI. It wants to **clear the stage** — and make clearing it feel like something.

---

## ✨ Features

### 🎨 Color
- **24-bit True Color** — Full RGB for both the line and the shade trail
- **Hex or RGB Input** — Set colors with `#RRGGBB` or `(r, g, b)`
- **Two Independent Colors** — The head of the wipe and the tail of the wipe are set separately
- **Automatic Interpolation** — The four shade stops are computed from your two endpoints

### 🌀 Motion
- **Four-Stage Shade Trail** — `█ ▓ ▒ ░` fading toward the background
- **Precomputed Frames** — Every rendered frame is a single `write()` call
- **Adjustable Speed** — One value controls how long each stage lingers
- **Full-Screen Clean** — The same wipe language applied top-to-bottom

### 🧩 Developer-Friendly
- **One Class** — Construct it, set colors, call `clean()`
- **Runtime Restyling** — Change the line color, shade color, or glyphs mid-run
- **Replaceable Shades** — Swap `█ ▓ ▒ ░` for any four characters
- **Terminal-Aware** — Reads width and height on construction

---

## 🌀 The Wipe Model

`sclide` is built on a single abstraction: **a horizontal wipe that erases what it passes**.

```
                       THE WIPE

     ┌─────────────────────────────────────────────────────┐
     │  ░░░░  ▒▒▒▒  ▓▓▓▓  ███████████████████████████████ │
     └─────────────────────────────────────────────────────┘
        shade4  shade3  shade2      head (line color)

        ←── tail ──┤        the line the wipe leaves behind
                    │
                    └── the wipe's head, moving right

     Stage 1: only the head exists.
     Stage 2: head + first shade.
     Stage 3: head + first + second shade.
     Stage 4: head + first + second + third shade.
     After:   the full line is drawn.
```

Once the line is on screen, calling `clean()` replays the same language — but **down the screen**, row by row, with a leading space to erase the text underneath.

A wipe has three parts:

```
┌──────────────────────────────────────────────┐
│                  A WIPE                      │
│                                              │
│   head     →  the solid line (─) in the      │
│               sclide color                   │
│                                              │
│   trail    →  up to four shade characters    │
│               fading toward the              │
│               degrading color                │
│                                              │
│   sweep    →  a leading space that           │
│               erases the cell underneath     │
│               as the wipe passes             │
│                                              │
└──────────────────────────────────────────────┘
```

That's it. Head, trail, sweep. Everything else is timing and color.

---

## 🎨 The Four Shades

The wipe's trail is made of **four shade characters**, each tinted by a color between the line color and the degrading color.

| Stage | Glyph | Color |
|---|---|---|
| Head | `─` | sclide color |
| Shade 1 | `█` | 100% sclide → 0% degrading |
| Shade 2 | `▓` | 75% sclide → 25% degrading |
| Shade 3 | `▒` | 50% sclide → 50% degrading |
| Shade 4 | `░` | 25% sclide → 75% degrading |

The **degrading color** is what the shade fades *toward*. If you set it to black (`#000000`), the trail fades into darkness. If you set it to your terminal's background color, the trail fades into invisibility. If you set it to another bright color, the trail becomes a gradient between two hues.

The interpolation is **linear per channel**, clamped to `[0, 255]`, and computed once per color change — not per frame.

```cpp
sclide wipe(5);

wipe.setSclideColor("#00FFAA");          // the line itself
wipe.setShadeDegradingColor("#001111");  // what the trail fades into

wipe.clean();
```

---

## 🏗 Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      Application Layer                      │
│   ┌──────────────┐  ┌────────────────┐  ┌───────────────┐   │
│   │ Constructor  │  │   setColor()   │  │    clean()    │   │
│   └──────────────┘  └────────────────┘  └───────────────┘   │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                    Color Interpolation                      │
│   ┌────────────────────────────────────────────────────┐    │
│   │   sclideColor  +  shadeDegradingColor              │    │
│   │        ↓                                           │    │
│   │   four shade stops, precomputed as ANSI strings    │    │
│   └────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                    Precomputed Frames                       │
│   ┌────────────────────────────────────────────────────┐    │
│   │   fullLinePreMade  •  firstShadePreMade            │    │
│   │   secondShadePreMade  •  thirdShadePreMade         │    │
│   │   fourthShadePreMade  •  spacePreMade              │    │
│   └────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                     Terminal Layer                          │
│   ┌────────────────────────────────────────────────────┐    │
│   │   Cursor positioning  •  Foreground color          │    │
│   │   Single write() per frame  •  Cursor hide/restore │    │
│   └────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
                            ↓
                    Terminal Display
```

`sclide` does almost all of its work **before the animation starts**. It builds six pre-rendered strings on construction — one for each shade stage, one for the full line, one for the blanking space. Then each frame is a single `write()` of a string that already exists.

That's the "fast track." No per-cell looping during the animation. No string concatenation per frame. Just cursor positioning and one write.

---

## 🚀 Quick Start

### Prerequisites

- **C++11 or higher** compiler
- **POSIX terminal** — Linux, macOS, BSD, WSL
- **24-bit true-color support** — Most modern terminals handle this
- **ANSI escape code support** — For cursor positioning

### Installation

`sclide` is a **single C++ class**. Drop it into your project.

```bash
# Clone or copy sclide.cpp into your project

# Compile with optimizations
g++ -O3 -std=c++11 your_app.cpp -o your_app

# Run
./your_app
```

### Minimal Example

```cpp
#include "sclide.cpp"

int main() {
    //                       starting row
    sclide wipe(              5);

    wipe.setSclideColor("#00FFAA");
    wipe.setShadeDegradingColor("#001111");
    wipe.setSpeed(0.05);     // seconds per stage

    wipe.clean();

    return 0;
}
```

That's it. The wipe draws, fades, and clears the screen below it.

---

## 🎯 Use Cases

### 1. Loading Screens
A `sclide` wipe across the top of the terminal, then a clean sweep as the program finishes. The transition *is* the feedback.

### 2. Scene Transitions
Wipe the terminal between two screens the way a video editor wipes between two shots — except it's all one row of characters.

### 3. Boot Sequences
Draw a header line with `sclide`, then wipe the body of the terminal clean as your program initializes. The "clean" is both literal and theatrical.

### 4. Installers & First-Run Wizards
Give the setup process a moment of motion before the first question. A wipe that clears the screen is a wipe that says *"we're starting now."*

### 5. Terminal Games
Cutscenes, level transitions, death screens — anywhere a scene change should *feel* like a scene change.

### 6. Anywhere `clear` Feels Too Sudden
`clear` is instant. `sclide` is a beat. Sometimes the beat is the point.

---

## 🌈 Why sclide

Most terminal programs treat screen-clearing as a utility: call `clear`, get on with it. `sclide` treats it as a **gesture**.

It doesn't want to render your UI. It doesn't want to own your input loop. It doesn't want to be a TUI framework. It wants to be **one line and one sweep** — the moment between two states of the screen.

If you have a transition in your terminal app that currently just... happens, `sclide` is what happens instead.

---

## ⚖️ Comparison

| | **sclide** | `clear` / `printf` | TUI Frameworks | Graphics Libraries |
|---|---|---|---|---|
| **Dependencies** | None | None | Several | Many |
| **Setup** | One class | Trivial | Framework to learn | Build system, drivers |
| **What It Does** | Wipe + clean | Instant clear | Full-screen UI | Full-screen pixels |
| **True Color** | Yes | No | Varies | N/A |
| **Motion** | 4-stage shade trail | None | Possible, manual | N/A |
| **Runs in Terminal** | Yes | Yes | Yes | No |
| **Learning Curve** | Minutes | Seconds | Hours to days | Days to weeks |
| **Scope** | One wipe | One clear | Everything | Everything |

`sclide` isn't trying to replace `clear`. It's trying to make `clear` feel like something.

---

## 🧬 Design Philosophy

`sclide` is small on purpose.

- It does **one wipe**.
- It uses **one line and four shades**.
- It exposes **one method** for clearing: `clean()`.
- It precomputes **everything it can** before the animation starts.

The whole effect is a single idea — *"a line that erases what it passes"* — applied twice: once across a row, once down the screen. Nothing more.

You're supposed to tune it. Change the line color. Change the degrading color. Slow it down, speed it up. Replace the block shades with something else. The source is short enough to read in one sitting, and the effect is simple enough to reshape in an afternoon.

It's a wipe for people who want **the transition to be part of the experience** — not just the space between two screens.

---

## 📚 API Reference

### Constructor

```cpp
sclide(int startingRow);
```

Creates a `sclide` wipe that draws at row `startingRow`. On construction, it reads the terminal size, precomputes all six frame strings, and interpolates the four shade colors.

- `startingRow` — 1-indexed row where the wipe begins

### `setSclideColor()`

```cpp
int setSclideColor(int r, int g, int b);
int setSclideColor(std::string hexCode);
```

Sets the color of the head of the wipe. Accepts either raw RGB or a `#RRGGBB` hex string. Recomputes the shade interpolation immediately.

### `setShadeDegradingColor()`

```cpp
int setShadeDegradingColor(int r, int g, int b);
int setShadeDegradingColor(std::string hexCode);
```

Sets the color the shade trail fades **toward**. Accepts either raw RGB or `#RRGGBB`. Recomputes the shade interpolation immediately.

### `resetSclideColor()`

```cpp
int resetSclideColor();
```

Resets the head color to white (`#FFFFFF`).

### `setSpeed()`

```cpp
int setSpeed(double newSpeed);
```

Sets the delay between stages, in **seconds**. Default is `0.1`.

### `changeShades()`

```cpp
int changeShades(std::vector<std::string> newShades);
```

Replaces the four shade glyphs. Defaults are `{"█", "▓", "▒", "░"}`. Rebuilds all precomputed frames.

### `clean()`

```cpp
int clean();
```

Runs the full wipe animation:

1. Draws the wipe across `startingRow` in four stages.
2. Sweeps down the screen, row by row, erasing as it goes.
3. Finishes with a smooth tail animation on the last visible row.
4. Restores the cursor and resets color.

Returns `0` on success.

---

## 📄 License

Licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

<div align="center">

**One line. One sweep. One transition.**

*"The wipe arrives like a wave — and erases like one."*

[⬆ Back to Top](#sclide)

</div>