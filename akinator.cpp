#include "akinator.h"

char        UNKNOWN[]   = "хз";
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

    printf ("Твой объект %s? (да или нет)\n", node->str);
    txSpeak ("Твой объект %s? (да или нет)\n", node->str);
    char ans[4] = "";
    scanf ("%s", ans);

    if (node->left)
    {
        if (strcmp (ans, "да") == 0)
            guess (node->left);
        else
            guess (node->right);
    }
    else
    {
        if (strcmp (ans, "да") == 0)
        {
            printf ("Я гений\n");
            txSpeak ("Я гений");
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
    printf ("Как зовут твой объект?\n");
    txSpeak ("Как зовут твой объект?");
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';
    printf ("Чем твой объект отличается от %s?\n", node->str);
    txSpeak ("Чем твой объект отличается от %s?\n", node->str);
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
    printf ("Введите имя файла.\n");
    txSpeak ("Введите имя файла");
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
        printf ("Такого файла нет.\n");
        txSpeak ("Такого файла нет");
        RETURN_ERROR(CORRECT, "");
    }

    printf ("Введите формат хранения дерева в базе. (PRE, INF, POST)\n");
    txSpeak ("Введите формат хранения дерева в базе");
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
        printf ("Некорректный формат.\n");
        txSpeak ("Некорректный формат");
        RETURN_ERROR(CORRECT, "");
    }

    if (action == OPEN_BASE)
        error = nodes_read (&(tree->root), file, format);
    else
        error = nodes_print (tree->root, file, format);

    fclose (file);
    return error;
}

Error define (Tree* tree)
{
    Error error = tree_verify (tree);
    PARSE_ERROR (tree, error);

    List* list = NULL;
    MAKE_LIST(&list, 8, true, true);

    char name[MAX_SIZE] = "";
    while (getchar () != '\n') ;
    get_name (name, "Кого мне нужно определить?");

    bool is_find = find (tree->root, name, list, -1);
    if (!is_find)
    {
        printf ("Нет такого.\n");
        txSpeak ("Нет такого");
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

    for (Iterator it = begin_it (list), it_end = end_it (list); it.index != it_end.index; it = next_it (it))
    {
        int val = 0;
        get_value (&it, &val);

        if (val == 1)
        {
            printf ("%s ", node->str);
            txSpeak ("%s ", node->str);
            node = node->left;
        }
        else
        {
            printf ("не %s ", node->str);
            txSpeak ("не %s ", node->str);
            node = node->right;
        }
    }
    printf ("\n");
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

void get_name (char* name, char* text)
{
    printf ("%s\n", text);
    txSpeak (text);
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';
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
    while (getchar () != '\n') ;
    get_name (name1, "Введите первый объект");
    get_name (name2, "Введите второй объект");

    bool is_find1 = find (tree->root, name1, list1, -1);
    if (!is_find1)
    {
        printf ("Первого объекта нет.\n");
        txSpeak ("Первого объекта нет");
        RETURN_ERROR(CORRECT, "");
    }

    bool is_find2 = find (tree->root, name2, list2, -1);
    if (!is_find2)
    {
        printf ("Второго объекта нет.\n");
        txSpeak ("Второго объекта нет");
        RETURN_ERROR(CORRECT, "");
    }

    Iterator it = {};
    list_pop_begin (list1, &it);
    list_pop_begin (list2, &it);

    print_compare (name1, name2, list1, list2, tree->root, tree->root);
    list_dtor (list1);
    list_dtor (list2);
    RETURN_ERROR(CORRECT, "");
}

void print_compare (char* name1, char* name2, List* list1, List* list2, Node* node1, Node* node2)
{
    Iterator it1 = begin_it (list1);
    Iterator it2 = begin_it (list2);
    Iterator it1_end = end_it (list1);
    Iterator it2_end = end_it (list2);
    int val1 = 0;
    int val2 = 0;
    get_value (&it1, &val1);
    get_value (&it2, &val2);
    if (val1 == val2)
    {
        printf ("%s и %s ", name1, name2);
        txSpeak ("%s и %s ", name1, name2);
    }

    bool was_print = false;
    while ((val1 == val2) && (it1.index != it1_end.index) && (it2.index != it2_end.index))
    {
        was_print = true;
        if (val1 == 1)
        {
            printf ("%s ", node1->str);
            txSpeak ("%s ", node1->str);
            node1 = node1->left;
            node2 = node2->left;
        }
        else
        {
            printf ("не %s ", node1->str);
            txSpeak ("не %s ", node1->str);
            node1 = node1->right;
            node2 = node2->right;
        }
        it1 = next_it (it1);
        it2 = next_it (it2);
        if (it1.index != it1_end.index)
            get_value (&it1, &val1);
        if (it2.index != it2_end.index)
            get_value (&it2, &val2);
    }

    if (it1.index != it1_end.index && was_print)
    {
        if (was_print)
        {
            printf ("но %s при этом ", name1);
            txSpeak ("но %s при этом ", name1);
        }
        else
        {
            printf ("%s ", name1);
            txSpeak ("%s", name1);
        }
    }

    while (it1.index != it1_end.index)
    {
        if (val1 == 1)
        {
            printf ("%s ", node1->str);
            txSpeak ("%s ", node1->str);
            node1 = node1->left;
        }
        else
        {
            printf ("не %s ", node1->str);
            txSpeak ("не %s ", node1->str);
            node1 = node1->right;
        }
        it1 = next_it (it1);
        if (it1.index != it1_end.index)
            get_value (&it1, &val1);
    }

    if (it2.index != it2_end.index)
    {
        printf ("а %s ", name2);
        txSpeak ("а %s ", name2);
    }

    while (it2.index != it2_end.index)
    {
        if (val2 == 1)
        {
            printf ("%s ", node2->str);
            txSpeak ("%s ", node2->str);
            node2 = node2->left;
        }
        else
        {
            printf ("не %s ", node2->str);
            txSpeak ("не %s ", node2->str);
            node2 = node2->right;
        }
        it2 = next_it (it2);
        if (it2.index != it2_end.index)
            get_value (&it2, &val2);
    }
    printf ("\n");
}

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
                printf ("Неверное действие.\n");
                txSpeak ("Неверное действие");
                break;

            case OPEN_BASE:
                error = tree_dtor (&tree);
                PARSE_ERROR(&tree, error);
                AKIN_CTOR(&tree);
                error = action_with_base (&tree, action);
                PARSE_ERROR(&tree, error);
                printf ("База открыта.\n");
                txSpeak ("База открыта");
                break;

            case SAVE_BASE:
                error = action_with_base (&tree, action);
                PARSE_ERROR(&tree, error);
                printf ("База сохранена.\n");
                txSpeak ("База сохранена");
                break;

            case NEW_BASE:
                if (tree.file)
                {
                    error = tree_dtor (&tree);
                    PARSE_ERROR(&tree, error);
                    AKIN_CTOR(&tree);
                }
                printf ("Новая база создана.\n");
                txSpeak ("Новая база создана");
                break;

            case GUESS:
                error = guess (tree.root);
                PARSE_ERROR(&tree, error);
                break;

            case SHOW_BASE:
                tree_graph_dump (&tree, error);
                break;

            case DEFINE:
                error = define (&tree);
                PARSE_ERROR(&tree, error);
                break;

            case COMPARE:
                error = compare (&tree);
                PARSE_ERROR(&tree, error);
                break;

            case END:
                break;

            default:
                break;
        }
    }
    printf ("Пока.\n");
    txSpeak ("Пока");
    RETURN_ERROR(CORRECT, "");
}

Actions choose_action ()
{
    printf ("Выбери действие: открыть базу [OB], создать новую базу [CB], сохранить базу [SB], показать базу [SHB], "
            "угадать объект [GP], сравнить объекты [CP], определить объект [DP], завершить работу [EW]\n");
    //txSpeak ("Выбери действие: открыть базу [OB], создать новую базу [CB], сохранить базу [SB], показать базу [SHB], "
            //"угадать объект [GP], сравнить объекты [CP], определить объект [DP], завершить работу [EW]");

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
