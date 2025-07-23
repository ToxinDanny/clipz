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
char** ClipzStack;
short stack_counter = 0;
HHOOK hHook = NULL;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
char* Copy();
BOOL SaveOnHotkeyMap(HKMap* map, char key, char* value);
BOOL PasteFromHotkeyMap(HKMap* map, char key);
