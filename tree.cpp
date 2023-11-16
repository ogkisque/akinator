#include "tree.h"

#include "Dotter.h"

const char* NAME_DOT        = "pic.dot";
const int   MAX_TEXT_SIZE   = 200;
const int   MAX_SIZE        = 100;

Error new_node (char* value, Node** adres)
{
    if (!adres)
        RETURN_ERROR(NULL_POINTER, "Null pointer of pointer of adress.");

    *adres = (Node*) calloc (1, sizeof (Node));

    if (!(*adres))
        RETURN_ERROR(MEM_ALLOC, "Error of allocation memory of node.");

    Error error = node_ctor (value, *adres);
    return error;
}

Error node_ctor (char* value, Node* node)
{
    node->str = strdup (value);
    if (!(node->str))
        RETURN_ERROR(MEM_ALLOC, "Error in allocation memory for value in node.");

    node->left  = NULL;
    node->right = NULL;
    RETURN_ERROR(CORRECT, "");
}

Error tree_ctor (Tree* tree, const char* name, const char* file, const char* func, int line)
{
    if (!tree)
        RETURN_ERROR(NULL_POINTER, "Null pointer of tree.");

    if (!name || !file || !func || line < 0)
        RETURN_ERROR(INCOR_PARAMS, "Incorrect parameters in constructor");

    tree->size = 0;
    tree->root = NULL;
    tree->name = name;
    tree->file = file;
    tree->func = func;
    tree->line = line;

    RETURN_ERROR(CORRECT, "");
}

Error tree_dtor (Tree* tree)
{
    if (!tree)
        RETURN_ERROR(NULL_POINTER, "Null pointer of pointer of tree.");

    nodes_dtor (tree->root);
    tree->size = 0;
    tree->root = NULL;
    RETURN_ERROR(CORRECT, "");
}

void nodes_dtor (Node* node)
{
    if (node->left)
        nodes_dtor (node->left);
    if (node->right)
        nodes_dtor (node->right);
    free (node->str);
    node->str = NULL;
    free (node);
    node = NULL;
}

Error tree_verify (Tree* tree)
{
    if (!tree)                                      RETURN_ERROR(NULL_POINTER,      "Null pointer of tree.");
    if (!tree->file || !tree->func || !tree->name)  RETURN_ERROR(INCOR_PARAMS,      "Null pointer of parameters of tree.");
    RETURN_ERROR(CORRECT, "");
}

void tree_dump (Tree* tree, Error error)
{
    printf (RED_COL);
    printf ("-------------------------------------\n");
    if (error.code != CORRECT)
    {
        print_error (error);
        if (!tree)
        {
            printf (OFF_COL);
            return;
        }

        printf ("Error in tree: %s\n"
                "Called from file: %s, func: %s, line: %d\n",
                tree->name, tree->file, tree->func, tree->line);
    }
    printf ("Size - %llu\n", tree->size);
    printf (YELLOW_COL);
    nodes_print (tree->root, stdout, PRE);
    printf ("\n");
    printf (RED_COL);
    printf ("-------------------------------------\n");
    printf (OFF_COL);
}

Error nodes_print (Node* node, FILE* file, Formats format)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file.");

    if (!node)
    {
        fprintf (file, "nil ");
        RETURN_ERROR(CORRECT, "");
    }

    fprintf (file, "{ ");
    if (format == PRE)
        fprintf (file, "<%s> ", node->str);
    nodes_print (node->left, file, format);

    if (format == INF)
        fprintf (file, "<%s> ", node->str);
    nodes_print (node->right, file, format);

    if (format == POST)
        fprintf (file, "<%s> ", node->str);
    fprintf (file, "} ");

    RETURN_ERROR(CORRECT, "");
}

Error nodes_read (Node** node, FILE* file, Formats format)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file.");

    char text[MAX_SIZE] = "";
    fscanf (file, "%s ", text);
    if (strcmp (text, "{") != 0)
    {
        RETURN_ERROR(CORRECT, "");
    }
    Error error = new_node (0, node);

    if (format == PRE)
    {
        error = read_value (file, node);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    nodes_read (&((*node)->left), file, format);

    if (format == INF)
    {
        error = read_value (file, node);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    nodes_read (&((*node)->right), file, format);

    if (format == POST)
    {
        error = read_value (file, node);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    fscanf (file, "%s ", text);
    return error;
}

Error read_value (FILE* file, Node** node)
{
    char value[MAX_SIZE] = "";
    read_name (file, value);

    (*node)->str = strdup (value);
    if (!((*node)->str))
        RETURN_ERROR(MEM_ALLOC, "Error in allocation memory for value in node.");

    RETURN_ERROR(CORRECT, "");
}

void read_name (FILE* file, char* value)
{
    char c = '<';
    fscanf (file, "%c", &c);
    fscanf (file, "%c", &c);
    for (int i = 0; c != '>'; fscanf (file, "%c", &c), i++)
        value[i] = c;
}

void print_error (Error error)
{
    printf ("Error: %s\n"
            "Code: %d\n"
            "File: %s, function: %s, line: %d\n",
            error.message,
            error.code,
            error.file, error.func, error.line);
}

void tree_graph_dump (Tree* tree, Error error)
{
    dtBegin (NAME_DOT);
    nodes_graph_dump (tree->root, 1);
    if (error.code != CORRECT)
        error_graph_dump (tree, error);
    dtEnd ();
    dtRender (NAME_DOT);
}

void nodes_graph_dump (Node* node, size_t counter)
{
    char text[MAX_TEXT_SIZE] = "";
    dtNodeStyle ().shape        ("box")
                  .style        ("rounded, filled")
                  .fontcolor    ("black")
                  .fillcolor    ("#F77152");
    dtLinkStyle ().style        ("bold")
                  .color        ("#4682B4");

    sprintf (text, "%s", node->str);
    dtNode ((int) counter, text);
    if (node->left)
    {
        nodes_graph_dump (node->left, counter * 2 + 1);
        dtLink ((int) counter, (int) counter * 2 + 1, "yes");
    }
    if (node->right)
    {
        nodes_graph_dump (node->right, counter * 2 + 2);
        dtLink ((int) counter, (int) counter * 2 + 2, "no");
    }
}

void error_graph_dump (Tree* tree, Error error)
{
    char text[MAX_TEXT_SIZE] = "";
    dtNodeStyle ().shape        ("box")
                  .style        ("rounded, filled")
                  .fontcolor    ("black")
                  .fillcolor    ("#FFFF00");

    if (!tree)
        sprintf (text,
                "Error: %s\n"
                "Code: %d\n"
                "File: %s, function: %s, line: %d\n",
                error.message, error.code, error.file, error.func, error.line);
    else
        sprintf (text,
                "Error in list: %s\n"
                "Called from file: %s, func: %s, line: %d\n"
                "Error: %s\n"
                "Code: %d\n"
                "File: %s, function: %s, line: %d\n",
                tree->name, tree->file, tree->func, tree->line,
                error.message, error.code, error.file, error.func, error.line);

    dtNode (0, text);
}
