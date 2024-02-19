#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define GAME_WIDTH      1920
#define GAME_HEIGHT     1080
#define GAME_EXE        L"D:\\GamePassGames\\Starfield\\Content\\Starfield.exe"

int main(void)
{
    wprintf(L"Getting current display settings ...\n");
    DEVMODEW original_mode = {0};
    BOOL enum_result = EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &original_mode);
    if (!enum_result)
    {
        wprintf(L"Get current display settings failed.\n");
        return 1;
    }
    wprintf(L"Current display settings: %lix%li\n", original_mode.dmPelsWidth, original_mode.dmPelsHeight);

    wprintf(L"Changing display settings to %ix%i ...\n", GAME_WIDTH, GAME_HEIGHT);
    DEVMODEW new_mode = {0};
    new_mode.dmSize = sizeof(DEVMODEW);
    new_mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    new_mode.dmPelsWidth = GAME_WIDTH;
    new_mode.dmPelsHeight = GAME_HEIGHT;
    LONG change_result = ChangeDisplaySettingsW(&new_mode, 0);
    if (change_result != DISP_CHANGE_SUCCESSFUL)
    {
        wprintf(L"Change display settings failed.\nERROR: %li\n", change_result);
        return 1;
    }

    wprintf(L"Starting process ...\n");
    _wsystem(GAME_EXE);

    wprintf(L"Press any key to restore display settings ...\n");
    getwchar();

    wprintf(L"Restoring display settings ...\n");
    LONG restore_result = ChangeDisplaySettingsW(&original_mode, 0);
    if (restore_result != DISP_CHANGE_SUCCESSFUL)
    {
        wprintf(L"Restore display settings failed.\nERROR: %li\n", restore_result);
        return 1;
    }

    return 0;
}