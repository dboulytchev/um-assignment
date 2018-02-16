
#ifndef _LIST_H_
#define _LIST_H_

#include "base_utils.h"

struct list_head {
  struct list_head* prev;
  struct list_head* next;
};

static inline void init_list_head(struct list_head* list) {
  list->prev = list;
  list->next = list;
}

static inline void list_add_after(struct list_head* new, struct list_head* head) {
  new->prev = head;
  new->next = head->next;
  new->prev->next = new;
  new->next->prev = new;
}

static inline void list_prepend(struct list_head* new, struct list_head* head) {
  list_add_after(new, head);
}

static inline void list_append(struct list_head* new, struct list_head* head) {
  list_add_after(new, head->prev);
}

static inline void list_del(struct list_head* entry) {
  entry->next->prev = entry->prev;
  entry->prev->next = entry->next;
  entry->next = NULL;
  entry->prev = NULL;
}

#define list_entry(ptr, type, member) \
  container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
  list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
  for (pos = (head)->next; pos != (head); pos = pos->next)

#define LIST_HEAD_INITIALIZER(name) \
  { .prev = &(name), .next = &(name) }

#endif // _LIST_H_
