#include <stdio.h>
#include <string.h>
#include <time.h> // For timestamping orders
#include "total.h"

int nextOrder_id()
{
    FILE *fp = fopen("order_history.txt", "r");
    if (fp == NULL)
        return 1;
    int count = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
            count++;
    }
    fclose(fp);
    return count + 1;
}



//****              we are working on placeOrder function                ****



// Updated to include the username of the person buying
// want to decresse the stock quantity when order is placed in placeOrder function my database "books.txt"
// 1,BookA,AuthorA,100.00,5
// 2,BookB,AuthorB,200.00,3
// 3,BookC,AuthorC,300.00,7
// 4,BookD,AuthorD,400.00,9


void placeOrder(char *username, int bookId, char *bookName, float price)
{
    FILE *fp = fopen("order_history.txt", "a");
    if (fp == NULL)
        return;

    int orderId = nextOrder_id();
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char date[20];
    sprintf(date, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);


    // CSV Format: OrderID, Username, BookID, BookName, Price, Date
    fprintf(fp, "%d,%s,%d,%s,%.2f,%s\n", orderId, username, bookId, bookName, price, date);
    
    fclose(fp);
    
    // Update stock quantity in books.txt
    FILE *fp_books = fopen("books.txt", "r");
    if (fp_books == NULL)
        return;
    //if book quantity ==0 , then cannot place order and print unsuccess message
    //  if(1) {
    //     char line[256];
    //     int found = 0;
    //     while (fgets(line, sizeof(line), fp_books)) {
    //         int id;
    //         char name[100], author[100];
    //         float price;
    //         int stock;

    //         sscanf(line, "%d,%99[^,],%99[^,],%f,%d", &id, name, author, &price, &stock);
    //         if (id == bookId) {
    //             if (stock <= 0) {
    //                 printf("\n\t\t[ERROR] Cannot place order. Book '%s' is out of stock.\n", name);
    //                 fclose(fp_books);
    //                 return;
    //             }
    //             found = 1;
    //             break;
    //         }
    //     }
    //     if (!found) {
    //         printf("\n\t\t[ERROR] Book ID #%d not found in inventory.\n", bookId);
    //         fclose(fp_books);
    //         return;
    //     }
    //     rewind(fp_books); // Reset file pointer to beginning for stock update
    // }
    FILE *fp_temp = fopen("temp_books.txt", "w");
    if (fp_temp == NULL)
        return;

    char line[256];
    while (fgets(line, sizeof(line), fp_books)) {
        int id;
        char name[100], author[100];
        float price;
        int stock;

        sscanf(line, "%d,%99[^,],%99[^,],%f,%d", &id, name, author, &price, &stock);

        if (id == bookId && stock > 0) {
            stock--;
            fprintf(fp_temp, "%d,%s,%s,%.2f,%d\n", id, name, author, price, stock);
            
        } else {
            fprintf(fp_temp, "%s", line);
        }
    }

    fclose(fp_books);
    fclose(fp_temp);

    remove("books.txt");
    rename("temp_books.txt", "books.txt");

    printf("\n\t\t[SUCCESS] Order #%d placed for %s\n", orderId, username);
}



void viewOrderHistory(char* currentUser) {
    FILE *fp = fopen("order_history.txt", "r");
    if (fp == NULL) {
        printf("\n\t\t[!] No order history found.\n");
        return;
    }

    char line[256];
    int found = 0;

    printf("\n\t\t================ YOUR ORDER HISTORY ================\n");
    printf("\t\t%-8s | %-15s | %-8s | %-10s\n", "ORDER ID", "BOOK NAME", "PRICE", "DATE");
    printf("\t\t----------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        int ordId, bkId;
        char user[50], bName[50], date[20];
        float price;

        // Parsing: OrderID, Username, BookID, BookName, Price, Date
        sscanf(line, "%d,%49[^,],%d,%49[^,],%f,%19[^\n]", 
               &ordId, user, &bkId, bName, &price, date);

        if (strcmp(user, currentUser) == 0) {
            printf("\t\t#%-7d | %-15.15s | $%-7.2f | %-10s\n", ordId, bName, price, date);
            found = 1;
        }
    }

    if (!found) {
        printf("\t\t          No orders found for your account.         \n");
    }
    printf("\t\t----------------------------------------------------\n");
    fclose(fp);
}