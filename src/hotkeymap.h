#pragma once

#ifndef VK_NUMPAD0
#define VK_NUMPAD0 0x60
#define VK_NUMPAD1 0x61
#define VK_NUMPAD2 0x62
#define VK_NUMPAD3 0x63
#define VK_NUMPAD4 0x64
#define VK_NUMPAD5 0x65
#define VK_NUMPAD6 0x66
#define VK_NUMPAD7 0x67
#define VK_NUMPAD8 0x68
#define VK_NUMPAD9 0x69
#endif

#include <stdio.h>
#include "memoryarena.h"

typedef struct _KeyValue {

	char key;
	char* value;

} KV;

typedef struct _HotKeyMap {

	KV* kv;

} HKMap;

BOOL try_add(HKMap* map, KV* kv);
BOOL try_get(HKMap* map, char key, KV* kv);
void print(HKMap* map);

