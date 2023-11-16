#include "akinator.h"

int main ()
{
    Error error = do_action ();
    if (error.code != CORRECT)
    {
        print_error (error);
        return 1;
    }
    return 0;
}
