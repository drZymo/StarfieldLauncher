# Simple launcher for Starfield

Lower desktop resolution, then starts the game.
Waits for key press to restore resolution.

## Compile

1. Edit defines `GAME_WIDTH`, `GAME_HEIGHT`, and `GAME_EXE` in [launcher.c](launcher.c).
2. Open `Developer Command Prompt for VS 2022` and go to this folder.
3. Compile: `cl /c /Wall launcher.c`
4. Link: `link /machine:X64 /subsystem:console /entry:Main /out:launcher.exe launcher.obj kernel32.lib user32.lib shell32.lib`
