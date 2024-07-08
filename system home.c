#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DEVICES 10

typedef struct Device {
    int id;
    char name[20];
    bool activated;
} Device;

typedef struct Stack {
    int top;
    int capacity;
    Device *devices[MAX_DEVICES];
} Stack;

typedef struct Graph {
    int numDevices;
    int connections[MAX_DEVICES][MAX_DEVICES];
} Graph;

Device *createDevice(char name[], int id);
Stack *createStack();
Graph *createGraph();
void deactivateDevice(Stack *stack);
void activateDevice(Device *device, Stack *stack);
void addConnection(Graph *graph, int device1, int device2);
Device *pop(Stack *stack);
void push(Stack *stack, Device *device);
int isEmpty(Stack *stack);

int main()
{
    int n, num1, num2;
    char name[20];
    Device *devices[MAX_DEVICES];
    Graph *graph = createGraph();
    Stack *stack = createStack();

    for (int i = 0; i < MAX_DEVICES; i++) {
        devices[i] = NULL;
    }

    do {
        printf("\n----------BASIC MENU----------\n");
        printf("1) Create Device\n2) Add connection\n3) Activate\n4) Deactivate\n5) Exit\n");
        printf("------------------------------\n");
        printf("Enter Choice: ");
        if (scanf("%d", &n) != 1) {
            while(getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        switch(n) {
            case 1:
                printf("\nEnter the name of the device: ");
                scanf("%19s", name);
                for (int i = 0; i < MAX_DEVICES; i++) {
                    if (devices[i] == NULL) {
                        devices[i] = createDevice(name, i + 1);
                        if (devices[i] != NULL) {
                            printf("Device created with name: %s and ID: %d\n", devices[i]->name, devices[i]->id);
                        } else {
                            printf("Failed to create device. Maximum number of devices reached!\n");
                        }
                        break;
                    }
                }
                break;
            case 2:
                printf("\nEnter the IDs between 1 to 10 to create a connection: ");
                scanf("%d %d", &num1, &num2);
                if (num1 > 0 && num1 <= MAX_DEVICES && num2 > 0 && num2 <= MAX_DEVICES) {
                    addConnection(graph, num1 - 1, num2 - 1);
                    printf("Connection added between devices %d and %d\n", num1, num2);
                } else {
                    printf("Invalid device IDs!\n");
                }
                break;
            case 3:
                printf("\nEnter the ID to activate the device: ");
                scanf("%d", &num1);
                if (num1 > 0 && num1 <= MAX_DEVICES) {
                    activateDevice(devices[num1 - 1], stack);
                } else {
                    printf("Invalid device ID!\n");
                }
                break;
            case 4:
                deactivateDevice(stack);
                break;
            case 5:
                while (!isEmpty(stack)) {
                    deactivateDevice(stack);
                }
                free(stack);
                free(graph);
                for (int i = 0; i < MAX_DEVICES; i++) {
                    free(devices[i]);
                }
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    } while(n != 5);

    return 0;
}

Device *createDevice(char name[], int id)
{
    Device *device = (Device *)malloc(sizeof(Device));
    if (device == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    device->id = id;
    strcpy(device->name, name);
    device->activated = false;
    return device;
}

Stack *createStack()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    stack->top = -1;
    stack->capacity = MAX_DEVICES;
    return stack;
}

int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

void push(Stack *stack, Device *device)
{
    if (stack->top == stack->capacity - 1)
    {
        printf("\nStack overflow!\n");
        return;
    }
    stack->devices[++stack->top] = device;
}

Device *pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        printf("\nStack underflow!\n");
        return NULL;
    }
    return stack->devices[stack->top--];
}

Graph *createGraph()
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    graph->numDevices = 0;
    for (int i = 0; i < MAX_DEVICES; i++)
    {
        for (int j = 0; j < MAX_DEVICES; j++)
        {
            graph->connections[i][j] = 0;
        }
    }
    return graph;
}

void addConnection(Graph *graph, int device1, int device2)
{
    graph->connections[device1][device2] = 1;
    graph->connections[device2][device1] = 1;
}

void activateDevice(Device *device, Stack *stack)
{
    device->activated = true;
    push(stack, device);
    printf("\nDevice %s activated.\n", device->name);
}

void deactivateDevice(Stack *stack)
{
    Device *device = pop(stack);
    if (device != NULL)
    {
        device->activated = false;
        printf("\nDevice %s deactivated.\n", device->name);
    }
}
