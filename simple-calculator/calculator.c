#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int operation, a, b, result;

	while (operation != 0) {
		printf("\n------------------------------\n");
		printf("  --- Simple Calculator ---  \n");
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
		else if (operation == 0) {
			printf("Bye !\n");
			return (0);
		}
		else {
			
			printf("Choose the first number (A) for the operation\n");
			scanf("%d", &a);
			printf("A : %d\n", a);

			printf("Choose the second number (B) for the operation\n");
			scanf("%d", &b);
			printf("B : %d\n", b);

			switch (operation) {
				case 1:
					result = a + b;
					printf("Result : %d\n", result);
					break;
				case 2:
					result = a - b;
					printf("Result : %d\n", result);
					break;
				case 3:
					result = a * b;
					printf("Result : %d\n", result);
					break;
				case 4:
					if (b == 0)
						printf("Error: division by zero\n");
					else
					{
						result = a / b;
						printf("Result : %d\n", result);
					}
					break;
			}
		}		
	}

	return (0);
}
