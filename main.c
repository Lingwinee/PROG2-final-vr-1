#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
        char city[MAX_ADDRESS];
        char street[MAX_ADDRESS];
        char barangay[MAX_ADDRESS];
        int zipCode;
    } address;
    
    DescriptionType propertyDescriptions[10];

    struct {
        int isHotShower, isAC, isFreeParking, isGuestSuppliesAvail;
        int isFreeWifi, isAnimalFriendly, isServiceFree, isKitchenWareAvail;
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
    "", "Single Bed", "Double Bed", "King Bed", "Queen Bed", "Single Room",
    "Double Room", "Triple Room", "Quadruple Room", "With Kitchen",
    "With Balcony", "With Ocean View", "With Mountain View", "With Garden",
    "With Pool", "With Hot Tub", "With Private Bathroom", "With Workspace",
    "With FirePlace", "Entire Apartment", "Entire House", "Entire Villa", "Entire Cottage"
};

const float desc_prices[DESC_MAX_TYPES] = {
    0, 20, 50, 80, 65, 20, 35, 50, 65, 20, 15, 35, 25, 20, 45, 30, 25, 15, 20, 100, 150, 200, 120
};

// Global variable to track number of listings
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

int main() {
    BNB listing[MAX_LISTINGS];
    int choice;
    
    initializeDefaultListings(listing);
    numListings = 5;
    updateAllPrices(listing);

    do {
        displayMenu();
        printf("Input choice: ");
        scanf("%d", &choice);
        fflush(stdin);

        switch (choice) {
            case 1: addListing(listing); updateAllPrices(listing); break;
            case 2: displayAll(listing); break;
            case 3: findProperty(listing); break;
            case 4: findIndex(listing); break;
            case 5: findPrice(listing); break;
            case 6: printf("Exiting Menu\n"); break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 6);

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
    scanf("%f", &lis[index].serviceFee);
    fflush(stdin);
    
    // Get property descriptions
    getDescription(lis, index);
    
    // Get amenities
    getAmenities(lis, index);
    do{
        printf("Enter Check-in Time (HH:MM e.g. 12:41): ");
        scanf("%d:%d", &lis[index].checkInTime.hour, &lis[index].checkInTime.minute);
        fflush(stdin);
    }while((lis[index].checkInTime.hour > 24 || lis[index].checkInTime.hour < 0) || (lis[index].checkInTime.minute > 59 || lis[index].checkInTime.minute < 0));

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
    int i = 0, choice;
    char enterMore = ' ';

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
    const char* amenityNames[] = {
        "hot shower", "AC", "Free Parking", "Guest Supplies", "Free Wifi",
        "Animal friendly", "service Free", "Kitchen ware available", "washing machine available"
    };
    int* amenityValues[] = {
        &lis[index].amenities.isHotShower, &lis[index].amenities.isAC,
        &lis[index].amenities.isFreeParking, &lis[index].amenities.isGuestSuppliesAvail,
        &lis[index].amenities.isFreeWifi, &lis[index].amenities.isAnimalFriendly,
        &lis[index].amenities.isServiceFree, &lis[index].amenities.isKitchenWareAvail,
        &lis[index].amenities.isWashingMachineAvail
    };
    
    for (int i = 0; i < MAX_AMENITIES; i++) {
        printf("Does the property have %s (Y/N)? ", amenityNames[i]);
        scanf(" %c", &response);
        *amenityValues[i] = (toupper(response) == 'Y') ? TRUE : FALSE;
    }
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
            i+1, lis[i].propertyName, location, "P", lis[i].price, 
            "P", lis[i].serviceFee, checkInTime, lis[i].propertyOwner);
    }
    
    printf("\nDisplaying %d listing(s)\n", numListings);
    printf("For detailed information, use \"3. Search Property or 4. Search Index\".\n\n");
}

void findProperty(BNB lis[]) {
    char searchName[MAX_PROPERTY_NAME];
    int found = 0;
    
    printf("Enter property name to search: ");
    fgets(searchName, MAX_PROPERTY_NAME, stdin);
    searchName[strcspn(searchName, "\n")] = 0;
    
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
void findIndex(BNB lis[]) {
    int index;
    printf("Enter the Index of property: ");
    scanf("%d", &index);
    
    if (index < 1 || index > numListings) {
        printf("Invalid index. Please enter a number between 1 and %d.\n", numListings);
        return;
    }
    
    displayBnB(lis, index - 1);
}

void findPrice(BNB lis[]) {
    int found = 0;
    float minPrice, maxPrice;
    
    printf("Enter price Range to find:\nMinimum Price: ");
    scanf("%f", &minPrice);
    printf("Maximum Price: ");
    scanf("%f", &maxPrice);
    
    printf("\nProperties within price range %.2f - %.2f:\n", minPrice, maxPrice);
    for (int i = 0; i < numListings; i++) {
        if (lis[i].price >= minPrice && lis[i].price <= maxPrice) {
            displayBnB(lis, i);
            found = 1;
        }
    }
    
    if (!found) printf("No properties found in that price range.\n");
}

float calculatePrice(BNB *listing) {
    float total = 0.0;
    
    for (int i = 0; i < MAX_DESC && listing->propertyDescriptions[i] != DESC_NONE; i++) {
        total += desc_prices[listing->propertyDescriptions[i]];
    }
    
    if (listing->amenities.isHotShower) total += 10.0;
    if (listing->amenities.isAC) total += 15.0;
    if (listing->amenities.isFreeParking) total += 10.0;
    if (listing->amenities.isGuestSuppliesAvail) total += 5.0;
    if (listing->amenities.isFreeWifi) total += 5.0;
    if (listing->amenities.isAnimalFriendly) total += 5.0;
    if (listing->amenities.isKitchenWareAvail) total += 8.0;
    if (listing->amenities.isWashingMachineAvail) total += 10.0;
    
    if (listing->amenities.isServiceFree) total *= 0.95;
    
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
        {DOUBLE_BED, SINGLE_BED, WITH_KITCHEN, WITH_MOUNTAIN_VIEW, WITH_FIREPLACE, ENTIRE_COTTAGE, DESC_NONE},
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
