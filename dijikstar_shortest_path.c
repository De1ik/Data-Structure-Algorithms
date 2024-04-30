#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int vertex;
    int values;
} NEIGH;

typedef struct {
    int vertex;
    NEIGH* neigh_arr;
    int size_neigh;
} NODE_GRAPH;


typedef struct {
    NODE_GRAPH *array;
    int size;
} GRAPH;


typedef struct node{
    int value;
    int vertex;
    int source;
} NODE;

typedef struct heap{
    int capacity;
    int size;
    NODE *heap_array;
} HEAP;


GRAPH *initialize_graph(int size){
    GRAPH *graph = (GRAPH*)malloc(sizeof(GRAPH));
    graph->size = size;
    graph->array = (NODE_GRAPH*)malloc(size * sizeof(NODE_GRAPH));

    for (int i = 0; i < size; i++){
        graph->array[i].vertex = i;
        graph->array[i].size_neigh = 0;
        graph->array[i].neigh_arr = NULL;
    }
    return graph;
}

int check_correct_vertex(int v1, int v2, GRAPH *graph){
    if (v1 == v2 || v1 >= graph->size || v2 >= graph->size || v1 < 0 || v2 < 0){
        return 0;
    }
    return 1;
}

int check_exist(int v1, int v2, GRAPH *graph){
    for (int i = 0; i < graph->array[v1].size_neigh; i++){
        if (graph->array[v1].neigh_arr[i].vertex == v2){
            return i;
        }
    }
    return -1;
}



//--------------------------------heap------------------------------




void swap(NODE *a, NODE *b) {
    NODE t = *a;
    *a = *b;
    *b = t;
}



void shift_up(int index, HEAP *heap){
    while (heap->heap_array[index].value < heap->heap_array[(index-1)/2].value){
        swap(&heap->heap_array[index], &heap->heap_array[(index-1)/2]);

        index = (index-1)/2;
    }
}

void shift_down(int index, HEAP *heap){
    while (index*2+1 < heap->size){
        int left = index*2+1;
        int right = index*2+2;
        int less_value_index = left;
        if (right < heap->size && heap->heap_array[right].value < heap->heap_array[left].value){
            less_value_index = right;
        }
        if (heap->heap_array[index].value <= heap->heap_array[less_value_index].value){
            break;
        }
        swap(&heap->heap_array[less_value_index], &heap->heap_array[index]);
        index = less_value_index;
    }
}


NODE extract_min(HEAP *heap){
    if (heap->size < 1){
        NODE node = {.value = -1, .vertex = -1, .source = -1};
        return node;
    }

    if (heap->size == 1) {
        heap->size--;
        return heap->heap_array[0];
    }

    NODE min_node = heap->heap_array[0];
    heap->heap_array[0] = heap->heap_array[heap->size-1];
    (heap->size)--;

    shift_down(0, heap);

    return min_node;
}

void insert_in_heap(HEAP *heap, int node, int dist) {
    if (heap->size == heap->capacity) {
        return;
    }
    heap->size++;
    int i = heap->size - 1;
    heap->heap_array[i].vertex = node;
    heap->heap_array[i].value = dist;
    shift_up( i, heap);
}



//-------------------------------------------------------------


int add_neigh(GRAPH *graph, int v1, int v2, int value){
    if (check_correct_vertex(v1, v2, graph) == 0) return 0;
    if (check_exist(v1, v2, graph) != -1) return 0;
    graph->array[v1].size_neigh++;
    graph->array[v1].neigh_arr = realloc(graph->array[v1].neigh_arr, sizeof(NEIGH)*graph->array[v1].size_neigh);
    graph->array[v1].neigh_arr[graph->array[v1].size_neigh-1].vertex = v2;
    graph->array[v1].neigh_arr[graph->array[v1].size_neigh-1].values = value;

    graph->array[v2].size_neigh++;
    graph->array[v2].neigh_arr = realloc(graph->array[v2].neigh_arr, sizeof(NEIGH)*graph->array[v2].size_neigh);
    graph->array[v2].neigh_arr[graph->array[v2].size_neigh-1].vertex = v1;
    graph->array[v2].neigh_arr[graph->array[v2].size_neigh-1].values = value;
    return 1;
}


int update(int v1, int v2, int new_val, GRAPH *graph){
    if (check_correct_vertex(v1, v2, graph) == 0) return 0;
    int was_found = 0;

    for (int i = 0; i < graph->array[v1].size_neigh; i++){
        if (graph->array[v1].neigh_arr[i].vertex == v2){
            int res = new_val + graph->array[v1].neigh_arr[i].values;
            if (res >= 0){
                graph->array[v1].neigh_arr[i].values = res;
            }
            else{
                return 0;
            }
            was_found = 1;
            break;
        }
    }

    if (was_found == 0){
        return 0;
    }

    for (int i = 0; i <  graph->array[v2].size_neigh; i++){
        if (graph->array[v2].neigh_arr[i].vertex == v1){
            int res = new_val + graph->array[v2].neigh_arr[i].values;
            if (res >= 0){
                graph->array[v2].neigh_arr[i].values = res;
            }
            else{
                return 0;
            }
            break;
        }
    }
    return 1;
}


int delete(int v1, int v2, GRAPH *graph){
    if (check_correct_vertex(v1, v2, graph) == 0) return 0;
    int was_found = 0;
    for (int i = 0; i < graph->array[v1].size_neigh; i++){
        if (was_found == 1){
            graph->array[v1].neigh_arr[i-1] = graph->array[v1].neigh_arr[i];
        }
        else if (graph->array[v1].neigh_arr[i].vertex == v2){
            was_found = 1;
        }
    }

    if (was_found == 0){
        return 0;
    }
    (graph->array[v1].size_neigh)--;

    was_found = 0;
    for (int i = 0; i < graph->array[v2].size_neigh; i++){
        if (was_found == 1){
            graph->array[v2].neigh_arr[i-1] = graph->array[v2].neigh_arr[i];
        }
        else if (graph->array[v2].neigh_arr[i].vertex == v1){
            was_found = 1;
        }
    }

    (graph->array[v2].size_neigh)--;
    return 1;
}


void print_path_to_vertex(int *values, int *source, int end, int start, int* print_start){
    int cur = end;
    int size = 1;
    int *path = (int*)malloc(sizeof(int)*size);
    // if (path == NULL) exit(1);
    while (cur != start){
        if (size == 1){
            path[0] = end;
            size++;
        }
        else {
            path = (int *) realloc(path, sizeof(int) * size);
            cur = source[cur];
            path[size - 1] = cur;
            size++;
        }
    }

    if (*print_start == 0) {
        (*print_start)++;
        printf("%d: [", values[end]);
    }
    else{
        printf("\n%d: [", values[end]);
    }

    for (int i = size - 2; i > -1; i--){
        if (i == 0){
            printf("%d]", path[i]);
        }
        else{
            printf("%d, ", path[i]);
        }

    }
    free(path);
}


void search(int start, int end, GRAPH *graph, int *print_start){
    int *done = calloc(graph->size, sizeof(int));

    int *values = (int*)malloc(sizeof(int)*graph->size);
    int *source = (int*)malloc(sizeof(int)*graph->size);

    for (int i = 0; i < graph->size; i++){
        //(value - source)
        values[i] = 2147483646;
        source[i] = -1;
    }

    HEAP *new_heap = (HEAP*)malloc(sizeof(HEAP));
    if (new_heap == NULL) exit(1);
    new_heap->size = 0;
    new_heap->capacity = graph->size * 2;
    new_heap->heap_array = (NODE*)malloc(sizeof(NODE)*new_heap->capacity);
    if (new_heap->heap_array == NULL) exit(1);


    values[start] = 0;
    source[start] = start;
    insert_in_heap(new_heap, start, values[start]);

    while(new_heap->size != 0){
        NODE cur_node = extract_min(new_heap);
        if (cur_node.value == -1){
            if (*print_start == 0){
                (*print_start)++;
                printf("search %d %d failed", start, end);
            }
            else{
                printf("\nsearch %d %d failed", start, end);
            }
            break;
        }

        int cur_vertex = cur_node.vertex;

        if (cur_vertex == end){
            print_path_to_vertex(values, source, end, start, print_start);
            break;
        }

        if (done[cur_vertex] == 1) continue;

        for (int i = 0; i < graph->array[cur_vertex].size_neigh; i++){
            int new_ver = graph->array[cur_vertex].neigh_arr[i].vertex;
            int new_val = graph->array[cur_vertex].neigh_arr[i].values;

            if (values[new_ver] > values[cur_vertex] + new_val){
                values[new_ver] = values[cur_vertex] + new_val;
                source[new_ver] = cur_vertex;

                insert_in_heap(new_heap, new_ver ,values[new_ver] );
            }
        }
        done[cur_vertex] = 1;
    }


    free(values);
    free(source);
    source = NULL;
    values = NULL;
    free(done);
    done = NULL;

    free(new_heap->heap_array);
    free(new_heap);
}





int main(){


    int g_size, rows;
    int v1, v2, value;
    int res;
    int print_start = 0;
    scanf("%d %d", &g_size, &rows);
    GRAPH *graph = initialize_graph(g_size);



    for (int i = 0; i < rows; i++){

        if (scanf(" (%d , %d , %d)", &v1, &v2, &value) == 3) {
        }
        else{
            exit(-1);
        }

        add_neigh(graph, v1, v2, value);
    }


    char type;
    while ((scanf("%c ", &type) != EOF))    {
        switch (type) {
            case 's':
                scanf("%d %d", &v1, &v2);
                if (check_correct_vertex(v1, v2, graph) == 0){
                    if (print_start == 0) {
                        print_start++;
                        printf("search %d %d failed", v1, v2);
                    }
                    else printf("\nsearch %d %d failed", v1, v2);
                }
                else {
                    search(v1, v2, graph, &print_start);
                }
                break;
            case 'u':
                scanf("%d %d %d", &v1, &v2, &value);
                res = update(v1, v2, value, graph);


                if (res == 0) {
                    if (print_start == 0) {
                        print_start++;
                        printf("update %d %d failed", v1, v2);
                    }
                    else printf("\nupdate %d %d failed", v1, v2);
                }
                break;
            case 'i':
                scanf("%d %d %d", &v1, &v2, &value);
                res = add_neigh(graph, v1, v2, value);
                if (res == 0){
                    if (print_start == 0) {
                        print_start++;
                        printf("insert %d %d failed", v1, v2);
                    }
                    else printf("\ninsert %d %d failed", v1, v2);
                }
                break;
            case 'd':
                scanf("%d %d", &v1, &v2);
                res = delete(v1, v2, graph);
                if (res == 0) {
                    if (print_start == 0) {
                        print_start++;
                        printf("delete %d %d failed", v1, v2);
                    }
                    else printf("\ndelete %d %d failed", v1, v2);
                }
                break;
        }
    }


    return 0;
}
