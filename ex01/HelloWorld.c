#include <stdio.h>

int main() {
    char name[100];
    printf("Please enter your name: ");
    scanf("%s", name);
    printf("Hello, %s! Welcome to GitHub.\n", name);
    return 0;
}
