#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    int id;
    char name[16];
    char surname[16];
    char date[16];
    int height;
    struct node *right;
    struct node *left;
} NODE;


NODE *create_node(int id, char name[16], char surname[16], char date[16]) {
    NODE *node = (NODE *) malloc(sizeof(NODE));
    node->right = NULL;
    node->left = NULL;
    node->id = id;
    node->height = 1;
    sscanf(name, "%s", node->name);
    sscanf(surname, "%s", node->surname);
    sscanf(date, "%s", node->date);

    return node;
}


int max(int a, int b) {
    if (a > b) return a;
    return b;
}


int found_height(NODE *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}


NODE *left_transformation(NODE *node) {
    if (node != NULL) {
        NODE *new_parent = node->right;
        NODE *parent_child = new_parent->left;

        new_parent->left = node;
        node->right = parent_child;

        node->height = 1 + max(found_height(node->right), found_height(node->left));
        new_parent->height = 1 + max(found_height(new_parent->right), found_height(new_parent->left));

        return new_parent;
    }
    return node;
}


NODE *right_transformation(NODE *node) {
    if (node == NULL){
        return node;
    }
    NODE *new_parent = node->left;
    NODE *parent_child = new_parent->right;

    new_parent->right = node;
    node->left = parent_child;

    node->height = 1 + max(found_height(node->right), found_height(node->left));
    new_parent->height = 1 + max(found_height(new_parent->right), found_height(new_parent->left));

    return new_parent;
}


NODE *insert(NODE *node, int id, char name[16], char surname[16], char date[16]) {
    if (node == NULL) {
        return create_node(id, name, surname, date);
    } else {
        if (node->id > id) {
            node->left = insert(node->left, id, name, surname, date);
        } else if (node->id < id) {
            node->right = insert(node->right, id, name, surname, date);
        } else {
            return node;
        }
    }

    node->height = 1 + max(found_height(node->left), found_height(node->right));

    int node_balance = found_height(node->left) - found_height(node->right);


    if (node_balance > 1 && id < node->left->id) {
        return right_transformation(node);
    } else if (node_balance < -1 && id > node->right->id) {
        return left_transformation(node);
    } else if (node_balance > 1 && id > node->left->id) {
        node->left = left_transformation(node->left);
        return right_transformation(node);
    } else if (node_balance < -1 && id < node->right->id) {
        node->right = right_transformation(node->right);
        return left_transformation(node);
    }

    return node;
}

NODE* remove_node(NODE *node, int id) {
    if (node == NULL) {
        return NULL;
    }
    NODE *cur = node;

    if (cur->id > id) {
        cur->left = remove_node(cur->left, id);
    } else if (cur->id < id) {
        cur->right = remove_node(cur->right, id);
    } else {

        if (cur->right == NULL){
            NODE* temp = cur->left;
            return temp;
        }
        else if(cur->left == NULL){
            NODE* temp = cur->right;
            return temp;
        }

        NODE* most_left = cur->right;
        while (most_left->left != NULL){
            most_left = most_left->left;
        }

        cur->id = most_left->id;
        sscanf(most_left->surname, "%s", cur->surname);
        sscanf(most_left->name, "%s", cur->name);
        sscanf(most_left->date, "%s", cur->date);

        cur->right = remove_node(cur->right, cur->id);
    }
    node = cur;

    node->height = 1 + max(found_height(node->left), found_height(node->right));



    return node;
}


void search(NODE* root, int id, int *start){
    if (root == NULL){
        return;
    }
    else if(root->id > id){
        search(root->left, id, start);
    }
    else if(root->id < id){
        search(root->right, id, start);
    }
    else{
        if ((*start) == 0){
            (*start) = 1;
            printf("%d %s %s %s", root->id, root->name, root->surname, root->date);
        }
        else{
            printf("\n%d %s %s %s", root->id, root->name, root->surname, root->date);
        }
    }
}


void search_interval(NODE* root, int id_left, int id_right, int *start){
    if (root == NULL){
        return;
    }
    else if (root->id >= id_left){
        search_interval(root->left, id_left, id_right, start);
    }

    if ((root->id >= id_left) && (root->id <= id_right)) {
        if ((*start) == 0) {
            (*start) = 1;
            printf("%d %s %s %s", root->id, root->name, root->surname, root->date);
        } else {
            printf("\n%d %s %s %s", root->id, root->name, root->surname, root->date);
        }
    }

    if (root->id <= id_right){
        search_interval(root->right, id_left, id_right, start);
    }
}




void delete(NODE *node) {
    if (node == NULL){
        return;
    }

    delete(node->left);
    delete(node->right);
    free(node);
}


int main() {
    int numb1, numb2=-1;
    int del_numb;
    char buffer[128];

    int id;
    char name[16];
    char surname[16];
    char date[16];
    int start = 0;

    NODE *root = NULL;
    char type;
    int temp;
    while (scanf("%c", &type) != EOF){
        switch (type) {
            case 'i':
                scanf("%d %s %s %s", &id, name, surname, date);
                root = insert(root, id, name, surname, date);
                break;

            case 's':
                fgets(buffer, 128, stdin);

                sscanf(buffer, "%d %d", &numb1, &numb2);
                if (numb2 != -1){
                    if (numb1 > numb2){
                        temp = numb1;
                        numb1 = numb2;
                        numb2 = temp;
                    }
                    search_interval(root, numb1, numb2, &start);
                }
                else{
                    search(root, numb1, &start);
                }
                numb2 = -1;
                break;

            case 'd':
                scanf("%d", &del_numb);
                root = remove_node(root, del_numb);
                break;
        }
    }
    // delete(root);
    return 0;
}

