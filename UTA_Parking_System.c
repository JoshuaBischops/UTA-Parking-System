/*
 * =============================================================================
 * UTA PARKING RESERVATION SYSTEM
 * =============================================================================
 * PERSONS:
 *   Person 1 - User Account & Login System
 *   Person 2 - Reservation System
 *   Person 3 - Parking Lot & Availability Manager
 *   Person 4 - Simulation Engine
 *   Person 5 - Reports, Analytics & Integration
 * =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =============================================================================
 * CONSTANTS
 * =============================================================================
 */
#define MAX_USERS           100
#define MAX_LOTS            10
#define MAX_SPACES          500
#define MAX_RESERVATIONS    1000
#define MAX_NAME_LEN        50
#define MAX_ID_LEN          20
#define MAX_PASSWORD_LEN    30
#define MAX_TIME_WINDOWS    10
#define USER_FILE           "users.dat"
#define RESERVATION_FILE    "reservations.dat"
#define LOT_FILE            "lots.dat"

/* =============================================================================
 * ENUMS
 * =============================================================================
 */
typedef enum { ROLE_STUDENT = 0, ROLE_FACULTY, ROLE_STAFF } UserRole;
typedef enum { STATUS_ACTIVE = 0, STATUS_CANCELLED, STATUS_COMPLETED } ReservationStatus;
typedef enum { LOT_AVAILABLE = 0, LOT_FULL, LOT_CLOSED } LotStatus;

/* =============================================================================
 * STRUCTS
 * =============================================================================
 */
typedef struct {
    char userID[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    UserRole role;
    int isLoggedIn;
} User;

typedef struct {
    int startHour;
    int startMin;
    int endHour;
    int endMin;
} TimeWindow;

typedef struct {
    char lotID[MAX_ID_LEN];
    char lotName[MAX_NAME_LEN];
    int totalSpaces;
    int availableSpaces;
    TimeWindow windows[MAX_TIME_WINDOWS];
    int windowCount;
    LotStatus status;
} ParkingLot;

typedef struct {
    char reservationID[MAX_ID_LEN];
    char userID[MAX_ID_LEN];
    char lotID[MAX_ID_LEN];
    int spaceNumber;
    TimeWindow timeSlot;
    char date[12];
    ReservationStatus status;
} Reservation;

typedef struct {
    char lotID[MAX_ID_LEN];
    float avgUtilization;
    float peakUtilization;
    int peakHour;
    float avgWaitTime;
    int noShowCount;
    int totalSimulatedArrivals;
} SimulationResult;

typedef struct {
    char lotID[MAX_ID_LEN];
    int totalReservations;
    int totalCancellations;
    int totalNoShows;
    float utilizationRate;
    float noShowRate;
    int busiestHour;
} AnalyticsReport;

/* =============================================================================
 * GLOBAL STATE
 * =============================================================================
 */
User        users[MAX_USERS];
ParkingLot  lots[MAX_LOTS];
Reservation reservations[MAX_RESERVATIONS];

int userCount        = 0;
int lotCount         = 0;
int reservationCount = 0;

User *currentUser = NULL;

/* =============================================================================
 * FUNCTION DECLARATIONS
 * =============================================================================
 */
void   initUsers();
int    registerUser(char *name, char *userID, char *password, UserRole role);
User  *loginUser(char *userID, char *password);
void   logoutUser();
void   saveUsersToFile();
void   loadUsersFromFile();
void   displayUserMenu();

void   initReservations();
int    makeReservation(char *userID, char *lotID, TimeWindow slot, char *date);
int    cancelReservation(char *reservationID);
void   viewReservations(char *userID);
int    isDoubleBooked(char *userID, TimeWindow slot, char *date);
int    isLotSlotFull(char *lotID, TimeWindow slot, char *date);
void   saveReservationsToFile();
void   loadReservationsFromFile();

void   initLots();
int    addLot(char *lotID, char *lotName, int totalSpaces);
void   addTimeWindow(char *lotID, TimeWindow window);
int    updateLotAvailability(char *lotID, int delta);
void   showLotStatus();
int    getLotAvailableSpaces(char *lotID, TimeWindow slot, char *date);
void   saveLotsToFile();
void   loadLotsFromFile();

SimulationResult runSimulation(char *lotID, int numDays);
void   simulateParkingDemand(char *lotID);
void   modelArrivalsAndNoShows(char *lotID, float noShowRate);
void   testReservationPolicy(int policyType);
float  calculateWaitTime(char *lotID, TimeWindow slot);
float  calculateUtilization(char *lotID);

AnalyticsReport generateReport(char *lotID);
void   showSystemStats();
void   displayUtilizationTrends();
void   displayNoShowRate();
void   mergeAndTestSystem();
void   runFullSystemTest();

void   clearInputBuffer();
void   printDivider();
void   printHeader(char *title);
void   seedDefaultLots();
void   runWeeklySimulationReport();

/* =============================================================================
 * PERSON 1: USER ACCOUNT & LOGIN SYSTEM
 * =============================================================================
 */

void initUsers() {                                              /* Resets user count, clears current session, loads users from file */
    userCount = 0;
    currentUser = NULL;
    loadUsersFromFile();
}

int registerUser(char *name, char *userID, char *password, UserRole role) { /* Adds a new user — checks for duplicates, validates inputs, saves to file */
    if (userCount >= MAX_USERS) {
        printf("Error: User limit reached.\n");
        return 0;
    }
    /* Check for duplicate userID */
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userID, userID) == 0) {
            printf("Error: UserID '%s' already exists.\n", userID);
            return 0;
        }
    }
    /* Validate inputs are not empty */
    if (strlen(name) == 0 || strlen(userID) == 0 || strlen(password) == 0) {
        printf("Error: Name, userID, and password cannot be empty.\n");
        return 0;
    }
    strncpy(users[userCount].name,     name,     MAX_NAME_LEN - 1);
    strncpy(users[userCount].userID,   userID,   MAX_ID_LEN - 1);
    strncpy(users[userCount].password, password, MAX_PASSWORD_LEN - 1);
    users[userCount].role       = role;
    users[userCount].isLoggedIn = 0;
    userCount++;
    saveUsersToFile();
    printf("User '%s' registered successfully.\n", userID);
    return 1;
}

User *loginUser(char *userID, char *password) {                 /* Searches users[] for matching credentials, sets currentUser on success */
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userID, userID) == 0 &&
            strcmp(users[i].password, password) == 0) {
            users[i].isLoggedIn = 1;
            currentUser = &users[i];
            printf("Login successful. Welcome, %s!\n", users[i].name);
            return &users[i];
        }
    }
    printf("Error: Invalid userID or password.\n");
    return NULL;
}

void logoutUser() {                                             /* Clears the current session and sets currentUser back to NULL */
    if (currentUser != NULL) {
        currentUser->isLoggedIn = 0;
        printf("User '%s' logged out.\n", currentUser->userID);
        currentUser = NULL;
    }
}

void saveUsersToFile() {                                        /* Writes the entire users[] array to users.dat as binary */
    FILE *fp = fopen(USER_FILE, "wb");
    if (fp == NULL) {
        printf("Error: Cannot open %s for writing.\n", USER_FILE);
        return;
    }
    fwrite(&userCount, sizeof(int), 1, fp);
    fwrite(users, sizeof(User), userCount, fp);
    fclose(fp);
}

void loadUsersFromFile() {                                      /* Reads users.dat into users[] array, validates count before loading */
    FILE *fp = fopen(USER_FILE, "rb");
    if (fp == NULL) {
        userCount = 0;
        return;
    }
    int result = fread(&userCount, sizeof(int), 1, fp);
    if (result != 1 || userCount < 0 || userCount > MAX_USERS) {
        userCount = 0;
        fclose(fp);
        return;
    }
    int itemsRead = fread(users, sizeof(User), userCount, fp);
    if (itemsRead != userCount) {
        userCount = itemsRead;
    }
    fclose(fp);
    printf("Loaded %d users.\n", userCount);
}

void displayUserMenu() {                                        /* Post-login menu loop — options vary by role (student vs faculty/staff) */
    if (currentUser == NULL) {
        printf("Error: No user logged in.\n");
        return;
    }

    int choice = 0;
    while (1) {
        printHeader("USER MENU");
        printf("  Logged in as: %s (%s)\n\n", currentUser->name,
            currentUser->role == ROLE_STUDENT ? "Student" :
            currentUser->role == ROLE_FACULTY ? "Faculty" : "Staff");
        printf("  1. Make a reservation\n");
        printf("  2. Cancel a reservation\n");
        printf("  3. View my reservations\n");
        if (currentUser->role == ROLE_FACULTY || currentUser->role == ROLE_STAFF) {
            printf("  4. View lot status\n");
        }
        printf("  0. Logout\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 1) {
            char lotID[MAX_ID_LEN];
            char date[12];
            TimeWindow slot;
            printf("\nAvailable lots to reserve from:\n");
            showLotStatus();
            printf("Enter Lot ID: ");
            scanf("%s", lotID);
            clearInputBuffer();
            printf("Enter date (YYYY-MM-DD): ");
            scanf("%s", date);
            clearInputBuffer();
            do {
                printf("Enter start hour (7-22, e.g. 7=7am, 13=1pm, 22=10pm): ");
                scanf("%d", &slot.startHour);
                clearInputBuffer();
                if (slot.startHour < 7 || slot.startHour > 22)
                    printf("Invalid hour. Must be between 7 and 22.\n");
            } while (slot.startHour < 7 || slot.startHour > 22);

            do {
                printf("Enter start minute (0-59): ");
                scanf("%d", &slot.startMin);
                clearInputBuffer();
                if (slot.startMin < 0 || slot.startMin > 59)
                    printf("Invalid minute. Must be between 0 and 59.\n");
            } while (slot.startMin < 0 || slot.startMin > 59);

            do {
                printf("Enter end hour (7-22): ");
                scanf("%d", &slot.endHour);
                clearInputBuffer();
                if (slot.endHour < 7 || slot.endHour > 22)
                    printf("Invalid hour. Must be between 7 and 22.\n");
                else if (slot.endHour < slot.startHour)
                    printf("End hour cannot be before start hour.\n");
            } while (slot.endHour < 7 || slot.endHour > 22 || slot.endHour < slot.startHour);

            do {
                printf("Enter end minute (0-59): ");
                scanf("%d", &slot.endMin);
                clearInputBuffer();
                if (slot.endMin < 0 || slot.endMin > 59)
                    printf("Invalid minute. Must be between 0 and 59.\n");
            } while (slot.endMin < 0 || slot.endMin > 59);
            clearInputBuffer();
            makeReservation(currentUser->userID, lotID, slot, date);

        } else if (choice == 2) {
            char resID[MAX_ID_LEN];
            viewReservations(currentUser->userID);
            printf("Enter Reservation ID to cancel: ");
            scanf("%s", resID);
            clearInputBuffer();
            cancelReservation(resID);

        } else if (choice == 3) {
            viewReservations(currentUser->userID);

        } else if (choice == 4 &&
                   (currentUser->role == ROLE_FACULTY || currentUser->role == ROLE_STAFF)) {
            showLotStatus();

        } else if (choice == 0) {
            logoutUser();
            break;

        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* =============================================================================
 * PERSON 2: RESERVATION SYSTEM
 * =============================================================================
 */

void initReservations() {                                       /* Resets reservation count and loads existing reservations from file */
    reservationCount = 0;
    loadReservationsFromFile();
}

int makeReservation(char *userID, char *lotID, TimeWindow slot, char *date) { /* Creates a reservation — checks double booking and lot capacity first */
    if (reservationCount >= MAX_RESERVATIONS) {
        printf("Error: Reservation limit reached (%d).\n", MAX_RESERVATIONS);
        return 0;
    }
    if (isDoubleBooked(userID, slot, date)) {
        printf("You already have a reservation at this time!\n");
        return 0;
    }
    if (isLotSlotFull(lotID, slot, date)) {
        printf("Sorry, this lot is full for the selected time slot!\n");
        return 0;
    }

    char reservationID[MAX_ID_LEN];
    sprintf(reservationID, "RES_%d", reservationCount + 1);

    Reservation newReservation;
    strcpy(newReservation.reservationID, reservationID);
    strcpy(newReservation.userID, userID);
    strcpy(newReservation.lotID, lotID);
    newReservation.timeSlot    = slot;
    strcpy(newReservation.date, date);
    newReservation.status      = STATUS_ACTIVE;
    newReservation.spaceNumber = -1;

    reservations[reservationCount] = newReservation;
    reservationCount++;

    updateLotAvailability(lotID, -1);
    saveReservationsToFile();
    printf("Reservation %s created successfully.\n", reservationID);
    return 1;
}

int cancelReservation(char *reservationID) {                    /* Marks a reservation as cancelled and frees the space back to the lot */
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].reservationID, reservationID) == 0) {
            if (reservations[i].status == STATUS_ACTIVE) {
                reservations[i].status = STATUS_CANCELLED;
                updateLotAvailability(reservations[i].lotID, 1);
                saveReservationsToFile();
                printf("Reservation %s cancelled successfully.\n", reservationID);
                return 1;
            } else {
                printf("Reservation is not active.\n");
                return 0;
            }
        }
    }
    printf("Reservation %s not found.\n", reservationID);
    return 0;
}

void viewReservations(char *userID) {                           /* Prints all active reservations belonging to the given userID */
    int found = 0;
    printHeader("YOUR ACTIVE RESERVATIONS");
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].userID, userID) == 0 &&
            reservations[i].status == STATUS_ACTIVE) {
            found = 1;
            printf("  Reservation ID: %s\n",   reservations[i].reservationID);
            printf("  Lot:            %s\n",   reservations[i].lotID);
            printf("  Date:           %s\n",   reservations[i].date);
            printf("  Time:           %02d:%02d - %02d:%02d\n",
                reservations[i].timeSlot.startHour, reservations[i].timeSlot.startMin,
                reservations[i].timeSlot.endHour,   reservations[i].timeSlot.endMin);
            printf("  Space Number:   %d\n",   reservations[i].spaceNumber);
            printf("  Status:         ACTIVE\n");
            printDivider();
        }
    }
    if (!found)
        printf("No active reservations found.\n");
}

int isDoubleBooked(char *userID, TimeWindow slot, char *date) { /* Returns 1 if the user already has an overlapping reservation on that date */
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].userID, userID) == 0 &&
            strcmp(reservations[i].date, date) == 0 &&
            reservations[i].status == STATUS_ACTIVE) {
            TimeWindow existing = reservations[i].timeSlot;
            int newStart   = slot.startHour * 60 + slot.startMin;
            int newEnd     = slot.endHour   * 60 + slot.endMin;
            int existStart = existing.startHour * 60 + existing.startMin;
            int existEnd   = existing.endHour   * 60 + existing.endMin;
            if (newStart < existEnd && newEnd > existStart)
                return 1;
        }
    }
    return 0;
}

int isLotSlotFull(char *lotID, TimeWindow slot, char *date) {   /* Returns 1 if all spaces in the lot are taken for the given time slot */
    int totalSpaces = 0;
    int lotFound    = 0;
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) {
            totalSpaces = lots[i].totalSpaces;
            lotFound    = 1;
            break;
        }
    }
    if (!lotFound) return 1;

    int reservedCount = 0;
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].lotID,  lotID) != 0) continue;
        if (strcmp(reservations[i].date,   date)  != 0) continue;
        if (reservations[i].status != STATUS_ACTIVE)    continue;
        TimeWindow existing = reservations[i].timeSlot;
        int newStart   = slot.startHour * 60 + slot.startMin;
        int newEnd     = slot.endHour   * 60 + slot.endMin;
        int existStart = existing.startHour * 60 + existing.startMin;
        int existEnd   = existing.endHour   * 60 + existing.endMin;
        if (newStart < existEnd && newEnd > existStart)
            reservedCount++;
    }
    return (reservedCount >= totalSpaces);
}

void saveReservationsToFile() {                                 /* Writes the reservations[] array to reservations.dat as binary */
    FILE *fp = fopen(RESERVATION_FILE, "wb");
    if (fp == NULL) {
        printf("Error: Cannot open %s for writing.\n", RESERVATION_FILE);
        return;
    }
    fwrite(&reservationCount, sizeof(int), 1, fp);
    fwrite(reservations, sizeof(Reservation), reservationCount, fp);
    fclose(fp);
}

void loadReservationsFromFile() {                               /* Reads reservations.dat into reservations[], validates count on load */
    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (fp == NULL) {
        reservationCount = 0;
        return;
    }
    int result = fread(&reservationCount, sizeof(int), 1, fp);
    if (result != 1 || reservationCount < 0 || reservationCount > MAX_RESERVATIONS) {
        reservationCount = 0;
        fclose(fp);
        return;
    }
    int itemsRead = fread(reservations, sizeof(Reservation), reservationCount, fp);
    if (itemsRead != reservationCount) {
        printf("Warning: Only read %d of %d reservations.\n", itemsRead, reservationCount);
        reservationCount = itemsRead;
    }
    fclose(fp);
    printf("Loaded %d reservations.\n", reservationCount);
}

/* =============================================================================
 * PERSON 3: PARKING LOT & AVAILABILITY MANAGER
 * =============================================================================
 */

void initLots() {                                               /* Resets lot count, loads lots from file, seeds defaults on first run */
    lotCount = 0;
    loadLotsFromFile();
    /* Seed default lots if none exist (first run) */
    if (lotCount == 0)
        seedDefaultLots();
}

int addLot(char *lotID, char *lotName, int totalSpaces) {       /* Adds a new parking lot to lots[], sets available = total, status = AVAILABLE */
    if (lotCount >= MAX_LOTS) {
        printf("Error: Max lots reached.\n");
        return 0;
    }
    strncpy(lots[lotCount].lotID,    lotID,    MAX_ID_LEN - 1);
    strncpy(lots[lotCount].lotName,  lotName,  MAX_NAME_LEN - 1);
    lots[lotCount].totalSpaces     = totalSpaces;
    lots[lotCount].availableSpaces = totalSpaces;
    lots[lotCount].status          = LOT_AVAILABLE;
    lots[lotCount].windowCount     = 0;
    lotCount++;
    saveLotsToFile();
    printf("Lot '%s' added successfully.\n", lotID);
    return 1;
}

void addTimeWindow(char *lotID, TimeWindow window) {            /* Adds an operating time window to a specific lot's window list */
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) {
            if (lots[i].windowCount >= MAX_TIME_WINDOWS) {
                printf("No time windows available for lot %s.\n", lotID);
                return;
            }
            lots[i].windows[lots[i].windowCount] = window;
            lots[i].windowCount++;
            printf("Time window added to lot %s.\n", lotID);
            return;
        }
    }
    printf("Lot '%s' not found.\n", lotID);
}

int updateLotAvailability(char *lotID, int delta) {             /* Adjusts available spaces by delta (+1 = cancel, -1 = reserve), updates lot status */
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) {
            lots[i].availableSpaces += delta;
            if (lots[i].availableSpaces < 0)
                lots[i].availableSpaces = 0;
            if (lots[i].availableSpaces > lots[i].totalSpaces)
                lots[i].availableSpaces = lots[i].totalSpaces;
            lots[i].status = (lots[i].availableSpaces == 0) ? LOT_FULL : LOT_AVAILABLE;
            return 1;
        }
    }
    return 0;
}

void showLotStatus() {                                          /* Prints ID, name, total/available spaces, and status for every lot */
    printHeader("PARKING LOT STATUS");
    if (lotCount == 0) {
        printf("  No lots in system.\n");
        return;
    }
    for (int i = 0; i < lotCount; i++) {
        printf("  ID:        %s\n",  lots[i].lotID);
        printf("  Name:      %s\n",  lots[i].lotName);
        printf("  Total:     %d\n",  lots[i].totalSpaces);
        printf("  Available: %d\n",  lots[i].availableSpaces);
        printf("  Status:    %s\n",
            lots[i].status == LOT_AVAILABLE ? "AVAILABLE" :
            lots[i].status == LOT_FULL      ? "FULL"      : "CLOSED");
        printDivider();
    }
}

int getLotAvailableSpaces(char *lotID, TimeWindow slot, char *date) { /* Returns remaining spaces in a lot for a specific date/time window */
    int totalSpaces = 0;
    int lotFound    = 0;

    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) {
            totalSpaces = lots[i].totalSpaces;
            lotFound    = 1;
            break;
        }
    }
    if (!lotFound) return 0;

    int reserved = 0;
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].lotID, lotID) != 0) continue;
        if (strcmp(reservations[i].date,  date)  != 0) continue;
        if (reservations[i].status != STATUS_ACTIVE)   continue;
        TimeWindow existing = reservations[i].timeSlot;
        int newStart   = slot.startHour * 60 + slot.startMin;
        int newEnd     = slot.endHour   * 60 + slot.endMin;
        int existStart = existing.startHour * 60 + existing.startMin;
        int existEnd   = existing.endHour   * 60 + existing.endMin;
        if (newStart < existEnd && newEnd > existStart)
            reserved++;
    }
    return totalSpaces - reserved;
}

void saveLotsToFile() {                                         /* Writes the lots[] array to lots.dat as binary */
    FILE *fp = fopen(LOT_FILE, "wb");
    if (fp == NULL) {
        printf("Error: Cannot open %s for writing.\n", LOT_FILE);
        return;
    }
    fwrite(&lotCount, sizeof(int), 1, fp);
    fwrite(lots, sizeof(ParkingLot), lotCount, fp);
    fclose(fp);
}

void loadLotsFromFile() {                                       /* Reads lots.dat into lots[], validates count before loading */
    FILE *fp = fopen(LOT_FILE, "rb");
    if (fp == NULL) {
        lotCount = 0;
        return;
    }
    int result = fread(&lotCount, sizeof(int), 1, fp);
    if (result != 1 || lotCount < 0 || lotCount > MAX_LOTS) {
        lotCount = 0;
        fclose(fp);
        return;
    }
    int itemsRead = fread(lots, sizeof(ParkingLot), lotCount, fp);
    if (itemsRead != lotCount) {
        printf("Warning: Only read %d of %d lots.\n", itemsRead, lotCount);
        lotCount = itemsRead;
    }
    fclose(fp);
    printf("Loaded %d lots.\n", lotCount);
}

/* Seed real UTA parking lots on first run (when no lots.dat exists).
 * Based on actual UTA Arlington campus lots.
 * Only runs if lotCount == 0 after loadLotsFromFile(). */
void seedDefaultLots() {                                        /* Populates 5 real UTA lots on first run when no lots.dat exists */
    printf("No lots found. Seeding default UTA parking lots...\n");

    addLot("LOT_B",   "Lot B - College Park",        120);
    addLot("LOT_C",   "Lot C - Central Campus",       80);
    addLot("LOT_E",   "Lot E - Engineering",           60);
    addLot("LOT_G",   "Lot G - Greek Row",             50);
    addLot("GARAGE1", "College Park Garage",          400);

    /* Add standard operating hours to each lot (7am - 10pm) */
    TimeWindow stdHours = {7, 0, 22, 0};
    addTimeWindow("LOT_B",   stdHours);
    addTimeWindow("LOT_C",   stdHours);
    addTimeWindow("LOT_E",   stdHours);
    addTimeWindow("LOT_G",   stdHours);
    addTimeWindow("GARAGE1", stdHours);

    printf("Default lots seeded.\n");
}

/* =============================================================================
 * PERSON 4: SIMULATION ENGINE
 * =============================================================================
 */

SimulationResult runSimulation(char *lotID, int numDays) {      /* Runs a multi-day simulation for a lot, returns utilization and arrival stats */
    SimulationResult result;
    memset(&result, 0, sizeof(SimulationResult));
    strncpy(result.lotID, lotID, MAX_ID_LEN - 1);

    /* Find the lot */
    int lotIndex = -1;
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) { lotIndex = i; break; }
    }
    if (lotIndex == -1) {
        printf("Simulation error: Lot '%s' not found.\n", lotID);
        return result;
    }

    srand((unsigned int)time(NULL));
    float totalUtil = 0.0f;
    float peakUtil  = 0.0f;
    int   peakHour  = 0;
    int   totalArrivals = 0;
    int   totalNoShows  = 0;

    /* Simulate each day */
    for (int day = 0; day < numDays; day++) {
        /* Simulate each hour */
        for (int hour = 0; hour < 24; hour++) {
            /* Demand peaks at 8am, 12pm, 5pm */
            float demandFactor = 0.1f;
            if (hour >= 8  && hour <= 9)  demandFactor = 0.9f;
            else if (hour >= 11 && hour <= 13) demandFactor = 0.7f;
            else if (hour >= 16 && hour <= 18) demandFactor = 0.8f;
            else if (hour >= 7  && hour <= 19) demandFactor = 0.4f;

            int arrivals = (int)(lots[lotIndex].totalSpaces * demandFactor *
                                 ((float)(rand() % 40 + 80) / 100.0f));
            int noShows  = (int)(arrivals * 0.1f); /* 10% no-show rate */
            totalArrivals += arrivals;
            totalNoShows  += noShows;

            float util = (float)(arrivals - noShows) / lots[lotIndex].totalSpaces;
            if (util > 1.0f) util = 1.0f;
            totalUtil += util;
            if (util > peakUtil) { peakUtil = util; peakHour = hour; }
        }
    }

    result.avgUtilization       = totalUtil / (numDays * 24);
    result.peakUtilization      = peakUtil;
    result.peakHour             = peakHour;
    result.avgWaitTime          = result.avgUtilization * 15.0f; /* rough estimate: max 15 min */
    result.noShowCount          = totalNoShows;
    result.totalSimulatedArrivals = totalArrivals;

    printf("\n--- Simulation Results for Lot %s (%d days) ---\n", lotID, numDays);
    printf("  Avg Utilization:  %.1f%%\n", result.avgUtilization  * 100);
    printf("  Peak Utilization: %.1f%% at hour %d\n", result.peakUtilization * 100, result.peakHour);
    printf("  Avg Wait Time:    %.1f min\n", result.avgWaitTime);
    printf("  Total Arrivals:   %d\n", result.totalSimulatedArrivals);
    printf("  No-Shows:         %d\n", result.noShowCount);
    return result;
}

void simulateParkingDemand(char *lotID) {                       /* Prints an hourly demand table for a lot with peak-hour modeling */
    printf("\n--- Demand Simulation for Lot %s ---\n", lotID);
    int lotIndex = -1;
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) { lotIndex = i; break; }
    }
    if (lotIndex == -1) { printf("Lot not found.\n"); return; }

    srand((unsigned int)time(NULL));
    printf("  Hour | Arrivals | Utilization\n");
    printDivider();
    for (int hour = 7; hour <= 20; hour++) {
        float demandFactor = 0.2f;
        if (hour >= 8  && hour <= 9)  demandFactor = 0.9f;
        else if (hour >= 11 && hour <= 13) demandFactor = 0.7f;
        else if (hour >= 16 && hour <= 18) demandFactor = 0.8f;
        else demandFactor = 0.4f;

        int arrivals = (int)(lots[lotIndex].totalSpaces * demandFactor);
        float util   = (float)arrivals / lots[lotIndex].totalSpaces * 100.0f;
        printf("  %4d | %8d | %8.1f%%\n", hour, arrivals, util);
    }
}

void modelArrivalsAndNoShows(char *lotID, float noShowRate) {   /* Applies a no-show rate to active reservations and prints expected arrivals */
    int lotIndex = -1;
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) { lotIndex = i; break; }
    }
    if (lotIndex == -1) { printf("Lot not found.\n"); return; }
    if (noShowRate < 0.0f) noShowRate = 0.0f;
    if (noShowRate > 1.0f) noShowRate = 1.0f;

    int totalReserved = 0;
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].lotID, lotID) == 0 &&
            reservations[i].status == STATUS_ACTIVE)
            totalReserved++;
    }
    int noShows  = (int)(totalReserved * noShowRate);
    int arrivals = totalReserved - noShows;
    printf("\n--- Arrival Model for Lot %s ---\n", lotID);
    printf("  Total Reserved:  %d\n", totalReserved);
    printf("  No-Show Rate:    %.0f%%\n", noShowRate * 100);
    printf("  Expected Arrivals: %d\n", arrivals);
    printf("  Expected No-Shows: %d\n", noShows);
}

void testReservationPolicy(int policyType) {                    /* Prints behavior description for a given policy (0=FCFS, 1=role priority, 2=time slots) */
    printHeader("RESERVATION POLICY TEST");
    switch (policyType) {
        case 0:
            printf("  Policy: First Come, First Served\n");
            printf("  Reservations filled in order of creation.\n");
            printf("  Total reservations in system: %d\n", reservationCount);
            break;
        case 1:
            printf("  Policy: Priority by Role (Faculty > Staff > Student)\n");
            int faculty = 0, staff = 0, students = 0;
            for (int i = 0; i < userCount; i++) {
                if (users[i].role == ROLE_FACULTY) faculty++;
                else if (users[i].role == ROLE_STAFF) staff++;
                else students++;
            }
            printf("  Faculty users: %d | Staff: %d | Students: %d\n",
                faculty, staff, students);
            break;
        case 2:
            printf("  Policy: Time-Based Slots Only\n");
            printf("  Only reservations within defined lot time windows accepted.\n");
            break;
        default:
            printf("  Unknown policy type: %d\n", policyType);
    }
}

float calculateWaitTime(char *lotID, TimeWindow slot) {         /* Estimates wait time in minutes based on how full the lot is at that slot */
    int available = getLotAvailableSpaces(lotID, slot, "");
    int lotIndex  = -1;
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) { lotIndex = i; break; }
    }
    if (lotIndex == -1 || lots[lotIndex].totalSpaces == 0) return 0.0f;

    float util = 1.0f - ((float)available / lots[lotIndex].totalSpaces);
    /* Wait scales from 0 min at 0% full to 30 min at 100% full */
    return util * 30.0f;
}

float calculateUtilization(char *lotID) {                       /* Returns utilization as 0.0-1.0 — (total - available) / total spaces */
    for (int i = 0; i < lotCount; i++) {
        if (strcmp(lots[i].lotID, lotID) == 0) {
            if (lots[i].totalSpaces == 0) return 0.0f;
            return (float)(lots[i].totalSpaces - lots[i].availableSpaces) /
                           lots[i].totalSpaces;
        }
    }
    return 0.0f;
}

/* =============================================================================
 * PERSON 5: REPORTS, ANALYTICS & INTEGRATION
 * =============================================================================
 */

AnalyticsReport generateReport(char *lotID) {                   /* Builds an analytics report for a lot — totals, cancellations, busiest hour */
    AnalyticsReport report;
    memset(&report, 0, sizeof(AnalyticsReport));
    strncpy(report.lotID, lotID, MAX_ID_LEN - 1);

    int hourCounts[24] = {0};

    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].lotID, lotID) != 0) continue;
        report.totalReservations++;
        if (reservations[i].status == STATUS_CANCELLED)
            report.totalCancellations++;
        /* Track busiest start hour */
        int h = reservations[i].timeSlot.startHour;
        if (h >= 0 && h < 24) hourCounts[h]++;
    }

    /* Find busiest hour */
    int maxCount = 0;
    for (int h = 0; h < 24; h++) {
        if (hourCounts[h] > maxCount) { maxCount = hourCounts[h]; report.busiestHour = h; }
    }

    report.utilizationRate = calculateUtilization(lotID);
    report.noShowRate = (report.totalReservations > 0)
        ? (float)report.totalCancellations / report.totalReservations
        : 0.0f;

    return report;
}

void showSystemStats() {                                        /* Prints system-wide totals — users, lots, reservations by status, avg utilization */
    printHeader("SYSTEM STATISTICS");
    printf("  Total Users:        %d\n", userCount);
    printf("  Total Lots:         %d\n", lotCount);
    printf("  Total Reservations: %d\n", reservationCount);

    int active = 0, cancelled = 0, completed = 0;
    for (int i = 0; i < reservationCount; i++) {
        if (reservations[i].status == STATUS_ACTIVE)    active++;
        if (reservations[i].status == STATUS_CANCELLED) cancelled++;
        if (reservations[i].status == STATUS_COMPLETED) completed++;
    }
    printf("  Active:             %d\n", active);
    printf("  Cancelled:          %d\n", cancelled);
    printf("  Completed:          %d\n", completed);

    float totalUtil = 0.0f;
    for (int i = 0; i < lotCount; i++)
        totalUtil += calculateUtilization(lots[i].lotID);
    if (lotCount > 0)
        printf("  Avg Lot Utilization: %.1f%%\n", (totalUtil / lotCount) * 100);
}

void displayUtilizationTrends() {                               /* Shows utilization rate per lot — spaces taken vs total */
    printHeader("UTILIZATION TRENDS BY LOT");
    for (int i = 0; i < lotCount; i++) {
        float util = calculateUtilization(lots[i].lotID) * 100.0f;
        printf("  %-15s | %5.1f%% utilized | %d/%d spaces taken\n",
            lots[i].lotID,
            util,
            lots[i].totalSpaces - lots[i].availableSpaces,
            lots[i].totalSpaces);
    }
}

void displayNoShowRate() {                                      /* Prints cancellation rate per lot as a proxy for no-shows */
    printHeader("NO-SHOW / CANCELLATION RATE BY LOT");
    for (int i = 0; i < lotCount; i++) {
        AnalyticsReport r = generateReport(lots[i].lotID);
        printf("  Lot %-10s | Total: %3d | Cancelled: %3d | Rate: %.1f%%\n",
            lots[i].lotID,
            r.totalReservations,
            r.totalCancellations,
            r.noShowRate * 100.0f);
    }
}

void mergeAndTestSystem() {                                     /* Prints current state of all global counters as an integration check */
    printHeader("INTEGRATION CHECK");
    printf("  Users loaded:        %d\n", userCount);
    printf("  Lots loaded:         %d\n", lotCount);
    printf("  Reservations loaded: %d\n", reservationCount);
    printf("  currentUser:         %s\n", currentUser ? currentUser->userID : "None");
    printf("  All modules initialized and sharing global state.\n");
}

void runFullSystemTest() {                                      /* 10-step automated test — register, login, reserve, cancel, simulate, report */
    printHeader("FULL SYSTEM TEST");
    int pass = 0, fail = 0;

    /* Step 1: Register a test user */
    int r = registerUser("Test Student", "TST001", "pass123", ROLE_STUDENT);
    printf("  [%s] Register user\n", r ? "PASS" : "FAIL"); r ? pass++ : fail++;

    /* Step 2: Register duplicate (should fail) */
    r = registerUser("Test Student", "TST001", "pass123", ROLE_STUDENT);
    printf("  [%s] Reject duplicate userID\n", !r ? "PASS" : "FAIL"); !r ? pass++ : fail++;

    /* Step 3: Add a lot */
    r = addLot("LOT_T1", "Test Lot 1", 5);
    printf("  [%s] Add lot\n", r ? "PASS" : "FAIL"); r ? pass++ : fail++;

    /* Step 4: Login */
    User *u = loginUser("TST001", "pass123");
    printf("  [%s] Login\n", u ? "PASS" : "FAIL"); u ? pass++ : fail++;

    /* Step 5: Make a reservation */
    TimeWindow slot = {9, 0, 10, 0};
    r = makeReservation("TST001", "LOT_T1", slot, "2026-05-01");
    printf("  [%s] Make reservation\n", r ? "PASS" : "FAIL"); r ? pass++ : fail++;

    /* Step 6: Detect double booking */
    r = makeReservation("TST001", "LOT_T1", slot, "2026-05-01");
    printf("  [%s] Detect double booking\n", !r ? "PASS" : "FAIL"); !r ? pass++ : fail++;

    /* Step 7: Cancel reservation */
    r = cancelReservation("RES_1");
    printf("  [%s] Cancel reservation\n", r ? "PASS" : "FAIL"); r ? pass++ : fail++;

    /* Step 8: Run simulation */
    SimulationResult sim = runSimulation("LOT_T1", 1);
    printf("  [%s] Run simulation\n", sim.totalSimulatedArrivals > 0 ? "PASS" : "FAIL");
    sim.totalSimulatedArrivals > 0 ? pass++ : fail++;

    /* Step 9: Generate report */
    AnalyticsReport rep = generateReport("LOT_T1");
    printf("  [%s] Generate report\n", strlen(rep.lotID) > 0 ? "PASS" : "FAIL");
    strlen(rep.lotID) > 0 ? pass++ : fail++;

    /* Step 10: Logout */
    logoutUser();
    printf("  [%s] Logout\n", currentUser == NULL ? "PASS" : "FAIL");
    currentUser == NULL ? pass++ : fail++;

    printDivider();
    printf("  Results: %d passed, %d failed\n", pass, fail);
}

/* =============================================================================
 * SHARED UTILITIES
 * =============================================================================
 */
void clearInputBuffer() {                                       /* Flushes leftover characters from stdin after scanf */
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printDivider() {                                           /* Prints a row of dashes as a visual separator */
    printf("------------------------------------------------------------\n");
}

void printHeader(char *title) {                                 /* Prints a titled section header with dividers above and below */
    printDivider();
    printf("  %s\n", title);
    printDivider();
}

/* =============================================================================
 * WEEKLY SIMULATION REPORT
 * Simulates 7 days across all lots and prints a summary:
 *   - Per-day utilization table for each lot
 *   - Peak utilization hour across the week
 *   - Most popular lot by total simulated arrivals
 * =============================================================================
 */
void runWeeklySimulationReport() {                              /* Simulates 7 days across all lots, prints peak hours, most popular lot, bar chart */
    if (lotCount == 0) {
        printf("No lots in system to simulate.\n");
        return;
    }

    printHeader("UTA PARKING - WEEKLY SIMULATION REPORT");

    char *dayNames[] = { "Monday", "Tuesday", "Wednesday", "Thursday",
                         "Friday", "Saturday", "Sunday" };

    /* Demand factors per day (weekends quieter) */
    float dayDemand[] = { 0.9f, 0.85f, 0.9f, 0.85f, 0.8f, 0.35f, 0.2f };

    /* Peak hours to check each day */
    int peakHours[] = { 8, 9, 11, 12, 13, 16, 17, 18 };
    int numPeakHours = 8;

    srand((unsigned int)time(NULL));

    /* Track most popular lot */
    int lotTotalArrivals[MAX_LOTS] = {0};

    /* Track overall peak */
    float overallPeakUtil   = 0.0f;
    int   overallPeakHour   = 0;
    char  overallPeakDay[20] = "";
    char  overallPeakLot[MAX_ID_LEN] = "";

    for (int l = 0; l < lotCount; l++) {
        printf("\n  Lot: %s (%s)\n", lots[l].lotID, lots[l].lotName);
        printf("  %-12s | %-10s | %-10s | %-8s\n",
               "Day", "Arrivals", "Peak Util", "Peak Hr");
        printDivider();

        for (int d = 0; d < 7; d++) {
            int dayArrivals  = 0;
            float dayPeakUtil = 0.0f;
            int   dayPeakHour = 0;

            for (int h = 0; h < numPeakHours; h++) {
                int hour = peakHours[h];

                /* Scale demand by day and hour position */
                float hourFactor = 0.5f;
                if (hour >= 8  && hour <= 9)  hourFactor = 1.0f;
                else if (hour >= 11 && hour <= 13) hourFactor = 0.8f;
                else if (hour >= 16 && hour <= 18) hourFactor = 0.85f;

                int arrivals = (int)(lots[l].totalSpaces * dayDemand[d] *
                                     hourFactor *
                                     ((float)(rand() % 20 + 90) / 100.0f));
                if (arrivals > lots[l].totalSpaces)
                    arrivals = lots[l].totalSpaces;

                float util = (float)arrivals / lots[l].totalSpaces;

                dayArrivals += arrivals;
                if (util > dayPeakUtil) {
                    dayPeakUtil = util;
                    dayPeakHour = hour;
                }
            }

            lotTotalArrivals[l] += dayArrivals;

            /* Check overall peak */
            if (dayPeakUtil > overallPeakUtil) {
                overallPeakUtil = dayPeakUtil;
                overallPeakHour = dayPeakHour;
                strncpy(overallPeakDay, dayNames[d], 19);
                strncpy(overallPeakLot, lots[l].lotID, MAX_ID_LEN - 1);
            }

            printf("  %-12s | %-10d | %8.1f%%  | %d:00\n",
                   dayNames[d], dayArrivals,
                   dayPeakUtil * 100.0f, dayPeakHour);
        }
        printDivider();
        printf("  Week Total Arrivals: %d\n", lotTotalArrivals[l]);
    }

    /* Find most popular lot */
    int maxArrivals  = 0;
    int popularIndex = 0;
    for (int l = 0; l < lotCount; l++) {
        if (lotTotalArrivals[l] > maxArrivals) {
            maxArrivals  = lotTotalArrivals[l];
            popularIndex = l;
        }
    }

    /* Summary */
    printHeader("WEEKLY SUMMARY");
    printf("  Most Popular Lot:  %s (%s) — %d total simulated arrivals\n",
           lots[popularIndex].lotID,
           lots[popularIndex].lotName,
           maxArrivals);
    printf("  Peak Utilization:  %.1f%% on %s at %d:00 in lot %s\n",
           overallPeakUtil * 100.0f,
           overallPeakDay,
           overallPeakHour,
           overallPeakLot);

    /* Utilization bar chart per lot */
    printf("\n  Utilization Overview (weekly avg):\n\n");
    for (int l = 0; l < lotCount; l++) {
        float avgUtil = (float)lotTotalArrivals[l] /
                        (lots[l].totalSpaces * 7 * numPeakHours);
        if (avgUtil > 1.0f) avgUtil = 1.0f;
        int bars = (int)(avgUtil * 30);
        printf("  %-10s [", lots[l].lotID);
        for (int b = 0; b < 30; b++)
            printf(b < bars ? "#" : "-");
        printf("] %5.1f%%\n", avgUtil * 100.0f);
    }
}

/* =============================================================================
 * MAIN
 * =============================================================================
 */
int main() {                                                    /* Entry point — initializes all modules, runs the main login/register menu loop */
    initUsers();
    initLots();
    initReservations();

    printHeader("UTA PARKING RESERVATION SYSTEM");

    int choice = 0;
    while (1) {
        printf("\n  1. Login\n");
        printf("  2. Register\n");
        printf("  3. Run Full System Test\n");
        printf("  4. View Weekly Parking Simulation\n");
        printf("  0. Exit\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 1) {
            char userID[MAX_ID_LEN], password[MAX_PASSWORD_LEN];
            printf("UserID: ");   scanf("%s", userID);   clearInputBuffer();
            printf("Password: "); scanf("%s", password); clearInputBuffer();
            if (loginUser(userID, password) != NULL)
                displayUserMenu();

        } else if (choice == 2) {
            char name[MAX_NAME_LEN], userID[MAX_ID_LEN], password[MAX_PASSWORD_LEN];
            int role;
            printf("Name: ");     scanf("%s", name);     clearInputBuffer();
            printf("UserID: ");   scanf("%s", userID);   clearInputBuffer();
            printf("Password: "); scanf("%s", password); clearInputBuffer();
            printf("Role (0=Student, 1=Faculty, 2=Staff): ");
            scanf("%d", &role);   clearInputBuffer();
            registerUser(name, userID, password, (UserRole)role);

        } else if (choice == 3) {
            runFullSystemTest();

        } else if (choice == 4) {
            runWeeklySimulationReport();

        } else if (choice == 0) {
            printf("Goodbye.\n");
            break;

        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}
