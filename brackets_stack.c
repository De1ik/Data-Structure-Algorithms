#include <stdlib.h>
#include <stdio.h>


int check_number(char a, char b){
    if (a == '{' && b == '}') return 1;
    else if (a == '<' && b == '>') return 1;
    else if (a == '[' && b == ']') return 1;
    else if (a == '(' && b == ')') return 1;
    else return 0;
}


int main(){
    int amount;
    char *array_m, *stack;
    array_m = (char*) malloc(sizeof(char) * 100000);
    int mistakes;
    int index_array;
    int index_stack;

    scanf("%d", &amount);

    for (int i = 0; i < amount; i++){
        scanf("%s", array_m);

        index_array = 0;
        index_stack = 0;
        mistakes = 0;
        while (array_m[index_array] != '\0'){
            if (array_m[index_array] == '(' || array_m[index_array] == '{' || array_m[index_array] == '[' || array_m[index_array] == '<'){
                array_m[index_stack] = array_m[index_array];
                index_stack++;
            }
            else{
                if (check_number(array_m[index_stack-1], array_m[index_array])){
                    index_stack--;
                }
                else{
                    if (index_stack > 0){
                        index_stack--;
                    }
                    mistakes+=1;
                };
            }
            index_array++;
        }
        mistakes += index_stack;
        if (i+1 == amount) printf("%d", mistakes);
        else printf("%d\n", mistakes);
    }
    free(array_m);
    return 0;
}





