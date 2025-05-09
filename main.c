
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <stdarg.h>

// Color definitions for Windows console
#define COLOR_RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define COLOR_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_BLUE FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define COLOR_CYAN FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define COLOR_MAGENTA FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define COLOR_YELLOW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY

#define FALSE 0
#define TRUE 1

#define MAX_PROPERTY_NAME 50
#define MAX_CONTACT_NO 20
#define MAX_ADDRESS 50
#define MAX_LISTINGS 255
#define MAX_DESC 25
#define MAX_AMENITIES 10

typedef enum {
    DESC_NONE = 0, SINGLE_BED, DOUBLE_BED, KING_BED, QUEEN_BED,
    SINGLE_ROOM, DOUBLE_ROOM, TRIPLE_ROOM, QUADRUPLE_ROOM,
    WITH_KITCHEN, WITH_BALCONY, WITH_OCEAN_VIEW, WITH_MOUNTAIN_VIEW,
    WITH_GARDEN, WITH_POOL, WITH_HOT_TUB, WITH_PRIVATE_BATHROOM,
    WITH_WORKSPACE, WITH_FIREPLACE, ENTIRE_APARTMENT, ENTIRE_HOUSE,
    ENTIRE_VILLA, ENTIRE_COTTAGE, DESC_MAX_TYPES
} DescriptionType;

// Define struct for AirBnB
typedef struct {
    char propertyName[MAX_PROPERTY_NAME];

    struct {
        char city[MAX_ADDRESS],
        street[MAX_ADDRESS],
        barangay[MAX_ADDRESS];
        int zipCode;
    } address;
    
    DescriptionType propertyDescriptions[10];

    struct {
        int isHotShower, isAC, isFreeParking, isGuestSuppliesAvail,
        isFreeWifi, isAnimalFriendly, isServiceFree, isKitchenWareAvail,
        isWashingMachineAvail, isAvailElevator;
    } amenities;
    
    struct {
        int hour, minute;
    } checkInTime;

    char contactNumber[MAX_CONTACT_NO];
    char propertyOwner[MAX_PROPERTY_NAME];

    float price, serviceFee;
} BNB;

//price is calculated according to the quality of property, eg. description and amenities
const char* descriptions[DESC_MAX_TYPES] = {
    "", "Single Bed", "Double Bed", "King Bed", "Queen Bed", "Single Room",
    "Double Room", "Triple Room", "Quadruple Room", "With Kitchen",
    "With Balcony", "With Ocean View", "With Mountain View", "With Garden",
    "With Pool", "With Hot Tub", "With Private Bathroom", "With Workspace",
    "With FirePlace", "Entire Apartment", "Entire House", "Entire Villa", "Entire Cottage"
};

const float desc_prices[DESC_MAX_TYPES] = {
    0, 20, 50, 80, 65, 20, 35, 50, 65, 20, 15, 35, 25, 20, 45, 30, 25, 15, 20, 100, 150, 200, 100
};

int numListings = 0;

// Function prototypes
void displayMenu();
void addListing(BNB lis[]);
void getDescription(BNB lis[], int index);
void getAmenities(BNB lis[], int index);
void displayBnB(BNB lis[], int index);
void displayAll(BNB lis[]);
void findProperty(BNB lis[]);
void findIndex(BNB lis[]);
void findPrice(BNB lis[]);
float calculatePrice(BNB *listing);
void updateAllPrices(BNB lis[]);
void initializeDefaultListings(BNB lis[]);
void printColoredCentered(int color, const char* format, ...);
void printColored(int color, const char* format, ...);
int getConsoleWidth();

//---------------------------------------------------------------Start of Main------------------------------------------------------------------
int main() {
    BNB listing[MAX_LISTINGS];
    int choice;
    
    initializeDefaultListings(listing);
    numListings = 5;
    updateAllPrices(listing);

    do {
        system("cls");
        displayMenu();
        printColoredCentered(COLOR_CYAN, "Input choice: ");
        scanf("%d", &choice);
        fflush(stdin);

        switch (choice) {
            case 1: addListing(listing); updateAllPrices(listing); break;
            case 2: displayAll(listing); break;
            case 3: findProperty(listing); break;
            case 4: findIndex(listing); break;
            case 5: findPrice(listing); break;
            case 6: printColoredCentered(COLOR_CYAN,"Exiting Menu\n"); break;
            default: printColoredCentered(COLOR_RED,"Invalid choice\n");
        }
    } while (choice != 6);

    return 0;
}
//---------------------------------------------------------------end of main------------------------------------------------------------------

// Function to get console width
int getConsoleWidth() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void printColored(int color, const char* format, ...) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD originalAttrs = csbi.wAttributes;
    
    // Set color
    SetConsoleTextAttribute(hConsole, color);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    // Restore original color
    SetConsoleTextAttribute(hConsole, originalAttrs);
}
void printColoredCentered(int color, const char* format, ...) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    WORD originalAttrs = csbi.wAttributes;
    
    // Format string
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsprintf(buffer, format, args);
    va_end(args);
    
    int len = strlen(buffer);
    int padding = (columns - len) / 2;
    
    // Set color
    SetConsoleTextAttribute(hConsole, color);

    // Print padding and text
    if (padding > 0) {
        printf("%*s%s", padding, "", buffer);
    } else {
        printf("%s", buffer);
    }
    
    // Restore original color
    SetConsoleTextAttribute(hConsole, originalAttrs);
}

void displayMenu() {
    printColoredCentered(COLOR_BLUE, "AirBnB Listing Management\n");
    printColoredCentered(COLOR_CYAN, "==============================\n");
    printColoredCentered(COLOR_WHITE, " 1. Input Listing\n");
    printColoredCentered(COLOR_WHITE, " 2. Show Listing\n");
    printColoredCentered(COLOR_WHITE, " 3. Search property\n");
    printColoredCentered(COLOR_WHITE, " 4. Search Index\n");
    printColoredCentered(COLOR_WHITE, " 5. Search Price\n");
    printColoredCentered(COLOR_WHITE, " 6. Exit\n");
    printColoredCentered(COLOR_CYAN, "==============================\n\n");
}

void addListing(BNB lis[]) {
    int index = numListings;

    printColoredCentered(COLOR_CYAN, "-Enter Property Name: ");
    fgets(lis[index].propertyName, MAX_PROPERTY_NAME, stdin);
    lis[index].propertyName[strcspn(lis[index].propertyName, "\n")] = 0;

    printColoredCentered(COLOR_CYAN, "-Property Address:\n");
    printColoredCentered(COLOR_WHITE, "Enter city: ");
    fgets(lis[index].address.city, MAX_ADDRESS, stdin);
    lis[index].address.city[strcspn(lis[index].address.city, "\n")] = 0;
    
    printColoredCentered(COLOR_WHITE, "Enter street: ");
    fgets(lis[index].address.street, MAX_ADDRESS, stdin);
    lis[index].address.street[strcspn(lis[index].address.street, "\n")] = 0;
    
    printColoredCentered(COLOR_WHITE, "Enter barangay: ");
    fgets(lis[index].address.barangay, MAX_ADDRESS, stdin);
    lis[index].address.barangay[strcspn(lis[index].address.barangay, "\n")] = 0;
    fflush(stdin);
    
    printColoredCentered(COLOR_CYAN, "-Enter zip code: ");
    scanf("%d", &lis[index].address.zipCode);
    fflush(stdin);

    printColoredCentered(COLOR_CYAN, "-Enter Service fee: ");
    scanf("%f", &lis[index].serviceFee);
    fflush(stdin);
    
    // Get property descriptions
    getDescription(lis, index);
    
    // Get amenities
    getAmenities(lis, index);

    do {
        printColoredCentered(COLOR_CYAN, "-Enter Check-in Time (HH:MM e.g. 12:41): ");
        scanf("%d:%d", &lis[index].checkInTime.hour, &lis[index].checkInTime.minute);
        fflush(stdin);
        if((lis[index].checkInTime.hour > 24 || lis[index].checkInTime.hour < 0) || 
            (lis[index].checkInTime.minute > 59 || lis[index].checkInTime.minute < 0))
            printColoredCentered(COLOR_RED, "Invalid time. Please enter a valid 24-hour format time.\n");
    } while((lis[index].checkInTime.hour > 24 || lis[index].checkInTime.hour < 0) || 
            (lis[index].checkInTime.minute > 59 || lis[index].checkInTime.minute < 0));

    printColoredCentered(COLOR_CYAN, "-Enter Contact Number (e.g. +63 933 321 0265): ");
    fgets(lis[index].contactNumber, MAX_CONTACT_NO, stdin);
    lis[index].contactNumber[strcspn(lis[index].contactNumber, "\n")] = 0;

    printColoredCentered(COLOR_CYAN, "-Enter Property Owner: ");
    fgets(lis[index].propertyOwner, MAX_PROPERTY_NAME, stdin);
    lis[index].propertyOwner[strcspn(lis[index].propertyOwner, "\n")] = 0;
    
    // Calculate the price for the new listing
    lis[index].price = calculatePrice(&lis[index]);
    
    numListings++;
    printColoredCentered(COLOR_GREEN, "Listing added successfully! (Press any key to continue)\n");
    getch();
}

void getDescription(BNB lis[], int index) { 
    int i = 0, choice;
    char enterMore = ' ';

    printColoredCentered(COLOR_CYAN, "Available descriptions:\n");
    for (int j = 2; j < DESC_MAX_TYPES; j+=2) {
        printColoredCentered(COLOR_MAGENTA, "%-2d. %-22s (Price: %.2f)\t\t%-2d. %-22s (Price: %.2f)\n", 
                            j-1, descriptions[j-1], desc_prices[j-1], j, descriptions[j], desc_prices[j]);
    }
    
    do {
        if (i >= 10) {
            printColoredCentered(COLOR_RED, "Maximum number of descriptions reached.\n");
            break;
        }
        
        printColoredCentered(COLOR_CYAN, "Enter description choice (1-%d): ", DESC_MAX_TYPES - 1);
        scanf("%d", &choice);
        fflush(stdin);
        
        if (choice >= 1 && choice < DESC_MAX_TYPES) {
            lis[index].propertyDescriptions[i++] = (DescriptionType)choice;
            printColoredCentered(COLOR_GREEN, "Added: %s\n", descriptions[choice]);
        } else {
            printColoredCentered(COLOR_RED, "Invalid choice. Please enter a number between 1 and %d.\n", DESC_MAX_TYPES - 1);
        }
        
        if (i < 10) {
            do {
                printColoredCentered(COLOR_CYAN, "Enter more (Y/N)? ");
                scanf(" %c", &enterMore);
                enterMore = toupper(enterMore);
                
                if (!(enterMore == 'Y' || enterMore == 'N')) {
                    printColoredCentered(COLOR_RED, "Please enter Y or N only.\n");
                }
                
            } while(!(enterMore == 'Y' || enterMore == 'N'));
            
            fflush(stdin);
        }
    } while (i < 10 && toupper(enterMore) == 'Y');
    
    // Mark the end of descriptions with DESC_NONE
    if (i < 10) {
        lis[index].propertyDescriptions[i] = DESC_NONE;
    }
}

void getAmenities(BNB lis[], int index) {
    char response;
        
    printColoredCentered(COLOR_CYAN, "Does the property have hot shower (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isHotShower = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);

    printColoredCentered(COLOR_CYAN, "Does the property have AC? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isAC = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);

    printColoredCentered(COLOR_CYAN, "Does the property have Free Parking? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isFreeParking = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);

    printColoredCentered(COLOR_CYAN, "Does the property have Guest Supplies? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isGuestSuppliesAvail = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);
    
    printColoredCentered(COLOR_CYAN, "Does the property have Free Wifi? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isFreeWifi = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);
    
    printColoredCentered(COLOR_CYAN, "Is the property Animal friendly? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isAnimalFriendly = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);
    
    printColoredCentered(COLOR_CYAN, "Is the service Free? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isServiceFree = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);
    
    printColoredCentered(COLOR_CYAN, "Is Kitchen ware available? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isKitchenWareAvail = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);
    
    printColoredCentered(COLOR_CYAN, "Is washing machine available? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isWashingMachineAvail = (toupper(response) == 'Y') ? TRUE : FALSE;
    fflush(stdin);
}
            
void displayBnB(BNB lis[], int index) {
    int indent = 20;
    
    printColoredCentered(COLOR_BLUE, "Property Details\n");
    printColoredCentered(COLOR_YELLOW, "--- Property Listing %d ---\n\n", index + 1);
    
    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Name: ");
    printColored(COLOR_WHITE, "%s\n", lis[index].propertyName);

    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Address: ");
    printColored(COLOR_WHITE, "%s, %s, %s, %d\n", 
        lis[index].address.street, 
        lis[index].address.barangay, 
        lis[index].address.city, 
        lis[index].address.zipCode);
    
    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Descriptions: ");
    
    for (int i = 0; i < 10 && lis[index].propertyDescriptions[i] != DESC_NONE; i++) {
        printColored(COLOR_WHITE, "%s", descriptions[lis[index].propertyDescriptions[i]]);
        if (lis[index].propertyDescriptions[i+1] != DESC_NONE && i+1 < 10) {
            printColored(COLOR_WHITE, ", ");
        }
    }
    printf("\n");
    
    printColored(COLOR_CYAN, "%*s%-15s\n", indent, "", "Amenities:");
    
    if (lis[index].amenities.isHotShower) 
        printColored(COLOR_WHITE, "%*s- Hot Shower\n", indent + 2, "");
    if (lis[index].amenities.isAC) 
        printColored(COLOR_WHITE, "%*s- Air Conditioning\n", indent + 2, "");
    if (lis[index].amenities.isFreeParking) 
        printColored(COLOR_WHITE, "%*s- Free Parking\n", indent + 2, "");
    if (lis[index].amenities.isGuestSuppliesAvail) 
        printColored(COLOR_WHITE, "%*s- Guest Supplies Available\n", indent + 2, "");
    if (lis[index].amenities.isFreeWifi) 
        printColored(COLOR_WHITE, "%*s- Free WiFi\n", indent + 2, "");
    if (lis[index].amenities.isAnimalFriendly) 
        printColored(COLOR_WHITE, "%*s- Animal Friendly\n", indent + 2, "");
    if (lis[index].amenities.isServiceFree) 
        printColored(COLOR_WHITE, "%*s- Service Free\n", indent + 2, "");
    if (lis[index].amenities.isKitchenWareAvail) 
        printColored(COLOR_WHITE, "%*s- Kitchenware Available\n", indent + 2, "");
    if (lis[index].amenities.isWashingMachineAvail) 
        printColored(COLOR_WHITE, "%*s- Washing Machine Available\n", indent + 2, "");
    
    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Price: ");
    printColored(COLOR_GREEN, "$%.2f\n", lis[index].price);
    
    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Service Fee: ");
    printColored(COLOR_GREEN, "$%.2f\n", lis[index].serviceFee);
    
    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Check-in Time: ");
    printColored(COLOR_WHITE, "%02d:%02d\n", lis[index].checkInTime.hour, lis[index].checkInTime.minute);
    
    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Contact: ");
    printColored(COLOR_WHITE, "%s\n", lis[index].contactNumber);
    
    printColored(COLOR_CYAN, "%*s%-15s", indent, "", "Owner: ");
    printColored(COLOR_WHITE, "%s\n", lis[index].propertyOwner);
}

void displayAll(BNB lis[]) {
    int consoleWidth = getConsoleWidth();
    int tableWidth = 90;
    int padding = (consoleWidth - tableWidth) / 2;
    
    printColoredCentered(COLOR_BLUE, "All Property Listings\n");
    
    if (numListings == 0) {
        printColoredCentered(COLOR_RED, "No listings available.\n");
        printColoredCentered(COLOR_YELLOW, "(Press any key to continue)\n");
        getch();
        return;
    }

    printColored(COLOR_CYAN, "%*s%-3s %-30s %-30s %-15s\n", 
        padding, "", "ID", "Property Name", "Location", "Owner");
    
    printColored(COLOR_BLUE, "%*s%s\n", 
        padding, "", "--------------------------------------------------------------------------------");
    
    for (int i = 0; i < numListings; i++) {
        char location[100];
        sprintf(location, "%s, %s", lis[i].address.barangay, lis[i].address.city);
        
        printColored(COLOR_WHITE, "%*s%-3d %-30.30s %-30.30s %-15.15s\n", 
            padding, "", i+1, lis[i].propertyName, location, lis[i].propertyOwner);
    }
    
    printColoredCentered(COLOR_GREEN, "Displaying %d listing(s)\n", numListings);
    printColoredCentered(COLOR_YELLOW, "For detailed information, use \"3. Search Property\" or \"4. Search Index\".\n");
    printColoredCentered(COLOR_YELLOW, "(Press any key to continue)\n");
    getch();
}

void findProperty(BNB lis[]) {
    char searchName[MAX_PROPERTY_NAME];
    char propertyNameLower[MAX_PROPERTY_NAME];
    char searchNameLower[MAX_PROPERTY_NAME];
    int found = 0;
    
    printColoredCentered(COLOR_CYAN, "Enter property name to search: ");
    fgets(searchName, MAX_PROPERTY_NAME, stdin);
    searchName[strcspn(searchName, "\n")] = 0;
    
    strcpy(searchNameLower, searchName);
    for (int j = 0; searchNameLower[j]; j++) {
        searchNameLower[j] = tolower(searchNameLower[j]);
    }
    
    for (int i = 0; i < numListings; i++) {
        strcpy(propertyNameLower, lis[i].propertyName);
        for (int j = 0; propertyNameLower[j]; j++) {
            propertyNameLower[j] = tolower(propertyNameLower[j]);
        }
        
        if (strstr(propertyNameLower, searchNameLower) != NULL) {
            displayBnB(lis, i);
            found = 1;
        }
    }
    
    if (!found) {
        printColoredCentered(COLOR_RED, "No properties found matching '%s'.\n", searchName);
    }
    printColoredCentered(COLOR_YELLOW, "(Press any key to continue)\n");
    
    getch();
}

void findIndex(BNB lis[]) {
    int index;
    
    printColoredCentered(COLOR_CYAN, "Enter the Index of property: ");
    scanf("%d", &index);
    
    if (index < 1 || index > numListings) {
        printColoredCentered(COLOR_RED, "Invalid index. Please enter a number between 1 and %d.\n", numListings);
        printColoredCentered(COLOR_YELLOW, "(Press any key to continue)\n");
        getch();
        return;
    }
    
    displayBnB(lis, index - 1);
    printColoredCentered(COLOR_YELLOW, "(Press any key to continue)\n");
    getch();
}

void findPrice(BNB lis[]) {
    int found = 0;
    float minPrice, maxPrice;
    
    printColoredCentered(COLOR_CYAN, "Enter price Range to find:\n");
    printColoredCentered(COLOR_CYAN, "Minimum Price: ");
    scanf("%f", &minPrice);
    printColoredCentered(COLOR_CYAN, "Maximum Price: ");
    scanf("%f", &maxPrice);
    
    printColoredCentered(COLOR_GREEN, "Properties within price range $%.2f - $%.2f:\n", minPrice, maxPrice);
    
    for (int i = 0; i < numListings; i++) {
        if (lis[i].price >= minPrice && lis[i].price <= maxPrice) {
            displayBnB(lis, i);
            found = 1;
        }
    }
    
    if (!found){
        printColoredCentered(COLOR_RED, "No properties found in that price range.\n");
    }
    printColoredCentered(COLOR_YELLOW, "(Press any key to continue)\n");
    getch();
}

float calculatePrice(BNB *listing) {
    float total = 0.0;
    
    // Add prices for all descriptions
    for (int i = 0; i < 10 && listing->propertyDescriptions[i] != DESC_NONE; i++) {
        total += desc_prices[listing->propertyDescriptions[i]];
    }
    
    // Add prices for amenities
    if (listing->amenities.isHotShower) total += 10.0;
    if (listing->amenities.isAC) total += 15.0;
    if (listing->amenities.isFreeParking) total += 10.0;
    if (listing->amenities.isGuestSuppliesAvail) total += 5.0;
    if (listing->amenities.isFreeWifi) total += 5.0;
    if (listing->amenities.isAnimalFriendly) total += 5.0;
    if (listing->amenities.isKitchenWareAvail) total += 8.0;
    if (listing->amenities.isWashingMachineAvail) total += 10.0;
    
    // Apply discount if service is free
    if (listing->amenities.isServiceFree) total *= 0.70; // 30% discount
    
    return total;
}

void updateAllPrices(BNB lis[]) {
    for (int i = 0; i < numListings; i++) {
        lis[i].price = calculatePrice(&lis[i]);
    }
}
void initializeDefaultListings(BNB lis[]) {
    // Listing 1: Beachfront Villa
    lis[0] = (BNB){
        "Beachfront Villa Paradise", {"Cebu City", "Oceanview Drive", "Mactan", 6015},
        {KING_BED, QUEEN_BED, WITH_KITCHEN, WITH_POOL, WITH_OCEAN_VIEW, ENTIRE_VILLA, DESC_NONE},
        {TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE}, {14, 0},
        "+63 933 123 4567", "Maria Santos", 0, 50.00
    };
    // Listing 2: City Apartment
    lis[1] = (BNB){
        "Modern Downtown Apartment", {"Manila", "Bonifacio Avenue", "Poblacion", 1200},
        {QUEEN_BED, WITH_KITCHEN, WITH_BALCONY, WITH_WORKSPACE, ENTIRE_APARTMENT, DESC_NONE},
        {TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE}, {15, 30},
        "+63 917 555 8888", "Juan Dela Cruz", 0, 20.00
    };
    // Listing 3: Mountain Retreat
    lis[2] = (BNB){
        "Serene Mountain Cottage", {"Baguio", "Pine Tree Road", "Camp John Hay", 2600},
        {DOUBLE_BED, SINGLE_BED, WITH_KITCHEN, WITH_MOUNTAIN_VIEW, WITH_FIREPLACE, DESC_NONE},
        {TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE}, {13, 0}, "+63 915 987 6543",
        "Elena Reyes", 0, 25.00
    };
    // Listing 4: Budget Room
    lis[3] = (BNB){
        "Cozy Budget Room", {"Quezon City", "University Avenue", "Diliman", 1101},
        {SINGLE_BED, SINGLE_ROOM, WITH_PRIVATE_BATHROOM, DESC_NONE},
        {TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE}, {14, 0},
        "+63 922 111 2222", "Paolo Mendoza", 0, 5.00
    };
    // Listing 5: Luxury House
    lis[4] = (BNB){
        "Luxury Family House", {"Davao", "Palm Avenue", "Lanang", 8000},
        {KING_BED, DOUBLE_BED, WITH_GARDEN, WITH_POOL, WITH_KITCHEN, ENTIRE_HOUSE, DESC_NONE},
        {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE}, {12, 0},
        "+63 945 777 9999", "Rodrigo Torres", 0,  40.00
    };
}


