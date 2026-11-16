# EcoTrack – Smart Campus Waste Management and Recycling System

Console-based C++ (C++11) OOP academic prototype for **Maharaja Agrasen Institute of Technology (MAIT), Delhi**.

- **Primary SDG:** SDG 12 – Responsible Consumption and Production  
- **Secondary SDG:** SDG 11 – Sustainable Cities and Communities

> This is a software prototype for record-keeping and awareness. It does **not** physically collect, weigh, classify, or recycle waste, and it is **not** deployed at MAIT or connected to actual campus services.

## Features
- Student registration/login (ID-based, basic academic prototype – not secure production auth)
- Waste reporting with 5 categories (Biodegradable, Recyclable, Non-recyclable, Hazardous, Electronic / E-waste)
- Duplicate report detection (flagged for admin review)
- Collection request management with fixed status flow: `Reported → Pending → Assigned → Collected → Processed → Closed`
- Recycling records (recycled qty cannot exceed collected qty)
- Separate E-waste records and handover details (stored only; no external recycler connection)
- Configurable reward points per kg (`data/rewards.cfg`), awarded only after admin verification and processed stage, no duplicates
- Admin statistics and summary reports (totals, category %, recycling rate, collection efficiency, e-waste handover %, pending lists, rewards)
- Persistent storage in plain text files (`data/*.txt`, `data/rewards.cfg`) using `|` separator

## Project Structure
```
EcoTrack/
├── src/                 # C++ source (.h/.cpp) - segregated by class/module
├── data/                # Text data files and rewards.cfg
├── Makefile
├── README.md
└── .gitignore
```

## Build & Run
Requirements: `g++` with C++11 support.

```bash
make
./ecotrack
```
Run from the project root so the program can find the `data/` directory.

**Demo admin login:** Admin ID `admin`

## Data Files
- `data/students.txt` — `studentId|name|department|semester|rewardPoints`
- `data/reports.txt` — `wasteId|studentId|category|quantity|unit|location|date|description|status|verified`
- `data/collections.txt` — `requestId|wasteId|assignedDate|status`
- `data/recycling.txt` — `wasteId|category|collectedQty|recycledQty|date`
- `data/ewaste.txt` — `wasteId|itemType|quantity|collectionStatus|handoverDetails`
- `data/rewards.cfg` — `category|pointsPerKg`

## OOPS Concepts Used
- **Encapsulation:** private/protected data with getters/setters and validation
- **Abstraction:** abstract base classes `User` and `Waste` with pure virtual functions
- **Inheritance:** `Student`/`Admin` ← `User`, `WasteReport`/`EWasteRecord` ← `Waste`
- **Polymorphism:** virtual `displayMenu()`, `getRole()`, `getSummary()` and runtime dispatch via base pointers
- **Constructors/Destructors:** default/parameterized ctors, virtual destructors in base classes
- **Function Overloading:** `ReportGenerator::summary()` overall and category-wise
- **STL:** `vector`, `map`, `set`, `find_if`/search helpers, sorting-ready structures

## Team Responsibilities
- **Prastuti (Member 1):** Concept & SDG 12 research, student management/registration/reporting/classification, input validation, student UI, integration support
- **Mitali Singh (Member 2):** OOPS architecture & hierarchy, base classes, collection management, processing/segregation logic, e-waste module, inheritance/polymorphism, testing & code review
- **Lara Sharma (Member 3):** Recycling management, rewards & awareness, admin management, file handling/persistence, reports & statistics, testing documentation

All members contribute to integration, debugging, testing, documentation, and presentation.

## Notes / Limitations
- Classification is based on student selection and admin review (no AI/image detection)
- `|` characters in text inputs are rejected/sanitized for safe file storage
- Malformed/missing data files are handled safely on load
