#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESC 100

// Structure to hold expense data
typedef struct Expense {
char category[30];
double amount;
char date[11];  // Format: YYYY-MM-DD
char description[MAX_DESC];
struct Expense* next;
} Expense;

// Function prototypes
Expense* loadExpenses(const char* filename);
void saveExpenses(Expense* head, const char* filename);
Expense* addExpense(Expense* head);
void viewExpenses(Expense* head);
Expense* deleteExpense(Expense* head);
void freeExpenses(Expense* head);
void menu();
void totalExpenses(Expense* head);
void exportToExcel(Expense* head, const char* filename); // New function

// Menu function
void menu() {
printf("\nExpense Tracker Menu\n");
printf("1. Add Expense\n");
printf("2. View Expenses\n");
printf("3. Delete Expense\n");
printf("4. Save and Exit\n");
printf("5. Export Expenses to Excel\n"); // New option
printf("Enter your choice: ");
}

// Add expense
Expense* addExpense(Expense* head) {
Expense* newExpense = (Expense*)malloc(sizeof(Expense));
if (!newExpense) {
printf("Memory allocation error\n");
return head;
}

printf("Enter category: ");  
fgets(newExpense->category, sizeof(newExpense->category), stdin);  
strtok(newExpense->category, "\n");  

printf("Enter amount: ");  
scanf("%lf", &newExpense->amount);  
getchar();  

printf("Enter date (YYYY-MM-DD): ");  
fgets(newExpense->date, sizeof(newExpense->date), stdin);  
strtok(newExpense->date, "\n");  

printf("Enter description: ");  
fgets(newExpense->description, sizeof(newExpense->description), stdin);  
strtok(newExpense->description, "\n");  

newExpense->next = head;  
head = newExpense;  

printf("Expense added successfully.\n");  
return head;

}

// View expenses
void viewExpenses(Expense* head) {
if (!head) {
printf("No expenses to show.\n");
return;
}

Expense* temp = head;  
int count = 1;  

printf("\nExpenses:\n");  
printf("No. | Category       | Amount    | Date       | Description\n");  
printf("-----------------------------------------------------------\n");  
while (temp) {  
    printf("%-4d | %-13s | %-9.2lf | %-10s | %s\n",  
           count++, temp->category, temp->amount, temp->date, temp->description);  
    temp = temp->next;  
}

}

// Delete expense
Expense* deleteExpense(Expense* head) {
if (!head) {
printf("No expenses to delete.\n");
return head;
}

viewExpenses(head);  
printf("Enter expense number to delete: ");  
int num;  
scanf("%d", &num);  
getchar();  

Expense* temp = head;  
Expense* prev = NULL;  
int count = 1;  

while (temp && count < num) {  
    prev = temp;  
    temp = temp->next;  
    count++;  
}  

if (!temp) {  
    printf("Invalid expense number.\n");  
    return head;  
}  

if (prev)  
    prev->next = temp->next;  
else  
    head = temp->next;  

free(temp);  
printf("Expense deleted.\n");  
return head;

}

// Load expenses from file
Expense* loadExpenses(const char* filename) {
FILE* file = fopen(filename, "rb");
if (!file) return NULL;

Expense* head = NULL;  
Expense temp;  

while (fread(&temp, sizeof(Expense), 1, file)) {  
    Expense* newExpense = (Expense*)malloc(sizeof(Expense));  
    if (!newExpense) break;  
    *newExpense = temp;  
    newExpense->next = head;  
    head = newExpense;  
}  

fclose(file);  
return head;

}

// Save expenses to file
void saveExpenses(Expense* head, const char* filename) {
FILE* file = fopen(filename, "wb");
if (!file) {
printf("Failed to open file for saving.\n");
return;
}

Expense* temp = head;  
while (temp) {  
    fwrite(temp, sizeof(Expense), 1, file);  
    temp = temp->next;  
}  

fclose(file);

}

// Free all expenses
void freeExpenses(Expense* head) {
Expense* temp;
while (head) {
temp = head;
head = head->next;
free(temp);
}
}

// New function to calculate and display total expenses
void totalExpenses(Expense* head) {
if (!head) {
printf("No expenses to calculate total.\n");
return;
}

double total = 0;  
Expense* temp = head;  
while (temp) {  
    total += temp->amount;  
    temp = temp->next;  
}  

printf("\nTotal Expenses: %.2lf\n", total);

}

// New function to export expenses to Excel (CSV)
void exportToExcel(Expense* head, const char* filename) {
if (!head) {
printf("No expenses to export.\n");
return;
}

FILE* file = fopen(filename, "w");  
if (!file) {  
    printf("Unable to create file.\n");  
    return;  
}  

// Write CSV header  
fprintf(file, "Category,Amount,Date,Description\n");  

double total = 0;  
Expense* temp = head;  
while (temp) {  
    fprintf(file, "%s,%.2lf,%s,%s\n", temp->category, temp->amount, temp->date, temp->description);  
    total += temp->amount;  
    temp = temp->next;  
}  

// Write total at the end  
fprintf(file, ",%.2lf,,Total\n", total);  

fclose(file);  
printf("Expenses exported to %s successfully.\n", filename);

}

// Main function
int main() {
Expense* expenses = NULL;
int choice;
const char* filename = "expenses.dat";

// Load existing expenses from file  
expenses = loadExpenses(filename);  

do {  
    menu();  
    scanf("%d", &choice);  
    getchar(); // consume newline  

    switch(choice) {  
        case 1:  
            expenses = addExpense(expenses);  
            break;  
        case 2:  
            viewExpenses(expenses);  
            totalExpenses(expenses); // Show total after viewing  
            break;  
        case 3:  
            expenses = deleteExpense(expenses);  
            break;  
        case 4:  
            saveExpenses(expenses, filename);  
            printf("Expenses saved. Exiting...\n");  
            break;  
        case 5:  
            exportToExcel(expenses, "expenses.csv"); // Export to Excel  
            break;  
        default:  
            printf("Invalid choice. Try again.\n");  
    }  
} while(choice != 4);  

freeExpenses(expenses);  
return 0;

}