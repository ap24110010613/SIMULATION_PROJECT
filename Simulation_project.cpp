// hotel.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ROOMS 5
#define NAME_LEN 100
#define PHONE_LEN 16
#define TYPE_LEN 20
#define INPUT_LEN 128

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN]; // phone acts as password
} Guest;

typedef struct {
    int number;
    char type[TYPE_LEN];
    int occupied; // 0 = false, 1 = true
    Guest guest;
} Room;

Room rooms[MAX_ROOMS];

/* Helpers */
void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n == 0) return;
    if (s[n-1] == '\n') s[n-1] = '\0';
}

/* lowercase copy into dest */
void to_lower_copy(const char *src, char *dest, size_t dest_size) {
    size_t i;
    for (i = 0; i + 1 < dest_size && src[i]; ++i)
        dest[i] = (char)tolower((unsigned char)src[i]);
    dest[i] = '\0';
}

int isValidPhone(const char *p) {
    if (strlen(p) != 10) return 0;
    for (int i = 0; i < 10; ++i)
        if (!isdigit((unsigned char)p[i])) return 0;
    return 1;
}

/* Initialize rooms */
void initRooms() {
    rooms[0].number = 101; strcpy(rooms[0].type, "Single"); rooms[0].occupied = 0;
    rooms[1].number = 102; strcpy(rooms[1].type, "Single"); rooms[1].occupied = 0;
    rooms[2].number = 103; strcpy(rooms[2].type, "Double"); rooms[2].occupied = 0;
    rooms[3].number = 104; strcpy(rooms[3].type, "Double"); rooms[3].occupied = 0;
    rooms[4].number = 105; strcpy(rooms[4].type, "Deluxe"); rooms[4].occupied = 0;
}

/* Show available */
void showAvailable() {
    printf("\nAvailable Rooms:\n");
    int any = 0;
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (!rooms[i].occupied) {
            printf(" Room %d (%s)\n", rooms[i].number, rooms[i].type);
            any = 1;
        }
    }
    if (!any) printf(" No rooms available.\n");
}

/* Show occupied rooms only */
void showOccupiedRoomsOnly() {
    printf("\nOccupied Rooms:\n");
    int any = 0;
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (rooms[i].occupied) {
            printf(" Room %d\n", rooms[i].number);
            any = 1;
        }
    }
    if (!any) printf(" No occupied rooms.\n");
}

/* Book a room */
void bookRoom() {
    char input[INPUT_LEN];
    printf("\n--- Book a Room ---\n");
    showAvailable();

    printf("Enter preferred type (Single/Double/Deluxe) or 'any': ");
    if (!fgets(input, sizeof input, stdin)) return;
    trim_newline(input);

    char lowered[INPUT_LEN];
    to_lower_copy(input, lowered, sizeof lowered);

    int index = -1;
    if (strcmp(lowered, "any") == 0) {
        for (int i = 0; i < MAX_ROOMS; ++i)
            if (!rooms[i].occupied) { index = i; break; }
    } else {
        for (int i = 0; i < MAX_ROOMS; ++i) {
            char room_type_lower[TYPE_LEN];
            to_lower_copy(rooms[i].type, room_type_lower, sizeof room_type_lower);
            if (!rooms[i].occupied && strcmp(room_type_lower, lowered) == 0) { index = i; break; }
        }
    }

    if (index == -1) {
        printf("No available room of that type.\n");
        return;
    }

    Guest g;
    printf("Enter your name: ");
    if (!fgets(g.name, sizeof g.name, stdin)) return;
    trim_newline(g.name);

    printf("Enter your phone number (10 digits): ");
    if (!fgets(g.phone, sizeof g.phone, stdin)) return;
    trim_newline(g.phone);

    if (!isValidPhone(g.phone)) {
        printf("Invalid phone number. Must be 10 digits.\n");
        return;
    }

    rooms[index].occupied = 1;
    rooms[index].guest = g;

    printf("\nRoom booked successfully!\n");
    printf("Your Room Number: %d\n", rooms[index].number);
    printf("Use your Phone Number to vacate.\n");
}

/* Vacate room */
void vacateRoom() {
    char input[INPUT_LEN];
    printf("\n--- Vacate (Checkout) ---\n");

    printf("Enter your room number: ");
    if (!fgets(input, sizeof input, stdin)) return;
    trim_newline(input);

    int roomNo = 0;
    char *endptr;
    roomNo = (int)strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') {
        printf("Invalid room number.\n");
        return;
    }

    int index = -1;
    for (int i = 0; i < MAX_ROOMS; ++i)
        if (rooms[i].number == roomNo && rooms[i].occupied)
            index = i;

    if (index == -1) {
        printf("This room is not occupied.\n");
        return;
    }

    printf("Enter your phone number : ");
    if (!fgets(input, sizeof input, stdin)) return;
    trim_newline(input);

    if (!isValidPhone(input)) {
        printf("Invalid phone number. Must be 10 digits.\n");
        return;
    }

    if (strcmp(input, rooms[index].guest.phone) != 0) {
        printf("Phone number does not match. Cannot vacate.\n");
        return;
    }

    rooms[index].occupied = 0;
    rooms[index].guest.name[0] = '\0';
    rooms[index].guest.phone[0] = '\0';
    printf("Room %d has been successfully vacated.\n", roomNo);
}

/* Main */
int main(void) {
    initRooms();
    char choice[INPUT_LEN];

    while (1) {
        printf("\n=== HOTEL RESERVATION SYSTEM ===\n");
        printf("1) Show Available Rooms\n");
        printf("2) Show Occupied Rooms\n");
        printf("3) Book a Room\n");
        printf("4) Vacate Room\n");
        printf("0) Exit\n");
        printf("Choose: ");

        if (!fgets(choice, sizeof choice, stdin)) break;
        trim_newline(choice);

        if (strcmp(choice, "1") == 0) showAvailable();
        else if (strcmp(choice, "2") == 0) showOccupiedRoomsOnly();
        else if (strcmp(choice, "3") == 0) bookRoom();
        else if (strcmp(choice, "4") == 0) vacateRoom();
        else if (strcmp(choice, "0") == 0) { printf("Thank you for visiting, visit again :) ..!\n"); break; }
        else printf("Invalid option. Try again.\n");
    }

    return 0;
}
