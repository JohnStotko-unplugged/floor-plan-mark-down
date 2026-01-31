#include <stdio.h>
#include <string.h>

#include "fpmd.h"

const char *VERSION = "1.0.0";

void printVersion()
{
    printf("Program Version: %s\n", VERSION);
}

void printHelp()
{
    printf("Usage: program [options]\n");
    printf("Options:\n");
    printf("  -h, --help    Show this help message\n");
    printf("  -v, --version Show program version\n");
}


int main(int argc, char *argv[])
{
    FILE* fp;
    fp = stdin;

    int action = FPMD_ACTION_DEFAULT;

    if(argc == 2)
    {
        char* arg = argv[1];
        
        if(strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
        {
            printHelp();
            return 0;
        }

        if(strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0)
        {
            printVersion();
            return 0;
        }

        if(strcmp(arg, "-t") == 0)
        {
            action = FPMD_ACTION_TOKENIZE;
        }
    }

    fpmd_convert(fp, stdout, action);
    
    if (fp != stdin)
		(void)fclose(fp);

    return 0;
}




