#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int operation_number;

	while (operation_number != 0) {
		printf("Welcome - Simple Calculator\n");

		printf("------------------------------\n");
		printf("1) Add\n");
		printf("2) Subtract\n");
		printf("3) Multiply\n");
		printf("4) Divide\n");
		printf("0) Quit\n");
		printf("------------------------------\n");

		printf("Choose a number for the arithmetic operation you want to apply\n"); 
		
		scanf("%d", &operation_number);

		printf("Choice : %d\n", operation_number);
	}

	printf("Bye !\n");
	return (0);
}
