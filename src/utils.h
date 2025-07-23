#pragma once

#include <windows.h>

BOOL SetClipboardText(const char* text);
char* GetClipboardText(); 
char* Copy();
void Paste();