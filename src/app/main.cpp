// ===========================================================================
//    Description:  RTex System - main entry 
// ===========================================================================
#include "rex.h"

extern int rex_main(int argc, char * argv[]);
int main(int argc, char *argv[]);


#ifdef _MSC_VER
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd)
{
    int argc = 1;
    char * argv[] = {lpCmdLine};
    return main(argc, argv);
}
#endif // _MSC_VER


int main(int argc, char * argv[])
{
    return rex_main(argc, argv);
}


