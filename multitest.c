#include <stdio.h>
#include "treapset.h"
#include <stdlib.h>

void basic_tests() {
	treapset *test = NULL;
	test = treap_insert(test, 10, "hello");
	test = treap_insert(test, 10, "goodbye");
	test = treap_insert(test, 5, "blarg");
	printf("%s\n", test->str);
	printf("%s\n", test->middle->str);
	test = treap_delete(test, 10);
	printf("\n%s\n", test->str);
}

int main() {
	seed_rand();
	basic_tests();
	return 0;
}
