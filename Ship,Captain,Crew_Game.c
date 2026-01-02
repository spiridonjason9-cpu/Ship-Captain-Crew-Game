#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    NUM_DICE represents the total number of dice used in the game.
    MAX_THROWS represents the maximum number of throws a player is allowed.
*/
#define NUM_DICE 5
#define MAX_THROWS 3

/*
    Function prototypes:
    - calculateCargo: adds the two cargo dice together.
    - displaySCC: shows progress for Ship (6), Captain (5), and Crew (4).
*/
int calculateCargo(int dice[]);
void displaySCC(int got6, int got5, int got4);

int main() {

    /*
        sortedDice stores the final dice layout:
        index 0 -> Ship (6)
        index 1 -> Captain (5)
        index 2 -> Crew (4)
        index 3-4 -> Cargo dice
    */
    int sortedDice[NUM_DICE] = {0,0,0,0,0};

    /*
        dice[] temporarily stores dice rolled during each throw.
        throwsLeft tracks how many throws remain.
        cargo stores the cargo score.
        choice stores user input for cargo rerolls.
    */
    int dice[NUM_DICE];
    int throwsLeft = MAX_THROWS;
    int cargo = 0;
    char choice;

    /*
        These flags track whether Ship, Captain, and Crew
        have already been obtained.
    */
    int got6 = 0, got5 = 0, got4 = 0;

    /*
        Seed the random number generator so dice rolls
        are different each time the program runs.
    */
    srand(time(NULL));

    /*
        Main game loop:
        Continues while the player still has throws remaining.
    */
    while (throwsLeft > 0) {

        /*
            Determine how many dice to roll.
            If Ship is secured, roll one fewer die.
            If Ship and Captain are secured, roll two fewer dice.
        */
        int numToRoll = NUM_DICE;
        if (got6) numToRoll = NUM_DICE - 1;
        if (got6 && got5) numToRoll = NUM_DICE - 2;

        printf("\n--- Throw (%d left) ---\n", throwsLeft);
        printf("Rolling %d dice: ", numToRoll);

        /*
            Roll the dice and display the results.
            Each die is a random number from 1 to 6.
        */
        for (int i = 0; i < numToRoll; i++) {
            dice[i] = (rand() % 6) + 1;
            printf("%d ", dice[i]);
        }
        printf("\n");

        /*
            One throw has now been used.
        */
        throwsLeft--;

        /*
            Step-by-step acquisition of Ship, Captain, and Crew.
            The program searches for:
            - 6 first (Ship)
            - 5 second (Captain)
            - 4 last (Crew)
            Each value can only be secured once.
        */
        if (!got6) {
            for (int i = 0; i < numToRoll; i++) {
                if (dice[i] == 6) {
                    sortedDice[0] = 6;
                    got6 = 1;
                    break;
                }
            }
        }

        if (got6 && !got5) {
            for (int i = 0; i < numToRoll; i++) {
                if (dice[i] == 5) {
                    sortedDice[1] = 5;
                    got5 = 1;
                    break;
                }
            }
        }

        if (got6 && got5 && !got4) {
            for (int i = 0; i < numToRoll; i++) {
                if (dice[i] == 4) {
                    sortedDice[2] = 4;
                    got4 = 1;
                    break;
                }
            }
        }

        /*
            Display current Ship-Captain-Crew progress
            after each throw.
        */
        displaySCC(got6, got5, got4);

        /*
            If Ship, Captain, and Crew are all obtained,
            the game moves on to cargo calculation.
        */
        if (got6 && got5 && got4) {

            /*
                Fill the cargo dice.
                Ignore the first 6, 5, and 4 found,
                and store the remaining dice as cargo.
            */
            int cargoIndex = 3;
            int used6 = 0, used5 = 0, used4 = 0;

            for (int i = 0; i < numToRoll; i++) {
                if (dice[i] == 6 && !used6) { used6 = 1; continue; }
                if (dice[i] == 5 && !used5) { used5 = 1; continue; }
                if (dice[i] == 4 && !used4) { used4 = 1; continue; }

                if (cargoIndex < NUM_DICE)
                    sortedDice[cargoIndex++] = dice[i];
            }

            /*
                If fewer than two cargo dice exist,
                generate random dice to fill the rest.
            */
            while (cargoIndex < NUM_DICE)
                sortedDice[cargoIndex++] = (rand() % 6) + 1;

            /*
                Calculate and display the cargo score.
            */
            cargo = calculateCargo(sortedDice);
            printf("Ship, Captain, and Crew obtained!\n");
            printf("Cargo score: %d\n", cargo);

            /*
                Allow the player to reroll cargo
                as long as throws remain.
            */
            while (throwsLeft > 0) {
                printf("Do you want to use one of your remaining throws to reroll cargo? (y/n): ");
                scanf(" %c", &choice);

                if (choice != 'y' && choice != 'Y')
                    break;

                throwsLeft--;

                sortedDice[3] = (rand() % 6) + 1;
                sortedDice[4] = (rand() % 6) + 1;

                cargo = calculateCargo(sortedDice);
                printf("New cargo dice: %d,%d\n", sortedDice[3], sortedDice[4]);
                printf("Updated cargo score: %d\n", cargo);
            }

            printf("\n=== GAME OVER ===\n");
            return 0;
        }

        /*
            Losing condition:
            If no throws remain and SCC was not completed,
            the player loses and cargo is zero.
        */
        if (throwsLeft == 0) {
            printf("Out of rolls. Ship, Captain, and Crew not obtained.\n");
            printf("Cargo score: 0\n");
            return 0;
        }
    }

    return 0;
}

/*
    calculateCargo:
    Returns the sum of the two cargo dice.
*/
int calculateCargo(int dice[]) {
    return dice[3] + dice[4];
}

/*
    displaySCC:
    Displays the current progress of Ship (6),
    Captain (5), and Crew (4).
*/
void displaySCC(int got6, int got5, int got4) {
    printf("SCC progress: ");
    if (got6) printf("6{0} "); else printf("- ");
    if (got5) printf("5{1} "); else printf("- ");
    if (got4) printf("4{2} "); else printf("- ");
    printf("\n");
}
