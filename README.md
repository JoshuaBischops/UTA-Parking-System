# UTA Parking Reservation System

A command-line parking reservation system built in C for UTA's parking infrastructure. The system handles user authentication, parking reservations, lot availability tracking, demand simulation, and analytics reporting.

---

## Team Work Split

| Person | Module | Responsibilities |
|--------|--------|-----------------|
| Person 1 | User Account & Login | User structs, file storage, login/auth, role handling, post-login menu |
| Person 2 | Reservation System | Reserve/cancel spaces, view reservations, prevent double booking, check lot availability |
| Person 3 | Parking Lot & Availability | Store lot info, track spaces, manage time windows, update availability after reservations |
| Person 4 | Simulation Engine | Simulate parking demand, model arrivals/no-shows, test reservation policies, calculate wait times |
| Person 5 | Reports & Integration | Admin reports, system stats, utilization trends, merge all modules, full system test |

---

## File Structure

```
UTA-Parking-System/
├── Parking_System_Framework.c   # Single shared source file — all code goes here
├── README.md                    # This file
├── users.dat                    # Generated at runtime by Person 1
├── reservations.dat             # Generated at runtime by Person 2
└── lots.dat                     # Generated at runtime by Person 3
```

---

## How to Compile and Run

```bash
gcc Parking_System_Framework.c -o parking_system
./parking_system
```

---

## Git Workflow — Read This Before You Touch the File

### Before you start coding every single session:
```bash
git pull
```
This gets the latest version. If you skip this you will get merge conflicts.

### After you finish coding:
```bash
git add Parking_System_Framework.c
git commit -m "Person X: describe what you implemented"
git push
```

### Commit message examples:
- `Person 1: implemented registerUser and loginUser`
- `Person 2: implemented makeReservation and isDoubleBooked`
- `Person 3: implemented initLots and updateLotAvailability`

---

## Ground Rules

1. **Only work in your labeled section.** Each section is clearly marked with your person number in the file.
2. **Do not modify structs or constants** at the top of the file without telling the whole team first. Everyone depends on them.
3. **Do not rename or change function signatures.** Other people's code calls your functions — changing the signature breaks their code.
4. **Always pull before you push.** No exceptions.
5. **Commit often.** Small commits are easier to debug than one giant commit at the end.
6. **If you hit a merge conflict**, do not panic. Message the team, figure out who has the latest version, and resolve it together.

---

## Cross-Module Dependencies

Some functions call functions from other modules. Do not change these interfaces:

- `Person 2 → Person 3:` `makeReservation()` calls `updateLotAvailability()` and `getLotAvailableSpaces()`
- `Person 2 → Person 3:` `cancelReservation()` calls `updateLotAvailability()`
- `Person 1 → Person 2:` `displayUserMenu()` calls `makeReservation()`, `cancelReservation()`, `viewReservations()`
- `Person 1 → Person 3:` `displayUserMenu()` calls `showLotStatus()`
- `Person 5 → All:` `mergeAndTestSystem()` and `runFullSystemTest()` call functions from every module

---

## Global Shared Data

These are defined once and shared across all modules. Do not redeclare them:

```c
User         users[MAX_USERS];
ParkingLot   lots[MAX_LOTS];
Reservation  reservations[MAX_RESERVATIONS];

int userCount;
int lotCount;
int reservationCount;

User *currentUser;   // Set by Person 1 on login
```

---

## Data Files

The system saves data to binary `.dat` files at runtime. These are auto-generated — do not manually edit them. If you need to reset the system, delete the `.dat` files and restart.

---

## Questions or Conflicts

If you have a question about another person's section or need to change a shared struct/function, bring it up with the team before making changes. One person changing a struct silently will break everyone else's code.
