#  Team 1 BCS-120
## "Best Stop"

This repository contains a group project for an inventory management system, designed for a sample electronic technology retail company, known as "Best Stop".

### Collaborators:
Christopher Miles  
Aniq Abid  
Kelvin Canales  
Matthew Ortega  
Kevin Romero  
Luke Ziccardi  


## Program Files

### Best Stop.cpp 
 The main C++ program for the company's inventory management system.
  
  #### Structures
  --`storeInfo{}`\
  Contains information parsed from the "storeInfo.dat" datafile (*see "Data Files"*)\
  --`userInfo{}`\
  Contains information parsed from the "users.dat" datafile (*see "Data Files"*)\
  --`InventoryData{}`\
  Contains a vector, `items[n]`, where 'n' is the index which contains each item in the "inventory.dat" datafile (*see "Data Files"*)\
  
  
  #### Functions
  
  ##### DataFile Loading Functions
---	`void loadStoreInfo(...);`  
Loads store information from "storeInfo.dat" datafile. \
Passes store info to a "storeInfo" structure.  \
Exits the program and writes output to log file if the file cannot be opened.  

---	`void login(...);`  
Gets user login attempt information from standard input.  
Parses user information from the "users.dat" datafile, checks for matches with the info provided from stdin.  \
If the login is authenticated, passes user info from the datafile into a "userInfo" structure.  \
Exits the program if the file cannot be opened or if the login information was not authenticated.  \
Writes output to the log file if the authentication was confirmed/denied, or if the users datafile was not found.

--- `void loadInventory(...);`
Loads the "inventory.dat" datafile into parallel arrays, to later be passed into the `InventoryData{}` structure. \
Writes output to the log file if the datafile was loaded, or was not found.

##### Display Functions

--- `void displayInventory(...);`
Main function for displaying the inventory data. \
Displays the inventory from the Structure Vector `items[n]`, where 'n' is the index. \
Takes on arguments `rangemin` and `rangemax`, which specify how many items to print. \
Used by other functions to display inventory data.

--- `void display_byStock(...);`
Displays inventory items by a minimum amount and a maximum amount in their stock, using the `displayInventory()` function.\

--- `void display_bySKU(...);`
Displays matching inventory items by SKU number, using the `displayInventory()` function.

--- `void display_byType(...);`
Displays matching inventory items by category name, using the `displayInventory()` function. \ 
Uses a reverse-sorting loop to sort each category name and strip out repeating category types. \
Then, allows the user to query the inventory by category name.\

##### File Append/Write Functions

--- `bool writechanges(...);`
Attempts to write changes from memory into the "inventory.dat" data file. \
Logs item changed by index and current SKU number, and the user that changed the item \
Or logs write error if the file could not be written to \
Returns True if information was wrote to file, False if the information could not be wrote. \
Optionally, allows the user to retry upon failure.

--- `void appendLogFile(...);`
Appends given information in a specific format to the output log file. \
(*see "Data Files", and "Log Messages"*)\

##### Inventory Editing Function
--- `void edit_inventoryData(...);`
Allows user to search for an inventory item by SKU (and displays it via `displayInventory`).\
(Note : Does not use `display_bySKU()`) \
Allows the user (granted they have the right privilege-level assignment value- (*see "Privilege Levels"*))- to change information about each `InventoryData{}` item (*see "Data Files" for the list of information that may be changed*). \
Writes unauthorized item changes to the output log file. \
Allows users with `Privilege 1` to delete items. \
After the user selects to quit from the edit menu, they may choose to save or discard their changes. \
Saves changes to memory in the `InventoryData{}` structure's `items` vector. \
After saving changes to memory, saves changes use the `writechanges()` function to write the information to the `inventory.dat` datafile. \
Allows the user to edit another inventory item if they wish, or return to the main menu.

##### Misc Functions
--- `char* getSystemDate();`
Gets the system date information using the built-in `ctime` function.\
Returns the system date in `Www Mon DD HH:MM:SS YYYY` format. 

## Data Files

#### inventory.dat

Main datafile for input and output. Inventory items are stored in this specific format: \
`(Delimiter: ",")`\
`	 - SKU Number`\
`	 - Item Type`\
`	 - Item Name`\
`	 - Number in Stock`\
`  - Price`\

#### users.dat

 Input file containing all of the employee information and (plaintext) credentials in this specific format: \
`(Delimiter: ",")`\
`	 - User ID Number`\
`	 - Employee Real Name`\
`	 - Username`\
`	 - Password (*plaintext)`\
`  - Privilege Level` (*see "Privilege Levels"*).

Whereas the first employee is the Root account, the second is the Manager, and the third is the System Administator's personal account. \

#### storeInfo.dat

Input file containing store information in this specific format:\
`    (Delimiter: ",")`\
`  - Local Store Name`\
`  - Store ID Number`\
`  - Store Local Address (Line 1)`\
`  - Store Town`\
`  - Store State Code`\
`	 - Store ZIP Code`\
`	 - Store Phone Number`\
`	 - Store Manager Name`\
`	 - Store Manager ID Number`\
`	 - Store Manager Extension`\
`	 - Store Manager Email`\
`	 - System Administrator Name`\
`	 - System Administrator Email`

#### log.dat

Output file containing log information in this specific format (*see "Log Messages"*):
`    (Delimiter: ",")`\
`  - Time` (`Mon DD YYYY HH:MM:SS`) (System time- *locally-generated*) \
`  - Message Type`\
`  - Message/Error Code`\ 
`  - Message/Error` \
`  - User/Account `\
`	 - Message Info`\


## Log Messages

| MESSAGE TYPES | MSG CODE | MSG NAME      | USERNAME  | MSG INFO                                                    | Description                                                                                   |
|---------------|----------|---------------|-----------|-------------------------------------------------------------|-----------------------------------------------------------------------------------------------|
| ERROR         | E1001    | READ_INV      | USER      | FATAL: Inventory.dat not found                              | file input error for inventory.dat                                                            |
| ERROR         | E1002    | READ_INV_LINE | USER      | ERR: Inventory.dat - potential data loss on line            | file input error at given line index, potential data loss                                     |
| ERROR         | E1003    | WRITE_INV     | USER      | ERR: Cannot write to inventory.dat                          | file output error for inventory.dat                                                           |
| ERROR         | E2003    | READ_USERS    | *ACCOUNT* | FATAL: Users.dat not found                                  | file input error for Users.dat, *ACCOUNT* is the username credentials the user tried to use   |
| ERROR         | E3004    | READ_STORE    | GUEST     | FATAL: storeInfo.dat not found                              | file input error for storeInfo.dat, *GUEST* because the user has not prompted to be logged in |
| INFO          | I4001    | LOAD_INV      | USER      | User loaded inventory.dat                                   | user successfully loaded inventory file                                                       |
| INFO          | I4002    | WRITE_INV     | USER      | User changed item at *index*, to *new SKU*                  | user successfully wrote changes to inventory file, log changed item                           |
| INFO          | I5001    | AUTH_LOGIN    | USER      | User login successful                                       | user successful login                                                                         |
| WARN          | W6001    | FAILED_LOGIN  | *ACCOUNT* | User login unsuccessful                                     | user unsuccessful login attempt, *ACCOUNT* is the username credentials the user tried to use  |
| WARN          | W6002    | UNAUTH_PRIV   | USER      | User with priv *privlev* attempted to change SKU *SKU*(...) | user unsucessfully tried to change an item *SKU* number without correct privilege level       |

**ERROR** - Critical component of the program not loaded, program may exit\
**INFO** - User changed item, logged in successfully, or other normal message\
**WARN** - User tried doing something they're not allowed to do, warn

