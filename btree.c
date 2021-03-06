#include "btree.h"
#include "token.h"
#include "singly_linked_list.h"
#include <stdlib.h>
#include <stdio.h>

TREE_NODE * rotate_left( TREE_NODE* root )
{
  TREE_NODE* node = NULL;
  node = root->left;
  root->left = node->right;
  node->right = root;
  root->height = max( height( root->left ), height( root->right ) ) + 1;
  node->height = max( height( node->left ), root->height ) + 1;
  return node;
}

TREE_NODE* rotate_right( TREE_NODE* root )
{
  TREE_NODE* node;

  node = root->right;
  root->right = node->left;
  node->left = root;

  root->height = max( height( root->left ), height( root->right ) ) + 1;
  node->height = max( height( node->right ), root->height ) + 1;

  return node;
}

TREE_NODE* double_left_rotate( TREE_NODE* root )
{
  root->left = rotate_right( root->left );
  return rotate_left( root );
}

TREE_NODE* double_right_rotate( TREE_NODE* root )
{
  root->right = rotate_left( root->right );
  return rotate_right( root );
}

void insert_avl(TREE_NODE** t, void *data, comparator comparator, int line ){
  int *line_number = malloc(sizeof(int));
  *line_number = line;
  if(!*t){//insert new node in tree
    *t = (TREE_NODE*)malloc(sizeof(TREE_NODE));
    (*t)->data = data;
    append(&( ( (W_TOKEN *)( (*t)->data ) )->list ), line_number);
    (*t)->height = 0;
    (*t)->left = (*t)->right = NULL;
  }
  else if (comparator(data , (*t)->data) < 0 ){
    insert_avl( &((*t)->left), data, comparator, line );
    if( height( (*t)->left ) - height( (*t)->right ) == 2 ){
      if (comparator(data , (*t)->left->data) < 0 ){
        *t = rotate_left( *t );
      }
      else if (comparator(data , (*t)->left->data) > 0 ){
        *t = double_left_rotate( *t );
      }
    }
  }
  else if (comparator(data , (*t)->data) > 0 ){
    insert_avl( &( (*t)->right ), data, comparator, line );
    if( height( (*t)->right ) - height( (*t)->left ) == 2 ){
      if (comparator(data , (*t)->right->data) > 0 ){
        *t = rotate_right( *t );
      }
      else if (comparator(data , (*t)->right->data) < 0 ){
        *t = double_right_rotate( *t );
      }
    }
  }
  else{//word already in tree, add line number to list
    append(&( ( (W_TOKEN *)( (*t)->data ) )->list ), line_number);
  }

  (*t)->height = max( height( (*t)->left ), height( (*t)->right ) ) + 1;
}

int max(int x, int y){
  return x > y ? x : y;
}

int same(TREE_NODE *root1, TREE_NODE *root2){
  if (root1 && root2)
  {
    if(( *( (int *)root1->data ) ) != ( *( (int *)root2->data ) )){
      return 0;
    }
    if(!same(root1->left, root2->left))
      return 0;
    if(!same(root1->right, root2->right))
      return 0;
  }
  else if(root1 && !root2)
    return 0;
  else if(root2 && !root1)
    return 0;

  return 1;

}

void insert(TREE_NODE **root, void *data, comparator comparator){
  if (!*root)
  {
    *root = (TREE_NODE *) malloc(sizeof(TREE_NODE) );
    (*root)->data = data;
    (*root)->right = NULL;
    (*root)->left = NULL;

  }
  else if (comparator(data , (*root)->data) )//left
  {
    insert(&(*root)->left, data, comparator);
  }
  else{
    insert(&(*root)->right, data, comparator);
  }

}

int tree_factor(TREE_NODE *root){
  static int factor = 0;
  if(root){
    factor = abs(balancing_factor(root)) > abs(factor) ? balancing_factor(root) : factor;
    tree_factor(root->left);
    tree_factor(root->right);
  }
  return abs(factor);
}

int balancing_factor(TREE_NODE * root){
  return height(root->left) - height(root->right);
}

static int height( TREE_NODE* n )
{
  if( n == NULL )
    return -1;
  else
    return n->height;
}

int is_leaf(TREE_NODE *root){
  return ( root->left == NULL ) && ( root->right == NULL );
}

void print_pre_left(TREE_NODE *root, print_function f){
  if (root)
  {
    f(root, 0);
    print_pre_left(root->left, f);
    print_pre_left(root->right, f);
  }

}

int tree_size(TREE_NODE *root){
  static int count = 0;
  if(root)
  {
    count++;
    tree_size(root->left);
    tree_size(root->right);
  }
  else
    return count;
  return count;
}

void print_all(TREE_NODE *root, print_function f, int level){
  if (root)
  {
    f(root, level);
    print_all(root->left, f, level + 1);
    print_all(root->right, f, level + 1);
  }

}
