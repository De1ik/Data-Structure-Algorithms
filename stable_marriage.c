#include <stdio.h>
#include <string.h>

// Number of girls/boys
#define N 10


int preferably(int favor[2 * N][N], int w, int m, int m1)
{
    for (int i = 0; i < N; i++)
    {
        if (favor[w][i] == m1)
            return 1;
        if (favor[w][i] == m)
            return 0;
    }
    return 0;
}


void stable_marriage(int favor[2 * N][N])
{

    int women_partner[N];
    int man_fee[N];

    memset(women_partner, -1, sizeof(women_partner));
    memset(man_fee, 0, sizeof(man_fee));
    int counter = N;


    while (counter > 0)
    {
        int m;
        for (m = 0; m < N; m++)
            if (man_fee[m] == 0)
                break;

        for (int i = 0; i < N && man_fee[m] == 0; i++)
        {
            int w = favor[m][i];


            if (women_partner[w - N] == -1)
            {
                women_partner[w - N] = m;
                man_fee[m] = 1;
                counter--;
            }
            else
            {
                int m1 = women_partner[w - N];


                if (preferably(favor, w, m, m1) == 0)
                {
                    women_partner[w - N] = m;
                    man_fee[m] = 1;
                    man_fee[m1] = 0;
                }
            }
        }
    }

    const char *names[] = {"Sofka", "Alisa", "Masha", "Uliana", "Dasha", "Milana", "Diana", "Zuzana", "Nastia", "klara"
            ,"Artem", "Dima_O", "Dmitro_K", "Julian", "Maksym", "Nazar_Mac", "Nazar_B", "Nazar_Stend", "Semen", "Roma"};


    printf("%24s\n","---------------------------------");
    printf("|%10s\t|%10s\t|\n", "Girls", "Boys");
    for (int i = 0; i < N; i++) {
        printf("%24s\n", "---------------------------------");
        printf("|%13s\t|%10s\t|\n", names[i + N], names[women_partner[i]]);
    }
    printf("%24s\n","---------------------------------");
}


int main()
{
    char sign = 's';
    char start_sign = 'q';


    do{
        printf("Hello!\n");
        printf("Now You will see the result for the couple girls-boys.\n");
        printf("The priority of choosing girls is more than the priority of boys.\n");
        printf("On the left will be the girls' names, next to them will be the boys' names.\n\n");
        printf("To start the program click 'ENTER':\n");
        scanf("%c", &start_sign);
    }while (start_sign == 's');


    while (sign != 'k') {
        int prefer[2 * N][N] = {
                {11, 12, 13, 14, 15, 16, 17, 18, 19, 10},
                {14, 11, 15, 12, 10, 16, 13, 17, 18, 19},
                {19, 18, 17, 16, 15, 14, 13, 12, 11, 10},
                {17, 11, 19, 18, 16, 15, 10, 12, 13, 14},
                {18, 11, 13, 14, 15, 16, 10, 19, 12, 17},
                {10, 12, 14, 16, 18, 11, 13, 15, 17, 19},
                {11, 13, 15, 17, 19, 10, 12, 14, 16, 18},
                {18, 16, 14, 13, 12, 11, 10, 19, 17, 15},
                {14, 15, 16, 10, 11, 12, 13, 17, 19, 18},
                {16, 10, 19, 15, 14, 17, 18, 13, 11, 12},

                {1,  3,  5,  7,  9,  0,  2,  4,  6,  8},
                {8,  6,  4,  3,  2,  1,  0,  9,  7,  5},
                {4,  5,  6,  0,  1,  2,  3,  7,  9,  8},
                {6,  0,  9,  5,  4,  7,  8,  3,  1,  2},
                {9,  8,  7,  6,  5,  4,  3,  2,  1,  0},
                {7,  1,  9,  8,  6,  5,  0,  2,  3,  4},
                {8,  1,  3,  4,  5,  6,  0,  9,  2,  7},
                {1,  2,  3,  4,  5,  6,  7,  8,  9,  0},
                {4,  1,  5,  2,  0,  6,  3,  7,  8,  9},
                {0,  2,  4,  6,  8,  1,  3,  5,  7,  9},
        };
        printf("\n");
        stable_marriage(prefer);

        printf("To over the program enter 'k':\n");
        printf("To restart the program click 'ENTER':\n");
        scanf("%c", &sign);
    }

    return 0;
}