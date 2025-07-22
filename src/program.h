#pragma once

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "memoryarena.h"
#include "utils.h"
#include "hotkeymap.h"

BOOL CopyMode = FALSE;
BOOL StackMode = FALSE;
MemArena* ClipzArena;
HKMap* ClipzHKMap;
KV** ClipzStack;
HHOOK hHook = NULL;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
char* GetSelectedText();
BOOL SaveOnHotkeyMap(HKMap* map, char key, char* value);
BOOL PasteFromHotkeyMap(HKMap* map, char key);
