#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <unistd.h>



#define ROWS 5
#define COLUMNS 8

char arr[COLUMNS][ROWS];
int myRandom, Y, B, O, R;
int score1 = 0, score2 = 0;
int turnCount = 0;

// Function declarations
void basicExplosion(int rows, int columns);
void board();
void RadomizeArray();
void updateCountAndSetCell(int row, int col, const char* color);
void simulateProcesses();
void processCell(int row, int col);
void GamePLay();
void rulebook();
void UI_Interface();
void saveGameState(const char* filename, const char* player1, const char* player2);
void loadGameState(const char* filename, char* player1, char* player2);

// Mutex for synchronization
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;


// Function to save the game state to a file
void saveGameState(const char* filename, const char* player1, const char* player2)
 {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    // Write player names and scores to the file
    fprintf(file, "%s %s %d %d\n", player1, player2, score1, score2);

    // Write game board data to the file
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            fprintf(file, "%c ", arr[j][i]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to load the game state from a file
void loadGameState(const char* filename, char* player1, char* player2)
 {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }

    // Read player names and scores from the file
    fscanf(file, "%s %s %d %d", player1, player2, &score1, &score2);

    // Read game board data from the file
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            fscanf(file, " %c", &arr[j][i]);
        }
    }

    fclose(file);
}
void board()
{
    printf("     1   2   3   4   5  \n");
    printf("   +---+---+---+---+---+\n");
    for (int i = 0; i < 8; ++i)
    {
        printf(" %d | ", i + 1);
        for (int j = 0; j < 5; ++j)
        {
            printf("%c | ", arr[i][j]);
        }
        printf("\n");
        printf("  +---+---+---+---+---+\n");
    }
}

void RadomizeArray()
{
    srand(time(0));
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            myRandom = rand() % 4 + 1;
            if (myRandom == 1)
            {
                arr[j][i] = 'R';
            }
            else if (myRandom == 2)
            {
                arr[j][i] = 'Y';
            }
            else if (myRandom == 3)
            {
                arr[j][i] = 'B';
            }
            else
            {
                arr[j][i] = 'O';
            }
        }
    }
}

void updateCountAndSetCell(int row, int col, const char* color)
{
    if (arr[row][col] == color[0]) {
        if (color[0] == 'R') R += 2;
        else if (color[0] == 'Y') Y += 2;
        else if (color[0] == 'B') B += 2;
        else O += 2;
        printf("%s = %c\n", color, color[0] == 'O' ? 'O' : color[0]);
    }

    int myRandom = rand() % 4 + 1;
    arr[row][col] = (myRandom == 1) ? 'R' : ((myRandom == 2) ? 'Y' : ((myRandom == 3) ? 'B' : 'O'));
}

void simulateProcesses() {
    for (int i = 0; i < ROWS; i++)
 {
        for (int j = 0; j < COLUMNS; j++)
 {
            // Wait for the semaphore to be available
            sem_wait(&sem);

            // Create a thread for each element in the array
            pthread_t tid;
            pthread_create(&tid, NULL, (void* (*)(void*))basicExplosion, (void*)(intptr_t)j);
        }
    }
}

void processCell(int row, int col)
{
    usleep(100000);

    if (arr[row][col] == 'O') {
        O++;
        printf("O = %d\n", O);
    }
    else {
        updateCountAndSetCell(row, col, &arr[row][col]);
    }

    if (row >= 2 && arr[row - 1][col] == arr[row + 1][col])
    {
        for (int i = 2; i <= 6 && row - i >= 0 && arr[row - i][col] == arr[row + 1][col]; i++)
        {
            updateCountAndSetCell(row - i, col, &arr[row - i][col]);
        }
    }
    else if (row >= 1 && arr[row - 1][col] == arr[row + 2][col])
    {
        for (int i = 1; i <= 5 && row - i >= 0 && arr[row - i][col] == arr[row + 2][col]; i++)
        {
            updateCountAndSetCell(row - i, col, &arr[row - i][col]);
        }
    }
    else if (row >= 2 && arr[row - 2][col] == arr[row + 2][col]) {
        for (int i = 2; i <= 6 && row - i >= 0 && arr[row - i][col] == arr[row + i][col]; i++)
        {
            updateCountAndSetCell(row - i, col, &arr[row - i][col]);
        }
    }
}


void GamePLay()
{
    int rows, coloumns;
    char player1[50], player2[50];
    int choice;
    int B1 = 0, temp = 0, totalB1 = 0, Y1 = 0, R1 = 0, temp2 = 0, temp3 = 0, totalR1 = 0, totalY1 = 0;
    int R2 = 0, O2 = 0, totalR2 = 0, temp4 = 0, totalO2 = 0, temp5 = 0;
    int B21 = 0, temp21 = 0, totalB21 = 0, Y21 = 0, R21 = 0, temp22 = 0, temp23 = 0, totalR21 = 0, totalY21 = 0;
    int R32 = 0, O32 = 0, totalR32 = 0, temp34 = 0, totalO32 = 0, temp35 = 0;

    B = 0;
    Y = 0;
    R = 0;
    O = 0;



    printf("Please enter the name of the first scientist: \n");
    scanf("%s", player1);
    printf("Please enter the name of the second scientist: \n");
    scanf("%s", player2);



    RadomizeArray();
    board();
    for (int i = 1; i <= 10; i++)
    {

        turnCount++;



        printf("Turn count: %d\n", turnCount);


        if (i % 2 != 0)
        {

            Y = 0, B = 0, O = 0, R = 0;
            printf("%s's turn\n", player1);
            printf("Enter the row no: ");
            scanf("%d", &rows);
            printf("Enter the column no: ");
            scanf("%d", &coloumns);
            basicExplosion(rows, coloumns);
            printf("Select from the potions below\n");
            printf("1- Potion of Wisdom\n");
            printf("2- Sands of Time\n");
            printf("3- Save Game\n");
            printf("Enter: ");
            scanf("%d", &choice);
            printf("\n");

            switch (choice)
            {
            case 1:
                printf("2 Blue\n3 Yellow\n1 Red Needed!\n");
                if (B1 != 2)
                {
                    printf("Input the value of Blue: ");
                    printf("Enter: ");
                    scanf("%d", &B1);
                    printf("\n");
                    while (B1 < 0 || B1 > 2)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Blue (again): \n");
                        printf("Enter: ");
                        scanf("%d", &B1);
                    }
                    printf("\n");
                    temp = B1;
                    totalB1 += temp;
                    B = B - B1;
                }
                if (Y1 != 3)
                {
                    printf("Input the value of Yellow: ");
                    scanf("%d", &Y1);
                    while (Y1 < 0 || Y1 > 3)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Yellow (again): \n");
                        printf("Enter: ");
                        scanf("%d", &Y1);
                    }
                    printf("\n");
                    temp2 = Y1;
                    totalY1 += temp2;
                    Y = Y - Y1;
                }
                if (R1 != 1)
                {
                    printf("Input the value of Red: ");
                    scanf("%d", &R1);
                    while (R1 < 0 || R1 > 1)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Red (again): \n");
                        printf("Enter: ");
                        scanf("%d", &R1);
                    }
                    printf("\n");
                    temp3 = R1;
                    totalR1 += temp3;
                    R = R - R1;
                }

                if (totalB1 == 2 && totalY1 == 3 && totalR1 == 1)
                {
                    score1 = score1 + 5;
                    B1 = 0, Y1 = 0, R1 = 0, totalB1 = 0, temp = 0, temp2 = 0, temp3 = 0, totalR1 = 0, totalY1 = 0;
                }

                printf("%s Score: %d\n", player1, score1);

                break;
            case 2:
                printf("4 Red\n1 Orange Needed\n");
                if (R2 != 4)
                {
                    printf("Input the value of Red: ");
                    scanf("%d", &R2);
                    while (R2 < 0 || R2 > 4)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Red (again): \n");
                        printf("Enter: ");
                        scanf("%d", &R2);
                    }
                    printf("\n");
                    temp4 = R2;
                    totalR2 += temp4;
                    R = R - R2;
                }
                if (O2 != 1)
                {
                    printf("Input the value of Orange: ");
                    scanf("%d", &O2);
                    while (O2 < 0 || O2 > 1)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Orange (again): \n");
                        printf("Enter: ");
                        scanf("%d", &O2);
                    }

                    printf("\n");
                    temp5 = O2;
                    totalO2 += temp5;
                    O = O - O2;
                }
                if (totalR2 == 4 && totalO2 == 1)
                {
                    score1 = score1 + 5;
                    int RD2 = 0, OR2 = 0, totalRD2 = 0, temp4 = 0, totalOR2 = 0, temp5 = 0;
                }
                printf("%s Score: %d\n", player1, score1);
                break;

            case 3:
                // Save the game
                saveGameState("saved.txt", player1, player2);
                printf("Game saved!\n");
                break;
            default:
                printf("Invalid choice\n");
                break;

            }
        }

        else
        {


            Y = 0, B = 0, O = 0, R = 0;
            printf("%s turn\n", player2);
            printf("Enter the row no: ");
            scanf("%d", &rows);
            printf("Enter the column no: ");
            scanf("%d", &coloumns);
            basicExplosion(rows, coloumns);
            printf("\n");
            printf("Select from the potions below: \n");
            printf("1- Potion of Wisdom\n");
            printf("2- Sands of Time\n");
            printf("3- Save Game\n");
            printf("Enter: ");
            scanf("%d", &choice);
            printf("\n");

            switch (choice)
            {

            case 1:
                printf("2 Blue\n 3 Yellow\n1 Red Needed\n");
                if (B21 != 2)
                {
                    printf("Input the value of Blue: ");
                    scanf("%d", &B21);
                    while (B21 < 0 || B21 > 2)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Blue (again): \n");
                        printf("Enter: ");
                        scanf("%d", &B21);
                    }
                    printf("\n");
                    temp21 = B21;
                    totalB21 += temp21;
                    B = B - B21;
                }
                if (Y21 != 3)
                {
                    printf("Input the value of Yellow: ");
                    scanf("%d", &Y21);
                    while (Y21 < 0 || Y21 > 3)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Yellow (again): \n");
                        printf("Enter: ");
                        scanf("%d", &Y21);
                    }
                    printf("\n");
                    temp22 = Y21;
                    totalY21 += temp22;
                    Y = Y - Y21;
                }
                if (R21 != 1)
                {
                    printf("Input the value of Red: ");
                    scanf("%d", &R21);
                    while (R21 < 0 || R21 > 1)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Red (again): \n");
                        printf("Enter: ");
                        scanf("%d", &R21);
                    }
                    printf("\n");
                    temp23 = R21;
                    totalR21 += temp23;
                    R = R - R21;
                }
                if (totalB21 == 2 && totalY21 == 3 && totalR21 == 1)
                {
                    score2 = score2 + 5;
                    B21 = 0, Y21 = 0, R21 = 0, totalB21 = 0, temp21 = 0, temp22 = 0, temp23 = 0, totalR21 = 0, totalY21 = 0;
                }
                printf("%s Score: %d\n", player2, score2);
                break;

            case 2:
                printf("4 Red\n1 Orange Needed\n");
                if (R32 != 4)
                {
                    printf("Input the value of Red: ");
                    scanf("%d", &R32);
                    while (R32 < 0 || R32 > 4)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Red (again): \n");
                        printf("Enter: ");
                        scanf("%d", &R32);
                    }
                    printf("\n");
                    temp34 = R32;
                    totalR32 += temp34;
                    R = R - R32;
                }
                if (O32 != 1)
                {
                    printf("Input the value of Orange: ");
                    scanf("%d", &O32);
                    while (O32 < 0 || O32 > 1)
                    {
                        printf("Wrong value\n");
                        printf("Input the value of Orange (again): \n");
                        printf("Enter: ");
                        scanf("%d", &O32);
                    }
                    printf("\n");
                    temp35 = O32;
                    totalO32 += temp35;
                    O = O - O32;
                }
                if (totalR32 == 4 && totalO32 == 1)
                {
                    score2 = score2 + 5;
                    R32 = 0, O32 = 0, totalR32 = 0, temp34 = 0, totalO32 = 0, temp35 = 0;
                }
                printf("%s Score: %d\n", player2, score2);
                break;

            case 3:
                // Save the game
                saveGameState("saved.txt", player1, player2);
                printf("Game saved!\n");
                break;
            default:
                printf("Invalid choice\n");
                break;


            }
        }
    }
    if (score1 > score2)
    {
        printf("Player 1 WON\n");
    }
    else
    {
        printf("Player 2 WON\n");
    }
}
void basicExplosion(int rows, int columns)
{
    processCell(rows, columns);
    board();
}

void rulebook()
{
    printf("When similar-colored ingredients come into contact, they erupt and swallow them as well! You'll brew your potions faster if the explosions are greater! Four to seven ingredients may be required for each potion creation. The holes on the Potion tiles themselves serve as a representation of them. Place the appropriate coloured marbles on your potion tiles.\n");
    printf("The potion is finished when each hole has a marble of the same colour. The quantity of Ingredients required to make a potion and the potency of the magical power it bestows determine how many points each potion is worth. A player gets a 4 - point Skill token when they finish 3 Potions with the same power or 5 Potions with 5 different powers. The player with the most points wins when the necessary quantity of skill tokens is obtained.\n");
}

void UI_Interface() {
    char player1[50], player2[50];
    char choice[50];

    for (int i = 0; i < 500000; i++) {
        printf("Welcome to the game\n");
        // Code for delay
    }
    system("clear");

   
    turnCount = 0;

    printf("\033[1;36m"); 
    printf("******************************************\n");
    printf("               POTION EXPLOSION\n");
    printf("******************************************\n");
    printf("\033[0m"); 

    printf("1- \033[1;32mPlay\n");
    printf("\033[0m"); 
    printf("2- \033[1;33mRulebook\n");
    printf("\033[0m"); 
    printf("3- \033[1;34mLoad Game\n");
    printf("\033[0m"); 
    printf("4- \033[1;31mExit\n");
    printf("\033[0m"); 

    printf("Enter the choice: ");
    scanf("%s", choice);

    if (strcmp(choice, "1") == 0) {
        system("clear");
        GamePLay();
    }
    else if (strcmp(choice, "2") == 0) {
        system("clear");
        rulebook();
    }
    else if (strcmp(choice, "3") == 0) {
        // Load the game state from the 'load.txt' file
        loadGameState("saved.txt", player1, player2);
        printf("\033[1;32m"); 
        printf("Game loaded!\n");
        printf("\033[0m");

      
        system("clear");
        GamePLay();
    }
    else if (strcmp(choice, "4") == 0) {
        exit(0);
    }
    else {
        printf("\033[1;31m"); // Set text color to red
        printf("Invalid input\n");
        printf("\033[0m"); // Reset text color
    }
}



pthread_t tid[COLUMNS * ROWS];

int main() 

{
    char player1[50], player2[50];

    // Load the game state from the 'saved.txt' file
    loadGameState("saved.txt", player1, player2);

    // Initialize the semaphore with the number of elements
    sem_init(&sem, 0, ROWS * COLUMNS);

   
    simulateProcesses();

    // Generate the random array
    RadomizeArray();

    // Wait for all threads to finish
    for (int i = 0; i < ROWS * COLUMNS; i++) 
{
        pthread_join(tid[i], NULL);
    }

    // Sleep to allow threads to finish
    usleep(1000);

    // Destroy the semaphore
    sem_destroy(&sem);

    // Save the game 
    saveGameState("saved.txt", player1, player2);

    UI_Interface(); 

    return 0;
}


