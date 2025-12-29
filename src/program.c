#include "program.h"

/*
    1) CopyMode:
        - Activates copy mode with Ctrl+Alt+A
        - Copy on Ctrl+<NUMPAD 1-9>
        - Paste from corresponding Ctrl+<NUMPAD 1-9>

    2) Quit: Ctrl+Alt+Q
        - Closes the application

    3) Print: Ctrl+Alt+P
        - Print on console the hotkeys

    4) StackMode: Ctrl+Alt+S (toggle on) -> Ctrl+0 (n-copies) -> Ctrl+Alt+S (toggle off) -> Ctrl+0 (n-paste)
        - Activates stack copy mode with Ctrl+Alt+S
        - Copy on Ctrl+0 n-times
        - Deactivates stack copy mode with Ctrl+Alt+S
        - Paste from Ctrl+0 n-times, LIFO style

    5) (TODO) TemplateMode: Ctrl+Alt+T -> Ctrl+<1-9> (copy template) -> Ctrl+Alt+A -> Ctrl+<NUMPAD 1-9> (copy text) ->
                            Ctrl+<1-9> -> Ctrl+<NUMPAD 1-9> (paste composed text)
        - Activates with Ctrl+Alt+T
        - Copy template (string with placeholder '%') on Ctrl+<1-9>
        - Copy text in copy mode on Ctrl+<NUMPAD 1-9>
        - Paste from corresponding Ctrl+<1-9> && Ctrl+<NUMPAD 1-9>

    6) (TODO) Refactoring; Improve error handling; Improve arena and data structures

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

    if (!RegisterHotKey(NULL, 4, MOD_CONTROL | MOD_ALT, 'S')) {
        fprintf(stderr, "Error: Hotkey impossible to register. (errno = %d)\n", GetLastError());
        return -1;
    }

    if (!RegisterHotKey(NULL, 5, MOD_CONTROL | MOD_ALT, 'T')) {
        fprintf(stderr, "Error: Hotkey impossible to register. (errno = %d)\n", GetLastError());
        return -1;
    }

    ClipzArena = create_arena(MB_16, DEF_ALIGN);
    assert(ClipzArena != NULL);

    ClipzHKMap = (HKMap*)enter_the_arena(ClipzArena, sizeof(HKMap));
    ClipzHKMap->kv = (KV*)enter_the_arena(ClipzArena, 10 * sizeof(KV));
    ClipzStack = enter_the_arena(ClipzArena, sizeof(char*));
    ClipzTemplates = enter_the_arena(ClipzArena, sizeof(char*));

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

                if (TemplateMode != TActive) {
                    CopyMode = TRUE;
                }
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 2) {
                printf("Shutting down...");
                shutdown = TRUE;
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 3) {
                print(ClipzHKMap);
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 4) {
                StackMode = !StackMode;
                printf("Stack Mode %s\n", StackMode ? "on" : "off");
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 5) {

                switch (TemplateMode) {

                    case TNotEnabled:
                        TemplateMode = TEnabled;
                        printf("Template Mode on\n");
                        break;

                    case TEnabled:
                        TemplateMode = TActive;
                        break;

                    default:
                        TemplateMode = TNotEnabled;
                        printf("Template Mode off\n");
                        break;
                }

            }
        }

        Sleep(10);
    }

    UnregisterHotKey(NULL, 1);
    UnregisterHotKey(NULL, 2);
    UnregisterHotKey(NULL, 3);
    UnregisterHotKey(NULL, 4);
    UnregisterHotKey(NULL, 5);
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

            if (p->vkCode >= '1' && p->vkCode <= '9') {

                char key = (char)p->vkCode;

                if (CopyMode) {
                    SaveOnHotkeyMap(ClipzHKMap, key, Copy());
                    CopyMode = FALSE;
                }
                else if (TemplateMode == TEnabled) {
                    SaveOnTemplateList(ClipzTemplates, p->vkCode, Copy());
                }
                else if (TemplateMode == TActive) {
                    hk_actual_key = key;
                    KV* kv = (KV*)enter_the_arena(ClipzArena, sizeof(KV));
                    try_get(ClipzHKMap, hk_actual_key, kv);
                    PasteFromTemplateList(ClipzTemplates, p->vkCode, kv->value);
                    TemplateMode = TEnabled;
                }
                else {
                    PasteFromHotkeyMap(ClipzHKMap, key);
                }

                return 1;
            }

            if (p->vkCode == '0') {

                if (StackMode) {
                    stack_counter++;
                    SaveOnStack(ClipzStack, Copy(), stack_counter);
                }
                else {
                    if (stack_counter != 0) {
                        PasteFromStack(ClipzStack, &stack_counter);
                    }
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

BOOL PasteFromHotkeyMap(HKMap* map, char key) {

    KV kv = { 0 };
    if (!try_get(map, key, &kv)) {
        return FALSE;
    }

    if (!SetClipboardText(kv.value)) {
        return FALSE;
    }

    Paste();

    return TRUE;
}

BOOL SaveOnStack(char** stack, char* text, short sc) {

    if (stack == NULL || text == NULL || strcmp(text, "") == 0 || strcmp(text, " ") == 0)
        return FALSE;

    size_t text_size = strlen(text) + 1;
    stack[sc - 1] = (char*)enter_the_arena(ClipzArena, text_size);
    memcpy(stack[sc - 1], text, text_size);

    printf("Stack %d: %s\n", sc - 1, text);

    return TRUE;
}

BOOL PasteFromStack(char** stack, short* sc) {

    if (stack == NULL || stack[*sc - 1] == NULL || !SetClipboardText(stack[*sc - 1])) {
        return FALSE;
    }

    Paste();

    stack = stack + 1;
    (*sc)--;

    return TRUE;
}

BOOL SaveOnTemplateList(char** templates, char key, char* text) {

    if (templates == NULL || text == NULL || strcmp(text, "") == 0 || strcmp(text, " ") == 0)
        return FALSE;

    int i = (int)key - 48;
    size_t text_size = strlen(text) + 1;
    templates[i - 1] = (char*)enter_the_arena(ClipzArena, text_size);
    memcpy(templates[i - 1], text, text_size);

    printf("Template %d: %s\n", i - 1, text);

    return TRUE;
}

BOOL PasteFromTemplateList(char** templates, char key, char* text) {

    if (templates == NULL || text == NULL || strcmp(text, "") == 0 || strcmp(text, " ") == 0)
        return FALSE;

    char* templ = templates[(int)key - '1'];
    char* pos = strchr(templ, '%');

    if (pos == NULL)
        return FALSE;

    int pos_i = pos - templ;
    int len = strlen(templ) + strlen(text);
    char* result = (char*)enter_the_arena(ClipzArena, len + 1);

    strncpy(result, templ, pos_i);
    result[pos_i] = '\0';

    strcat(result, text);
    strcat(result, templ + pos_i + 1);

    if (!SetClipboardText(result)) {
        return FALSE;
    }

    Paste();

    return TRUE;
}
