#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

#define GAME_WIDTH      1920
#define GAME_HEIGHT     1080
#define GAME_EXE        L"D:\\GamePassGames\\Starfield\\Content\\Starfield.exe"

HANDLE stdout;
HANDLE stdin;

static BOOL InitConsole(void);
static void Log(const wchar_t* message, ...);

void Main(void)
{
    if (!InitConsole())
    {
        ExitProcess(1);
        return;
    }
    
    Log(L"Getting current display settings ...\n");
    DEVMODEW original_mode = {0};
    BOOL enum_result = EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &original_mode);
    if (!enum_result)
    {
        Log(L"Get current display settings failed.\n");
        ExitProcess(2);
        return;
    }
    Log(L"Current display settings: %1!i!x%2!i!\n", original_mode.dmPelsWidth, original_mode.dmPelsHeight);

    Log(L"Changing display settings to %1!i!x%2!i! ...\n", GAME_WIDTH, GAME_HEIGHT);
    DEVMODEW new_mode = {0};
    new_mode.dmSize = sizeof(DEVMODEW);
    new_mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    new_mode.dmPelsWidth = GAME_WIDTH;
    new_mode.dmPelsHeight = GAME_HEIGHT;
    LONG change_result = ChangeDisplaySettingsW(&new_mode, 0);
    if (change_result != DISP_CHANGE_SUCCESSFUL)
    {
        Log(L"Change display settings failed.\nERROR: %1!i!\n", change_result);
        ExitProcess(3);
        return;
    }

    Log(L"Starting process ...\n");
    ShellExecuteW(NULL, NULL, GAME_EXE, NULL, NULL, SW_NORMAL);

    Log(L"Press ENTER to restore display settings ...\n");
    wchar_t buffer[2] = {0};
    DWORD chars_read;
    ReadConsoleW(stdin, buffer, 1, &chars_read, NULL);

    Log(L"Restoring display settings ...\n");
    LONG restore_result = ChangeDisplaySettingsW(&original_mode, 0);
    if (restore_result != DISP_CHANGE_SUCCESSFUL)
    {
        Log(L"Restore display settings failed.\nERROR: %1!i!\n", restore_result);
        ExitProcess(4);
        return;
    }

    ExitProcess(0);
    return;
}

BOOL InitConsole(void)
{
    stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdout == INVALID_HANDLE_VALUE || stdout == NULL) return FALSE;
    stdin = GetStdHandle(STD_INPUT_HANDLE);
    if (stdin == INVALID_HANDLE_VALUE || stdin == NULL) return FALSE;
    return TRUE;
}

void Log(const wchar_t* message, ...)
{
    wchar_t buffer[1024];

    va_list args;
    va_start(args, message);
    DWORD count = FormatMessageW(FORMAT_MESSAGE_FROM_STRING, message, 0, 0, buffer, 1024, &args);
    va_end(args);

    WriteConsoleW(stdout, buffer, count, NULL, NULL);
}
