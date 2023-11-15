#include "akinator.h"
//#include "windows.h"

int main ()
{
    //SetConsoleOutputCP(CP_UTF8);

    Error error = do_action ();
    if (error.code != CORRECT)
    {
        print_error (error);
        return 1;
    }
    return 0;
}
