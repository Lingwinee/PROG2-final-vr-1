#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

#define MAX_PROPERTY_NAME 50
#define MAX_CONTACT_NO 20
#define MAX_ADDRESS 50
#define MAX_LISTINGS 255

#define MAX_AMENITIES 10

typedef enum {
    DESC_NONE = 0,
    SINGLE_BED,
    DOUBLE_BED,
    KING_BED,
    QUEEN_BED,
    SINGLE_ROOM,
    DOUBLE_ROOM,
    TRIPLE_ROOM,
    QUADRUPLE_ROOM,
    WITH_KITCHEN,
    WITH_BALCONY,
    WITH_OCEAN_VIEW,
    WITH_MOUNTAIN_VIEW,
    WITH_GARDEN,
    WITH_POOL,
    WITH_HOT_TUB,
    WITH_PRIVATE_BATHROOM,
    WITH_WORKSPACE,
    WITH_FIREPLACE,
    ENTIRE_APARTMENT,
    ENTIRE_HOUSE,
    ENTIRE_VILLA,
    ENTIRE_COTTAGE,
    
    DESC_MAX_TYPES
} DescriptionType;

// Define struct for AirBnB
typedef struct {
    char propertyName[MAX_PROPERTY_NAME];

    struct {
        char city[MAX_ADDRESS];
        char street[MAX_ADDRESS];
        char barangay[MAX_ADDRESS];
        int zipCode;
    } address;
    
    DescriptionType propertyDescriptions[10];

    struct {
        int isHotShower;
        int isAC;
        int isFreeParking;
        int isGuestSuppliesAvail;
        int isFreeWifi;
        int isAnimalFriendly;
        int isServiceFree;
        int isKitchenWareAvail;
        int isWashingMachineAvail;
    } amenities;
    
    struct {
        int hour;
        int minute;
    } checkInTime;

    char contactNumber[MAX_CONTACT_NO]; // format +63 933 321 0265
    char propertyOwner[MAX_PROPERTY_NAME];

    float price, serviceFee;
} BNB;

//price is calculated according to the quality of property, eg. description and amenities
const char* descriptions[DESC_MAX_TYPES] = {
    "",
    "Single Bed",
    "Double Bed",
    "King Bed",
    "Queen Bed",
    "Single Room",
    "Double Room", 
    "Triple Room",
    "Quadruple Room",
    "With Kitchen",
    "With Balcony",
    "With Ocean View",
    "With Mountain View",
    "With Garden",
    "With Pool",
    "With Hot Tub",
    "With Private Bathroom",
    "With Workspace",
    "With FirePlace",
    "Entire Apartment",
    "Entire House",
    "Entire Villa",
    "Entire Cottage"
};

const float desc_prices[DESC_MAX_TYPES] = {
    0,     // DESC_NONE
    20,    // SINGLE_BED
    50,    // DOUBLE_BED
    80,    // KING_BED
    65,    // QUEEN_BED
    20,    // SINGLE_ROOM
    35,    // DOUBLE_ROOM
    50,    // TRIPLE_ROOM
    65,    // QUADRUPLE_ROOM
    20,    // WITH_KITCHEN
    15,    // WITH_BALCONY
    35,    // WITH_OCEAN_VIEW
    25,    // WITH_MOUNTAIN_VIEW
    20,    // WITH_GARDEN
    45,    // WITH_POOL
    30,    // WITH_HOT_TUB
    25,    // WITH_PRIVATE_BATHROOM
    15,    // WITH_WORKSPACE
    20,    // WITH_FIREPLACE
    100,   // ENTIRE_APARTMENT
    150,   // ENTIRE_HOUSE
    200,   // ENTIRE_VILLA
    120    // ENTIRE_COTTAGE
};

// Global variable to track number of listings
int numListings = 0;

// ------------------------------------------------------ func prototypes
void displayMenu();
void addListing(BNB lis[]);
void getDescription(BNB lis[], int index);
void getAmenities(BNB lis[], int index);
void displayBnB(BNB lis[], int index);
void displayAll(BNB lis[]);
void findProperty(BNB lis[]);
void findIndex(BNB lis[]);
void findPrice(BNB lis[]);
void showMenu(BNB listing);

//automatically calculates price of all entries, everytime new entry is addded, including 5 default initialized
float calculatePrice(BNB *listing);
void updateAllPrices(BNB lis[]);

void initializeDefaultListings(BNB lis[]);

int main() {
    BNB listing[MAX_LISTINGS];
    int choice;
    
    // Initialize with 5 default listings
    initializeDefaultListings(listing);
    numListings = 5;
    
    // Calculate prices for default listings
    updateAllPrices(listing);

    do {
        displayMenu();
        printf("Input choice: ");
        scanf("%d", &choice);
        fflush(stdin);

        switch (choice) {
            case 1: 
                addListing(listing);
                updateAllPrices(listing);
                break;
            case 2:
                displayAll(listing);
                break;
            case 3:
                findProperty(listing);
                break;
            case 4:
                findIndex(listing);
                break;
            case 5:
                findPrice(listing);
                break;
            case 6:
                printf("Exiting Menu\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 6); // Fixed: Changed from 5 to 6 to match the menu

    return 0;
}

void displayMenu() {
    printf("-----AirBnB-----\n");
    printf(" 1. Input Listing\n");
    printf(" 2. Show Listing\n");
    printf(" 3. Search property\n");
    printf(" 4. Search Index\n");
    printf(" 5. Search Price\n");
    printf(" 6. Exit\n");
}


void addListing(BNB lis[]) {
    int index = numListings;

    printf("Enter Property Name: ");
    fgets(lis[index].propertyName, MAX_PROPERTY_NAME, stdin);
    lis[index].propertyName[strcspn(lis[index].propertyName, "\n")] = 0;

    printf("Enter Address:\n");
    printf("Enter City: ");
    fgets(lis[index].address.city, MAX_ADDRESS, stdin);
    lis[index].address.city[strcspn(lis[index].address.city, "\n")] = 0;
    
    printf("Enter street: ");
    fgets(lis[index].address.street, MAX_ADDRESS, stdin);
    lis[index].address.street[strcspn(lis[index].address.street, "\n")] = 0;
    
    printf("Enter barangay: ");
    fgets(lis[index].address.barangay, MAX_ADDRESS, stdin);
    lis[index].address.barangay[strcspn(lis[index].address.barangay, "\n")] = 0;
    fflush(stdin);
    
    printf("Enter Zip Code: ");
    scanf("%d", &lis[index].address.zipCode);
    fflush(stdin);

    printf("Enter Service Fee: ");
    scanf("%f", &lis[index].serviceFee); // Fixed: Changed %d to %f for float
    fflush(stdin);
    
    // Get property descriptions
    getDescription(lis, index);
    
    // Get amenities
    getAmenities(lis, index);
    
    printf("Enter Check-in Time (HH:MM e.g. 12:41): ");
    scanf("%d:%d", &lis[index].checkInTime.hour, &lis[index].checkInTime.minute);
    fflush(stdin);

    printf("Enter Contact Number (e.g. +63 933 321 0265): ");
    fgets(lis[index].contactNumber, MAX_CONTACT_NO, stdin);
    lis[index].contactNumber[strcspn(lis[index].contactNumber, "\n")] = 0;

    printf("Enter Property Owner: ");
    fgets(lis[index].propertyOwner, MAX_PROPERTY_NAME, stdin);
    lis[index].propertyOwner[strcspn(lis[index].propertyOwner, "\n")] = 0;
    
    // Calculate the price for the new listing
    lis[index].price = calculatePrice(&lis[index]);
    
    numListings++;
    printf("Listing added successfully!\n");
}

void getDescription(BNB lis[], int index) {
    int i = 0;
    char enterMore = ' ';
    int choice;
    
    printf("Available descriptions:\n");
    for (int j = 1; j < DESC_MAX_TYPES; j++) {
        printf("%d. %s (Price: %.2f)\n", j, descriptions[j], desc_prices[j]);
    }
    
    do {
        if (i >= 10) {
            printf("Maximum number of descriptions reached.\n");
            break;
        }
        
        printf("Enter description choice (1-%d): ", DESC_MAX_TYPES - 1);
        scanf("%d", &choice);
        fflush(stdin);
        
        if (choice >= 1 && choice < DESC_MAX_TYPES) {
            lis[index].propertyDescriptions[i++] = (DescriptionType)choice;
            printf("Added: %s\n", descriptions[choice]);
        } else {
            printf("Invalid choice.\n");
        }
        
        if (i < 10) {
            printf("Enter more (Y/N)? ");
            scanf(" %c", &enterMore);
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
        
    printf("Does the property have hot shower (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isHotShower = (toupper(response) == 'Y') ? TRUE : FALSE;
	
	printf("Does the property have AC? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isAC = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    printf("Does the property have Free Parking? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isFreeParking = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    printf("Does the property have Guest Supplies?  (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isGuestSuppliesAvail = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    printf("Does the property have Free Wifi? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isFreeWifi = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    printf("Is the property Animal friendly? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isAnimalFriendly = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    printf("Is the service Free? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isServiceFree = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    printf("Is Kitchen ware available? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isKitchenWareAvail = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    printf("Is washing machine available? (Y/N)? ");
    scanf(" %c", &response);
    lis[index].amenities.isWashingMachineAvail = (toupper(response) == 'Y') ? TRUE : FALSE;
    
    fflush(stdin);
}

void displayBnB(BNB lis[], int index) {
    printf("\n--- Property Listing %d ---\n", index + 1);
    printf("Name: %s\n", lis[index].propertyName);
    printf("Address: %s, %s, %s, %d\n", 
        lis[index].address.street, 
        lis[index].address.barangay, 
        lis[index].address.city, 
        lis[index].address.zipCode);
    
    printf("Descriptions: ");
    for (int i = 0; i < 10 && lis[index].propertyDescriptions[i] != DESC_NONE; i++) {
        printf("%s", descriptions[lis[index].propertyDescriptions[i]]);
        if (lis[index].propertyDescriptions[i+1] != DESC_NONE && i+1 < 10) {
            printf(", ");
        }
    }
    printf("\n");
    
    printf("Amenities:\n");
    if (lis[index].amenities.isHotShower) printf("- Hot Shower\n");
    if (lis[index].amenities.isAC) printf("- Air Conditioning\n");
    if (lis[index].amenities.isFreeParking) printf("- Free Parking\n");
    if (lis[index].amenities.isGuestSuppliesAvail) printf("- Guest Supplies Available\n");
    if (lis[index].amenities.isFreeWifi) printf("- Free WiFi\n");
    if (lis[index].amenities.isAnimalFriendly) printf("- Animal Friendly\n");
    if (lis[index].amenities.isServiceFree) printf("- Service Free\n");
    if (lis[index].amenities.isKitchenWareAvail) printf("- Kitchenware Available\n");
    if (lis[index].amenities.isWashingMachineAvail) printf("- Washing Machine Available\n");
    
    printf("Price: %.2f\n", lis[index].price);
    printf("Service Fee: %.2f\n", lis[index].serviceFee);
    printf("Check-in Time: %02d:%02d\n", lis[index].checkInTime.hour, lis[index].checkInTime.minute);
    printf("Contact Number: %s\n", lis[index].contactNumber);	
    printf("Owner: %s\n", lis[index].propertyOwner);
}

void displayAll(BNB lis[]) {
    if (numListings == 0) {
        printf("No listings available.\n");
        return;
    }
    
    // Print table header
    printf("\n%-3s %-30s %-30s %-15s %-10s %-10s %-10s\n", 
        "ID", "Property Name", "Location", "Price", "Service Fee", "Check-in", "Owner");
    printf("%-3s %-30s %-30s %-15s %-10s %-10s %-10s\n",
        "---", "------------------------------", "------------------------------", 
        "---------------", "----------", "----------", "----------");
    
    // Print each listing as a row
    for (int i = 0; i < numListings; i++) {
        char location[50];
        sprintf(location, "%s, %s", lis[i].address.barangay, lis[i].address.city);
        
        char checkInTime[10];
        sprintf(checkInTime, "%02d:%02d", lis[i].checkInTime.hour, lis[i].checkInTime.minute);
        
        printf("%-3d %-30.30s %-30.30s %-2s%-13.2f %-2s%-8.2f %-10s %-10.25s\n", 
            i+1, 
            lis[i].propertyName, 
            location,
            "P", lis[i].price, 
            "P", lis[i].serviceFee,
            checkInTime,
            lis[i].propertyOwner);
    }
    
    printf("\nDisplaying %d listing(s)\n", numListings);
    printf("For detailed information, use \"3. Search Property or 4. Search Index\".\n\n");
}

void findProperty(BNB lis[]) {
    char searchName[MAX_PROPERTY_NAME];
    int found = 0;
    
    printf("Enter property name to search: ");
    fgets(searchName, MAX_PROPERTY_NAME, stdin);
    searchName[strcspn(searchName, "\n")] = 0; // Remove newline
    
    for (int i = 0; i < numListings; i++) {
        if (strstr(lis[i].propertyName, searchName) != NULL) {
            displayBnB(lis, i);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No properties found matching '%s'.\n", searchName);
    }
}
void findIndex(BNB lis[]){
    int index;
    printf("Enter the Index of property: ");
    scanf("%d", &index);
    
    // Validate index
    if (index < 1 || index > numListings) {
        printf("Invalid index. Please enter a number between 1 and %d.\n", numListings);
        return;
    }
    
    displayBnB(lis, index - 1);
}

void findPrice(BNB lis[]) {
    int found = 0; // Fixed: Initialize found variable
    float minPrice, maxPrice;
    
    printf("Enter price Range to find:\n");
    printf("Minimum Price: ");
    scanf("%f", &minPrice);
    printf("Maximum Price: ");
    scanf("%f", &maxPrice);
    
    printf("\nProperties within price range %.2f - %.2f:\n", minPrice, maxPrice);
    for (int i = 0; i < numListings; i++) { // Fixed: Using numListings instead of count
        if (lis[i].price >= minPrice && lis[i].price <= maxPrice) {
            displayBnB(lis, i);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No properties found in that price range.\n");
    }
}

void initializeDefaultListings(BNB lis[]) {
    // Listing 1: Beachfront Villa
    lis[0] = (BNB){
        .propertyName = "Beachfront Villa Paradise",
        .address = {
            .city = "Cebu City",
            .street = "Oceanview Drive",
            .barangay = "Mactan",
            .zipCode = 6015
        },
        .propertyDescriptions = {KING_BED, QUEEN_BED, WITH_KITCHEN, WITH_POOL, WITH_OCEAN_VIEW, ENTIRE_VILLA, DESC_NONE},
        .amenities = {
            .isHotShower = TRUE,
            .isAC = TRUE,
            .isFreeParking = TRUE,
            .isGuestSuppliesAvail = TRUE,
            .isFreeWifi = TRUE,
            .isAnimalFriendly = FALSE,
            .isServiceFree = FALSE,
            .isKitchenWareAvail = TRUE,
            .isWashingMachineAvail = TRUE
        },
        .checkInTime = {14, 0},
        .contactNumber = "+63 933 123 4567",
        .propertyOwner = "Maria Santos",
        .price = 0,
        .serviceFee = 50.00
    };
    
    // Listing 2: City Apartment
    lis[1] = (BNB){
        .propertyName = "Modern Downtown Apartment",
        .address = {
            .city = "Manila",
            .street = "Bonifacio Avenue",
            .barangay = "Poblacion",
            .zipCode = 1200
        },
        .propertyDescriptions = {QUEEN_BED, WITH_KITCHEN, WITH_BALCONY, WITH_WORKSPACE, ENTIRE_APARTMENT, DESC_NONE},
        .amenities = {
            .isHotShower = TRUE,
            .isAC = TRUE,
            .isFreeParking = FALSE,
            .isGuestSuppliesAvail = TRUE,
            .isFreeWifi = TRUE,
            .isAnimalFriendly = FALSE,
            .isServiceFree = FALSE,
            .isKitchenWareAvail = TRUE,
            .isWashingMachineAvail = TRUE
        },
        .checkInTime = {15, 30},
        .contactNumber = "+63 917 555 8888",
        .propertyOwner = "Juan Dela Cruz",
        .price = 0,
        .serviceFee = 20.00
    };
    
    // Listing 3: Mountain Retreat
    lis[2] = (BNB){
        .propertyName = "Serene Mountain Cottage",
        .address = {
            .city = "Baguio",
            .street = "Pine Tree Road",
            .barangay = "Camp John Hay",
            .zipCode = 2600
        },
        .propertyDescriptions = {DOUBLE_BED, SINGLE_BED, WITH_KITCHEN, WITH_MOUNTAIN_VIEW, WITH_FIREPLACE, ENTIRE_COTTAGE, DESC_NONE},
        .amenities = {
            .isHotShower = TRUE,
            .isAC = FALSE,
            .isFreeParking = TRUE,
            .isGuestSuppliesAvail = TRUE,
            .isFreeWifi = TRUE,
            .isAnimalFriendly = TRUE,
            .isServiceFree = FALSE,
            .isKitchenWareAvail = TRUE,
            .isWashingMachineAvail = FALSE
        },
        .checkInTime = {13, 0},
        .contactNumber = "+63 915 987 6543",
        .propertyOwner = "Elena Reyes",
        .price = 0,
        .serviceFee = 25.00
    };
    
    // Listing 4: Budget Room
    lis[3] = (BNB){
        .propertyName = "Cozy Budget Room",
        .address = {
            .city = "Quezon City",
            .street = "University Avenue",
            .barangay = "Diliman",
            .zipCode = 1101
        },
        .propertyDescriptions = {SINGLE_BED, SINGLE_ROOM, WITH_PRIVATE_BATHROOM, DESC_NONE},
        .amenities = {
            .isHotShower = TRUE,
            .isAC = TRUE,
            .isFreeParking = FALSE,
            .isGuestSuppliesAvail = TRUE,
            .isFreeWifi = TRUE,
            .isAnimalFriendly = FALSE,
            .isServiceFree = TRUE,
            .isKitchenWareAvail = FALSE,
            .isWashingMachineAvail = FALSE
        },
        .checkInTime = {14, 0},
        .contactNumber = "+63 922 111 2222",
        .propertyOwner = "Paolo Mendoza",
        .price = 0,
        .serviceFee = 5.00
    };
    
    // Listing 5: Luxury House
    lis[4] = (BNB){
        .propertyName = "Luxury Family House",
        .address = {
            .city = "Davao",
            .street = "Palm Avenue",
            .barangay = "Lanang",
            .zipCode = 8000
        },
        .propertyDescriptions = {KING_BED, DOUBLE_BED, WITH_GARDEN, WITH_POOL, WITH_KITCHEN, ENTIRE_HOUSE, DESC_NONE},
        .amenities = {
            .isHotShower = TRUE,
            .isAC = TRUE,
            .isFreeParking = TRUE,
            .isGuestSuppliesAvail = TRUE,
            .isFreeWifi = TRUE,
            .isAnimalFriendly = TRUE,
            .isServiceFree = FALSE,
            .isKitchenWareAvail = TRUE,
            .isWashingMachineAvail = TRUE
        },
        .checkInTime = {12, 0},
        .contactNumber = "+63 945 777 9999",
        .propertyOwner = "Rodrigo Torres",
        .price = 0,
        .serviceFee = 40.00
    };
}

// Fixed: Implemented the calculatePrice function
float calculatePrice(BNB *listing) {
    float total = 0.0;
    
    // Add up all the description prices
    for (int i = 0; i < 10 && listing->propertyDescriptions[i] != DESC_NONE; i++) {
        total += desc_prices[listing->propertyDescriptions[i]];
    }
    
    // Add bonuses for amenities
    if (listing->amenities.isHotShower) total += 10.0;
    if (listing->amenities.isAC) total += 15.0;
    if (listing->amenities.isFreeParking) total += 10.0;
    if (listing->amenities.isGuestSuppliesAvail) total += 5.0;
    if (listing->amenities.isFreeWifi) total += 5.0;
    if (listing->amenities.isAnimalFriendly) total += 5.0;
    if (listing->amenities.isKitchenWareAvail) total += 8.0;
    if (listing->amenities.isWashingMachineAvail) total += 10.0;
    
    // If service is free, consider adding a discount
    if (listing->amenities.isServiceFree) {
        total *= 0.95; // 5% discount for free service
    }
    
    return total;
}

// Update prices for all listings
void updateAllPrices(BNB lis[]) {
    for (int i = 0; i < numListings; i++) {
        lis[i].price = calculatePrice(&lis[i]);
    }
}
