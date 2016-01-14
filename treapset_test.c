#include "treapset.h"
#include <stdio.h>

#define TREAPTYPE int


void inorder(treapset *root) {
	if (root->left != NULL)
		inorder(root->left);
	printf("%lld,", root->data);
	if (root->right != NULL)
		inorder(root->right);
}

void bigtest() {
	long long i;
	treapset *test;
	test = treap_insert(NULL, 0);
	for (i=1; i < 1000000; i++)
		test = treap_insert(test, i*i);

	inorder(test);

	printf("\n\n\n\n\n\n\n\n\n\n\n");

	for (i=100000; i < 1000000; i++)
		test = treap_delete(test, i*i);
	
	inorder(test);


}

void array_tests() {
	treapset *test;
	test = treap_insert(NULL, 0);
	long long i;
	for (i=1; i < 10; i++)
		test = treap_insert(test, i);
	
	printf("%d\n", treap_length(test));

	long long *array = treap_to_array(test);
	printf("%lld.%lld.%lld.%lld.%lld.%lld.%lld.%lld.%lld.%lld\n", array[0], array[1], 
	array[2], array[3], array[4], array[5], array[6], array[7], array[8], array[9]);

	//printf("%lld\n", array[0]);
}

void basic_tests() {
	treapset *test;

	test = treap_insert(NULL, 5);
	test = treap_insert(test, 4);
	test = treap_insert(test, 3);
	test = treap_insert(test, 10);
	test = treap_insert(test, 1);
	test = treap_insert(test, 8);
	test = treap_insert(test, 7);
	test = treap_insert(test, 2);
	test = treap_insert(test, 6);
	test = treap_insert(test, 9);

	inorder(test);
	//preorder(test);
	printf("\n");

	test = treap_delete(test, 6);
	//preorder(test);
	printf("\n");
	test = treap_delete(test, 3);

	inorder(test);
	//preorder(test);
	printf("\n");

	printf("\n");
	if (treap_search(test, 4))
		printf("GOOD!\n");
	if (treap_search(test, 6))
		printf("BAD!\n");
}




int main(int argc, char **argv) {

	//basic_tests();

	//bigtest();

	array_tests();

	return 0;
}
