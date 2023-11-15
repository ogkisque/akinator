#include "akinator.h"

char        UNKNOWN[]   = "��";
const int   MAX_SIZE    = 100;

Error akin_ctor (Tree* tree, const char* name, const char* file, const char* func, int line)
{
    Error error = tree_ctor (tree, name, file, func, line);
    PARSE_ERROR (tree, error);

    Node* node = NULL;
    error = new_node (UNKNOWN, &node);
    PARSE_ERROR(tree, error);

    tree->root = node;
    tree->size = 1;
    RETURN_ERROR(CORRECT, "");
}

Error guess (Node* node)
{
    if (!node)
        RETURN_ERROR(NULL_POINTER, "Null pointer of node.");

    printf ("���� ������ %s? (�� ��� ���)\n", node->str);
    txSpeak ("���� ������ %s? (�� ��� ���)\n", node->str);
    char ans[4] = "";
    scanf ("%s", ans);

    if (node->left)
    {
        if (strcmp (ans, "��") == 0)
            guess (node->left);
        else
            guess (node->right);
    }
    else
    {
        if (strcmp (ans, "��") == 0)
        {
            printf ("� �����\n");
            txSpeak ("� �����");
            RETURN_ERROR(CORRECT, "");
        }
        else
        {
            Error error = add_person (node);
            return error;
        }
    }

    RETURN_ERROR(CORRECT, "");
}

Error add_person (Node* node)
{
    while (getchar () != '\n') ;
    char name[MAX_SIZE] = "";
    char differ[MAX_SIZE] = "";
    printf ("��� ����� ���� ������?\n");
    txSpeak ("��� ����� ���� ������?");
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';
    printf ("��� ���� ������ ���������� �� %s?\n", node->str);
    txSpeak ("��� ���� ������ ���������� �� %s?\n", node->str);
    fgets (differ, MAX_SIZE, stdin);
    differ[strlen (differ) - 1] = '\0';

    Error error = new_node (node->str, &(node->right));
    PARSE_ERROR_WITHOUT_TREE(error);

    error = new_node (name, &(node->left));
    PARSE_ERROR_WITHOUT_TREE(error);

    node->str = strdup (differ);
    if (!(node->str))
        RETURN_ERROR(MEM_ALLOC, "Error in allocation memory for value in node.");

    RETURN_ERROR(CORRECT, "");
}

Error action_with_base (Tree* tree, Actions action)
{
    while (getchar () != '\n') ;
    printf ("������� ��� �����.\n");
    txSpeak ("������� ��� �����");
    char name[MAX_SIZE] = "";
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';

    FILE* file = NULL;
    if (action == OPEN_BASE)
        file = fopen (name, "r");
    else
        file = fopen (name, "w");

    if (!file)
    {
        printf ("������ ����� ���.\n");
        txSpeak ("������ ����� ���");
        RETURN_ERROR(CORRECT, "");
    }

    printf ("������� ������ �������� ������ � ����. (PRE, INF, POST)\n");
    txSpeak ("������� ������ �������� ������ � ����");
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';
    Error error = {};
    error.code = CORRECT;

    Formats format = PRE;
    if (strcmp (name, "PRE") == 0)
        format = PRE;
    else if (strcmp (name, "INF") == 0)
        format = INF;
    else if (strcmp (name, "POST") == 0)
        format =  POST;
    else
    {
        printf ("������������ ������.\n");
        txSpeak ("������������ ������");
        RETURN_ERROR(CORRECT, "");
    }

    if (action == OPEN_BASE)
        error = nodes_read (&(tree->root), file, format);
    else
        error = nodes_print (tree->root, file, format);

    fclose (file);
    return error;
}
/*
Error define (Tree* tree)
{
    Error error = tree_verify (tree);
    PARSE_ERROR (tree, error);

    List* list = NULL;
    MAKE_LIST(&list, 8, true, true);

    char name[MAX_SIZE] = "";
    printf ("���� ��� ����� ����������?\n");
    txSpeak ("���� ��� ����� ����������?");
    while (getchar () != '\n') ;
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';

    bool is_find = find (tree->root, name, list, -1);
    if (!is_find)
    {
        printf ("��� ������.\n");
        txSpeak ("��� ������");
        RETURN_ERROR(CORRECT, "");
    }

    Iterator it = {};
    list_pop_begin (list, &it);

    printf ("%s ", name);
    txSpeak ("%s ", name);
    print_define (tree->root, list);
    list_dtor (list);
    RETURN_ERROR(CORRECT, "");
}

void print_define (Node* node, List* list)
{
    if (get_size (list) == 0)
        return;

    Iterator it = begin_it (list);
    int val = 0;
    get_value (&it, &val);
    list_pop_begin (list, &it);

    if (val == 1)
    {
        printf ("%s", node->str);
        txSpeak ("%s", node->str);
    }
    else
    {
        printf ("�� %s", node->str);
        txSpeak ("�� %s", node->str);
    }

    if (get_size (list) == 0)
    {
        printf (".\n");
    }
    else
    {
        printf (", ");
        if (val == 1)
            print_define (node->left, list);
        else
            print_define (node->right, list);
    }
}

bool find (Node* node, char name[], List* list, int val_list)
{
    if (!node)
        return false;

    Iterator it = {};
    list_push_end (list, val_list, &it);

    if (strcmp (node->str, name) == 0)
        return true;

    bool fin = find (node->left, name, list, 1);
    if (fin)
        return true;

    fin = find (node->right, name, list, 0);
    if (fin)
        return true;

    list_pop_end (list, &it);
    return false;
}

Error compare (Tree* tree)
{
    Error error = tree_verify (tree);
    PARSE_ERROR (tree, error);

    List* list1 = NULL;
    List* list2 = NULL;
    MAKE_LIST(&list1, 8, true, true);
    MAKE_LIST(&list2, 8, true, true);

    char name1[MAX_SIZE] = "";
    char name2[MAX_SIZE] = "";
    printf ("������� ������ ������\n");
    txSpeak ("������� ������ ������");
    while (getchar () != '\n') ;
    fgets (name1, MAX_SIZE, stdin);
    name1[strlen (name1) - 1] = '\0';
    printf ("������� ������ ������\n");
    txSpeak ("������� ������ ������");
    while (getchar () != '\n') ;
    fgets (name2, MAX_SIZE, stdin);
    name2[strlen (name2) - 1] = '\0';

    bool is_find1 = find (tree->root, name1, list1, -1);
    if (!is_find1)
    {
        printf ("������� ������� ���.\n");
        txSpeak ("������� ������� ���");
        RETURN_ERROR(CORRECT, "");
    }
    bool is_find2 = find (tree->root, name2, list2, -1);
    if (!is_find2)
    {
        printf ("������� ������� ���.\n");
        txSpeak ("������� ������� ���");
        RETURN_ERROR(CORRECT, "");
    }

    Iterator it = {};
    list_pop_begin (list1, &it);
    list_pop_begin (list2, &it);

    print_compare (list1, list2);
    RETURN_ERROR(CORRECT, "");
}
*/
Error do_action ()
{
    Actions action = INCORRECT;
    Tree tree = {};
    Error error = {};
    AKIN_CTOR(&tree);

    while (action != END)
    {
        action = choose_action ();
        switch (action)
        {
            case INCORRECT:
                printf ("�������� ��������.\n");
                txSpeak ("�������� ��������");
                break;

            case OPEN_BASE:
                error = tree_dtor (&tree);
                PARSE_ERROR(&tree, error);
                AKIN_CTOR(&tree);
                error = action_with_base (&tree, action);
                PARSE_ERROR(&tree, error);
                printf ("���� �������.\n");
                txSpeak ("���� �������");
                break;

            case SAVE_BASE:
                error = action_with_base (&tree, action);
                PARSE_ERROR(&tree, error);
                printf ("���� ���������.\n");
                txSpeak ("���� ���������");
                break;

            case NEW_BASE:
                if (tree.file)
                {
                    error = tree_dtor (&tree);
                    PARSE_ERROR(&tree, error);
                    AKIN_CTOR(&tree);
                }
                printf ("����� ���� �������.\n");
                txSpeak ("����� ���� �������");
                break;

            case GUESS:
                error = guess (tree.root);
                PARSE_ERROR(&tree, error);
                break;

            case SHOW_BASE:
                tree_graph_dump (&tree, error);
                break;

            case DEFINE:
                //error = define (&tree);
                //PARSE_ERROR(&tree, error);
                break;

            case COMPARE:
                //error = compare (&tree);
                //PARSE_ERROR(&tree, error);
                break;

            case END:
                break;

            default:
                break;
        }
    }
    printf ("����.\n");
    txSpeak ("����");
    RETURN_ERROR(CORRECT, "");
}

Actions choose_action ()
{
    printf ("������ ��������: ������� ���� [OB], ������� ����� ���� [CB], ��������� ���� [SB], �������� ���� [SHB], "
            "������� ������ [GP], �������� ������� [CP], ���������� ������ [DP], ��������� ������ [EW]\n");
    //txSpeak ("������ ��������: ������� ���� [OB], ������� ����� ���� [CB], ��������� ���� [SB], �������� ���� [SHB], "
            //"������� ������ [GP], �������� ������� [CP], ���������� ������ [DP], ��������� ������ [EW]");

    char ans[6] = "";
    scanf ("%s", ans);
    if (strcmp (ans, "[OB]") == 0)
        return OPEN_BASE;
    if (strcmp (ans, "[CB]") == 0)
        return NEW_BASE;
    if (strcmp (ans, "[GP]") == 0)
        return GUESS;
    if (strcmp (ans, "[CP]") == 0)
        return COMPARE;
    if (strcmp (ans, "[SB]") == 0)
        return SAVE_BASE;
    if (strcmp (ans, "[SHB]") == 0)
        return SHOW_BASE;
    if (strcmp (ans, "[DP]") == 0)
        return DEFINE;
    if (strcmp (ans, "[EW]") == 0)
        return END;
    return INCORRECT;
}
