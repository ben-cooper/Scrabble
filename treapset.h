/*change to fit your program*/
#ifndef TREAPTYPE
#define TREAPTYPE unsigned long
#endif

typedef struct treap {
	int priority;
	TREAPTYPE data;
	char *str;
	struct treap *left;
	struct treap *right;
	struct treap *middle;
} treapset;


extern treapset *treap_rrotate(treapset *root);
extern treapset *treap_lrotate(treapset *root);
extern treapset *node_init(treapset *root, TREAPTYPE value, int priority,
			char *str);
extern treapset *treap_insert_p(treapset *root, TREAPTYPE value,
			int priority, char *str);
extern treapset *treap_insert(treapset *root, TREAPTYPE value, char *str);
extern treapset *treap_delete(treapset *root, TREAPTYPE value);
extern int treap_search(treapset *root, TREAPTYPE value);
extern treapset *treap_find(treapset *root, TREAPTYPE value);
extern int treap_length(treapset *root);
extern TREAPTYPE *treap_to_array(treapset *root);
extern void treap_to_array_h(treapset *root, TREAPTYPE *array, int *current);
extern void seed_rand();
extern void destroy_treap(treapset *root, int freewords);
extern int treap_height(treapset *root);
extern void sort_words(treapset *root);
