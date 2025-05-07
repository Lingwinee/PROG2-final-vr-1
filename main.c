
#include <stdio.h>
#include <ctype.h>

#define FALSE 0
#define TRUE 1

#define MAX_PROPERTY_NAME 50
#define MAX_CONTACT_NO 20

#define MAX_ADDRESS 50

typedef enum {
    DESC_NONE = 0,
    SINGLE_BED,
    DOUBLE_BED,
    SINGLE_ROOM,
    DOUBLE_ROOM,
    TRIPLE_ROOM,
    QUADRUPLE_ROOM,
    WITH_KITCHEN,

    DESC_MAX_TYPES
} DescriptionType;

// Define struct for AirBnB
typedef struct AirBnB {
    char propertyName[MAX_PROPERTY_NAME];

    struct address{
        char city[MAX_ADDRESS];
        char street[MAX_ADDRESS];
        char barangay[MAX_ADDRESS];
        int zipCode;
    };
    
    DescriptionType propertyDescriptions[10];

    struct amenities {
        int isHotShower,   isAC,
            isFreeParking, isGuestSuppliesAvail,
            isFreeWifi,    isAnimalFriendly,
            isServiceFree, isKitchenWareAvail,
            isFreeParking, isWashingMachineAvail;
    };
    struct checkInTime{
        int hour;
        int minute;
    }

    char contactNumber[MAX_CONTACT_NO];//format +63 933 321 0265
    char propertyOwner[MAX_PROPERTY_NAME];

    float price, serviceFee;
}BNB;

const char* descriptions[DESC_MAX_TYPES] = {
    "",
    "Single Bed",
    "Double Bed",
    "Single Room",
    "Double Room", 
    "Triple Room",
    "Quadruple Room",
    "With Kitchen"
};
const float desc_prices[DESC_MAX_TYPES] = {
    0,    // DESC_NONE
    20,   // SINGLE_BED
    50,   // DOUBLE_BED
    20,   // SINGLE_ROOM
    35,   // DOUBLE_ROOM
    50,   // TRIPLE_ROOM
    65,   // QUADRUPLE_ROOM
    20    // WITH_KITCHEN
};


// ------------------------------------------------------ func prototypes
void displayMenu();

BNB addListing();
void getDescription(BNB lis[], int index);
void getAmenities(BNB lis[]);
void findProperty(BNB lis[]);
void findprice(BNB lis[]);

void displayBnB(BNB lis[], int index);
void displayAll(BNB lis[]);






    
}

int main() {
    struct AirBnB listing[255];

    listing[0] = {"JoshProperty", {"1st street", "Linao", 6046}  };

    



    printf("Enter Amenities: ");
    fgets(listing.amenities, sizeof(listing.amenities), stdin);

    printf("Enter Price: ");
    scanf("%f", &listing.price);

    printf("Enter Service Fee: ");
    scanf("%f", &listing.serviceFee);
    getchar(); // To consume leftover newline character


    void showmenu(struct AirBnB listing){
    printf("\n--- Property Listing ---\n");
    printf("Name: %s", listing.propertyName);
    printf("Address: %s", listing.address);
    printf("Description: %s", listing.description);
    printf("Amenities: %s", listing.amenities);
    printf("Price: %.2f\n", listing.price);
    printf("Service Fee: %.2f\n", listing.serviceFee);
    printf("Check-in Time: %s", listing.checkInTime);
    printf("Contact Number: %s", listing.contactNumber);
    printf("Owner: %s", listing.propertyOwner);
    };
    return 0;
}
void displayMenu(){
	printf("-----AirBnB-----\n 1. Input Listing\n 2. Show Listing\n 3. Search property\n 4. Search Price\n 5. Exit\n");

}

void addlisting(BNB lis[]){
    int index = ;

    printf("Enter Property Name: ");
    fgets(listing.propertyName, sizeof(listing.propertyName), stdin);

    printf("Enter Address:\n");
    printf("Enter City: ");
    fgets(&lis[index].address.city, MAX_ADDRESS + 1, stdin);
    printf("Enter street: ");
    fgets(&lis[index].address.street, MAX_ADDRESS + 1, stdin);
    printf("Enter barangay: ");
    fgets(&lis[index].address.barangay, MAX_ADDRESS + 1, stdin);
    printf("Enter Zip Code: ");
    fflush(stdin);
    scanf("%d", &lis[index].address.zipCode)

    
	printf("Enter Check-in Time(HH:MM eg 12:41): ");
    scanf("%d:%d", &lis[index].CheckInTime.hour, &lis[index].CheckInTime.minute);

    printf("Enter Contact Number(eg +63 933 321 0265): ");
    fgets(&lis[index].contactNumber, MAX_CONTACT_NO + 1, stdin);

    printf("Enter Property Owner: ");
    fgets(&lis[index].propertyOwner, MAX_PROPERTY_NAME + 1, stdin);
	
};



void getDescription(BNB lis){
    int EnterMore = ' ';
    do{
        printf("Enter Description: ");
        fgets(lis.description, sizeof(lis.description), stdin);
        do{
            printf("Enter more(Y/N? ");
            scanf(" %c", &EnterMore);
        }while(!(toupper(EnterMore) == 'Y' || toupper(EnterMore) == 'N'));
    }while(toupper(EnterMore) == 'Y');
};
