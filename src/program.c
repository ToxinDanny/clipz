#include "program.h"

/*
    1) CopyMode:
        - Activates copy mode with Ctrl+Alt+A 
            -> Copy on Ctrl+<NUMPAD 1-9> 
        - Paste from corresponding Ctrl+<NUMPAD 1-9>

    2) Quit -> Ctrl+Alt+Q

    3) Print -> Ctrl+Alt+P:
        - Print on console the hotkeys' map

    4) (TODO) StackMode: 
        - Activates stack copy with Ctrl+Alt+S 
            -> Copy on Ctrl+0 n-times
        - Paste LIFO 

    5) (TODO) TemplateMode: 
        - Activates with Ctrl+Alt+T 
            -> Copy template (string with placeholder '%') on Ctrl+<1-9> 
            -> Copy text on Ctrl+<NUMPAD 1-9>
        - Paste from corresponding Ctrl+<1-9> && Ctrl+<NUMPAD 1-9>

    6) (TODO) Refactoring; Better error handling; Better arena and data structures 

    7) (NICE TO HAVE) Packaging app as background service && windows service tag 

*/

int main() {

    if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 'A')) {
        fprintf(stderr, "Error: Hotkey impossible to register. (errno = %d)\n", GetLastError());
        return -1;
    }

    if (!RegisterHotKey(NULL, 2, MOD_CONTROL | MOD_ALT, 'Q')) {
        fprintf(stderr, "Error: Hotkey impossible to register. (errno = %d)\n", GetLastError());
        return -1;
    }

    if (!RegisterHotKey(NULL, 3, MOD_CONTROL | MOD_ALT, 'P')) {
        fprintf(stderr, "Error: Hotkey impossible to register. (errno = %d)\n", GetLastError());
        return -1;
    }

    ClipzArena = create_arena(MB_16, DEF_ALIGN);
    assert(ClipzArena != NULL);

    ClipzHKMap = (HKMap*)enter_the_arena(ClipzArena, sizeof(HKMap));
    ClipzHKMap->kv = (KV*)enter_the_arena(ClipzArena, 10 * sizeof(KV));

    BOOL shutdown = FALSE;

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (!hHook) {
        fprintf(stderr, "Error: Keystroke impossible to intercept. (errno = %d)", GetLastError());
        shutdown = TRUE;
        return -1;
    }

    MSG msg = { 0 };

    // Application loop
    while (!shutdown) {

        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {

            if (msg.message == WM_HOTKEY && msg.wParam == 1) {
                CopyMode = TRUE;
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 2) {
                printf("Shutting down...");
                shutdown = TRUE;
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 3) {
                print(ClipzHKMap);
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 4) {
                StackMode = TRUE;
            }
        }

        Sleep(10);
    }

    UnregisterHotKey(NULL, 1);
    UnregisterHotKey(NULL, 2);
    UnregisterHotKey(NULL, 3);
    destroy_arena(ClipzArena);
    return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    static BOOL ctrlDown = FALSE;

    if (nCode == HC_ACTION) {

        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        if (p->vkCode == VK_LCONTROL || p->vkCode == VK_RCONTROL) {
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
                ctrlDown = TRUE;
            else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
                ctrlDown = FALSE;

            return CallNextHookEx(hHook, nCode, wParam, lParam);
        }

        if (wParam == WM_KEYDOWN && ctrlDown) {

            if (p->vkCode >= VK_NUMPAD1 && p->vkCode <= VK_NUMPAD9) {
                    
                char key = (char)('1' + (p->vkCode - VK_NUMPAD1));

                if (CopyMode) {
                    SaveOnHotkeyMap(ClipzHKMap, key, GetSelectedText());
                    CopyMode = FALSE;
                }
                else {
                    PasteFromHotkeyMap(ClipzHKMap, key);
                }

                return 1;
            }
        }
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL SaveOnHotkeyMap(HKMap* map, char key, char* value) {

    KV* kv = (KV*)enter_the_arena(ClipzArena, sizeof(KV));

    kv->key = key;
    kv->value = value;

    BOOL res = try_add(map, kv);

    if (res) {
        printf("Ctrl + %c = \"%s\"\n", key, value);
    }

    return res;
}

char* GetSelectedText() {

    INPUT inputs[4] = { 0 };

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'C';

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'C';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
    Sleep(80); 

    return GetClipboardText();
}

BOOL PasteFromHotkeyMap(HKMap* map, char key) {

    KV kv = { 0 };
    if (!try_get(map, key, &kv)) {
        return FALSE;
    }

    if (!SetClipboardText(kv.value)) {
        return FALSE;
    }

    INPUT inputs[4] = { 0 };

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'V';

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
    return TRUE;
}