//
// Test report utilities so the make test output looks like
//
// Running 
//
//
//

#include <stdio.h>

const int TEST_SUCCESS = 0;
const int TEST_FAILURE = 1;

const char* COLOR_NORMAL = "\x1B[0m";
const char* COLOR_RED = "\x1B[31m";
const char* COLOR_GREEN = "\x1B[32m";

void report_test_start(const char* test_name)
{
    printf("    %-40s", test_name);
}

int test_failed(const char* message)
{
    printf("%sFAILED%s\n", COLOR_RED, COLOR_NORMAL);
    printf("        %sERROR:%s %s", COLOR_RED, COLOR_NORMAL, message);
    return TEST_FAILURE;
}

int test_passed()
{
    printf("%sPASSED%s\n", COLOR_GREEN, COLOR_NORMAL);
    return TEST_SUCCESS;
}