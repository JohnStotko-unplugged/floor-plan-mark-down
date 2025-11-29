//
//
//
//
//
//
//

#include <stdio.h>

const int TEST_SUCCESS = 0;
const int TEST_FAILURE = 1;

int test_failed(const char* message)
{
    printf("FAILED\n");
    printf("    ERROR: %s", message);
    return TEST_FAILURE;
}

int test_passed()
{
    printf("PASSED\n");
    return TEST_SUCCESS;
}