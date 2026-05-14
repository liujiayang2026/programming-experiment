
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ATTEMPTS 10

int main(void) {
    /* Initialize random seed (ensures different numbers each run) */
    srand((unsigned int)time(NULL));

    int playAgain = 1;   /* Whether to restart the game */

    while (playAgain) {
        /* 1. Generate a random number between 1 and 100 */
        int target = rand() % 100 + 1;
        int guess = 0;
        int attempts = 0;          /* Number of attempts made */

        printf("\n========== Guess the Number ==========\n");
        printf("A target number has been generated (1 - 100).\n");
        printf("You have %d attempts. Enter your guess:\n", MAX_ATTEMPTS);

        /* 2. Guessing loop */
        while (attempts < MAX_ATTEMPTS) {
            printf("Attempt %d: ", attempts + 1);

            /* Basic input validation */
            if (scanf("%d", &guess) != 1) {
                printf("Invalid input. Please enter an integer!\n");
                /* Clear the input buffer */
                while (getchar() != '\n');
                continue;
            }

            attempts++;

            if (guess == target) {
                printf("Congratulations! You got it! The number was %d.\n",
                       target);
                break;  /* Exit guessing loop early on correct answer */
            } else if (guess < target) {
                printf("Too low. Try a higher number.\n");
            } else {
                printf("Too high. Try a lower number.\n");
            }

            /* Display remaining attempts */
            if (attempts < MAX_ATTEMPTS) {
                printf("%d attempt(s) remaining.\n",
                       MAX_ATTEMPTS - attempts);
            } else {
                printf("Sorry, you've used all %d attempts. "
                       "The correct number was %d.\n",
                       MAX_ATTEMPTS, target);
            }
        }

        /* 3. Ask whether to play again */
        char choice;
        printf("\nPlay again? (y/n): ");
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            playAgain = 0;
            printf("Thanks for playing! Goodbye.\n");
        }
    }

    return 0;
}
