/*
 * =============================================================================
 * UTA PARKING RESERVATION SYSTEM
 * =============================================================================
 * This file is the master framework for the UTA Parking Project.
 * Each section is labeled by person. Add your code in the designated area.
 * DO NOT modify structs or function signatures without team approval.
 *
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

/* User roles */
typedef enum {
    ROLE_STUDENT = 0,
    ROLE_FACULTY,
    ROLE_STAFF
} UserRole;

/* Reservation status */
typedef enum {
    STATUS_ACTIVE = 0,
    STATUS_CANCELLED,
    STATUS_COMPLETED
} ReservationStatus;

/* Lot status */
typedef enum {
    LOT_AVAILABLE = 0,
    LOT_FULL,
    LOT_CLOSED
} LotStatus;

/* =============================================================================
 * STRUCTS
 * =============================================================================
 */

/* ---- PERSON 1: User struct ---- */
typedef struct {
    char userID[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    UserRole role;
    int isLoggedIn;
} User;

/* ---- PERSON 3: Time window struct ---- */
typedef struct {
    int startHour;   /* 0-23 */
    int startMin;    /* 0-59 */
    int endHour;
    int endMin;
} TimeWindow;

/* ---- PERSON 3: Parking lot struct ---- */
typedef struct {
    char lotID[MAX_ID_LEN];
    char lotName[MAX_NAME_LEN];
    int totalSpaces;
    int availableSpaces;
    TimeWindow windows[MAX_TIME_WINDOWS];
    int windowCount;
    LotStatus status;
} ParkingLot;

/* ---- PERSON 2: Reservation struct ---- */
typedef struct {
    char reservationID[MAX_ID_LEN];
    char userID[MAX_ID_LEN];
    char lotID[MAX_ID_LEN];
    int spaceNumber;
    TimeWindow timeSlot;
    char date[12];   /* Format: YYYY-MM-DD */
    ReservationStatus status;
} Reservation;

/* ---- PERSON 4: Simulation result struct ---- */
typedef struct {
    char lotID[MAX_ID_LEN];
    float avgUtilization;
    float peakUtilization;
    int peakHour;
    float avgWaitTime;
    int noShowCount;
    int totalSimulatedArrivals;
} SimulationResult;

/* ---- PERSON 5: Analytics report struct ---- */
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
 * (All modules share these — do not redeclare locally)
 * =============================================================================
 */
User         users[MAX_USERS];
ParkingLot   lots[MAX_LOTS];
Reservation  reservations[MAX_RESERVATIONS];

int userCount        = 0;
int lotCount         = 0;
int reservationCount = 0;

User *currentUser = NULL;   /* Pointer to currently logged-in user */

/* =============================================================================
 * FUNCTION DECLARATIONS
 * =============================================================================
 */

/* ---- PERSON 1: User Account & Login ---- */
void   initUsers();
int    registerUser(char *name, char *userID, char *password, UserRole role);
User  *loginUser(char *userID, char *password);
void   logoutUser();
void   saveUsersToFile();
void   loadUsersFromFile();
void   displayUserMenu();

/* ---- PERSON 2: Reservation System ---- */
void   initReservations();
int    makeReservation(char *userID, char *lotID, TimeWindow slot, char *date);
int    cancelReservation(char *reservationID);
void   viewReservations(char *userID);
int    isDoubleBooked(char *userID, TimeWindow slot, char *date);
int    isLotSlotFull(char *lotID, TimeWindow slot, char *date);
void   saveReservationsToFile();
void   loadReservationsFromFile();

/* ---- PERSON 3: Parking Lot & Availability ---- */
void   initLots();
int    addLot(char *lotID, char *lotName, int totalSpaces);
void   addTimeWindow(char *lotID, TimeWindow window);
int    updateLotAvailability(char *lotID, int delta);
void   showLotStatus();
int    getLotAvailableSpaces(char *lotID, TimeWindow slot, char *date);
void   saveLotsToFile();
void   loadLotsFromFile();

/* ---- PERSON 4: Simulation Engine ---- */
SimulationResult runSimulation(char *lotID, int numDays);
void   simulateParkingDemand(char *lotID);
void   modelArrivalsAndNoShows(char *lotID, float noShowRate);
void   testReservationPolicy(int policyType);
float  calculateWaitTime(char *lotID, TimeWindow slot);
float  calculateUtilization(char *lotID);

/* ---- PERSON 5: Reports, Analytics & Integration ---- */
AnalyticsReport generateReport(char *lotID);
void   showSystemStats();
void   displayUtilizationTrends();
void   displayNoShowRate();
void   mergeAndTestSystem();
void   runFullSystemTest();

/* ---- SHARED UTILITIES ---- */
void   clearInputBuffer();
void   printDivider();
void   printHeader(char *title);

/* =============================================================================
 * PERSON 1: USER ACCOUNT & LOGIN SYSTEM
 * ============================================================================= 
 * TODO Person 1: Implement all functions in this section
 */

void initUsers() {
    userCount = 0;
    currentUser = NULL;
    loadUsersFromFile();
    /* TODO Person 1: Add any additional initialization */
}

int registerUser(char *name, char *userID, char *password, UserRole role) {
    /* TODO Person 1: Implement registration
     * - Check if userID already exists
     * - Validate inputs
     * - Add to users[] array
     * - Save to file
     * - Return 1 on success, 0 on failure
     */
    return 0;
}

User *loginUser(char *userID, char *password) {
    /* TODO Person 1: Implement login
     * - Search users[] for matching userID + password
     * - Set currentUser pointer
     * - Set isLoggedIn = 1
     * - Return pointer to user on success, NULL on failure
     */
    return NULL;
}

void logoutUser() {
    /* TODO Person 1: Implement logout
     * - Set currentUser->isLoggedIn = 0
     * - Set currentUser = NULL
     */
}

void saveUsersToFile() {
    /* TODO Person 1: Write users[] array to USER_FILE using fwrite */
}

void loadUsersFromFile() {
    /* TODO Person 1: Read from USER_FILE into users[] array using fread */
}

void displayUserMenu() {
    /* TODO Person 1: Display post-login menu based on role
     * - Student: reserve, cancel, view reservations
     * - Faculty/Staff: same + view lot status
     * Call functions from Person 2 and Person 3 as needed
     */
}

/* =============================================================================
 * PERSON 2: RESERVATION SYSTEM
 * =============================================================================
 * TODO Person 2: Implement all functions in this section
 */

void initReservations() {
    reservationCount = 0;
    loadReservationsFromFile();
    /* TODO Person 2: Add any additional initialization */
}

int makeReservation(char *userID, char *lotID, TimeWindow slot, char *date) 
{
    //checking if double booked
    if (isDoubleBooked(userID, slot, date)) 
    {
        printf("You already have a reservation at this time!\n");
        return 0;
    }

    //checking if lot is full for this slot
    if (isLotSlotFull(lotID, slot, date)) 
    {  
        printf("Sorry, this lot is full for the selected time slot!\n");
        return 0;
    }

    //generating unique reservation ID
    char reservationID[MAX_ID_LEN];
    sprintf(reservationID, "RES_%d", reservationCount + 1);

    // Creating new reservation and adding to array
    Reservation newReservation;
    strcpy(newReservation.reservationID, reservationID);
    strcpy(newReservation.userID, userID);
    strcpy(newReservation.lotID, lotID);
    newReservation.timeSlot = slot;  
    strcpy(newReservation.date, date);
    newReservation.status = STATUS_ACTIVE;  
    newReservation.spaceNumber = -1; 
    reservations[reservationCount] = newReservation;
    reservationCount++;

    //updating lot availability
    updateLotAvailability(lotID,-1);

    //saving to file
    saveReservationsToFile();

    /* TODO Person 2: Implement reservation creation
     * - Call isDoubleBooked() first
     * - Call isLotSlotFull() first
     * - Generate unique reservationID
     * - Add to reservations[] array
     * - Call updateLotAvailability() from Person 3
     * - Save to file
     * - Return 1 on success, 0 on failure
     */
    return 1;
}

int cancelReservation(char *reservationID) {
    /* TODO Person 2: Implement cancellation
     * - Find reservation by ID
     * - Set status to STATUS_CANCELLED
     * - Call updateLotAvailability() to free the space
     * - Save to file
     * - Return 1 on success, 0 if not found
     */
    return 0;
}

void viewReservations(char *userID) {
    /* TODO Person 2: Print all active reservations for a given userID */
}

int isDoubleBooked(char *userID, TimeWindow slot, char *date) {
    /* TODO Person 2: Check if user already has a reservation in this time slot
     * - Return 1 if double booked, 0 if clear
     */
    return 0;
}

int isLotSlotFull(char *lotID, TimeWindow slot, char *date) {
    /* TODO Person 2: Check if lot has available spaces for this slot/date
     * - Return 1 if full, 0 if space available
     */
    return 0;
}

void saveReservationsToFile() 
{
    FILE *file=fopen(RESERVATION_FILE,"wb");
    if(file==NULL)
    {
        printf("Error: Cannot open reservations.dat for reading!\n");
        return;
    }

    //saving reservations to file
    fwrite(&reservationCount, sizeof(int), 1, file);
    fwrite(reservations, sizeof(Reservation), reservationCount, file);
    fclose(file);
    printf("Reservations saved to reservations.dat\n");
}

void loadReservationsFromFile() 
{
    FILE *file=fopen(RESERVATION_FILE,"rb");
    if(file==NULL)
    {
        printf("No existing reservations file.\n");
        reservationCount = 0;
        return;
    }

    //reading reservations from file
    int result = fread(&reservationCount, sizeof(int), 1, file);
    if (result != 1) 
    {
        printf("Error reading reservation count!\n");
        reservationCount = 0;
        fclose(file);
        return;
    }
    if (reservationCount < 0 || reservationCount > MAX_RESERVATIONS) 
    {
        printf("Invalid reservation count in file!\n");
        reservationCount = 0;
        fclose(file);
        return;
    }
    int itemsRead = fread(reservations, sizeof(Reservation), reservationCount, file);
    if (itemsRead != reservationCount) 
    {
        printf("Warning: Only read %d of %d reservations\n", itemsRead, reservationCount);
        reservationCount = itemsRead;
    }
    fclose(file);
    printf("Loaded %d reservations\n", reservationCount);
}

/* =============================================================================
 * PERSON 3: PARKING LOT & AVAILABILITY MANAGER
 * =============================================================================
 * TODO Person 3: Implement all functions in this section
 */

void initLots() {
    lotCount = 0;
    loadLotsFromFile();
    /* TODO Person 3: Add any additional initialization */
}

int addLot(char *lotID, char *lotName, int totalSpaces) {
    
     if (lotCount >= MAX_LOTS) {
        return 0; // returns val of 0 
     }
     // takes user input val. and puts it in struct 
     strcpy(lots[lotCount].lotID, lotID);
     strcpy(ltos[lotCount].lotName, lotName);

     lots[lotCount].totalSpaces = totalSpaces;
     lots[lotCount].availableSpaces = toatalSpaces;
     // implements status/ shows availability //
     lots[lotCount].status  = LOT_AVAILABLE;

     letCount++;
     return 1;

}
 // look back up at struct in order to update this part 
 /*typedef struct {
    char lotID[MAX_ID_LEN];
    char lotName[MAX_NAME_LEN];
    int totalSpaces;
    int availableSpaces;
    TimeWindow windows[MAX_TIME_WINDOWS];
    int windowCount;
    LotStatus status;
} ParkingLot;*/
/*
 // other struct for specific time intervals 
 typedef struct {
 
    int startHour; // 0-23
    int strtMin;  // 0-59
    it endHour;
    int endMin; 

    TimeWindow;
    }

*/
void addTimeWindow(char *lotID, TimeWindow window) {
    /* TODO Person 3: Add an operating time window to a specific lot */
  /*for (int i = 0; i < lotCount; i++) {
// find if there is a matching time window 

        if (strcmp(lots[i].lotID , lotID ) == 0){

            // check availability of time window desired 
            if (lots[i].windowCount >= MAX_TIME_WINDOWS) {
                printf("no time windows available \n");


                return;




            }
        }



    }  */  
}

int updateLotAvailability(char *lotID, int delta) {
  
    // for loop specifically to run delta+ and - 
    for (int i = 0; i < lotCount; i++){
        if (strcmp(lots[i].lotID) == 0) {
            lots[i].availableSpaces +=delta;

            // keeps values in check, outlier check 
            if (lots[i].availableSpaces < 0)
            lots[i].availableSpaces = 0;

            if (lots[i].availableSpaces > lots[i].totalSpaces)
            lots[i].availableSpaces = lots[i].totalSpaces;

            //updates LotSTatus full or available 
            if (lots[i].availableSpaces == 0) {
                lots[i].status = LOT_FULL;
            }
            else {
                 lots[i].status = LOT_AVAILABLE;
            }
        return 1;
        
        }
     
    }
    return 0;
}

void showLotStatus() {
    
    // shows all data that user is able to see once status is updated 
    for(int i = 0; i < lotCount; i++){
        //lotID
        printf("ID: %s\n", lots[i].lotID);
        // name 
        printf("Name: %s\n",lots[i].lotName);
        // total SPACES
        printf("Total: %d\n", lots[i].totalSpaces);
        // total AVAILABLE spaces 
        printf("Available: %d\n", lots[i].availableSpaces);

        // status update 
        if (lots[i].status == LOT_AVAILABLE) {
            printf("Status: AVAILABLE\n");
        }
        else {
            printf("Status: FULL\n");
        }
    printf("-----------------------\n");
    }
}

int getLotAvailableSpaces(char *lotID, TimeWindow slot, char *date) {
    /* TODO Person 3: Return number of available spaces for a specific slot/date
     * - Should account for existing reservations in that window
     */


     for(int i = 0; i < lotCount; i++){
        if (strcmp(lots[i].lotID,lotID) == 0) {
            return lots[i].availableSpaces;
        }
     }
    return 0;
}

void saveLotsToFile() {
    /* TODO Person 3: Write lots[] to LOT_FILE using fwrite */
}

void loadLotsFromFile() {
    /* TODO Person 3: Read from LOT_FILE into lots[] using fread */
}

/* =============================================================================
 * PERSON 4: SIMULATION ENGINE
 * =============================================================================
 * TODO Person 4: Implement all functions in this section
 */

SimulationResult runSimulation(char *lotID, int numDays) {
    /* TODO Person 4: Master simulation runner
     * - Run simulateParkingDemand() across numDays
     * - Aggregate results into SimulationResult struct
     * - Return the result
     */
    SimulationResult result;
    memset(&result, 0, sizeof(SimulationResult));
    strncpy(result.lotID, lotID, MAX_ID_LEN);
    return result;
}

void simulateParkingDemand(char *lotID) {
    /* TODO Person 4: Simulate a day of parking demand for a given lot
     * - Model arrivals by time of day
     * - Account for no-shows
     * - Track when lot hits capacity
     */
}

void modelArrivalsAndNoShows(char *lotID, float noShowRate) {
    /* TODO Person 4: Model arrival patterns and apply no-show rate
     * - noShowRate is 0.0 to 1.0 (e.g. 0.1 = 10% no-show)
     * - Update simulation counters accordingly
     */
}

void testReservationPolicy(int policyType) {
    /* TODO Person 4: Test different reservation policies
     * - policyType 0: first come first served
     * - policyType 1: priority by role (faculty first)
     * - policyType 2: time-based slots only
     * - Print comparison results
     */
}

float calculateWaitTime(char *lotID, TimeWindow slot) {
    /* TODO Person 4: Calculate estimated wait time for a given lot/slot
     * - Return wait time in minutes as float
     */
    return 0.0f;
}

float calculateUtilization(char *lotID) {
    /* TODO Person 4: Calculate current utilization rate for a lot
     * - Return value between 0.0 and 1.0
     */
    return 0.0f;
}

/* =============================================================================
 * PERSON 5: REPORTS, ANALYTICS & INTEGRATION
 * =============================================================================
 * TODO Person 5: Implement all functions in this section
 */

AnalyticsReport generateReport(char *lotID) {
    /* TODO Person 5: Build and return an AnalyticsReport for a given lot
     * - Pull data from reservations[] and lots[]
     * - Calculate rates and trends
     */
    AnalyticsReport report;
    memset(&report, 0, sizeof(AnalyticsReport));
    strncpy(report.lotID, lotID, MAX_ID_LEN);
    return report;
}

void showSystemStats() {
    /* TODO Person 5: Print system-wide stats
     * - Total users, total reservations, total cancellations
     * - Overall utilization across all lots
     */
}

void displayUtilizationTrends() {
    /* TODO Person 5: Show utilization by hour across all lots
     * - Identify peak hours
     * - Show demand trends
     */
}

void displayNoShowRate() {
    /* TODO Person 5: Calculate and display no-show rate per lot
     * - Compare across lots
     */
}

void mergeAndTestSystem() {
    /* TODO Person 5: Integration function
     * - Call init functions from all modules
     * - Run a sample workflow end to end
     * - Verify all modules talk to each other correctly
     */
}

void runFullSystemTest() {
    /* TODO Person 5: Full system test
     * - Create test users
     * - Create test lots
     * - Make and cancel reservations
     * - Run simulation
     * - Generate reports
     * - Print pass/fail for each step
     */
}

/* =============================================================================
 * SHARED UTILITIES
 * =============================================================================
 */

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printDivider() {
    printf("------------------------------------------------------------\n");
}

void printHeader(char *title) {
    printDivider();
    printf("  %s\n", title);
    printDivider();
}

/* =============================================================================
 * MAIN
 * =============================================================================
 */

int main() {
    /* Initialize all modules */
    initUsers();
    initLots();
    initReservations();

    printHeader("UTA PARKING RESERVATION SYSTEM");

    /* TODO Person 1: Replace this block with your login/register menu loop
     * The flow should be:
     * 1. Show login or register options
     * 2. On successful login, call displayUserMenu()
     * 3. Loop until user exits
     */

    printf("System initialized. Waiting for Person 1 login implementation.\n");

    /* TODO Person 5: Uncomment when ready for full integration test */
    /* runFullSystemTest(); */

    return 0;
}
