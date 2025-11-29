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
    printf("  <filename>    Specify input file. If absent, program \n");
    printf("                will read from standard input\n");
}



int main(int argc, char *argv[])
{
    FILE* fp;
    fp = stdin;

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

        fp = fopen(arg, "r");
        if(fp == NULL)
        {
            perror("Error opening file");
            return 1;
        }
    }

    fpmd_convert(fp, stdout);
    
    if (fp != stdin)
		(void)fclose(fp);

    return 0;
}




