# Tower Defense

**Developer:** Zayaan Ali Shah (25L-0732)

## Description
A 2D top-down Tower Defense game built entirely in C++. The game features a wave-based Normal Mode, an infinitely scaling Endless Mode, multiple enemy types (Poison Frogs, Tribal Warriors, Beetles, Flyers), and an interactive shop to buy and upgrade Cannon, Sniper, and Machine Gun towers.

---

## How to Compile and Run
This project was developed using Microsoft Visual Studio. To run the game on your local machine:

1. Clone or download this repository to your computer.
2. Open the project folder and double-click the `.sln` (Solution) file to open it in Visual Studio.
3. Ensure your build configuration at the top of the screen is set to **Debug** or **Release**, and **x64**.
4. Press **F5** or click the green **Local Windows Debugger** button at the top of the screen to compile and launch the game.

*Note: All necessary asset files (.png, .wav, .ogg, .ttf) are included in the root directory and will automatically load when the executable runs.*

---

## GUI Library & Installation
This project uses **SFML (Simple and Fast Multimedia Library) Version 3.0.0** for rendering graphics, handling window events, and playing audio. 

**To install and link SFML 3.0.0 for Visual Studio:**
1. Download the **Visual C++ 2022 (64-bit)** version of SFML 3.0.0 from the official SFML website.
2. Extract the folder to a permanent location on your hard drive (e.g., `C:\SFML-3.0.0`).
3. In Visual Studio, right-click the Project in the Solution Explorer and go to **Properties**.
4. Under **C/C++ -> General**, add the SFML `\include` folder path to the *Additional Include Directories*.
5. Under **Linker -> General**, add the SFML `\lib` folder path to the *Additional Library Directories*.
6. Under **Linker -> Input**, add the required dependencies to *Additional Dependencies* (e.g., `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib`, `sfml-audio.lib`).
7. Finally, copy the required `.dll` files from the SFML `\bin` folder and paste them directly into your project directory next to your `.vcxproj` file.

---

## Known Issues and Limitations
* **Platform Restriction:** The project is currently configured exclusively for Windows via Visual Studio (`.vcxproj` and `.sln`). Compiling on Linux or macOS will require writing a custom CMakeLists file.
* **Save State:** There is currently no persistent save feature; closing the application will reset all progress, gold, and high scores.
* **Resolution:** The game window is strictly hardcoded to 800x600. Maximizing or resizing the window is disabled to prevent aspect ratio distortion and UI misalignment.
