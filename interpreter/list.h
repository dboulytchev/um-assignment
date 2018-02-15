
#ifndef _LIST_H_
#define _LIST_H_

struct list_head {
  struct list_head* next;
  struct list_head* prev;
};

static inline void init_list_head(struct list_head* list) {
  list->next = list;
  list->prev = list;
}

static inline void list_add(struct list_head* new, struct list_head* head) {
  new->prev = head;
  new->next = head->next;
  new->prev->next = new;
  new->next->prev = new;
}

static inline void list_del(struct list_head* entry) {
  entry->next->prev = entry->prev;
  entry->prev->next = entry->next;
}

#define list_entry(ptr, type, member) \
  container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
  list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
  for (pos = (head)->next; pos != (head); pos = pos->next)

#endif // _LIST_H_
