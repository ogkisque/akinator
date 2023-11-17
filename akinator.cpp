#include "akinator.h"

char            UNKNOWN[]       = "хз";
const int       MAX_SIZE        = 100;
const char*     TEMP_FILE_NAME  = "temporary.txt";
const char*     INFO_FILE_NAME  = "files_info.txt";
const char*     FILES_DIR       = "files/";


#define PRINT_SAY(...)          \
        printf (__VA_ARGS__);   \
        txSpeak (__VA_ARGS__);


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

    PRINT_SAY("Твой объект %s? (да или нет)\n", node->str);
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
            PRINT_SAY("Я гений\n");
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
    PRINT_SAY("Как зовут твой объект?\n");
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';
    PRINT_SAY("Чем твой объект отличается от %s?\n", node->str);
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

void print_file_names (FILE* file)
{
    char name[MAX_SIZE] = "";
    char format[MAX_SIZE] = "";
    while (fscanf (file, "%s %s", name, format) == 2)
        printf ("%s\n", name);
}

Formats parse_format (char* format)
{
    if (strcmp (format, "PRE"))
        return PRE;
    if (strcmp (format, "INF"))
        return INF;
    if (strcmp (format, "POST"))
        return POST;
    return PRE;
}

Formats get_format (FILE* file, char* name_file)
{
    char name[MAX_SIZE] = "";
    char format[MAX_SIZE] = "";
    while (fscanf (file, "%s %s", name, format) == 2)
    {
        if (strcmp (name_file, name) == 0)
            return parse_format (format);
    }
    return PRE;
}

Error action_with_base (Tree* tree, Actions action)
{
    FILE* info = fopen (INFO_FILE_NAME, "r");

    while (getchar () != '\n') ;
    if (action == OPEN_BASE)
    {
        PRINT_SAY("Введите имя файла для открытия. Вам доступны:\n");
    }
    else
    {
        PRINT_SAY("Введите имя файла для сохранения. Можете создать новый либо сохранить в следующие:\n");
    }
    print_file_names (info);

    char name[MAX_SIZE] = "";
    fgets (name, MAX_SIZE, stdin);
    name[strlen (name) - 1] = '\0';
    char file_name[MAX_SIZE] = "";
    strcpy (file_name, FILES_DIR);
    strcat (file_name, name);

    FILE* file = NULL;
    FILE* new_info = NULL;
    if (action == OPEN_BASE)
    {
        file = fopen (file_name, "r");
    }
    else
    {
        file = fopen (file_name, "w");
    }

    if (!file)
    {
        PRINT_SAY("Такого файла нет.\n");
        RETURN_ERROR(CORRECT, "");
    }

    Error error = {};
    error.code = CORRECT;

    if (action == OPEN_BASE)
    {
        Formats format = get_format (info, name);
        error = nodes_read (&(tree->root), file, format);
        fclose (info);
    }
    else
    {
        fclose (info);
        new_info = fopen (INFO_FILE_NAME, "a");
        add_file (new_info, name);
        error = nodes_print (tree->root, file, PRE);
        fclose (new_info);
    }

    fclose (file);
    return error;
}

void add_file (FILE* info, char* file_name)
{
    if (have_name (info, file_name))
        return;

    fprintf (info, "%s PRE", file_name);
}

bool have_name (FILE* info, char* file_name)
{
    char name[MAX_SIZE] = "";
    char format[MAX_SIZE] = "";
    while (fscanf (info, "%s %s", name, format) == 2)
    {
        if (strcmp (file_name, name) == 0)
            return true;
    }
    return false;
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
        PRINT_SAY("Нет такого.\n");
        RETURN_ERROR(CORRECT, "");
    }

    Iterator it = {};
    list_pop_begin (list, &it);

    PRINT_SAY("%s ", name);
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
            PRINT_SAY("%s ", node->str);
            node = node->left;
        }
        else
        {
            PRINT_SAY("не %s ", node->str);
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
    PRINT_SAY("%s\n", text);
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
        PRINT_SAY("Первого объекта нет.\n");
        RETURN_ERROR(CORRECT, "");
    }

    bool is_find2 = find (tree->root, name2, list2, -1);
    if (!is_find2)
    {
        PRINT_SAY("Второго объекта нет.\n");
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
        PRINT_SAY("%s и %s ", name1, name2);
    }

    bool was_print = false;
    while ((val1 == val2) && (it1.index != it1_end.index) && (it2.index != it2_end.index))
    {
        was_print = true;
        if (val1 == 1)
        {
            PRINT_SAY("%s ", node1->str);
            node1 = node1->left;
            node2 = node2->left;
        }
        else
        {
            PRINT_SAY("не %s ", node1->str);
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
            PRINT_SAY("но %s при этом ", name1);
        }
        else
        {
            PRINT_SAY("%s ", name1);
        }
    }

    while (it1.index != it1_end.index)
    {
        if (val1 == 1)
        {
            PRINT_SAY("%s ", node1->str);
            node1 = node1->left;
        }
        else
        {
            PRINT_SAY("не %s ", node1->str);
            node1 = node1->right;
        }
        it1 = next_it (it1);
        if (it1.index != it1_end.index)
            get_value (&it1, &val1);
    }

    if (it2.index != it2_end.index)
        PRINT_SAY("а %s ", name2);

    while (it2.index != it2_end.index)
    {
        if (val2 == 1)
        {
            PRINT_SAY("%s ", node2->str);
            node2 = node2->left;
        }
        else
        {
            PRINT_SAY("не %s ", node2->str);
            node2 = node2->right;
        }
        it2 = next_it (it2);
        if (it2.index != it2_end.index)
            get_value (&it2, &val2);
    }
    printf ("\n");
}

void greetings (Tree* tree)
{
    PRINT_SAY("Привет, вернуться к состоянию, на котором ты закончил(а) в предыдущий раз? (да или нет)\n");

    char ans[5] = "";
    scanf ("%s", ans);
    if (ans == "нет")
        return;

    FILE* file = fopen (TEMP_FILE_NAME, "r");
    nodes_read (&(tree->root), file, PRE);
    fclose (file);
}

Error do_action ()
{
    Actions action = INCORRECT;
    Tree tree = {};
    Error error = {};
    AKIN_CTOR(&tree);
    greetings (&tree);
    FILE* file = fopen (TEMP_FILE_NAME, "w");

    while (action != END)
    {
        action = choose_action ();
        switch (action)
        {
            case INCORRECT:
                PRINT_SAY ("Неверное действие\n")
                break;

            case OPEN_BASE:
                error = tree_dtor (&tree);
                PARSE_ERROR(&tree, error);
                AKIN_CTOR(&tree);
                error = action_with_base (&tree, action);
                PARSE_ERROR(&tree, error);
                PRINT_SAY ("База открыта\n")
                break;

            case SAVE_BASE:
                error = action_with_base (&tree, action);
                PARSE_ERROR(&tree, error);
                PRINT_SAY ("База сохранена\n")
                break;

            case NEW_BASE:
                if (tree.file)
                {
                    error = tree_dtor (&tree);
                    PARSE_ERROR(&tree, error);
                    AKIN_CTOR(&tree);
                }
                PRINT_SAY ("Новая база создана\n")
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
                nodes_print (tree.root, file, PRE);
                break;

            default:
                break;
        }
    }
    PRINT_SAY ("Пока\n")
    fclose (file);
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
