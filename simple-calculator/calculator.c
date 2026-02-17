#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int operation;

	printf("Welcome - Simple Calculator\n");

	while (operation != 0) {
		printf("------------------------------\n");
		printf("------------------------------\n");
		printf("1) Add\n");
		printf("2) Subtract\n");
		printf("3) Multiply\n");
		printf("4) Divide\n");
		printf("0) Quit\n");
		printf("------------------------------\n");

		printf("Choose a number for the arithmetic operation you want to apply\n"); 
		
		scanf("%d", &operation);

		printf("Choice : %d\n", operation);

		if (operation < 0 || operation > 4)
			printf("Invalid choice\n");
	}

	printf("Bye !\n");
	return (0);
}
