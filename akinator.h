#ifndef AKINATOR_HEADER
#define AKINATOR_HEADER
#define  TX_USE_SPEAK

#include "TXLib.h"
#include "tree.h"
//#include "list.h"

#define AKIN_CTOR(tree) \
        akin_ctor (tree, #tree, __FILE__, __func__, __LINE__)

enum Actions
{
    GUESS       = 0,
    OPEN_BASE   = 1,
    SAVE_BASE   = 2,
    COMPARE     = 3,
    END         = 4,
    NEW_BASE    = 5,
    SHOW_BASE   = 6,
    DEFINE      = 7,
    INCORRECT   = 8
};

Error   akin_ctor           (Tree* tree, const char* name, const char* file, const char* func, int line);
Error   guess               (Node* node);
Error   add_person          (Node* node);
Actions choose_action       ();
Error   do_action           ();
Error   action_with_base    (Tree* tree, Actions action);
//Error   define              (Tree* tree);
//void    print_define        (Node* node, List* list);
//bool    find                (Node* node, char name[], List* list, int val_list);
Error   compare             (Tree* tree);

#endif //AKINATOR_HEADER
