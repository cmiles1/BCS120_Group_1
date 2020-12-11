
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
  \--`storeInfo`
  Contains information parsed from the "storeInfo.dat" datafile (*see "Data Files"*)
    --`userInfo`
  Contains information parsed from the "users.dat" datafile (*see "Data Files"*)
  
  #### Functions
---	`void loadStoreInfo(StoreInfo* store) `
Loads store information from "storeInfo.dat" datafile. 
Passes store info to a "storeInfo" structure
Exits the program if the file cannot be opened.

---	`void login(UserInfo* seluser)`
Gets user login attempt information from standard input.
Parses user information from the "users.dat" datafile, checks for matches with the info provided from stdin.
If the login is authenticated, passes user info from the datafile into a "userInfo" structure.
Exits the program if the file cannot be opened or if the login information was not authenticated.

## Data Files

#### inventory.dat

Input file containing all inventory data

#### users.dat

 Input file containing all of the employee information and (plaintext) credentials in the specified format:
`(Delimiter: ",")`
`	 - User ID Number`
`	 - Employee Real Name`
`	 - Username`
`	 - Password (*plaintext)`

#### storeInfo.dat

Input file containing store information in the specified format:
`    (Delimiter: ",")`
`  - Local Store Name`
`  - Store ID Number`
` - Store Local Address (Line 1)`
` - Store Town`
` - Store State Code`
`	 - Store ZIP Code`
`	 - Store Phone Number`
`	 - Store Manager Name`
`	 - Store Manager ID Number`
`	 - Store Manager Extension`
`	 - Store Manager Email`
`	 - System Administrator Name`
`	 - System Administrator Email`
