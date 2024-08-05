#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_TABLE_SIZE 100 // Choose an appropriate size

typedef struct Book {
    char book_no[100];
    char title[100];
    char author[100];
    char publisher[100];
    float price;
    int copies_no;
    int copies_issued;
    struct Book *next;
    struct Book *next_hash;
    int hash_key;
}Book;

Book *head=NULL;

typedef struct Users
{
    char name[30];
    int borrow_count;
    struct Users *nextUser;
    struct Users *nextUserHash;
    int hash_key;
}Users;

Book* hash_table_books[HASH_TABLE_SIZE] = {NULL}; // Initialize to NULL
Users* hash_table_users[HASH_TABLE_SIZE] = {NULL}; // Initialize to NULL
Users *usr=NULL;
void PressAnyKey(){
printf("\t\t\t\tPress any key to Continue.........");
    while(getchar()!='\n');
    while(getchar()!='\n');
}
void clearScreen()
{
    system("cls");
}

int hash_function(const char *name) {
    // You can implement a simple hash function based on the characters of the name
    int hash_key = 0;
    while (*name) {
        hash_key += *name++;
    }
    return hash_key % HASH_TABLE_SIZE; // HASH_TABLE_SIZE is the size of your hash table
}

void initialize_books() {
    FILE *fp;
    fp = fopen("Library.txt", "r");
    if (fp == NULL) {
        printf("Error Opening File!!\n\n");
        exit(1);
    }

    char temp_author[100];
    char temp_publisher[100];
    char temp_book_no[100];
    char temp_name[100];
    float temp_price;
    int temp_copies_no;
    int temp_copies_issued;

    struct Book* current = NULL;
    struct Book* prev = NULL;

    while (fscanf(fp, "%s %s %s %s %f %d %d\n", temp_name, temp_author, temp_publisher, temp_book_no, &temp_price, &temp_copies_no, &temp_copies_issued) != EOF) {
        struct Book* newBook = (struct Book*)malloc(sizeof(struct Book));
        if (newBook == NULL) {
            printf("Memory allocation error!\n");
            exit(1);
        }

        if (head == NULL) {
            head = newBook;
        } else {
            prev->next = newBook;
        }

        current = newBook;

        strcpy(current->title, temp_name);
        strcpy(current->author, temp_author);
        strcpy(current->publisher, temp_publisher);
        strcpy(current->book_no, temp_book_no);
        current->price = temp_price;
        current->copies_no = temp_copies_no;
        current->copies_issued = temp_copies_issued;
        current->next = NULL;
        int hash_key = hash_function(temp_name);
        newBook->hash_key = hash_key;
        newBook->next_hash = hash_table_books[hash_key];
        hash_table_books[hash_key] = newBook;
        prev = current;
    }

    fclose(fp);
}


void initialize_users() {
    FILE *fp;
    fp = fopen("Users.txt", "r");
    if (fp == NULL) {
        printf("Error Opening File!!\n\n");
        exit(1);
    }
    char temp_name[100];
    int temp_copies_issued;

    struct Users* current = NULL;
    struct Users* prev = NULL;
    while (fscanf(fp, "%s %d\n", temp_name,&temp_copies_issued) != EOF) {
        struct Users* newUser = (struct Users*)malloc(sizeof(struct Users));
        if (newUser == NULL)
            {
            printf("Memory allocation error!\n");
            exit(1);
        }
        if (usr == NULL) {
            usr = newUser;
        } else {
            prev->nextUser = newUser;
        }
        current=newUser;
        strcpy(current->name, temp_name);
        current->borrow_count = temp_copies_issued;
        current->nextUser = NULL;
        int hash_key = hash_function(temp_name);
        newUser->hash_key = hash_key;
        newUser->nextUserHash = hash_table_users[hash_key];
        hash_table_users[hash_key] = newUser;
        prev = current;
    }

    fclose(fp);
}

void save_books() {
    FILE *fp;
    fp = fopen("Library.txt", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    struct Book* current = head;

    while (current != NULL) {
        fprintf(fp, "%s %s %s %s %f %d %d\n", current->title, current->author, current->publisher, current->book_no, current->price, current->copies_no, current->copies_issued);
        current = current->next;
    }

    fclose(fp);
}


void save_users() {
    FILE *fp;
    fp = fopen("Users.txt", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1) ;
    }

    struct Users* current=usr; // Assuming usr is the head of your user list

    // Check if the list is empty

    while (current != NULL) {
        fprintf(fp, "%s %d\n", current->name, current->borrow_count);
        current = current->nextUser;
    }

    fclose(fp);
}

void add_book() {
    clearScreen();

    // Allocate memory for the new book
    struct Book* newBook = (struct Book*)malloc(sizeof(struct Book));
    if (newBook == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }

    printf("\n\n\n\n");
    printf("\t\t\t\tEnter Book name: ");
    scanf(" %s", newBook->title);
    printf("\n\t\t\t\tEnter Author name: ");
    scanf(" %s", newBook->author);
    printf("\n\t\t\t\tEnter Publisher name: ");
    scanf(" %s", newBook->publisher);
    printf("\n\t\t\t\tEnter Book No: ");
    scanf("%s", newBook->book_no);
    printf("\n\t\t\t\tEnter Price: ");
    scanf("%f", &newBook->price);
    printf("\n\t\t\t\tEnter Number of copies: ");
    scanf("%d", &newBook->copies_no);
    newBook->copies_issued = 0;

    // Link the new book in the linked list
    newBook->next = head;
    head = newBook;

    // Link the new book in the hash table
    int hash_key = hash_function(newBook->title);
    newBook->hash_key = hash_key;
    newBook->next_hash = hash_table_books[hash_key];
    hash_table_books[hash_key] = newBook;

    // Save the updated books to file
    save_books();

    printf("Book added successfully.\n");
    PressAnyKey();
    clearScreen();
}

struct Users *add_user() {
    struct Users* newUser = (struct Users*)malloc(sizeof(struct Users));
    if (newUser == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }

    clearScreen();
    printf("\n\n\n\n\n\n");
    printf("\t\t\t\tEnter Borrower Name:");
    scanf("%s", newUser->name);

    newUser->borrow_count = 0;

    // Link the new user in the linked list
    newUser->nextUser = usr;
    usr = newUser;

    // Link the new user in the hash table
    int hash_key = hash_function(newUser->name);
    newUser->hash_key = hash_key;
    newUser->nextUserHash = hash_table_users[hash_key];
    hash_table_users[hash_key] = newUser;

    // Save the updated users to file
    save_users();

    printf("\n\n\n\t\t\t\tNew User %s Successfully Added\n\n", newUser->name);
    PressAnyKey();
    clearScreen();
    return newUser;
}



void all_book_details() {
    Book *curr = head;

    // Check if the list is empty
    if (curr == NULL) {
        printf("\nNo books\n");
        return;
    }
    clearScreen();
    printf("\n\n\n=======================================================================================================================\n\n\n\n");
    while (curr != NULL) {
        printf("\t\t\t\tBook Name: %s\n\t\t\t\tCopies Available: %d\n\t\t\t\tCopies Issued: %d\n\n", curr->title, curr->copies_no, curr->copies_issued);
        curr = curr->next;
    }
    printf("\n\n\n=======================================================================================================================\n\n\n\n");
    PressAnyKey();
    clearScreen();
}


void all_user_details() {
    Users *user = usr;

    // Check if the list is empty
    if (user == NULL) {
        printf("NO USERS\n");
        return;
    }

    clearScreen();
    printf("\n\n\n=======================================================================================================================\n\n\n\n");
    while (user != NULL) {
        printf("\t\t\t\tUser Name: %s\n\t\t\t\tCopies Issued: %d\n\n", user->name, user->borrow_count);
        user = user->nextUser;
    }
    printf("\n\n\n=======================================================================================================================\n\n\n\n");
    PressAnyKey();
    clearScreen();
}
void delete_book() {
    clearScreen();
    char book_name[100];
    printf("\n\n\n\n\n\n");
    printf("\t\t\t\tEnter Book name to delete: ");
    scanf(" %s", book_name);

    int hash_key = hash_function(book_name);
    struct Book* current = hash_table_books[hash_key];
    struct Book* prev = NULL;

    while (current != NULL) {
        if (strcmp(book_name, current->title) == 0) {
            // Unlink the book from the hash table
            if (prev == NULL) {
                hash_table_books[hash_key] = current->next_hash;
            } else {
                prev->next_hash = current->next_hash;
            }

            // Unlink the book from the linked list
            if (current == head) {
                head = current->next;
            } else {
                prev = head;
                while (prev->next != current) {
                    prev = prev->next;
                }
                prev->next = current->next;
            }

            // Save the updated books to file
            save_books();

            // Free memory for the deleted book
            free(current);

            printf("\n\n\n\t\t\t\tBook '%s' deleted successfully.\n\n", book_name);
            PressAnyKey();
            clearScreen();
            return;
        }

        prev = current;
        current = current->next_hash;
    }

    printf("\n\n\n\t\t\t\tBook '%s' not found.\n\n", book_name);
    PressAnyKey();
    clearScreen();
}
void delete_user() {
    clearScreen();
    char user_name[30];
    printf("\n\n\n\n\n\n");
    printf("\t\t\t\tEnter User name to delete: ");
    scanf(" %29s", user_name);

    int hash_key = hash_function(user_name);
    struct Users* current = hash_table_users[hash_key];
    struct Users* prev = NULL;

    while (current != NULL) {
        if (strcmp(user_name, current->name) == 0) {
            // Unlink the user from the hash table
            if (prev == NULL) {
                hash_table_users[hash_key] = current->nextUserHash;
            } else {
                prev->nextUserHash = current->nextUserHash;
            }

            // Unlink the user from the linked list
            if (current == usr) {
                usr = current->nextUser;
            } else {
                prev = usr;
                while (prev->nextUser != current) {
                    prev = prev->nextUser;
                }
                prev->nextUser = current->nextUser;
            }

            // Save the updated users to file
            save_users();

            // Free memory for the deleted user
            free(current);

            printf("\n\n\n\t\t\t\tUser '%s' deleted successfully.\n\n", user_name);
            PressAnyKey();
            clearScreen();
            return;
        }

        prev = current;
        current = current->nextUserHash;
    }

    printf("\n\n\n\t\t\t\tUser '%s' not found.\n\n", user_name);
    PressAnyKey();
    clearScreen();
}

void library_admin() {
    int adminPassword = 1234;
    int enteredPassword;
    clearScreen();
    printf("\n\n\n=======================================================================================================================\n\n\n\n");
    printf("\n\t\t\t\tEnter Admin Pass:\n\t\t\t\t\t");
    scanf("%d", &enteredPassword);

    if (enteredPassword != adminPassword) {
        printf("\n\n\t\t\t\tWrong Password!!\n\n");
        exit(0);
    } else {
        printf("\n\n\t\t\t\tSuccesfully Logged in!!\n\n\n");
        PressAnyKey();
        clearScreen();
        int choice;
        while (1) {
            clearScreen();
            printf("\n\t\t\t\tEnter Input:\n\t\t\t\t1) Add a book\n\t\t\t\t2) All Book Details\n\t\t\t\t3) All User Details\n\t\t\t\t4) Delete Book\n\t\t\t\t5) Delete User\n\t\t\t\t6) Exit\n\t\t\t\tChoose Your Choice\n\t\t\t\t");
            scanf("%d", &choice);
            printf("\n\n\n=======================================================================================================================\n\n\n\n");
            switch (choice) {
                case 1:
                    add_book();
                    break;
                case 2:
                    all_book_details();
                    break;
                case 3:
                    all_user_details();
                    break;
                case 4:
                    delete_book();
                    break;
                case 5:
                    delete_user();
                    break;
                case 6:
                    return;
                default:
                    printf("\n\t\t\t\tInvalid input\n");
                    break;
            }
        }
    }
}

Book* search_book(const char *book_name) {
    int hash_key = hash_function(book_name);
    Book *curr = hash_table_books[hash_key]; // hash_table_books is an array of Book pointers

    while (curr != NULL) {
        if (strcmp(book_name, curr->title) == 0) {
            return curr;
        }
        curr = curr->next_hash; // Traverse the linked list in case of hash collisions
    }

    printf("\n\n\t\t\t\tNO SUCH BOOK '%s' found\n\t\t\t\t", book_name);
    return NULL;
}

Users* search_user(const char *user_name) {
    int hash_key = hash_function(user_name);
    Users *user = hash_table_users[hash_key]; // hash_table_users is an array of Users pointers

    while (user != NULL) {
        if (strcmp(user_name, user->name) == 0) {
            printf("\n\n\t\t\t\tWelcome Back %s \n\n", user->name);
            PressAnyKey();
            clearScreen();
            return user;
        }
        user = user->nextUserHash; // Traverse the linked list in case of hash collisions
    }

    printf("\n\n\t\t\t\tUSER NOT FOUND\n");
    return NULL;
}

void issue_book(Users *user) {
    char booki[30];
    clearScreen();
    clearScreen();
    printf("\n\n\n=======================================================================================================================\n\n\n\n");

    printf("\n\t\t\t\tEnter a book name: ");
    scanf("%s", booki);
    Book *x = search_book(booki);

    if (x == NULL) {
        printf("\n\t\t\t\tNo such Book\n");
        return;
    }

    printf("\n\n\n\t\t\t\tBook name: %s\n\t\t\t\tConfirm issue of book (1. Yes 2. NO): ", x->title);
    int i;
    scanf("%d", &i);
    printf("\n");

    if (i == 1 && x->copies_no > 0 && user->borrow_count < 3) {
        printf("\n\n\n\n\t\t\t\tBook Issued:\n\t\t\t\t%s\n\t\t\t\tTitle: %s\n\t\t\t\tAuthor: %s\n\t\t\t\tPublisher: %s\n\n", x->book_no, x->title, x->author, x->publisher);
        x->copies_no--;
        x->copies_issued++;
        user->borrow_count++;
        save_books();
    printf("\n\n\n=======================================================================================================================\n\n\n\n");
        PressAnyKey();
        clearScreen();
    } else if (i != 1) {
        printf("\n\n\t\t\t\tBook issue canceled.\n");
    printf("\n\n\n====================================================================================\n\n\n\n");

        PressAnyKey();
        clearScreen();
    } else if (x->copies_no <= 0) {
        printf("\n\n\t\t\t\tNo available copies of the book.\n");
        printf("\n\n\n====================================================================================\n\n\n\n");
        PressAnyKey();
        clearScreen();

    } else if (user->borrow_count >= 3) {
        printf("\n\n\n\t\t\tYou have exceeded the limit to borrow books.\nReturn a book to receive a new one.\n\n");
    printf("\n\n\n====================================================================================\n\n\n\n");
    PressAnyKey();
    clearScreen();

    } else {
        printf("\n\n\n\t\t\t\tReturning to the main menu.\n\n");
        printf("\n\n\n====================================================================================\n\n\n\n");

        PressAnyKey();
        clearScreen();
    }
}

void return_book(Users *user) {
    char booki[30];
    clearScreen();
    printf("\n\n\n====================================================================================\n\n\n\n");
    printf("\n\n\n\t\t\t\tEnter book name: ");
    scanf("%s", booki);
    printf("\n");

    Book *x = search_book(booki);

    if (x == NULL) {
        printf("\n\t\t\t\tNo such Book in Domain\n");
        return;
    }

    if (user->borrow_count > 0 && x->copies_issued > 0) {
        printf("\n\n\n\t\t\t\tBook Returned:\n\t\t\t\tTitle: %s\n\t\t\t\tAuthor: %s\n\t\t\t\tPublisher: %s\n\n", x->title, x->author, x->publisher);
        x->copies_no++;
        x->copies_issued--;
        user->borrow_count--;
        printf("\n\n\n====================================================================================\n\n\n\n");
        PressAnyKey();
        clearScreen();
        save_books();
    } else if (user->borrow_count <= 0) {
        printf("\n\n\t\t\t\tNo books borrowed.\n");
        printf("\n\n\n====================================================================================\n\n\n\n");
        PressAnyKey();
        clearScreen();
    } else {
        printf("\n\n\n\t\t\t\tUnable to return the book. Invalid book name or book not issued.\n");
        printf("\n\n\n====================================================================================\n\n\n\n");
        PressAnyKey();
        clearScreen();
    }
}

void user_book_details() {
    clearScreen();
    printf("\n\n\n====================================================================================\n\n\n\n");
    char booki[30];
    printf("\n\n\n\n\t\t\t\tEnter book name: ");
    scanf("%29s", booki);
    printf("\n");

    Book *x = search_book(booki);

    if (x == NULL) {
        printf("\n\n\n\t\t\t\tNo such BOOK\n");
        return;
    }

    printf("\n\n\n\t\t\t\tBook Details:\n");
    printf("\n\t\t\t\tBook Name: %s\n\t\t\t\tBook No: %s\n\t\t\t\tAuthor: %s\n\t\t\t\tPublisher: %s\n\t\t\t\tPrice: %.2f\n\t\t\t\tCopies Available: %d\n\n",x->title, x->book_no, x->author, x->publisher, x->price, x->copies_no);
    printf("\n\n\n====================================================================================\n\n\n\n");
    PressAnyKey();
    clearScreen();
}

void borrowed_books(Users *user) {
    clearScreen();
    printf("\n\n\n====================================================================================\n\n\n\n");
    if (user->borrow_count > 0) {
        Book *curr = head;
        clearScreen();
        printf("\n\n\n\t\t\t\tBorrowed Books by %s:\n", user->name);

        while (curr != NULL) {
            if (curr->copies_issued > 0) {
                printf("\n\n\n\t\t\t\tBook Name: %s\n\t\t\t\tBook No: %s\n\t\t\t\tAuthor: %s\n\t\t\t\tPublisher: %s\n\t\t\t\tCopies Issued: %d\n\n",curr->title, curr->book_no, curr->author, curr->publisher, curr->copies_issued);
            }
            curr = curr->next;
        }
    } else {
        printf("\n\n\n\t\t\t\t%s has not borrowed any books.\n", user->name);
    }
    printf("\n\n\n====================================================================================\n\n\n\n");
    PressAnyKey();
    clearScreen();
}


int main() {
    initialize_books();
    initialize_users();
    int choice;
login:
    clearScreen();
    printf("\n\n\n\t\t\t\tWho is The User?\n\n\t\t\t\t1)Library Admin\n\t\t\t\t2)Student\n\t\t\t\t3)Exit\n\n\t\t\t\tChoose Option:");
    scanf("%d",&choice);
    if(choice==3)
    {
        return 0;
    }
    else if(choice==1)
    {
        clearScreen();
        library_admin();
        clearScreen();
        printf("\n\n\t\t\t\tDo You want to go to 1)Login or 2)Exit\n\t\t\t\t");
        scanf("%d",&choice);
        if(choice==1)
        {
            goto login;
        }
        else if(choice==2)
        {
            printf("\n\n\n\n\t\t\t\tExiting");
            return 0;
        }
    }
    char NAME[100];
    printf("\n\t\t\t\tEnter Your NAME:");
    scanf(" %[^\n]", NAME);
    Users* student = (Users *)malloc(sizeof(Users));
    student = search_user(NAME);
    clearScreen();
    if (student == NULL) {
        printf("\n\n\n\t\t\t\tUser Doesn't Exist\n");
        printf("\n\t\t\t\tDo You Want to create a new User?\n\t\t\t\t1) Yes\t2) No (exit)\n\t\t\t\t");
        scanf("%d", &choice);

        if (choice == 1) {
            student = add_user();
        }
        else{
            printf("\n\n\t\t\t\tExiting");
            return 0;
        }
    }

    while (1) {
        clearScreen();
        printf("\n\t\t\t\tEnter an Input:\n\t\t\t\t1) Issue a book\n\t\t\t\t2) Return a book\n\t\t\t\t3) Book Details\n\t\t\t\t4) Borrowed Books\n\t\t\t\t5) Log Out\n\t\t\t\t6) Exit\n");
        int input;
        printf("\n\t\t\t\tChoose an Option:");
        scanf("%d",&input);
        printf("\n");

        switch (input) {
            case 1:
                issue_book(student);
                break;
            case 2:
                return_book(student);
                break;
            case 3:
                user_book_details();
                break;
            case 4:
                borrowed_books(student);
                break;
            case 5:
                goto login ;
                break;
            case 6:
                return 0;
            default:
                printf("\n\t\t\t\tInvalid input. Please enter a valid option.\n");
                break;
        }
    }
}
