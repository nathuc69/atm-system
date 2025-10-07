# ATM Management System (C Language Project)

## Objective

This project demonstrates your ability to understand and apply programming logic and adapt to a new programming language — **C** in this case. You will be working on an **ATM Management System** and extending it with several new features while maintaining and improving existing functionality.

## Instructions

You are provided with an initial codebase that includes basic functionality such as:

- Login / Register
- Create new accounts
- View list of owned accounts

Your job is to **optimize**, **refactor**, and **implement** the remaining functionalities described below.

---

## Project Structure

```
.
|
├── data
│   ├── records.txt        # Stores accounts informations
│   ├── user.txt           # Store users informations
├── Makefile               # Compile automation
└── src
    ├── auth.c             # Authentication logic
    ├── header.h           # Header file
    ├── main.c             # Entry point
    └── system.c           # Core system logic

```

---

## Data Format

### `user.txt`
```
<user_id> <name> <password>
Example:
0 Alice 1234password
1 Michel password1234
```

### `records.txt`
```
<record_id> <user_id> <username> <account_id> <creation_date> <country> <phone> <balance> <type>
Example:
0 0 Alice 0 10/02/2020 german 986134231 11090830.00 current
1 1 Michel 2 10/10/2021 portugal 914134431 1920.42 savings
```

---

## Features to Implement

### 1. **User Registration**
- Names must be unique (no duplicates allowed).
- Save new users to `atm.db`.

### 2. **Update Existing Account Information**
- Users can update:
  - **Country**
  - **Phone number**
- Prompt for account ID, then prompt which field to update.
- Save changes in `records.txt`.

### 3. **Check Details of a Specific Account**
- Ask for the account ID.
- Display interest details based on account type:
  - `savings`: 7%
  - `fixed01`: 4%
  - `fixed02`: 5%
  - `fixed03`: 8%
  - `current`: No interest
- Example output:
```
You will get $5.97 as interest on day 10 of every month.
```

### 4. **Make Transactions**
- Withdraw or deposit money.
- **Not allowed** for fixed01, fixed02, fixed03 account types — display an error message if attempted.

### 5. **Remove Existing Account**
- Users can delete their own account.
- Changes must be saved in `records.txt`.

### 6. **Transfer Ownership**
- Transfer an account to another user by:
  1. Identifying the account
  2. Identifying the destination user
  3. Updating `records.txt`

---

---

## You Will Learn

- C programming fundamentals
- File manipulation (reading/writing)
- Process communication using pipes
- Memory management
- Using `Makefile` for builds

---

## How to Compile

```bash
make
```

Then run the executable:

```bash
./atm
```

---

## Author

*Allain Nathan*

---

