#include "hotkeymap.h"

BOOL try_add(HKMap* map, KV* kv) {

	if (map == NULL || kv == NULL) {
		fprintf(stderr, "Error: map or kv is null. (errno = %d)", GetLastError());
		return FALSE;
	}

	if (kv->value == NULL || strcmp(kv->value, "") == 0 || strcmp(kv->value, " ") == 0)
		return FALSE;
	
	map->kv[(int)kv->key - 48].key = kv->key;
	map->kv[(int)kv->key - 48].value = kv->value;

	return TRUE;
}

BOOL try_get(HKMap* map, char key, KV* kv) {

	if (map->kv[(int)key - 48].key <= 0)
		return FALSE;

	if (kv == NULL)
		return TRUE;

	kv->key = map->kv[(int)key - 48].key;
	kv->value = map->kv[(int)key - 48].value;

	return TRUE;
}

void print(HKMap* map) {

	if (map == NULL || map->kv == NULL)
		return;

	system("cls");

	for (int i = 1; i <= 9; i++)
	{
		if (map->kv[i].key <= '\0' || map->kv[i].value == NULL || strcmp(map->kv[i].value, "") == 0 || strcmp(map->kv[i].value, " ") == 0)
			continue;

		printf("Ctrl + %c = \"%s\"\n", map->kv[i].key, map->kv[i].value);
	}
}