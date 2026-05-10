# 📚 Library & Resource Management System (LRMS)

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Architecture](https://img.shields.io/badge/Architecture-Layered%20OOP-orange?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Stable-brightgreen?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

<br/>

> A robust, console-based **Library and Resource Management System** engineered in **C++17**.  
> Built on a strict multi-layered architecture with advanced OOP principles, generic file-based persistence, and a full administrative workflow engine.

</div>

---

## 👥 Development Team

| Full Name | Roll Number | Section |
| :--- | :---: | :---: |
| **Rameen Zulfi** | CS-25112 | C |
| **Syeda Bareerah Kazmi** | CS-25118 | C |
| **Syed Waqar Wasif** | CS-25123 | C |
| **Ali Faizyab Khan** | CS-25142 | C |

---

## 📂 Project Architecture & Folder Structure

The project strictly enforces a **multi-tiered layered architecture**, cleanly separating data persistence, business logic, validation, and the presentation interface. No layer communicates with a non-adjacent layer directly.

<details>
<summary><b>📁 Click to expand the full directory tree</b></summary>

```text
LRMS/
├── build/                      # Compiled binaries and CMake build artifacts
├── src/                        # Primary source code root
│   ├── dataStore/              # Data Layer — Generic repositories & pipe-delimited .txt files
│   │   ├── BaseRepository.h    # Template<T> base class for all file I/O
│   │   ├── AdminUserStore.h/cpp
│   │   ├── AssetStore.h/cpp
│   │   ├── FinancialStore.h/cpp
│   │   ├── admins.txt
│   │   ├── users.txt
│   │   ├── resources.txt
│   │   ├── categories.txt
│   │   ├── transactions.txt
│   │   ├── fines.txt
│   │   ├── fund_requests.txt
│   │   ├── borrowing_history.txt
│   │   └── membership_types.txt
│   ├── domain/                 # Core Domain Entities (Plain C++ classes)
│   │   ├── Person.h            # Abstract base class (pure virtual getRole)
│   │   ├── User.h
│   │   ├── Administrator.h
│   │   ├── Resource.h
│   │   ├── Category.h
│   │   ├── Transaction.h
│   │   ├── Fine.h
│   │   ├── FundRequest.h
│   │   ├── BorrowingHistory.h
│   │   └── MembershipType.h
│   ├── PDFGenerator/           # PDF Publishing Utility (wkhtmltopdf wrapper)
│   │   ├── PdfGenerator.h
│   │   └── PdfGenerator.cpp
│   ├── presentation/           # UI / Presentation Layer
│   │   ├── AuthMenu.h/cpp      # Login gateway
│   │   ├── AdminMenu.h/cpp     # Full admin portal (~600 lines)
│   │   ├── UserMenu.h/cpp      # Member portal
│   │   ├── InputForms.h        # Reusable form prompts (static methods)
│   │   ├── ConsoleUtils.h      # Safe input helpers (namespace)
│   │   └── Session.h           # ActiveSession struct (userId, adminId, isExit)
│   ├── services/               # Business Logic / Service Layer
│   │   ├── AuthenticationService.h/cpp
│   │   ├── AdminService.h/cpp
│   │   └── UserService.h/cpp
│   ├── Utility/                # Cross-cutting utility helpers
│   │   ├── date.h
│   │   └── date.cpp            # Date parsing, normalization, overdue calculation
│   ├── validation/             # Centralized Data Validation Checkpoint
│   │   ├── validator.h
│   │   └── validator.cpp       # Throws on fatal errors, collects minor ones
│   └── main.cpp                # Application entry point & DI composition root
├── tools/                      # Bundled external tools
│   └── wkhtmltopdf.exe         # PDF converter (Windows)
├── .gitignore
├── CMakeLists.txt              # CMake build configuration
├── LICENSE
└── README.md
```

</details>

---

## 🚀 Getting Started

This project uses **CMake** as its build system. Choose either method below to compile and run the application.

### Prerequisites

- A **C++17** compatible compiler — GCC 9+, Clang 10+, or MSVC 2019+
- **CMake** version **3.16 or higher**

---

### ⚙️ Option 1 — Command Line (Windows / macOS / Linux)

Open a terminal in the project root directory (the folder containing `CMakeLists.txt`) and run the following commands:

```bash
# Step 1: Create and enter the build directory
mkdir build
cd build

# Step 2: Generate platform-specific build files
cmake ..

# Step 3: Compile the project
cmake --build .
```

```bash
# Step 4: Run the executable

# On Windows:
.\LibraryManagementSystem.exe

# On Linux / macOS:
./LibraryManagementSystem
```

---

### 🖥️ Option 2 — Visual Studio Code

1. Ensure you have the **C/C++** and **CMake Tools** extensions installed in VS Code.
2. Open the project's root folder in VS Code (`File → Open Folder`).
3. A prompt may appear at the bottom-right to configure the project — click **"Yes"**.  
   *(Alternatively: `Ctrl+Shift+P` → type `CMake: Configure` → select your compiler kit.)*
4. Click the **⚙️ Build** button on the bottom status bar, or press **`F7`**.
5. After a successful build, click the **▶️ Run** button on the status bar, or press **`Shift+F5`**.

---

## 🧠 Object-Oriented Programming Concepts Applied

The codebase is engineered as a strict demonstration of modern C++ OOP. Every concept listed below has a concrete, traceable implementation in the source code.

---

### 🔷 Compile-Time Polymorphism — Operator Overloading

The **insertion operator `<<`** is overloaded as a `friend` function in every domain class (`User`, `Resource`, `Fine`, `Transaction`, etc.). This provides a unified, clean interface for both console display and file serialization without exposing private members through unnecessary getters.

```cpp
// In Fine.h — operator<< for console output
friend std::ostream &operator<<(std::ostream &os, const Fine &f) {
    os << "Fine ID: " << f.fineId << " | Amount: $" << f.fineAmount;
    return os;
}
```

---

### 🔷 Compile-Time Polymorphism — Function Overloading

The `Validator` namespace demonstrates function overloading, where the **same function name `validate()`** handles completely different types. The compiler resolves the correct version at compile time based on the argument's type — zero runtime overhead.

```cpp
// In validator.h — two overloads, resolved at compile time
ValidationResult validate(const Resource &resource);
ValidationResult validate(const User &user);
```

---

### 🔶 Run-Time Polymorphism — Function Overriding

The abstract `Person` base class declares a **pure virtual function** `getRole()`, making it an interface contract. The concrete derived classes `User` and `Administrator` **override** it to return their specific system role. The `override` keyword enforces compiler-level safety.

```cpp
// Person.h — abstract contract
virtual std::string getRole() const = 0;

// User.h — runtime override
std::string getRole() const override { return "Member"; }

// Administrator.h — runtime override
std::string getRole() const override { return "Administrator"; }
```

---

### 🔶 Exception Handling — Polymorphic Catching

The presentation layer (`AdminMenu`, `UserMenu`) uses a **single `catch (const std::exception &e)` block** to intercept multiple distinct exception types dynamically thrown by the service layer (`std::logic_error`, `std::underflow_error`, `std::invalid_argument`). This is runtime polymorphism applied to error handling.

```cpp
// In AdminMenu.cpp — one catch block handles many exception types
try {
    adminService.addResource(newResource); // may throw std::underflow_error
}
catch (const std::exception &ex) {        // catches ALL std::exception descendants
    std::cout << RED << ex.what() << RESET;
}
```

---

### 🔷 Inheritance — Domain & Data Layers

**Domain Layer:** `User` and `Administrator` inherit shared identity attributes (`id`, `username`, `password`, `email`, `isActive`) from the abstract `Person` base class, eliminating duplication across two distinct account types.

**Data Layer:** All nine store classes (`UserStore`, `ResourceStore`, `FineStore`, etc.) inherit complete file I/O functionality from the `BaseRepository<T>` template class, requiring zero per-store serialization boilerplate.

```cpp
class User          : public Person              { /* user-specific fields */ };
class Administrator : public Person              { /* admin-specific fields */ };
class UserStore     : public BaseRepository<User>{ /* only business queries */ };
```

---

### 🔷 Abstraction

`AdminService` and `UserService` encapsulate all complexity — file I/O, ID generation, fine calculation, inventory management — behind simple, intention-revealing method names. The presentation layer never directly touches a store or file; it simply calls actions.

```cpp
// AdminMenu.cpp — abstraction in action. No knowledge of HOW this works.
adminService.processBorrowRequest(txnId, approve, simulatedToday);
adminService.updateDailyFines(systemDate);
```

---

### 🔷 Aggregation & Dependency Injection

Store objects are created **once** in `main.cpp` and **injected** into services via constructor references. This achieves **loose coupling** and **shared state** — both `AdminService` and `UserService` operate on the exact same `UserStore` instance, so a change made by one is instantly visible to the other.

```cpp
// main.cpp — DI composition root
UserStore userStore("users.txt");                         // created once
AdminService adminService(userStore, fineStore, ...);    // injected by reference
UserService  userService (userStore, resourceStore, ...); // same instance reused
```

---

### 🔷 Templates — Generic Programming

`BaseRepository<T>` is a single template class that provides a complete, reusable file persistence engine for **any** domain type. Without it, nine functionally-identical store classes would require thousands of lines of duplicated I/O code.

```cpp
// BaseRepository.h
template <typename T>
class BaseRepository {
protected:
    std::vector<T> arr;      // in-memory cache
    void loadFromFile();     // deserialize on construction
    void saveToFile();       // serialize on every mutation
};

// The compiler generates a specialized version for each type:
// BaseRepository<User>, BaseRepository<Fine>, BaseRepository<Resource> ...
```

---

### 🔷 Single Responsibility Principle

Each component in the system has exactly **one reason to change**:

| Component | Single Responsibility |
| :--- | :--- |
| `Resource.h` | Holds resource data and its serialization contract |
| `ResourceStore` | Handles file persistence of Resource objects |
| `AdminService` | Orchestrates library business rules |
| `AdminMenu` | Manages console UI rendering and user input only |
| `Validator` | Validates domain object data integrity |
| `makePdf()` | Converts a string report to a PDF file |

---

## 📊 System Status

All core subsystems compile and operate without fatal errors on the production build.

| Layer | Status |
| :--- | :---: |
| ✅ Domain Layer | **Working** |
| ✅ DataStore Layer | **Working** |
| ✅ Service Layer | **Working** |
| ✅ Presentation Layer | **Working** |
| ✅ PDF Generator | **Working** |
| ✅ Validation Engine | **Working** |

> Core functionalities — authentication, borrowing workflow, fine calculation, reporting, and fund management — perform as expected without crashes or data corruption.

---

## 📚 References & Materials Used

**Books**
- *The Pragmatic Programmer* — David Thomas & Andrew Hunt
- *Object Oriented Programming in C++ (4th Ed.)* — Robert Lafore

**Web Communities & Documentation**
- [Stack Overflow](https://stackoverflow.com)
- [GeeksforGeeks](https://www.geeksforgeeks.org)
- [cppreference.com](https://en.cppreference.com)

**AI Assistants** *(Used for debugging support and architectural planning)*
- Anthropic Claude · Google Gemini · OpenAI ChatGPT · DeepSeek

**Other Media**
- C++ Architecture & Design Pattern tutorials on YouTube

---

<div align="center">

> Works on our machines. We have submitted the machines as supplementary material. — **Section C**, CS Batch 2025

</div>