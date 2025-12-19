#include <stdio.h>
#include "total.h"
#include <string.h>


// void showBookDetails(int id);
void userPage() {
    int choice;
    int bookId;
    char res[10];

    do {
        printf("\n===== USER PAGE =====\n");
        printf("1. Book List\n");
        printf("2. Order List\n");
        printf("3. View Cart\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showBookList();
                
                printf("Do you want to view Details? Enter Yes or No");
                scanf("%s",res);
                if(strcmp(res,"Yes")==0){
                 printf("\nSelect a book ID to view details: \n");
                 scanf("%d", &bookId);
                showBookDetails(bookId);
                }
               
                break;

            case 2:
                printf("\n[Order List Selected]\n");
                break;

            case 3:
                printf("\n[View Cart Selected]\n");
                break;

            case 0:
                printf("\nUser Logged Out.\n");
                break;

            default:
                printf("\nInvalid Choice!\n");
        }

    } while (choice != 0);
}


// void showBookDetails(int id) {
//     FILE *fp = fopen("books.dat", "rb");
//     if(!fp) {
//         printf("No books available!\n");
//         return;
//     }

//     Book b;
//     int found = 0;

//     while(fread(&b, sizeof(Book), 1, fp)) {
//         if(b.id == id) {
//             printf("\n--- BOOK DETAILS ---\n");
//             printf("ID: %d\n", b.id);
//             printf("Title: %s\n", b.title);
//             printf("Author: %s\n", b.author);
//             printf("Price: %.2f\n", b.price);
//             printf("Quantity: %d\n", b.quantity);
//             found = 1;
//             break;
//         }
//     }

//     if(!found)
//         printf("Book not found!\n");

//     fclose(fp);
// }














