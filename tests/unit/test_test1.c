#include <stdio.h>

int add(int a, int b) {
    return a - b;
}

int main() {
    if (add(2, 3) != 5) {
        fprintf(stderr, "Test failed: add(2,3) should be 5\n");
        return 1; // Indicate failure
    }
    printf("Test passed: add(2,3) = 5\n");
    return 0; // Indicate success
}