#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int vertex;
    long values;
} NEIGH;

typedef struct {
    int vertex;
    NEIGH *neigh_arr;
    int size_neigh;
} NODE_GRAPH;


typedef struct {
    NODE_GRAPH *array;
    int size;
    int rows;
} GRAPH;


typedef struct node {
    long value;
    int vertex;
    int source;
} NODE;

typedef struct heap {
    int capacity;
    int size;
    NODE *heap_array;
} HEAP;


GRAPH *initialize_graph(int size) {
    GRAPH *graph = (GRAPH *) malloc(sizeof(GRAPH));
    graph->size = size;
    graph->rows = 0;
    graph->array = (NODE_GRAPH *) malloc(size * sizeof(NODE_GRAPH));

    for (int i = 0; i < size; i++) {
        graph->array[i].vertex = i;
        graph->array[i].size_neigh = 0;
        graph->array[i].neigh_arr = NULL;
    }
    return graph;
}


int check_correct_vertex(int v1, int v2, GRAPH *graph) {
    if (v1 == v2 || v1 >= graph->size || v2 >= graph->size || v1 < 0 || v2 < 0) {
        return 0;
    }
    return 1;
}

int check_exist(int v1, int v2, GRAPH *graph) {
    for (int i = 0; i < graph->array[v1].size_neigh; i++) {
        if (graph->array[v1].neigh_arr[i].vertex == v2) {
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


void shift_up(int index, HEAP *heap) {
    while (heap->heap_array[index].value < heap->heap_array[(index - 1) / 2].value) {
        swap(&heap->heap_array[index], &heap->heap_array[(index - 1) / 2]);

        index = (index - 1) / 2;
    }
}

void shift_down(int index, HEAP *heap) {
    while (index * 2 + 1 < heap->size) {
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int less_value_index = left;
        if (right < heap->size && heap->heap_array[right].value < heap->heap_array[left].value) {
            less_value_index = right;
        }
        if (heap->heap_array[index].value <= heap->heap_array[less_value_index].value) {
            break;
        }
        swap(&heap->heap_array[less_value_index], &heap->heap_array[index]);
        index = less_value_index;
    }
}


NODE extract_min(HEAP *heap) {
    if (heap->size < 1) {
        NODE node = {.value = -1, .vertex = -1, .source = -1};
        return node;
    }

    if (heap->size == 1) {
        heap->size--;
        return heap->heap_array[0];
    }

    NODE min_node = heap->heap_array[0];
    heap->heap_array[0] = heap->heap_array[heap->size - 1];
    (heap->size)--;

    shift_down(0, heap);

    return min_node;
}

void insert_in_heap(HEAP *heap, int node, long dist) {
    if (heap->size == heap->capacity) {
        return;
    }
    heap->size++;
    int i = heap->size - 1;
    heap->heap_array[i].vertex = node;
    heap->heap_array[i].value = dist;
    shift_up(i, heap);
}



//-------------------------------------------------------------




int add_neigh(GRAPH *graph, int v1, int v2, long value) {
    if (check_correct_vertex(v1, v2, graph) == 0) return 0;
    if (check_exist(v1, v2, graph) != -1) return 0;
    graph->array[v1].size_neigh++;
    graph->array[v1].neigh_arr = realloc(graph->array[v1].neigh_arr, sizeof(NEIGH) * graph->array[v1].size_neigh);
    graph->array[v1].neigh_arr[graph->array[v1].size_neigh - 1].vertex = v2;
    graph->array[v1].neigh_arr[graph->array[v1].size_neigh - 1].values = value;

    graph->array[v2].size_neigh++;
    graph->array[v2].neigh_arr = realloc(graph->array[v2].neigh_arr, sizeof(NEIGH) * graph->array[v2].size_neigh);
    graph->array[v2].neigh_arr[graph->array[v2].size_neigh - 1].vertex = v1;
    graph->array[v2].neigh_arr[graph->array[v2].size_neigh - 1].values = value;
    return 1;
}

int update(int v1, int v2, long new_val, GRAPH *graph) {
    if (check_correct_vertex(v1, v2, graph) == 0) return 0;
    int was_found = 0;

    for (int i = 0; i < graph->array[v1].size_neigh; i++) {
        if (graph->array[v1].neigh_arr[i].vertex == v2) {
            long res = new_val + graph->array[v1].neigh_arr[i].values;
            if (res >= 0) {
                graph->array[v1].neigh_arr[i].values = res;
            } else {
                return 0;
            }
            was_found = 1;
            break;
        }
    }

    if (was_found == 0) {
        return 0;
    }

    for (int i = 0; i < graph->array[v2].size_neigh; i++) {
        if (graph->array[v2].neigh_arr[i].vertex == v1) {
            long res = new_val + graph->array[v2].neigh_arr[i].values;
            if (res >= 0) {
                graph->array[v2].neigh_arr[i].values = res;
            } else {
                return 0;
            }
            break;
        }
    }
    return 1;
}


int delete(int v1, int v2, GRAPH *graph) {
    if (check_correct_vertex(v1, v2, graph) == 0) return 0;
    int was_found = 0;
    for (int i = 0; i < graph->array[v1].size_neigh; i++) {
        if (was_found == 1) {
            graph->array[v1].neigh_arr[i - 1] = graph->array[v1].neigh_arr[i];
        } else if (graph->array[v1].neigh_arr[i].vertex == v2) {
            was_found = 1;
        }
    }

    if (was_found == 0) {
        return 0;
    }
    (graph->array[v1].size_neigh)--;

    was_found = 0;
    for (int i = 0; i < graph->array[v2].size_neigh; i++) {
        if (was_found == 1) {
            graph->array[v2].neigh_arr[i - 1] = graph->array[v2].neigh_arr[i];
        } else if (graph->array[v2].neigh_arr[i].vertex == v1) {
            was_found = 1;
        }
    }

    (graph->array[v2].size_neigh)--;
    return 1;
}


void merge(int **path, int start, int center, int end) {
    int start_index, end_index, path_index;
    int lower = center - start + 1;
    int higher = end - center;


    int left[lower][2], right[higher][2];

    for (start_index = 0; start_index < lower; start_index++) {
        left[start_index][0] = path[start + start_index][0];
        left[start_index][1] = path[start + start_index][1];
    }
    for (end_index = 0; end_index < higher; end_index++) {
        right[end_index][0] = path[center + 1 + end_index][0];
        right[end_index][1] = path[center + 1 + end_index][1];
    }

    start_index = 0;
    end_index = 0;
    path_index = start;
    while (start_index < lower && end_index < higher) {
        if (left[start_index][0] < right[end_index][0] || (left[start_index][0] == right[end_index][0] && left[start_index][1] < right[end_index][1])) {
            path[path_index][0] = left[start_index][0];
            path[path_index][1] = left[start_index][1];
            start_index++;
        } else {
            path[path_index][0] = right[end_index][0];
            path[path_index][1] = right[end_index][1];
            end_index++;
        }
        path_index++;
    }


    while (start_index < lower) {
        path[path_index][0] = left[start_index][0];
        path[path_index][1] = left[start_index][1];
        start_index++;
        path_index++;
    }

    while (end_index < higher) {
        path[path_index][0] = right[end_index][0];
        path[path_index][1] = right[end_index][1];
        end_index++;
        path_index++;
    }
}


void sorting(int **arr, int start, int end) {
    if (end > start) {
        int center = start + (end - start) / 2;

        sorting(arr, start, center);
        sorting(arr, center + 1, end);

        merge(arr, start, center, end);
    }
}



void print_path_to_vertex(int find, const long *values, const int *source, const int * node_in_path, int *print_start, int size_rows) {
    int size = 0;
    long final_value = 0;
    int **path = (int **) malloc(sizeof(int *) * (size_rows));


    for (int i = 0; i < size_rows; i++) {
        if (node_in_path[i] == -1) break;
        int vertex = node_in_path[i];
        if ((source[vertex] == -1) || (source[vertex] == vertex)) continue;
        final_value += values[vertex];
        size++;
        path[size - 1] = (int *) malloc(sizeof(int) * 2);
        if (path[size - 1] == NULL) exit(1);


        if (vertex > source[vertex]) {
            path[size - 1][1] = vertex;
            path[size - 1][0] = source[vertex];
        } else {
            path[size - 1][0] = vertex;
            path[size - 1][1] = source[vertex];
        }

    }

    if (size == 0){
        if (*print_start == 0) {
            (*print_start)++;
            printf("search %d failed", find);
        }
        else{
            printf("\nsearch %d failed", find);
        }
    }
    else {

        if (*print_start == 0) {
            (*print_start)++;
            printf("%ld: [", final_value);
        } else {
            printf("\n%ld: [", final_value);
        }

        sorting(path, 0, size-1);
        for (int i = 0; i < size; i++) {
            if (i == size - 1) {
                printf("(%d, %d)]", path[i][0], path[i][1]);
            } else {
                printf("(%d, %d), ", path[i][0], path[i][1]);
            }
            free(path[i]);
            path[i] = NULL;
        }
        free(path);
        path = NULL;
    }
}


void search(int start, GRAPH *graph, int *print_start) {
    int *done = calloc(graph->size, sizeof(int));

    long *values = (long *) malloc(sizeof(long) * graph->size);
    int *source = (int *) malloc(sizeof(int) * graph->size);
    int *node_in_path = (int *) malloc(sizeof(int) * graph->size);
    int size_path = 0;

    for (int i = 0; i < graph->size; i++) {
        //(value - source)
        values[i] = 2147483646;
        source[i] = -1;
        node_in_path[i] = -1;
    }

    HEAP *new_heap = (HEAP *) malloc(sizeof(HEAP));
    if (new_heap == NULL) exit(1);
    new_heap->size = 0;
    new_heap->capacity = graph->size * 2;
    new_heap->heap_array = (NODE *) malloc(sizeof(NODE) * new_heap->capacity);
    if (new_heap->heap_array == NULL) exit(1);


    values[start] = 0;
    source[start] = start;
    node_in_path[size_path] = start;
    size_path++;
    insert_in_heap(new_heap, start, values[start]);


    while (new_heap->size != 0) {
        NODE cur_node = extract_min(new_heap);
        if (cur_node.value == -1) {
            if (*print_start == 0) {
                (*print_start)++;
                printf("search %d failed", start);
            } else {
                printf("\nsearch %d failed", start);
            }
            break;
        }

        int cur_vertex = cur_node.vertex;
        if (done[cur_vertex] == 1) continue;


        for (int i = 0; i < graph->array[cur_vertex].size_neigh; i++) {
            int new_ver = graph->array[cur_vertex].neigh_arr[i].vertex;
            if (done[new_ver] == 1) continue;
            long new_val = graph->array[cur_vertex].neigh_arr[i].values;


            if (values[new_ver] > new_val) {
                values[new_ver] = new_val;
                if (source[new_ver] == -1){
                    node_in_path[size_path] = new_ver;
                    size_path++;
                }
                source[new_ver] = cur_vertex;

                insert_in_heap(new_heap, new_ver, values[new_ver]);
            }
        }
        done[cur_vertex] = 1;
    }

//    for (int i = 0; i < size_path; i++){
//        printf("[%d]", node_in_path[i]);
//    }
//    printf("\n");


    print_path_to_vertex(start, values, source, node_in_path, print_start, graph->size);

    free(values);
    free(source);
    source = NULL;
    values = NULL;
    free(done);
    done = NULL;

    free(new_heap->heap_array);
    free(new_heap);
    free(node_in_path);
}


void show_graph(GRAPH *graph) {
    for (int i = 0; i < graph->size; i++) {
        printf("%d: [", graph->array[i].vertex);
        for (int j = 0; j < graph->array[i].size_neigh; j++) {
            printf(" [%d, %ld]", graph->array[i].neigh_arr[j].vertex, graph->array[i].neigh_arr[j].values);
        }
        printf(" ]\n");
    }
}


int main() {

    int g_size;
    int v1, v2;
    long value;
    int res;
    int print_start = 0;
    scanf("%d ", &g_size);
    GRAPH *graph = initialize_graph(g_size);
    char type;


    while (1) {
        if (scanf("(%d , %d , %ld) ", &v1, &v2, &value) == 3) {
            add_neigh(graph, v1, v2, value);
            graph->rows++;
        } else {
            break;
        }

    }


    while ((scanf("%c", &type) != EOF)) {
        switch (type) {
            case 's':
                scanf("%d", &v1);
                if (v1 >= graph->size){
                    if (print_start == 0) {
                        print_start++;
                        printf("insert %d failed", v1);
                    } else printf("\ninsert %d failed", v1);
                }
                else{
                    search(v1, graph, &print_start);
                }
                break;
            case 'u':
                scanf("%d %d %ld", &v1, &v2, &value);
                res = update(v1, v2, value, graph);


                if (res == 0) {
                    if (print_start == 0) {
                        print_start++;
                        printf("update %d %d failed", v1, v2);
                    } else printf("\nupdate %d %d failed", v1, v2);
                }
                break;
            case 'i':
                scanf("%d %d %ld", &v1, &v2, &value);
                res = add_neigh(graph, v1, v2, value);
                if (res == 0) {
                    if (print_start == 0) {
                        print_start++;
                        printf("insert %d %d failed", v1, v2);
                    } else printf("\ninsert %d %d failed", v1, v2);
                }
                break;
            case 'd':
                scanf("%d %d", &v1, &v2);
                res = delete(v1, v2, graph);
                if (res == 0) {
                    if (print_start == 0) {
                        print_start++;
                        printf("delete %d %d failed", v1, v2);
                    } else printf("\ndelete %d %d failed", v1, v2);
                }
                break;

            case 'p':
                show_graph(graph);
                break;
        }
    }


    return 0;
}