#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main()
{
 //   write_to_binary();
    populate_list();
    menu();

    return 0;
}

void menu()
{
    char code[10];
    char author[50], painting[50];
    float price;
    int year;

    int choice;
    while(choice != 6)
    {
        printf("What do you want to do?\n1 - Display gallery\n2 - Add a painting to gallery\n3 - Remove a painting from gallery\n4 - Change a painting\n5 - Display all paintings of an author\n6 - Exit\n");
        scanf("%d%*c", &choice);
        switch(choice)
        {
        case 1:
            display_records();
            break;
        case 2:
            printf("Enter code: ");
            fgets(code, 10, stdin);
            strtok(code, "\n");
            printf("Enter author: ");
            fgets(author, 50, stdin);
            strtok(author, "\n");
            printf("Enter painting: ");
            fgets(painting, 50, stdin);
            strtok(painting, "\n");
            printf("Enter price: ");
            scanf("%f", &price);
            printf("Enter year: ");
            scanf("%d", &year);
            add_painting(code, author, painting, price, year);
            break;
        case 3:
            printf("Enter code: ");
            fgets(code, 10, stdin);
            strtok(code, "\n");
            remove_painting(code);
            break;
        case 4:
            printf("Enter code of painting to change: ");
            fgets(code, 10, stdin);
            strtok(code, "\n");
            printf("Enter author to change to: ");
            fgets(author, 50, stdin);
            strtok(author, "\n");
            printf("Enter painting to change to: ");
            fgets(painting, 10, stdin);
            strtok(painting, "\n");
            printf("Enter price to change to: ");
            scanf("%f", &price);
            printf("Enter year to change to: ");
            scanf("%d", &year);
            change_painting(code, author, painting, price, year);
            break;
        case 5:
            printf("Enter author to display paintings of: ");
            fgets(author, 50, stdin);
            strtok(author, "\n");
            print_paintings(author);
            break;
        case 6:
            exit(0);
            break;
        default:
            printf("Invalid number. Try again.");
        }
    }
}

int change_painting(char code[10], char author[50], char painting[50], float price, int year)
{
    painting_t painting_change;
    FILE *fp;
    FILE *temp;
    int found = 0;

    fp = fopen("menu.bin", "rb");
    if(!fp)
    {
		perror("Unable to open file");
        printf("Add a painting to create file\n\n");
		return -1;
	}
    temp = fopen("temp.bin", "wb");
    if(!fp)
    {
        perror("Unable to open file");
        return -1;
    }

    while(fread(&painting_change, sizeof(painting_t), 1, fp) != NULL)
    {
        if(strcmp(painting_change.code, code) == 0)
        {
            strcpy(painting_change.code, code);
            strcpy(painting_change.author, author);
            strcpy(painting_change.author, author);
            painting_change.price = price;
            painting_change.year = year;
            found = 1;
        }
        fwrite(&painting_change, sizeof(painting_t), 1, temp);
    }
    fclose(fp);
    fclose(temp);

    remove("menu.bin");
    rename("temp.bin", "menu.bin");
    if(found == 1)
    {
        current = head;
        while(current != NULL)
        {
            if(strcmp(current->code, code) == 0)
            {
                strcpy(current->code, code);
                strcpy(current->author, author);
                strcpy(current->painting, painting);
                current->price = price;
                current->year = year;
                break;
            }
            current = current->next;
        }
        printf("Painting %s has been changed!\n\n", code);
    }
    else
        printf("Painting %s not found.\n\n", code);
}

int compare (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa < fb) - (fa > fb);
}

void print_paintings(char author[50])
{
    int i;
    int count = 0;
    float *prices;

    prices = malloc(1);

    current = head;
    while(current != NULL)
    {
        if(strcmp(current->author, author) == 0)
        {
            prices = (float*) realloc(prices, ++count);
            prices[count-1] = current->price;
        }
        current = current->next;
    }

    qsort (prices, count, sizeof(float), compare);
    printf("--------------------------------- \n");
    if(count > 0)
    {
        printf("All paintings of %s are: \n\n", author);

        for(i = 0; i < count; )
        {
            if(current == NULL)
            {
                current = head;
            }

            if(strcmp(current->author, author) == 0 && prices[i] == current->price)
            {
                printf("Code: %s\n", current->code);
                printf("Author: %s\n", current->author);
                printf("Painting name: %s\n", current->painting);
                printf("Price: %.2f\n", current->price);
                printf("Year: %d\n\n", current->year);
                i++;
            }
            current = current->next;
        }
    }
    else
        printf("%s has no paintings in the gallery! \n", author);

    printf("--------------------------------- \n\n");
}

int add_painting(char code[10], char author[50], char painting[50], float price, int year)
{
    painting_t *new_painting;
    new_painting = (painting_t*) malloc(sizeof(painting_t));
    FILE *fp;
    fp = fopen("menu.bin", "ab");

    if (!fp)
    {
		perror("Unable to open file");
		return -1;
	}

    strcpy(new_painting->code, code);
    strcpy(new_painting->author, author);
    strcpy(new_painting->painting, painting);
    new_painting->price = price;
    new_painting->year = year;
    fwrite(new_painting, sizeof(painting_t), 1, fp);
    if(head != NULL)
    {
        current = head;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_painting;
        current->next->next = NULL;
    }
    else
    {
        head = new_painting;
        head->next = NULL;
    }
    fclose(fp);
    printf("Painting %s has been added!\n\n", code);
}
int remove_painting(char code[10])
{
    painting_t painting_delete;
    painting_t *previous;
    int found = 0;
    FILE *fp;
    FILE *temp;

    fp = fopen("menu.bin", "rb");
    if(!fp)
    {
		perror("Unable to open file");
        printf("Add a painting to create file\n\n");
		return -1;
	}
    temp = fopen("temp.bin", "wb");
    if(!fp)
    {
        perror("Unable to open file");
        return -1;
    }


    while(fread(&painting_delete, sizeof(painting_t), 1, fp) != NULL)
    {
        if(strcmp(painting_delete.code, code) != 0)
        {
            fwrite(&painting_delete, sizeof(painting_t), 1, temp);
        }
        else found = 1;
    }
    fclose(fp);
    fclose(temp);

    remove("menu.bin");
    rename("temp.bin", "menu.bin");

    current = head;
    if(found == 1)
    {
        while(current != NULL)
        {
            if(strcmp(current->code, code) == 0)
            {
                if(current == head)
                {
                    head = current->next;
                    free(current);
                }
                else
                {
                    previous->next = current->next;
                    free(current);
                }
            }
            previous = current;
            current = current->next;
        }
        printf("Painting %s has been removed!\n\n", code);
    }
    else
        printf("Painting %s not found!\n\n", code);
}


int write_to_binary()
{
    painting_t newrecord;
    FILE *fp;
    fp = fopen("menu.bin", "wb");

    if (!fp)
    {
		perror("Unable to open file");
		return -1;
	}

    strcpy(newrecord.code, "A1");
    strcpy(newrecord.author, "Georgi");
    strcpy(newrecord.painting, "Gora");
    newrecord.price = 100.0;
    newrecord.year = 1997;
    fwrite(&newrecord, sizeof(painting_t), 1, fp);

    strcpy(newrecord.code, "A2");
    strcpy(newrecord.author, "Petar");
    strcpy(newrecord.painting, "More");
    newrecord.price = 250.7;
    newrecord.year = 1998;
    fwrite(&newrecord, sizeof(painting_t), 1, fp);

    strcpy(newrecord.code, "A3");
    strcpy(newrecord.author, "Petar");
    strcpy(newrecord.painting, "Pustinq");
    newrecord.price = 260.2;
    newrecord.year = 1999;
    fwrite(&newrecord, sizeof(painting_t), 1, fp);
    fclose(fp);
}

void display_records()
{
    printf("--------------------------------- \n");
    if(head == NULL)
        printf("There are no paintings in the gallery\n");
    else
        printf("All paintings in the gallery are: \n\n");

    current=head;
    while(current != NULL)
    {
        printf("Code: %s\n", current->code);
		printf("Author: %s\n", current->author);
		printf("Painting name: %s\n", current->painting);
		printf("Price: %.2f\n", current->price);
		printf("Year: %d\n\n", current->year);
		current = current->next;
    }
    printf("--------------------------------- \n\n");
}

int populate_list()
{
	FILE *fp;
	painting_t myrecord;

	fp=fopen("menu.bin","rb");
	if (!fp)
    {
		perror("Unable to open file");
		printf("Add a painting to create file\n\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	if(ftell(fp) == 0)
    {
        printf("WARNING! The file is empty!\n\n");
        fclose(fp);
        return 0;
    }
    rewind(fp);

	head = (painting_t*) malloc(sizeof(painting_t));

	fread(head, sizeof(painting_t), 1, fp);
    current = head;
    current->next = NULL;
	while(fread(&myrecord,sizeof(painting_t),1,fp) != NULL) {
		current->next = (painting_t*) malloc(sizeof(painting_t));
		current = current->next;

		strcpy(current->code, myrecord.code);
		strcpy(current->author, myrecord.author);
        strcpy(current->painting, myrecord.painting);
		current->price = myrecord.price;
		current->year = myrecord.year;
        current->next = NULL;
	}
	fclose(fp);
	return 0;
}


