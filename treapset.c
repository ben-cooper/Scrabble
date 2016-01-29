#include <stdio.h>
#include <stdlib.h>
#include "treapset.h"
#include <time.h>

void destroy_treap(treapset *root, int freewords) {
	/*recursively destroying treap*/

	if (root->left != NULL)
		destroy_treap(root->left, freewords);
	if (root->right != NULL)
		destroy_treap(root->right, freewords);
	if (root->middle != NULL)
		destroy_treap(root->middle, freewords);

	if (freewords == 1)
		free(root->str);
	free(root);
}

treapset *treap_rrotate(treapset *root) {
	/*right rotation*/
	treapset *result;
	result = root->left;
	root->left = root->left->right;
	result->right = root;
	return result;
}

treapset *treap_lrotate(treapset *root) {
	/*left rotation*/
	treapset *result;
	result = root->right;
	root->right = root->right->left;
	result->left = root;
	return result;
}

treapset *node_init(treapset *root, TREAPTYPE value, int priority, char *str) {
	treapset *new_node;
	if ((new_node = (treapset *) malloc(sizeof(treapset))) == NULL) {
		fprintf(stderr, "Out of memory!\n");
		destroy_treap(root, 1);
		exit(1);
	}
	new_node->data = value;
	new_node->priority = priority;
	new_node->str = str;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->middle = NULL;

	return new_node;
}

treapset *treap_insert_p(treapset *root, TREAPTYPE value, int priority, char *str) {
	treapset *new_node;
	treapset *temp;
	/*base case*/
	if (root == NULL) {
		return node_init(root, value, priority, str);
	} else if (value < root->data) {
		/*recursive insert into left*/
		root->left = treap_insert_p(root->left, value, priority, str);
		if (root->priority > root->left->priority)
			root = treap_rrotate(root);
	} else if (value > root->data) {
		/*recursive insert into right*/
		root->right = treap_insert_p(root->right, value, priority, str);
		if (root->priority > root->right->priority)
			root = treap_lrotate(root);
	} else {
		/*elements are equal - insert into middle*/
		new_node = node_init(root, value, priority, str);
		temp = root->middle;
		root->middle = new_node;
		new_node->middle = temp;
	}
	return root;

}

void seed_rand() {
	time_t t;
	srand((unsigned) time(&t));
}

treapset *treap_insert(treapset *root, TREAPTYPE value, char *str) {
	return treap_insert_p(root, value, rand(), str);
}

treapset *treap_delete(treapset *root, TREAPTYPE value) {
	treapset *result;
	/*base case*/
	if (root == NULL)
		return NULL;
	if (root->data == value) {
		if (root->middle != NULL) {
			/*there is a middle child which will replace the deleted node*/
			root->middle->left = root->left;
			root->middle->right = root->right;
			result = root->middle;
			free(root->str);
			free(root);
			return result;
		}
		else if (root->right == NULL) {
			if (root->left == NULL)  {
				/*no children*/
				free(root->str);
				free(root);
				return NULL;
			}
			/*left child only*/
			result = root->left;
			free(root->str);
			free(root);
			return result;
		} else if (root->left == NULL){
			/*right child only*/
			result = root->right;
			free(root->str);
			free(root);
			return result;
		} else {
			/*both children*/
			/*bubbling down with lower priority child until leaf*/
			if (root->left->priority <= root->right->priority) {
				root = treap_rrotate(root);
				root->right = treap_delete(root->right, value);
			} else {
				root = treap_lrotate(root);
				root->left = treap_delete(root->left, value);
			}
		}
	} else if (value < root->data) {
		root->left = treap_delete(root->left, value);
	} else {
		root->right = treap_delete(root->right, value);
	}
	return root;

}


int treap_search(treapset *root, TREAPTYPE value) {
	if (root == NULL)
		return 0;
	if (value < root->data)
		return treap_search(root->left, value);
	if (value > root->data)
		return treap_search(root->right, value);
	return 1;
}

treapset *treap_find(treapset *root, TREAPTYPE value) {
	if (root == NULL)
		return NULL;
	if (value < root->data)
		return treap_find(root->left, value);
	if (value > root->data)
		return treap_find(root->right, value);
	return root;
}


int treap_length(treapset *root) {
	int counter = 1;
	if (root->left != NULL)
		counter += treap_length(root->left);
	if (root->right != NULL)
		counter += treap_length(root->right);
	if (root->middle != NULL)
		counter += treap_length(root->middle);
	return counter;
}

void treap_to_array_h(treapset *root, TREAPTYPE *array, int *current) {
	if (root->left != NULL) 
		treap_to_array_h(root->left, array, current);

	if (root->middle != NULL)
		treap_to_array_h(root->middle, array, current);

	array[*current] = root->data;
	*current += 1;

	if (root->right != NULL)
		treap_to_array_h(root->right, array, current);
}

TREAPTYPE *treap_to_array(treapset *root) {
	/*this counter will keep track of the current array index*/
	TREAPTYPE *result;
	int count = 0;
	int *counter = &count;
	/*calculating length of treap*/
	if ((result = (TREAPTYPE *) malloc(treap_length(root) * sizeof(TREAPTYPE))) == NULL) {
		fprintf(stderr, "Out of memory!\n");
		destroy_treap(root, 1);
		exit(1);
	}

	treap_to_array_h(root, result, counter);
	return result;


}

int treap_height(treapset *root) {
	int left;
	int right;

	if (root == NULL)
		return -1;

	left = treap_height(root->left);
	right = treap_height(root->right);

	if (left >= right) {
		return 1 + left;
	} else {
		return 1 + right;
	}
}

void sort_words(treapset *root) {
	if (root->left != NULL) 
		sort_words(root->left);
		
	printf("%s", root->str);
	if (root->middle != NULL)
		sort_words(root->middle);

	if (root->right != NULL)
		sort_words(root->right);

}




