#include <stdio.h>
#include <stdlib.h>


//convert number in string foram to 2 part of int -> int and fractional
void make_int(char buffer[100], long *main, long *fact) {
    int counter = 0;
    int comma = 0;
    sscanf(buffer, "%ld", &(*main));


    while (buffer[counter] != '\0') {
        if (buffer[counter] == ',') {
            comma = counter;
        }
        counter++;
    }
    if (buffer[comma] == ',') {
        buffer[comma] = '1';
        sscanf(buffer + comma, "%ld", fact);
    } else {
        (*fact) = 1;
        comma = counter - 1;
    }

    for (int i = 0; i < 10 - (counter - comma); i++) {
        (*fact) *= 10;
    }
}

int finder(int amount, long *main_number, long *fractional_number, int main_find, long fact_find) {
    int index;
    int left = 0;
    int right = amount - 1;


    //binary search
    while (left <= right) {

        index = left + ((right - left) / 2);

        if (((main_number[index] == main_find) && (fractional_number)[index] == fact_find)) {
            return index + 1;
        } else if ((main_number[index] < main_find) ||
                   ((main_number[index] == main_find) && (main_number[index] >= 0) &&
                    (fractional_number)[index] < fact_find) ||
                   ((main_number[index] == main_find) && (main_number[index] < 0) &&
                    (fractional_number)[index] > fact_find)) {
            left = index + 1;
        } else {
            right = index - 1;
        }
    }
    return 0;
}


int main() {
    int numbers_amount;
    int find_amount;
    long *main_number = NULL;
    long *fractional_number = NULL;
    char buffer[100];

    //make the array for saving input data
    scanf("%d", &numbers_amount);
    main_number = (long *) malloc(numbers_amount * sizeof(long));
    fractional_number = (long *) malloc(numbers_amount * sizeof(long));

    //filling the arrays
    for (int i = 0; i < numbers_amount; i++) {
        scanf("%s", buffer);
        make_int(buffer, main_number + i, fractional_number + i);
    }

    //input data which we need find
    scanf("%d", &find_amount);
    long main_find, fact_find;
    for (int i = 0; i < find_amount; i++) {
        scanf("%s", buffer);
        make_int(buffer, &main_find, &fact_find);

        int res = finder(numbers_amount, main_number, fractional_number, main_find, fact_find);

        if (i + 1 != find_amount) { printf("%d\n", res); }
        else { printf("%d", res); }
    }

    // FREE
    free(main_number);
    free(fractional_number);

    return 0;
}
