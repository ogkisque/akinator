#ifndef TREE_HEADER
#define TREE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "colors.h"
#include "error.h"

#define RETURN_ERROR_AND_DUMP(list, code, message)                                          \
        {                                                                                   \
            tree_dump (tree, Error {code, __LINE__, __FILE__, __func__, message});          \
            tree_graph_dump (tree, Error {code, __LINE__, __FILE__, __func__, message});    \
            return Error {code, __LINE__, __FILE__, __func__, message};                     \
        }

#define PARSE_ERROR(tree, error)            \
        if (error.code != CORRECT)          \
        {                                   \
            tree_dump (tree, error);        \
            tree_graph_dump (tree, error);  \
            return error;                   \
        }

#define PARSE_ERROR_WITHOUT_TREE(error)     \
        if (error.code != CORRECT)          \
        {                                   \
            return error;                   \
        }

#define TREE_CTOR(tree) \
        tree_ctor (tree, #tree, __FILE__, __func__, __LINE__)

enum Formats
{
    PRE =   0,
    POST =  1,
    INF =   2
};

struct Node
{
    char*  str;
    Node*  left;
    Node*  right;
};

struct Tree
{
    Node*       root;
    size_t      size;

    const char* name;
    const char* file;
    const char* func;
    int         line;
};

Error   tree_ctor           (Tree* tree, const char* name, const char* file, const char* func, int line);
Error   tree_dtor           (Tree* tree);
Error   nodes_print         (Node* node, FILE* file, Formats format);
Error   nodes_read          (Node** node, FILE* file, Formats format);
Error   read_value          (FILE* file, Node** node);
void    tree_graph_dump     (Tree* tree, Error error);
Error   new_node            (char* value, Node** adres);
Error   node_ctor           (char* value, Node* node);
void    nodes_dtor          (Node* node);
Error   tree_verify         (Tree* tree);
void    tree_dump           (Tree* tree, Error error);
void    nodes_graph_dump    (Node* node, size_t counter);
void    error_graph_dump    (Tree* tree, Error error);
void    print_error         (Error error);
void    read_name           (FILE* file, char* value);
#endif //TREE_HEADER
