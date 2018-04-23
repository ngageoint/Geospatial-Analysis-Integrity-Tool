/************
GAIT (Geospatial Analysis Integrity Tool) is a geospatial data validation tool developed by the Institute for Defense Analyses (IDA) for the National Geospatial-Intelligence Agency (NGA).  

This source code was used to generate GAIT 26 executable software in accordance with Amendment 6 to Task Order DH-8-3815 under Contract HQ0034-14-D-0001.

IDA is furnishing this item "as is". IDA was not tasked or funded to generate developer documentation or to provide support for this source code. IDA does not provide any warranty of the item whatsoever, whether express, implied, or statutory, including, but not limited to, any warranty of fitness for a particular purpose or any warranty that the contents of the item will be error-free. In no event shall NGA or IDA be held liable for damages arising, directly or indirectly, from the use of this source code. 

This material may be reproduced by the U.S. Government pursuant to its unlimited use rights under DFARS 252.227-7014 [Feb 2014].

The Institute for Defense Analyses (IDA) is a Federally Funded Research and Development Center that provides scientific and technical expertise on issues important to national security to the Office of the Secretary of Defense, Joint Staff, Unified Commands, and Defense Agencies. 

© 2017 Institute for Defense Analyses
  4850 Mark Center Drive
  Alexandria, Virginia 22311-1882
  703.845-2000
  www.ida.org
************/



/******************************************************************

  rb.c

  A generalized Red Black Tree implementation.

  This code is so far as I can manage an exact C language
  implemenation of the pseudocode algorithms given in the book
  "Algorithms" by  Cormen, Leiserson, and Rivest.

  Notes:

  1. I currently do not have a routine to delete an individual node.
     If we need one, I'll include it later.


  2. The Programmer *using* this utility does not generally have
     visibility into a "node".  This is all inside the tree.  The
	 current node is maintained by the library.

 ******************************************************************/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *color_message[] = {"Red", "Black"};
char *rb_message[] = { "Okay", "Duplicate Key", "No Tree",
                       "Key Not Found", "Structural Error in Tree", "Undefined Error" };


#include "rb.h"



/* Convenience macros */


#define IS_LEFT_NODE(node)  (node->parent && (node->parent->left == node) ? 1 : 0)
#define IS_RIGHT_NODE(node) (node->parent && (node->parent->right == node) ? 1 : 0)



/* private function prototypes --

   the public prototypes are over in rb.h

*/

void rbPPaux(RedBlackTree *, RedBlackNode *);
void p(char *,int);
void rbRotateLeft(RedBlackTree *, RedBlackNode *);
void rbRotateRight(RedBlackTree *, RedBlackNode *);
void rbDestroyAllNodes(RedBlackTree *, RedBlackNode *);
RB_status rbInsertNode(RedBlackTree *, RedBlackNode *);
RB_status rbTreeInsert(RedBlackTree *, RedBlackNode *);




char *getColorMessage( NodeColor c)
{
  if (c < 2) return color_message[c];

  return "Unknown color";

}

char *getRBMessage( RB_status status)
{

  if (status < 5) return rb_message[status];

  return "Unknown status";

}



void rbFreeRedBlackTree( RedBlackTree *tree)
{


  RB_status status;
  void *data;

  if (!tree) return;

  status = rbGetFirst(tree, &data);
  while (  status == rbOkay) {
    tree->freeNodeFunction(data);
    status = rbGetNext(tree, &data);
  }

  free(tree);

}


void rbPrettyPrint( RedBlackTree *tree)
{


  RB_status status;
  void *data;

  if (!tree) return;

  status = rbGetFirst(tree, &data);
  while (  status == rbOkay) {
    tree->prettyPrintNodeFunction(data);
    status = rbGetNext(tree, &data);
  }

  free(tree);

}




/******************************************************************
  Create and initialize a red/black tree.
    func = key comparison function
*/
RedBlackTree *rbCreateRedBlackTree( int  (*compare_func)( void *,  void *),
				    void (*free_func)( void *),
				    void (*pp_func)( void *),
                                    int keyOffset, int keyLength, int flags)
{
  RedBlackTree *tree;

  tree = (RedBlackTree *) malloc(sizeof(RedBlackTree));

  if ( !tree ) {
    printf("%s (%d) out of memory allocating space", __FILE__, __LINE__);
    exit(1);
  }

  tree->keyOffset = keyOffset;
  tree->keyLength = keyLength;
  tree->flags = flags;                      /* FLAGS ARE CURRENTLY UNUSED */
  tree->freeNodeFunction = free_func;
  tree->prettyPrintNodeFunction = pp_func;
  tree->root = RB_NILL;

  /* if key compare function is passed as a parameter, set it; otherwise
     default to integer comparison
  */
  if (compare_func) {
    tree->keyCompareFunction = compare_func;
  } else {
    tree->keyCompareFunction = compInteger;
  }

  return tree;
}


RedBlackNode *rbCreateRedBlackNode( void *data)
{
  RedBlackNode *node;

  node = (RedBlackNode *) malloc(sizeof(RedBlackNode));
  if ( !node ) {
    printf("%s (%d) out of memory allocating space", __FILE__, __LINE__);
    exit(1);
  }

  node->data =  data;
  node->color = nBlack;
  node->parent = RB_NILL;
  node->left = RB_NILL;
  node->right = RB_NILL;

  return node;
}


void rbRotateLeft(RedBlackTree *tree, RedBlackNode *x)
{
  RedBlackNode *y;

  if (!x) {
    printf("x is nill in rotateleft\n");
    return;
  }


  y = x->right;

  if (y) x->right = y->left;

  if (y && y->left)
    y->left->parent = x;
  y->parent = x->parent;

  if (!x->parent) {
    tree->root = y;
  } else {
    if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
  }

  y->left = x;
  x->parent = y;

}

void rbRotateRight(RedBlackTree *tree, RedBlackNode *x)
{
  RedBlackNode *y;

  if (!x) {
    printf("x is nill in rotate right\n");
    return;
  }


  y = x->left;

  x->left = y->right;
  if (y->right) y->right->parent = x;

  if (y) y->parent = x->parent;

  if (x->parent) {
    if (x == x->parent->right) {
      x->parent->right = y;
    } else {
      x->parent->left = y;
    }
  } else {
    tree->root = y;
  }

  y->right = x;
  if (x) x->parent = y;
}


RB_status rbTreeInsert(RedBlackTree *tree, RedBlackNode *node)
{
  RedBlackNode *y = RB_NILL;
  RedBlackNode *x;
  int compare_result;
  /* int *d = (int *) node->data; */

  x = tree->root;
  while ( x ) {
    y = x;
    compare_result = tree->keyCompareFunction( getKey(tree,node), getKey(tree,x));
    if ( compare_result == 0) {
      return rbDuplicate;
    } else if (compare_result > 0) {
      x = x->left;
    } else {
      x = x->right;
    }
  }


  node->parent = y;
  if ( !y ) {
    tree->root = node;
  } else {

    compare_result = tree->keyCompareFunction(getKey(tree,node), getKey(tree,y) );

    if ( compare_result == 0) {
      return rbDuplicate;
    } else if (compare_result > 0) {
      y->left = node;
    } else {
      y->right = node;
    }

  }

  return rbOkay;
}


void p(char *s, int x)
{

  printf("%s %d\n", s, x);

}
 
RB_status rbInsertNode(RedBlackTree *tree, RedBlackNode *x)
{

  RB_status status = rbOkay;
  RedBlackNode *y;

  if (!tree) {
    printf("%s (%d) attempt to add node to a non-existent tree", __FILE__, __LINE__);
    return rbNoTree;
  }
 
/***printf("inserting node\n");
***/
  if (!tree->root) {
/***printf("inserting at root\n");
***/
    tree->root = x;
    x->parent = RB_NILL;
    return status;
  }

  status = rbTreeInsert(tree, x);

  if (status == rbDuplicate) return status;

  
  x->color = nRed;

  while ( (x != tree->root) && (x->parent->color == nRed) ) {
    if (x->parent == x->parent->parent->left) {
      y = x->parent->parent->right;
      if (y && (y->color == nRed)) {
	x->parent->color = nBlack;
	y->color = nBlack;
	x->parent->parent->color = nRed;
	x = x->parent->parent;
      } else {
	if (x == x->parent->right) {
	  x = x->parent;
	  rbRotateLeft(tree, x);
	}
	x->parent->color = nBlack;
	x->parent->parent->color = nRed;
	rbRotateRight(tree, x->parent->parent);
      }
    } else {
      y = x->parent->parent->left;
      if (y && (y->color == nRed)) {
	x->parent->color = nBlack;
	y->color = nBlack;
	x->parent->parent->color = nRed;
	x = x->parent->parent;
      } else {
	if (x == x->parent->left) {
	  x = x->parent;
	  rbRotateRight(tree, x);
	}
	x->parent->color = nBlack;
	x->parent->parent->color = nRed;
	rbRotateLeft(tree, x->parent->parent);

      }
    }

  }

  tree->root->color = nBlack;

  return status;
}



RB_status rbInsertData(RedBlackTree *tree, void *data)
{
  RedBlackNode *node;

  node = (RedBlackNode *) malloc(sizeof(RedBlackNode));
/***printf("data insert\n");
***/
  node = rbCreateRedBlackNode(data);
  return rbInsertNode( tree, node);
}


int compInteger(void *x1, void *y1)
{

  int *x = (int *) x1;
  int *y = (int *) y1;

  return *y - *x;
}
/*
int compString(char  *s, char *t)
{
  return strcmp(s,t);
}


double epsilon = .001;

int compDouble(double  *x, double *y)
{

	if ( (*y - *x) <= epsilon ) {
		return 0;
	} else if ( *y - *x > 0 ) {
		return 1;
	} else {
		return -1;
	}
}
*/

void rbPPaux(RedBlackTree *tree, RedBlackNode *node)
{

  int *idata;

  if (!node) return;

  rbPPaux(tree, node->left);

  idata =  (int *) node->data;
  printf("%10d %5s", *idata, getColorMessage(node->color));
  if (node->left) {
    idata =  (int *) node->left->data;
    printf(" L:%10d ", *idata);
  } else {
	printf("              ");
  }
  if (node->right) {
    idata =  (int *) node->right->data;
    printf(" R:%10d ", *idata);
  } else {
	printf("              ");
  }
  if (node->parent) {
    idata =  (int *) node->parent->data;
    printf(" P:%10d ", *idata);
  } else {
	printf("       (ROOT) ");
  }

  printf("\n");
  rbPPaux(tree, node->right);

}

void rbPP(RedBlackTree *tree)
{

  if (!tree) return;

  if (tree->root)
    rbPPaux(tree, tree->root);
  else
    printf("Tree is empty\n");

  printf("\n");
}

void rbTestRoutine()
{
  int i;

  int *v, *result;
  RB_status status;
  RedBlackTree *tree;

  tree = rbCreateRedBlackTree(compInteger, NULL, NULL, 0, sizeof(int), 0);

  for (i = 1; i< 15; i++) {
    v = (int *) malloc(sizeof(int));
    *v = (int) rand();
    status = rbInsertData(tree, (void *) v);
  }

  rbPP(tree);

  status = rbGetLast(tree, (void **) &result);
  printf("*** %d\n", *result);
  while ( rbGetPrevious(tree, (void **) &result) == rbOkay) {
    printf("*** %d\n", *result);    
  }

  rbPP(tree);
  rbDestroyTree(tree);
}

/*
typedef  struct KEYSS {
    int a;
    int b;
    int c;
  } KEY;
  */

RB_status rbGetData(RedBlackTree *tree, void *key, void **data)
{

  RedBlackNode *node; 
  int found = 0;
  int compare_result;
  /*
  KEY *k1, *k2;
  */

  node = tree->root;

  /*
    k1 = (KEY *) getKey(tree, node);
printf("k1: %d %d %d\n", k1->a, k1->b, k1->c);
*/


  while (node && !found) {
    /*
    k1 = (KEY *) getKey(tree, node);
    k2 = (KEY *) key;

printf("k1: %d %d %d\n", k1->a, k1->b, k1->c);
printf("k2: %d %d %d\n", k2->a, k2->b, k2->c);
*/

    compare_result = tree->keyCompareFunction(getKey(tree,node), key);

    if (compare_result == 0) {
      found = 1;
    } else if (compare_result > 0) {
      node = node->right;
    } else {
      node = node->left;
    }
  }

  if (found) {
    *data = node->data;
    return rbOkay;
  } else {
    *data = 0;
    return rbKeyNotFound;
  }

}


/* If the tree is more than 50 deep, something is seriously screwed up
 */
#define TREE_CHECK(c) if(c++ >50) { return rbBadTreeStructure; }

RB_status rbGetNext(RedBlackTree *tree, void **data)
{
  RedBlackNode *node;

  int count = 0;


  if (!tree) return rbNoTree;

  node = tree->currentNode;

  if (!node) return rbKeyNotFound;

  if (node->right) {
    node = node->right;
    while (node->left) {
      node = node->left;
      TREE_CHECK(count);
    }
    tree->currentNode = node;
    *data = (void *) node->data;
    return rbOkay;
  }

  if (!node->parent) return rbKeyNotFound;


  if (IS_LEFT_NODE(node)) {

    node = node->parent;
    tree->currentNode = node;
    *data = (void *) node->data;
    return rbOkay;
  } else if ( IS_RIGHT_NODE(node) ) {
    while (IS_RIGHT_NODE(node) ) {
      node = node->parent;
      TREE_CHECK(count);
    }
    if (node->parent) {
      node = node->parent;
      tree->currentNode = node;
      *data = (void *) node->data;
      return rbOkay;
    } else {
      tree->currentNode = RB_NILL;
      return rbKeyNotFound;
    }
  }  

  return rbBadTreeStructure;

}

RB_status rbGetPrevious(RedBlackTree *tree, void **data)
{

  RedBlackNode *node;

  int count = 0;


  if (!tree) return rbNoTree;

  node = tree->currentNode;

  if (!node) return rbKeyNotFound;

  if (node->left) {
    node = node->left;
    while (node->right) {
      node = node->right;
      TREE_CHECK(count);
    }
    tree->currentNode = node;
    *data = (void *) node->data;
    return rbOkay;
  }

  if (!node->parent) return rbKeyNotFound;


  if (IS_RIGHT_NODE(node)) {
    node = node->parent;
    tree->currentNode = node;
    *data = (void *) node->data;
    return rbOkay;
  } else if ( IS_LEFT_NODE(node) ) {
    while (IS_LEFT_NODE(node) ) {
      node = node->parent;
      TREE_CHECK(count);
    }
    if (node->parent) {
      node = node->parent;
      tree->currentNode = node;
      *data = (void *) node->data;
      return rbOkay;
    } else {
      tree->currentNode = RB_NILL;
      return rbKeyNotFound;
    }
  }  

  return rbBadTreeStructure;

}



RB_status rbGetFirst(RedBlackTree *tree, void **data)
{
  RedBlackNode *node;

  if (!tree) return rbKeyNotFound;

  node = tree->root;

  if (!node) return rbKeyNotFound;

  while (node->left) {
    node = node->left;
  }

  tree->currentNode = node;
  *data = (void *) node->data;
  
  return rbOkay;
}

RB_status rbGetLast(RedBlackTree *tree, void **data)
{
  RedBlackNode *node;

  if (!tree) return rbNoTree;

  node = tree->root;

  if (!node) return rbKeyNotFound;

  while (node->right) {
    node = node->right;
  }

  tree->currentNode = node;
  *data = (void *) node->data;
  
  return rbOkay;
}



void rbDestroyAllNodes(RedBlackTree *tree, RedBlackNode *node)
{

  if (!node) return;

  rbDestroyAllNodes(tree, node->left);
  rbDestroyAllNodes(tree, node->right);

  if (tree->freeNodeFunction) {
    tree->freeNodeFunction(node);
  } else {
    free(node->data);
  }

  free (node);
}

void rbDestroyTree(RedBlackTree *tree)
{

  if (!tree) return;

  rbDestroyAllNodes(tree, tree->root);


  free (tree);

}

