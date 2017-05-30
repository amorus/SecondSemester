
typedef struct painting
{
    char code[10];
    char author[50];
    char painting[50];
    float price;
    int year;
    struct painting *next;
} painting_t;

painting_t *head = NULL;
painting_t *current = NULL;

//shows the menu
void menu();

//creates a linked list from the binary file menu.bin
int populate_list();

//adds a painting to the gallery
int add_painting(char code[10], char author[50], char painting[50], float price, int year);

//removes a painting with the given number
int remove_painting(char code[10]);

//changes a painting
int change_painting(char code[10], char author[50], char painting[50], float price, int year);

//qsort standard function
int compare (const void * a, const void * b);

//shows all paintings of given author by descending price
void print_paintings(char author[50]);

//shows all paintings of the gallery
void display_records();

//writes 3 control elements in the binary file(not used)
int write_to_binary();
