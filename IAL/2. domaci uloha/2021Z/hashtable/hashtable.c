#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++) {
		(*table)[i] = NULL;
	}
}

ht_item_t *ht_search(ht_table_t *table, char *key) {
	ht_item_t *item = (*table)[get_hash(key)];
	while (item != NULL) {
		if (strcmp(key, item->key) == 0) {
			return item;
		}
    item = item->next;
	}
	return NULL;
}

void ht_insert(ht_table_t *table, char *key, float value) {
	ht_item_t *item = ht_search(table, key);
	if (item == NULL) {
    ht_item_t *insert_item = (ht_item_t *) malloc(sizeof(ht_item_t));
    if (insert_item != NULL) {
      insert_item->key = key;
      insert_item->value = value;
      insert_item->next = NULL;
      int hash = get_hash(key);
      if ((item = (*table)[hash])) {	// synonyma? tak prvek pred synonyma.
        insert_item->next = item;
      }
      (*table)[hash] = insert_item;
    }
	} else {
		item->value = value;
  }
}

float *ht_get(ht_table_t *table, char *key) {
	ht_item_t *item = ht_search(table, key);
	return item ? &(item->value) : NULL;
}

void ht_delete(ht_table_t *table, char *key) {
  int hash = get_hash(key);
	ht_item_t *item = (*table)[hash];
	ht_item_t *previous_item = NULL, *next_item = NULL;
	while (item != NULL) {
		next_item = item->next;
		if (strcmp(key, item->key) == 0) {
			if (previous_item == NULL) {
				(*table)[hash] = next_item;
			} else {
			  previous_item->next = next_item;
      }
      free(item);
      return;
		}
    previous_item = item;
    item = item->next;
	}
}

void ht_delete_all(ht_table_t *table) {
  ht_item_t *next_item, *current_item;
	for (int i = 0; i < HT_SIZE; i++) {
		next_item = (*table)[i];
		while (next_item != NULL) {
			current_item = next_item;
			next_item = next_item->next;
			free(current_item);
		}
		(*table)[i] = NULL;
	}
}