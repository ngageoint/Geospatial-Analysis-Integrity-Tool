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


#ifndef __RB_H
#define __RB_H


/* Flags:

  There are no flags at this point.
  The following are dummies to show the kind of thing
  they "might" be used for.

 */
#define RB_FIXED_LENGTH_STRING_KEY 1
#define RB_KEY_STRING_IS_POINTER 2
#define RB_ALLOW_DUPLICATES 4

#define flagAllowDuplicates(tree) (tree->flags & RB_ALLOW_DUPLICATES)
#define flagFixedLengthStringKey(tree) \
   (tree->flags & RB_FIXED_LENGTH_STRING_KEY)
#define flagKeyStringIsPointer(tree) \
   (tree->flags & RB_KEY_STRING_IS_POINTER)




/* Definition of a node in the Red/Black Tree
 */
typedef enum NCOLOR {nRed, nBlack} NodeColor;

struct RBNode {
  struct RBNode *left;
  struct RBNode *right;
  struct RBNode *parent;
  NodeColor color;
  void *data;
};
typedef struct RBNode RedBlackNode;
#define RB_NILL (RedBlackNode *) 0




/* Definition of a Red/Black Tree
 */
struct RBTree {
  int keyOffset;
  int keyLength;
  int flags;
  int (*keyCompareFunction) (void *, void *);
  void (*prettyPrintNodeFunction) (void *);
  void (*freeNodeFunction) (void *);
  struct RBNode *root;
  struct RBNode *currentNode;
};
typedef struct RBTree RedBlackTree;
#define NO_TREE (RedBlackTree *) 0


typedef enum { rbOkay, rbDuplicate, rbNoTree,
               rbKeyNotFound, rbBadTreeStructure } RB_status;




/* These are the basic functions you need to use a red black tree
 */
RedBlackTree *rbCreateRedBlackTree( int  (*func1) (void *, void *),
                                    void (*func2) (void *),
                                    void (*func3) (void *),
                                    int, int, int);
void rbDestroyTree(RedBlackTree *);
RB_status rbInsertData(RedBlackTree *, void *);
RB_status rbGetData(RedBlackTree *, void *, void **);
RB_status rbGetFirst(RedBlackTree *, void **);
RB_status rbGetLast(RedBlackTree *, void **);
RB_status rbGetNext(RedBlackTree *, void **);
RB_status rbGetPrevious(RedBlackTree *, void **);
void rbTestRoutine(void);



/* To compare data for the red-black tree, you can
   use these functions, or define your own.

   Note:
      The comparison function will determine the 'lexical'
      ordering of the data - which key 'comes before' another
      key in the context of the problem space.

      If you right this function, the arguments should be of
      the form
         int compType ( KeyType *key1, KeyType *key2)
         where key is the type of the key
	 the return type of the function is integer -
	 if the keys are equal, the function should return 0,
         if key1 < key2, the function should return > 0,
         if key1 > key2, the function should return < 0
         [note that this works like the strcmp() function]
*/
int compInteger( void *, void *);
/*
int compDouble(  double *, double *);
int compString(  char *, char *);
int compShort( short *, short *);
int compLong( long *, long *);
*/

/* The following functions should not be needed to use
   the Red Black Tree functions.
*/
RB_status rbInsertNode(RedBlackTree *, RedBlackNode *);
RedBlackNode *rbCreateRedBlackNode( void * );
void rbPP(RedBlackTree *);
void rbDestroyAllNodes(RedBlackTree *, RedBlackNode *);
RB_status rbGetData(RedBlackTree *, void *, void **);
RB_status rbInsertData(RedBlackTree *, void *);
RB_status rbGetNext(RedBlackTree *, void **);
RB_status rbGetPrevious(RedBlackTree *, void **);
RB_status rbGetFirst(RedBlackTree *, void **);
RB_status rbGetLast(RedBlackTree *, void **);
char *getColorMessage( NodeColor );
char *getRBMessage( RB_status);
void rbFreeRedBlackTree(RedBlackTree *);
void rbPrettyPrint( RedBlackTree *tree);

/* Possibly useful macros to get better use of the KeyCompareFunction
 */
#define keyEQ(tree, k1, k2) (tree->keyCompareFunction(k1, k2) == 0 ? 1 : 0)
#define keyNE(tree, k1, k2) (tree->keyCompareFunction(k1, k2) != 0 ? 1 : 0)
#define keyLT(tree, k1, k2) (tree->keyCompareFunction(k1, k2) <  0 ? 1 : 0)
#define keyLE(tree, k1, k2) (tree->keyCompareFunction(k1, k2) <= 0 ? 1 : 0)
#define keyGT(tree, k1, k2) (tree->keyCompareFunction(k1, k2) >  0 ? 1 : 0)
#define keyGE(tree, k1, k2) (tree->keyCompareFunction(k1, k2) >= 0 ? 1 : 0)


/* note this deals ONLY WITH POINTERS! */
#define getKey(tree, node) ( (void *) ( (long) node->data + (long) tree->keyOffset))



#endif


