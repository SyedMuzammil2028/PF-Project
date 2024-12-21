#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_ITEMS 100
#define MAX_SALES 100  // Maximum number of sales entries
#define FILE_NAME "inventory_data.txt" //File name for storing data
#define STOCK_THRESHOLD 5  // Stock alert threshold
#define ISLAMIC_EVENT_DISCOUNT 0.10  // 10% discount for Islamic events
#define EMPLOYEE_SALARY_RATE 0.15  // Rate for employee salary

// Structure to hold information about users
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

// Structure to hold information about items
struct Item {
    char id[10];   // ID as string with category prefix
    char name[50];
    char category[20]; // Item category (e.g., Sports, Clothing, Grocery)
    float purchasePrice;  // Cost price per item
    float sellingPrice;  // Selling price per item
    int quantity;
    int soldQuantity;
    float profit;
    float originalSellingPrice;  // Store original selling price for reset
};

// Structure to hold sales information
struct Sale {
    char itemId[10];  // ID of the sold item
    char itemName[50];  // Name of the sold item
    int quantitySold;  // Quantity sold
    float totalAmount;  // Total amount for the sale
};

// Global variables

// Array to hold the inventory
struct Item inventory[MAX_ITEMS];
int itemCount = 0;

// Variable to track the current user
char currentUser[MAX_USERNAME_LENGTH];
char userFileName[100];

// Global profit tracking
float totalProfit = 0;

// Array to hold user information
struct User users[MAX_USERS];
int userCount = 0;

// Variables for event date
struct tm eventDate; // To store the date of the event
int discountApplied = 0; // Flag to check if the discount is currently applied

// Sales history
struct Sale salesHistory[MAX_SALES];
int salesCount = 0; // Count of sales entries

// Function prototypes
void registerUser();
int loginUser();
void mainMenu();
void addItem();
void displayInventory();
void SellItem();
void generateReport();
void saveData();
void loadData();
void removeItem();
void applyDiscountOnEvent();
void resetDiscount();
void sortItemsByCategory();
void showFinanceSection();
void showSalesHistory();
void resetInventory();
void generateID(char *category, char *id);
void saveUsers();
void loadUsers();

// Entry point
int main() {
    // Load users data
    loadUsers();

    printf("\n===================================================================================================\n");
    printf("\t\t\tW E L C O M E   T O   M A R T  1 . 0   S Y S T E M !");
    printf("\n===================================================================================================\n");

    int loggedIn = 0;

    while (!loggedIn) {
        printf("\n--------------------------------------- M A I N   M E N U -----------------------------------------\n");
        printf("| %-10s | %-30s \n", "Option", "Description");
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("| %-10s | %-30s \n", "1", "Register");
        printf("| %-10s | %-30s \n", "2", "Login");
        printf("| %-10s | %-30s \n", "3", "Exit");
        printf("---------------------------------------------------------------------------------------------------\n");

        printf("\nEnter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loggedIn = loginUser();
                break;
            case 3:
                printf("\n-----------------------------------------------------------------------------------------------------\n");
                printf("\t\t\t\t  E X I T I N G   T H E   S Y S T E M\n");
                printf("\t\t\t\t    Thank you for using MART System.\n", currentUser);
                printf("-----------------------------------------------------------------------------------------------------\n");
                printf("\t\t\t      Designed by Muzammil & Co-Developed by Anas\n");
                printf("-----------------------------------------------------------------------------------------------------\n");

                exit(0);
            default:
                printf("\nInvalid choice. Please select a valid option (1-3).\n");
        }
    }

    printf("\nLogin successful! Loading data...\n");
    loadData();
    mainMenu();
    return 0;
}

// Register a new user
void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("\t\t\t      R E G I S T R A T I O N   F A I L E D ");
        printf("\n-----------------------------------------------------------------------------------------------------\n");
        printf("Sorry, user limit reached. Cannot register more users.\n");
        return;
    }

    printf("----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t          U S E R   R E G I S T R A T I O N");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    printf("Enter a username: ");
    scanf("%s", users[userCount].username);

    printf("Enter a password: ");
    scanf("%s", users[userCount].password);

    userCount++;
    saveUsers();

    printf("----------------------------------------------------------------------------------------------------\n");
    printf("\t\t        Registration successful! You can now log in.");
    printf("\n----------------------------------------------------------------------------------------------------\n");
}

// Log in a user
int loginUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t                U S E R   L O G I N");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    printf("Enter your username: ");
    scanf("%s", username);

    printf("Enter your password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            strcpy(currentUser, username);
            snprintf(userFileName, sizeof(userFileName), "%s_inventory.txt", currentUser);

            printf("-----------------------------------------------------------------------------------------------------\n");
            printf("\t\t\t      Login successful! Welcome, %s.", currentUser);
            printf("\n-----------------------------------------------------------------------------------------------------\n");
            return 1;
        }
    }

    printf("\n-----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t   Invalid username or password. Please try again.");
    printf("\n-----------------------------------------------------------------------------------------------------\n");
    return 0;
}

// Main menu function
void mainMenu() {
    int choice;
        printf("\n-----------------------------------------------------------------------------------------------------\n");
        printf("\t\tW E L C O M E   T O   M A R T  1 . 0   M A N A G E M E N T   S Y S T E M");

    while (1) {
        // Add a stylish header for the main menu
        printf("\n-----------------------------------------------------------------------------------------------------\n");
        printf("\t\t\t     M A R T  1 . 0   A D M I N   C O N S O L E");
        printf("\n-----------------------------------------------------------------------------------------------------\n");
        printf("Select an option from the menu below:\n");
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("| %-5s | %-30s \n", "1", "Add Item to Inventory");
        printf("| %-5s | %-30s \n", "2", "View Inventory");
        printf("| %-5s | %-30s \n", "3", "Purchase Item (Billing)");
        printf("| %-5s | %-30s \n", "4", "Remove Item");
        printf("| %-5s | %-30s \n", "5", "Generate Report");
        printf("| %-5s | %-30s \n", "6", "Apply Discount for Event");
        printf("| %-5s | %-30s \n", "7", "Reset Discount");
        printf("| %-5s | %-30s \n", "8", "Finance Section");
        printf("| %-5s | %-30s \n", "9", "Show Sales History");
        printf("| %-5s | %-30s \n", "10", "Sort Items by Category");
        printf("| %-5s | %-30s \n", "11", "Reset Inventory (Clear All Data)");
        printf("| %-5s | %-30s \n", "12", "Save and Logout");
        printf("-----------------------------------------------------------------------------------------------------\n");
        
        // Prompt for choice with improved formatting
        printf("\nEnter your choice (1-12): ");
        scanf("%d", &choice);

        // Switch case to handle user input
        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                displayInventory();
                break;
            case 3:
                SellItem();
                break;
            case 4:
                removeItem();
                break;
            case 5:
                generateReport();
                break;
            case 6:
                applyDiscountOnEvent();
                break;
            case 7:
                resetDiscount();
                break;
            case 8:
                showFinanceSection();
                break;
            case 9:
                showSalesHistory();
                break;
            case 10:
                sortItemsByCategory();
                break;
            case 11:
                resetInventory();
                break;
            case 12:
                // Save inventory data to file before exiting
                saveData();
                // Logout Screen with a simple logo and "Goodbye" message
                printf("-----------------------------------------------------------------------------------------------------\n");
                printf("\t\t\t  L O G G I N G   O U T    G O O D B Y E , %s !\n", currentUser);
                printf("-----------------------------------------------------------------------------------------------------\n");
                printf("\t\t\t   Designed by Muzammil & Co-Developed by Anas\n");
                printf("-----------------------------------------------------------------------------------------------------\n");


                exit(0);
            default:
                printf("-----------------------------------------------------------------------------------------------------\n");
                printf("Invalid choice. Please select a valid option (1-12).\n");
                printf("-----------------------------------------------------------------------------------------------------\n");
        }
    }
}

// Function to add multiple items to the inventory in a loop
void addItem() {
    char choice = 'y';

    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t       A D D   I T E M   T O   I N V E N T O R Y");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    while (choice == 'y' || choice == 'Y') {
        if (itemCount >= MAX_ITEMS) {
            printf("Inventory is full!\n");
            return;
        }

        printf("\nSpecify item category [ e.g., Grocery, Dairy, Meat, Crockery... ]: ");
        scanf("%s", inventory[itemCount].category);

        // Generate item ID based on the first 3 letters of category
        generateID(inventory[itemCount].category, inventory[itemCount].id);

        printf("Enter item name: ");
        scanf("%s", inventory[itemCount].name);
        printf("Enter purchase price [per item]: ");
        scanf("%f", &inventory[itemCount].purchasePrice);
        printf("Enter selling price [per item]: ");
        scanf("%f", &inventory[itemCount].sellingPrice);
        printf("Enter item quantity: ");
        scanf("%d", &inventory[itemCount].quantity);

        inventory[itemCount].originalSellingPrice = inventory[itemCount].sellingPrice;  // Store original selling price
        inventory[itemCount].soldQuantity = 0;  // Initial sold quantity
        inventory[itemCount].profit = 0;  // Initial profit

        itemCount++;
        printf("\nItem added successfully! ID: %s\n", inventory[itemCount - 1].id);

        // Ask if the user wants to add another item
        printf("Do you want to add another item? (y/n): ");
        scanf(" %c", &choice);
    }
    saveData();  // Save data after adding all items

    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t         A D D I T I O N   C O M P L E T E D");
    printf("\n----------------------------------------------------------------------------------------------------\n");
}

// Function to generate a unique ID based on the first three letters of the category
void generateID(char *category, char *id) {
    char prefix[4];

    // Get the first three letters of the category and convert to uppercase
    strncpy(prefix, category, 3);
    for (int i = 0; i < 3; i++) {
        prefix[i] = toupper(prefix[i]);
    }
    prefix[3] = '\0';  // Null-terminate the prefix

    // Format the ID: <PREFIX><3-digit counter> (e.g., SPO001, CLO002)
    sprintf(id, "%s%03d", prefix, itemCount + 1);
}

// Function to display the current inventory with stock alerts
void displayInventory() {
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\t\tI N V E N T O R Y");
    printf("\n-----------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-18s | %-10s | %-12s | %-12s |\n", 
           "ID", "Name", "Category", "Price", "Quantity", "Stock Alert");
    printf("-----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < itemCount; i++) {
        printf("| %-10s | %-20s | %-18s | $%-9.2f | %-12d | %-12s |\n", 
               inventory[i].id, inventory[i].name, inventory[i].category,
               inventory[i].sellingPrice, inventory[i].quantity, 
               (inventory[i].quantity <= STOCK_THRESHOLD) ? "LOW STOCK" : "");
    }

    printf("-----------------------------------------------------------------------------------------------------\n");
}

// Function for selling an item (Billing counter by bar code)
void SellItem() {
    char id[10];
    int qty;
    char choice = 'y'; // Variable to determine if the user wants to purchase multiple items

    // Add a heading for the billing section
    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\tS E L L I N G   I T E M   ( B I L L I N G   C O U N T E R )");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    while (choice == 'y' || choice == 'Y') {
        printf("\nEnter item ID to be scan: ");
        scanf("%s", id);
        printf("Enter quantity: ");
        scanf("%d", &qty);

        int itemFound = 0; // Flag to check if the item ID exists
        for (int i = 0; i < itemCount; i++) {
            if (strcmp(inventory[i].id, id) == 0) {
                itemFound = 1; // Item found

                if (inventory[i].quantity >= qty) {
                    float total = qty * inventory[i].sellingPrice;
                    inventory[i].quantity -= qty;
                    inventory[i].soldQuantity += qty;

                    // Calculate profit for this transaction
                    float itemProfit = qty * (inventory[i].sellingPrice - inventory[i].purchasePrice);
                    inventory[i].profit += itemProfit;
                    totalProfit += itemProfit;

                    // Log the sale
                    if (salesCount < MAX_SALES) {
                        strcpy(salesHistory[salesCount].itemId, inventory[i].id);
                        strcpy(salesHistory[salesCount].itemName, inventory[i].name);
                        salesHistory[salesCount].quantitySold = qty;
                        salesHistory[salesCount].totalAmount = total;
                        salesCount++;  // Increment sales count
                    }

                    printf("\nItem sold successfully! Total price: %.2f\n", total);
                    saveData();  // Save data after selling
                    } 

                    else {
                    printf("\nNot enough stock available.\n");
                }
                break;
            }
        }

        if (!itemFound) {
            printf("\nInvalid item ID. Please try again.\n");
        }

        printf("\nDo you want to scan another item? (y/n): ");
        scanf(" %c", &choice); // Prompt for selling another item
    }
}

// Function to remove an item from the inventory
void removeItem() {
    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t R E M O V E   I T E M   F R O M   I N V E N T O R Y");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    char id[10];
    int found = 0;
    char choice = 'y'; // Variable to determine if the user wants to remove multiple items

    while (choice == 'y' || choice == 'Y') {
        printf("Enter item ID to remove: ");
        scanf("%s", id);

        for (int i = 0; i < itemCount; i++) {
            if (strcmp(inventory[i].id, id) == 0) {
                found = 1;

                int j;
                for (j = i; j < itemCount - 1; j++) {
                    inventory[j] = inventory[j + 1];  // Shift items left
                }
                itemCount--;
                printf("Item removed successfully!\n");
                saveData();  // Save data after removal
                break;
            }
        }
        if (!found) {
            printf("Item not found in inventory.\n");
        }

        printf("Do you want to remove another item? (y/n): ");
        scanf(" %c", &choice); // Prompt for removing another item
    }
}

// Function to generate a detailed report
void generateReport() {
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\t\tS A L E S   R E P O R T");
    printf("\n-----------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-18s | %-15s | %-15s |\n", 
           "ID", "Name", "Category", "Sold Quantity", "Profit");
    printf("-----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].soldQuantity > 0) {
            printf("| %-10s | %-20s | %-18s | %-15d | $%-15.2f |\n", 
                   inventory[i].id, inventory[i].name, inventory[i].category,
                   inventory[i].soldQuantity, inventory[i].profit);
        }
    }

    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("| %-72s | $%-15.2f |\n", "Total Profit", totalProfit);
    // printf("-----------------------------------------------------------------------------------------------------\n");
}

// Function to apply a discount on Islamic event days
void applyDiscountOnEvent() {
    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t    A P P L Y   D I S C O U N T   F O R   E V E N T");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    // Get the current date
    time_t t = time(NULL);
    struct tm *currentDate = localtime(&t);

    // Ask for event date input
    printf("Enter the event date (YYYY MM DD): ");
    scanf("%d %d %d", &eventDate.tm_year, &eventDate.tm_mon, &eventDate.tm_mday);
    eventDate.tm_year -= 1900; // tm_year is years since 1900
    eventDate.tm_mon -= 1;     // tm_mon is 0-11

    // Check if the event date has already passed
    if (difftime(mktime(&eventDate), mktime(currentDate)) < 0) {
        printf("\n[Error] The event date has already passed. No discount can be applied.\n");
        return; // Exit if the date has passed
    }

    // Apply the discount
    if (!discountApplied) {
        for (int i = 0; i < itemCount; i++) {
            inventory[i].sellingPrice -= inventory[i].sellingPrice * ISLAMIC_EVENT_DISCOUNT;
        }
        printf("\n[Success] A 10%% discount has been successfully applied to all items.\n");
        discountApplied = 1; // Set the discount-applied flag
    } else {
        printf("\n[Notice] Discount has already been applied for this event.\n");
    }

    saveData(); // Save the updated prices
}

// Function to reset the discount
void resetDiscount() {
    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t  R E S E T   D I S C O U N T   T O   O R I G I N A L   P R I C E S");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    if (!discountApplied) {
        printf("\n[Notice] No discount is currently applied. Nothing to reset.\n");
        return;
    }

    for (int i = 0; i < itemCount; i++) {
        inventory[i].sellingPrice = inventory[i].originalSellingPrice; // Restore original price
    }

    printf("\n[Success] All discounts have been removed. Original prices have been restored for all items.\n");

    discountApplied = 0; // Reset the discount-applied flag
    saveData(); // Save the updated prices
}

// Function to show the finance section
void showFinanceSection() {
    // Financial Overview Section
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\t   F I N A N C I A L   O V E R V I E W");
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-18s | %-15s | %-15s | %-10s |\n", 
           "ID", "Name", "Category", "Purchase Price", "Selling Price", "Profit");
    printf("---------------------------------------------------------------------------------------------------------\n");

    // Displaying the item details
    for (int i = 0; i < itemCount; i++) {
        printf("| %-10s | %-20s | %-18s | $%-14.2f | $%-14.2f | $%-9.2f |\n", 
               inventory[i].id, inventory[i].name, inventory[i].category, 
               inventory[i].purchasePrice, inventory[i].sellingPrice, inventory[i].profit);
    }

    // Display total profit
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("| %-70s | Total Profit: $%-10.2f |\n", " ", totalProfit);

    // Employee Salaries Section
    float employeeSalaries = totalProfit * EMPLOYEE_SALARY_RATE;
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\t   E M P L O Y E E   S A L A R I E S");
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("| %-30s | Total: $%-24.2f |\n", "Salary (15% of Total Profit)", employeeSalaries);
    printf("---------------------------------------------------------------------------------------------------------\n");
}

// Function to show the sales history
void showSalesHistory() {
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\t\tS A L E S   H I S T O R Y");
    printf("\n-----------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-30s | %-15s |\n", 
           "Item ID", "Item Name", "Quantity Sold", "Total Amount");
    printf("-----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < salesCount; i++) {
        printf("| %-10s | %-20s | %-30d | $%-14.2f |\n", 
               salesHistory[i].itemId, salesHistory[i].itemName, 
               salesHistory[i].quantitySold, salesHistory[i].totalAmount);
    }

    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("| %-48s | Total Entries: %-10d |\n", " ", salesCount);
    printf("-----------------------------------------------------------------------------------------------------\n");
}

// Function to sort items by category (no predefined limits)
void sortItemsByCategory() {
    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t       S O R T   I T E M S   B Y   C A T E G O R Y");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    if (itemCount == 0) {
        printf("\n[Notice] The inventory is empty. Add items to sort them by category.\n");
        return;
    }

    // Temporary variable for swapping
    struct Item temp;

    // Bubble sort algorithm based on category
    for (int i = 0; i < itemCount - 1; i++) {
        for (int j = i + 1; j < itemCount; j++) {
            if (strcmp(inventory[i].category, inventory[j].category) > 0) {
                // Swap items if out of order
                temp = inventory[i];
                inventory[i] = inventory[j];
                inventory[j] = temp;
            }
        }
    }

    printf("\n[Success] Items have been sorted by category.\n");
    printf("Here is the updated inventory:\n");
    displayInventory();  // Show sorted inventory (ensure this function is defined)
}

// Function to reset the entire inventory
void resetInventory() {
    printf("\n----------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t     R E S E T   E N T I R E   I N V E N T O R Y");
    printf("\n----------------------------------------------------------------------------------------------------\n");

    char choice;
    printf("Are you sure you want to reset the entire inventory? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        itemCount = 0;  // Reset item count to zero
        totalProfit = 0;  // Reset total profit
        salesCount = 0; // Reset sales count

        printf("\n[Success] Inventory has been reset! All data has been cleared.\n");
        saveData();  // Save the empty state
    } else {
        printf("\n[Notice] Inventory reset was canceled. No data was cleared.\n");
    }
}

// Function to save the inventory data to a file
void saveData() {
    FILE *file = fopen(userFileName, "w");
    if (file == NULL) {
        printf("Error saving inventory data for user %s.\n", currentUser);
        return;
    }

    // Write the number of items
    fprintf(file, "%d\n", itemCount);
    
    // Write each item's data to the file
    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%s %s %s %.2f %.2f %d %d %.2f %.2f\n", 
                inventory[i].id, inventory[i].name, inventory[i].category, 
                inventory[i].purchasePrice, inventory[i].sellingPrice, 
                inventory[i].quantity, inventory[i].soldQuantity, inventory[i].profit,
                inventory[i].originalSellingPrice); // Save original selling price
    }

    fclose(file);  // Close the file
     printf("\nInventory saved successfully for user %s.\n", currentUser);
}

// Function to load the inventory data from a file
void loadData() {
    FILE *file = fopen(userFileName, "r");
    if (file == NULL) {
        printf("No inventory data found for user %s. Starting fresh.\n", currentUser);
        itemCount = 0;
        return;
    }

    // Read the number of items
    fscanf(file, "%d", &itemCount);

    // Read each item's data from the file
    for (int i = 0; i < itemCount; i++) {
        fscanf(file, "%s %s %s %f %f %d %d %f %f", 
               inventory[i].id, inventory[i].name, inventory[i].category, 
               &inventory[i].purchasePrice, &inventory[i].sellingPrice, 
               &inventory[i].quantity, &inventory[i].soldQuantity, &inventory[i].profit,
               &inventory[i].originalSellingPrice); // Load original selling price
    }

    fclose(file);  // Close the file
    printf("Inventory loaded successfully for user %s. %d items in inventory.\n", currentUser,itemCount);
}

// Save users data to a file
void saveUsers() {
    FILE *file = fopen("users_data.txt", "w");
    if (file == NULL) {
        printf("Error saving user data.\n");
        return;
    }

    fprintf(file, "%d\n", userCount);
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }

    fclose(file);
}

// Load users data from a file
void loadUsers() {
    FILE *file = fopen("users_data.txt", "r");
    if (file == NULL) {
        printf("No user data found. Starting fresh.\n");
        userCount = 0;
        return;
    }

    fscanf(file, "%d", &userCount);
    for (int i = 0; i < userCount; i++) {
        fscanf(file, "%s %s", users[i].username, users[i].password);
    }

    fclose(file);
}