#include "../sclide.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

int main() {
    // ─────────────────────────────────────────────────────────
    // 1. FLUSH A LOT OF TEXT INTO THE TERMINAL
    //    Let's pretend it's a real source file being printed.
    // ─────────────────────────────────────────────────────────

    std::vector<std::string> fakeCode = {
        "#include <iostream>",
        "#include <vector>",
        "#include <string>",
        "#include <algorithm>",
        "",
        "template <typename T>",
        "class Pipeline {",
        "private:",
        "    std::vector<T> stages;",
        "    std::string name;",
        "",
        "public:",
        "    Pipeline(std::string n) : name(std::move(n)) {}",
        "",
        "    Pipeline& addStage(T stage) {",
        "        stages.push_back(std::move(stage));",
        "        return *this;",
        "    }",
        "",
        "    void run() const {",
        "        std::cout << \"Running pipeline: \" << name << \"\\n\";",
        "        for (const auto& stage : stages) {",
        "            stage();",
        "        }",
        "        std::cout << \"Pipeline complete.\\n\";",
        "    }",
        "};",
        "",
        "int main() {",
        "    Pipeline<std::function<void()>> pipe(\"demo\");",
        "",
        "    pipe.addStage([]() {",
        "        std::cout << \"  [1] Loading assets...\\n\";",
        "    });",
        "",
        "    pipe.addStage([]() {",
        "        std::cout << \"  [2] Compiling shaders...\\n\";",
        "    });",
        "",
        "    pipe.addStage([]() {",
        "        std::cout << \"  [3] Binding buffers...\\n\";",
        "    });",
        "",
        "    pipe.addStage([]() {",
        "        std::cout << \"  [4] Starting render loop...\\n\";",
        "    });",
        "",
        "    pipe.run();",
        "    return 0;",
        "}",
    };

    // Print the code line by line, like a file being dumped.
    for (const auto& line : fakeCode) {
        std::cout << line << "\n";
    }

    // Throw in some noise to fill the screen properly.
    for (int i = 0; i < 8; ++i) {
        std::cout << "// log line " << i
                  << " — buffering, flushing, syncing, retrying...\n";
    }

    std::cout.flush();

    // Let the user actually see the mess for a moment.
    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    // ─────────────────────────────────────────────────────────
    // 2. DROP THE SCLIDE AND CLEAN EVERYTHING
    //    Row 54 is the bottom of your terminal, so the wipe
    //    starts at the last visible line and eats its way up
    //    the screen — wait, no: clean() sweeps DOWN from the
    //    starting row, so we place it near the top to sweep
    //    the whole buffer off-screen.
    //
    //    Given a 54×210 terminal, we start the wipe at row 1
    //    so it draws the line at the very top, then wipes the
    //    remaining 53 rows below it clean.
    // ─────────────────────────────────────────────────────────

    sclide wipe(1);

    wipe.setSclideColor("#00FFAA");           // neon mint head
    wipe.setShadeDegradingColor("#001A0F");   // fades into near-black green
    wipe.setSpeed(0.02);                      // fast — 20ms per stage

    wipe.clean();

    // ─────────────────────────────────────────────────────────
    // 3. (OPTIONAL) SAY SOMETHING AFTER THE STORM
    //    Cursor is back at row 1, terminal is clean.
    // ─────────────────────────────────────────────────────────

    std::cout << "Clean.\n";

    return 0;
}