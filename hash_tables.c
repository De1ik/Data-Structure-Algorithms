


#include <stdio.h>
#include <stdlib.h>

#define DELETED ((void*) 1)



typedef struct {
    char name[16];
    char surname[16];
    char date[12];
    double balance;
} person;


//helpful function
//
int is_name_equal(const char *name, const char *name_check) {
    int i;
    for (i=0; name[i] != '\0' && name_check[i] != '\0'; i++) {
        if (name[i] != name_check[i]) {
            return 0;
        }
    }
    if (name[i] == '\0' && name_check[i] == '\0')
        return 1;
    return 0;
}

void check_start(int *start){
    if (*start == 0){
        *start = 1;
    }
    else {
        printf("\n");
    }
}


double update_balance(double person_balance, double new_balance){
    if (person_balance + new_balance < 0){
        return -1;
    }
    return person_balance + new_balance;
}

//+=
int find_next_prime(int prev_size, double resize) {
    int res = (int) (prev_size * resize);

    while (1) {
        if (res % 2 == 0 || res % 3 == 0) {
            res++;
            continue;
        }
        int prime = 1;
        for (int i = 4; i * i <= res; i++) {
            if (res % i == 0) {
                res++;
                prime = 0;
                break;
            }
        }
        if (prime == 1)
            return res;
    }
}

//++
double string_to_double(char *string_balance){
    int i = 0;
    while (string_balance[i] != '\0' && string_balance[i] != ','){
        i++;
    }
    if (string_balance[i] == ','){
        string_balance[i] = '.';
    }
    double balance;
    sscanf(string_balance, "%lf", &balance);
    return balance;
}


//++
void double_to_string_print(double balance){
    int i = 0;
    char string_balance[16];


    sprintf(string_balance, "%.2lf", balance);

    while (string_balance[i] != '\0' && string_balance[i] != '.'){
        i++;
    }
    if (string_balance[i] == '.'){
        string_balance[i] = ',';
    }

    printf("%s", string_balance);

}
//-----------------------------------------------


person *create_person(char *name, char *surname, char *date, double balance) {
    person *new_user = (person *) malloc(sizeof(person));
    if (new_user == NULL) exit(1);

    sscanf(name, "%s", new_user->name);
    sscanf(surname, "%s", new_user->surname);
    sscanf(date, "%s", new_user->date);
    new_user->balance = balance;
    return new_user;
}


//+
unsigned hash(const char *name, const char *surname, const char *date, int table_size) {
    unsigned long long hash = 5381;
    int maxValue = 10000007;

    int i = 0;
    while (name[i] != '\0' && surname[i] != '\0' && date[i] != '\0') {
        hash += name[i] + 3 * surname[i] + 8 * date[i];
        hash = (hash * (name[i] + surname[i] + date[i])) % maxValue;
        i++;
    }

    return hash % table_size;
}

//+
unsigned hash_step(const char *name, const char *surname, const char *date, int table_size) {
    unsigned long long hash = 5381;
    int maxValue = 10000007;

    int i = 0;
    while (name[i] != '\0' && surname[i] != '\0' && date[i] != '\0') {
        hash += 6 * name[i] + 4 * surname[i] + 2 * date[i];
        hash = (hash * (name[i] + surname[i] + date[i])) % maxValue;
        i++;
    }

    if (hash % table_size == 0){
        return 1;
    }

    return hash % table_size;
}

person **resize_table(int *table_size, person **hash_table, int *deleted_element, double resize) {

    int prev_size = *table_size;
    if (hash_table == NULL){
        exit(1);
    }


    *table_size = find_next_prime(*table_size, resize);
    person **new_hash_table = (person **) calloc(*table_size, sizeof(person *));
    if (new_hash_table == NULL) exit(1);
    *deleted_element = 1;

    for (int i = 0; i < prev_size; i++) {

        if (hash_table[i] != NULL && hash_table[i] != DELETED) {

            unsigned index = hash(hash_table[i]->name, hash_table[i]->surname, hash_table[i]->date, *table_size);
            unsigned step = hash_step(hash_table[i]->name, hash_table[i]->surname, hash_table[i]->date, *table_size);
            if (step == -1){
                continue;
            }

            int j_step = 0;
            for (int j = 0; j < *table_size; j++, j_step+=(int)step) {
                int new_index = ((int)index + j_step) % *table_size;
                if (new_hash_table[new_index] == NULL) {
                    new_hash_table[new_index] = hash_table[i];
                    break;
                }
            }
        }
    }
    free(hash_table);
    hash_table = NULL;
    return new_hash_table;
}

//++++++++++++++++++
int hash_table_insert(person *p, int *table_size, int *cell_used, int *deleted_element, person ***hash_table_p) {
    if (p == NULL) {
        return 0;
    } else {
        if (hash_table_p == NULL && *hash_table_p == NULL) {
            return 0;
        }

        if ((*cell_used + *deleted_element) > *table_size * 0.85) {
            *hash_table_p = resize_table(table_size, *hash_table_p, deleted_element, 1.2);
            if (*hash_table_p == NULL) {
                return 0;
            }
        }
        else if ((*cell_used) > *table_size * 0.75) {
            *hash_table_p = resize_table(table_size, *hash_table_p, deleted_element, 1.5);
            if (*hash_table_p == NULL) {
                return 0;
            }
        }




        int index = hash(p->name, p->surname, p->date, *table_size);
        int step = hash_step(p->name, p->surname, p->date, *table_size);

        if (step == -1){
            return 0;
        }

        int first_delete = -1;
        int i_step = 0;

        for (int i = 0; i < *table_size; i++, i_step += step) {
            int new_index = ((int) index + i_step) % *table_size;

            if ((*hash_table_p)[new_index] == DELETED) {
                if (first_delete == -1) {
                    first_delete = new_index;
                }
            } else if ((*hash_table_p)[new_index] != NULL && (*hash_table_p)[new_index] != DELETED) {
                if (is_name_equal(p->name, (*hash_table_p)[new_index]->name) &&
                    is_name_equal(p->surname, (*hash_table_p)[new_index]->surname) &&
                    is_name_equal(p->date, (*hash_table_p)[new_index]->date)) {
                    return 0;
                }
            } else if ((*hash_table_p)[new_index] == NULL && first_delete == -1) {
                (*hash_table_p)[new_index] = p;
                *cell_used += 1;
                return 1;
            } else {
                (*deleted_element)--;
                (*hash_table_p)[first_delete] = p;
                *cell_used += 1;
                return 1;
            }
        }
    }
    return 0;
}

//+
person *search_by_name(char *name, char *surname, char *date, int table_size, person **hash_table) {
    unsigned index = hash(name, surname, date, table_size);
    unsigned step = hash_step(name, surname, date, table_size);

    if (step == -1){
        return NULL;
    }

    int i_step = 0;

    for (int i = 0; i < table_size; i++) {
        int new_index = ((int) index + i_step) % table_size;

        if (hash_table[new_index] == NULL){
            break;
        }



        if (hash_table[new_index] != NULL && hash_table[new_index] != DELETED
            && is_name_equal(name, hash_table[new_index]->name)
            && is_name_equal(surname, hash_table[new_index]->surname)
            && is_name_equal(date, hash_table[new_index]->date)) {
            return hash_table[new_index];
        }
        i_step += (int) step;
    }
    return NULL;
}


//+
int remove_person(char *name, char *surname, char *date, int table_size, person **hash_table,  int *cell_used, int *deleted_element) {
    unsigned index = hash(name, surname, date, table_size);
    unsigned step = hash_step(name, surname, date, table_size);

    if (step == -1){
        return 0;
    }

    int i_step = 0;

    for (int i = 0; i < table_size; i++) {
        int new_index = ((int) index + i_step) % table_size;

        if (hash_table[new_index] == NULL){
            break;
        }

        if (hash_table[new_index] != NULL && hash_table[new_index] != DELETED
            && is_name_equal(name, hash_table[new_index]->name)
            && is_name_equal(surname, hash_table[new_index]->surname)
            && is_name_equal(date, hash_table[new_index]->date)) {
            free(hash_table[new_index]);
            hash_table[new_index] = DELETED;
            *cell_used-=1;
            *deleted_element+=1;
            return 1;
        }
        i_step+=(int) step;
    }
    return 0;
}


int main() {

    int table_size = 101;
    int cell_used = 1;
    int deleted_element = 1;

    person **hash_table = (person **) calloc(table_size, sizeof(person *));
    if (hash_table == NULL) exit(1);
    //start
    char name[16];
    char surname[16];
    char date[16];
    char string_balance[16];
    int start = 0;


    char type;
    while (scanf("%c", &type) != EOF) {
        switch (type) {
            case 'i':
                scanf(" %s %s %s %s", name, surname, date, string_balance);


                double balance = string_to_double(string_balance);
                person *new_user = create_person(name, surname, date, balance);
                int res_insert = hash_table_insert(new_user, &table_size, &cell_used, &deleted_element, &hash_table);

                if (res_insert == 0) {
                    check_start(&start);
                    printf("insert failed");
                }

                break;
            case 'u':
                scanf(" %s %s %s %s", name, surname, date, string_balance);
                person *update_user = search_by_name(name, surname, date, table_size, hash_table);

                if (update_user == NULL) {
                    check_start(&start);
                    printf("update failed");
                }
                else{

                    double balance_update = string_to_double(string_balance);


                    double res_update = update_balance(update_user->balance, balance_update);


                    if (res_update < 0) {
                        check_start(&start);
                        printf("update failed");
                    } else {
                        //no print
                        update_user->balance = res_update;
                    }
                }

                break;
            case 's':
                scanf(" %s %s %s", name, surname, date);

                person *search_user = search_by_name(name, surname, date, table_size, hash_table);



                if (search_user != NULL) {
                    check_start(&start);
                    double_to_string_print(search_user->balance);
                } else {
                    check_start(&start);
                    printf("search failed");
                }
                break;
            case 'd':
                scanf(" %s %s %s", name, surname, date);
                int res = remove_person(name, surname, date, table_size, hash_table, &cell_used, &deleted_element);
                if (res == 0) {
                    check_start(&start);
                    printf("delete failed");
                }
                break;
        }
    }
    return 0;
}
