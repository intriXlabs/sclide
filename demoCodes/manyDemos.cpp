#include "../sclide.cpp"

int main() {
    sclide mySclide(5); // Starting row is 5
    mySclide.setSclideColor("#FF0000"); // Set speed to 0.05 seconds
    mySclide.setSpeed(0.05); // Set speed to 0.05 seconds
    mySclide.clean(); // Clean the screen with the sclide effect

    mySclide.setSclideColor("#00FF00"); // Change sclide color to green
    mySclide.changeShades({"X", "x", "+", "-"}); // Change shades to 3 (full, first shade, second shade, third shade)
    mySclide.clean(); // Clean the screen with the new settings

    mySclide.setSclideColor("#0000FF"); // Change sclide color to blue
    mySclide.changeShades({"@", "#", "*", "-"}); // Change shades to 4 (full, first shade, second shade, third shade)
    mySclide.clean(); // Clean the screen with the new settings

    mySclide.setSclideColor("#FFFF00"); // Change sclide color to yellow
    mySclide.changeShades({"_", "_", "_", "_"}); // Change shades to 4 (full, first shade, second shade, third shade)
    mySclide.clean(); // Clean the screen with the new settings

    mySclide.setSclideColor("#FF00FF"); // Change sclide color to magenta
    mySclide.changeShades({"=", "=", "=", "="}); // Change shades to 4 (full, first shade, second shade, third shade)
    mySclide.clean(); // Clean the screen with the new settings

    mySclide.setSclideColor("#00FFFF"); // Change sclide color to cyan
    mySclide.changeShades({"~", "~", "~", "~"}); // Change shades to 4 (full, first shade, second shade, third shade)
    mySclide.clean(); // Clean the screen with the new settings
    return 0;
}