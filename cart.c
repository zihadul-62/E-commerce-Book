#include <stdio.h>
#include <sqlite3.h>
#include "total.h"

void addToCart(char *username, int bookId, char *bookName, float price) {
    sqlite3_stmt *res;
    const char *sql = "INSERT INTO cart (username, book_id, book_name, price) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) == SQLITE_OK) {
        sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);
        sqlite3_bind_int(res, 2, bookId);
        sqlite3_bind_text(res, 3, bookName, -1, SQLITE_STATIC);
        sqlite3_bind_double(res, 4, price);
        if (sqlite3_step(res) == SQLITE_DONE) printf("\n\t\t[SUCCESS] Added to cart!");
        sqlite3_finalize(res);
    }
}

void viewCart(char *username) {
    sqlite3_stmt *res;
    const char *sql = "SELECT id, book_name, price FROM cart WHERE username = ?;";
    printf("\n\t\t--- YOUR SHOPPING CART ---\n");
    int count = 0;
    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) == SQLITE_OK) {
        sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);
        while (sqlite3_step(res) == SQLITE_ROW) {
            count++;
            printf("\t\tItem %d: %-15s | $%.2f\n", count, 
                   sqlite3_column_text(res, 1), sqlite3_column_double(res, 2));
        }
        sqlite3_finalize(res);
    }

    if (count == 0) {
        printf("\t\tYour cart is empty.\n");
        return;
    }

    printf("\n\t\t[1] Checkout All  [0] Back: ");
    int choice; scanf("%d", &choice);
    if (choice == 1) checkout(username);
}

void checkout(char *username) {
    sqlite3_stmt *res;
    const char *sql_select = "SELECT book_id, book_name, price FROM cart WHERE username = ?;";
    
    // 1. Process items from cart to orders
    if (sqlite3_prepare_v2(db, sql_select, -1, &res, 0) == SQLITE_OK) {
        sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);
        
        while (sqlite3_step(res) == SQLITE_ROW) {
            int b_id = sqlite3_column_int(res, 0);
            const char *b_name = (const char *)sqlite3_column_text(res, 1);
            float b_price = (float)sqlite3_column_double(res, 2);
            
            // Record in order history
            placeOrder(username, b_id, (char *)b_name, b_price);
        }
        sqlite3_finalize(res);
    }

    // 2. Clear the cart
    const char *sql_delete = "DELETE FROM cart WHERE username = ?;";
    sqlite3_stmt *del_res;
    if (sqlite3_prepare_v2(db, sql_delete, -1, &del_res, 0) == SQLITE_OK) {
        sqlite3_bind_text(del_res, 1, username, -1, SQLITE_STATIC);
        sqlite3_step(del_res);
        sqlite3_finalize(del_res);
        printf("\n\t\t[SUCCESS] Checkout complete. All items moved to Order History!\n");
    }
}