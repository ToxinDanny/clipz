#pragma once

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "memoryarena.h"
#include "utils.h"
#include "hotkeymap.h"

typedef enum _TMode { TNotEnabled, TEnabled, TActive } TMode;

BOOL CopyMode = FALSE;
BOOL StackMode = FALSE;
TMode TemplateMode = TNotEnabled;
MemArena* ClipzArena;
HKMap* ClipzHKMap;
char** ClipzStack;
short stack_counter = 0;
char** ClipzTemplates;
char hk_actual_key = '\0';
HHOOK hHook = NULL;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL SaveOnHotkeyMap(HKMap* map, char key, char* value);
BOOL PasteFromHotkeyMap(HKMap* map, char key);
BOOL SaveOnStack(char** stack, char* text, short sc);
BOOL PasteFromStack(char** stack, short* sc);
BOOL SaveOnTemplateList(char** templates, char key, char* text);
BOOL PasteFromTemplateList(char** templates, char key, char* text);
