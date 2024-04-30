#include <stdlib.h>
#include <stdio.h>

int prime_numb(int check_numb){
    int div = 5;

    if (check_numb == 1 || check_numb == 0){
        return 0;
    }
    else if (check_numb <= 3){
        return 1;
    }
    else if (check_numb % 2 == 0 || check_numb % 3 == 0) {
        return 0;
    }
    while (div * div <= check_numb) {
        if (check_numb % div == 0 || check_numb % (div + 2) == 0){
            return 0;
        }
        div += 6 ;
    }
    return 1;
}


int main(){
    int start, end;
    scanf("%d %d", &start, &end);
    int first = 1;
    for (int i = start; i <= end; i++) {
        if (prime_numb(i)) {
            if (first){
                printf("%d", i);
                first = 0;
            }
            else printf(", %d", i);
        }
    }
    return 0;
}
