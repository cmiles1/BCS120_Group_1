// Best Stop.cpp : This file contains the 'main' function for an inventory management system for an example company, called "Best Stop".
//				   This project was created for a Group Project in a C++ programming course

/*
	Program Author :
					Christopher Miles

	Collaborators :
					Aniq Abid
					Kelvin Canales
					Matthew Ortega
					Kevin Romero
					Luke Ziccardi
*/

#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#pragma warning(disable : 4996)

using namespace std;

// **** Structure Prototypes ****
// Stores info related to the store
struct StoreInfo {
	// Store information
	string name;
	int storenumber = 0;
	string address;
	string town;
	string state;
	string zip;
	string phone;
	// Store manager info
	string manager;
	int managerID = 0;
	int managerExt = 0;
	string managerEmail;
	// Store system admin info
	string sysadmin;
	string sysadminEmail;

} store;

struct UserInfo {
	int userid = 0; // Unique user id
	string employeename;
	string username;
	string password;
	int privlev = 0; // no access
} seluser;

// Holds the inventory item data in a structure -- struct items stored in vector
struct InventoryData {
	int itemSKU = 0;
	string itemtype;
	string itemname;
	int numInStock = 0;
	float price = 0.00f;
};

// **** End of Structure Prototypes ****


// **** Function Prototypes ****

// Loads store information from file
void loadStoreInfo(StoreInfo* store); 
// Loads user data file, prompts login and authenticates them
void login(UserInfo* seluser);
// Load the inventory from file
void loadInventory(UserInfo* seluser, int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], unsigned const int InvSize);

// Display inventory items in range (from memory)
void displayInventory(vector<InventoryData> &items, unsigned const int InvSize, int rangemin, int rangemax);


// Display functions
void display_byStock(vector<InventoryData> &items, unsigned const int InvSize);
void display_bySKU(vector<InventoryData> &items, unsigned const int InvSize);
void display_byType(vector<InventoryData> &items, unsigned const int InvSize, string categories[]);


// Writes changes to file
bool writechanges(UserInfo* seluser, vector<InventoryData> &items, unsigned const int InvSize, unsigned int selindex);

// Edit inventory by item SKU
void edit_inventoryData(vector<InventoryData> &items, UserInfo* seluser, unsigned const int InvSize);

// Misc functions
char* getSystemDate(); // function for getting the system date
void appendLogFile(string msgType, string msgCode, string msgName, string username, string msgInfo);

// **** End of Function Prototypes ****


int main() {

	// Read in Store Info data
	StoreInfo store;
	loadStoreInfo(&store);

	// Create a banner message-of-the-day
	string motd = "WARNING: Unauthorized Access is Strictly Prohibited";

	// Welcome the customer / display message-of-the-day
	cout << "********************************************************************************" << endl;
	cout << "Welcome to the " << store.name << " Inventory Management System!" << endl;
	cout << motd << endl << endl;
	cout << "\tWe are located at:" << endl
		<< "\t\t" << store.address << ", " << store.town << ", " << store.state << " " << store.zip << endl << endl;
	cout << "\tOur phone number is:  " << endl
		<< "\t\t" << store.phone << endl << endl;

	cout << "\tManager: " << endl
		<< "\t\t" << store.manager << endl
		<< "\t\tID: " << store.managerID << endl
		<< "\t\tExt: " << store.managerExt << endl
		<< "\t\tEmail: " << store.managerEmail << endl << endl;
	cout << "\tSystem Admin:" << endl
		<< "\t\t" << store.sysadmin << endl
		<< "\t\tEmail: " << store.sysadminEmail << endl << endl;
	cout << "********************************************************************************" << endl;

	// Prompt authentication and read in users datafile
	cout << "Please authenticate before continuing" << endl << endl;
	UserInfo seluser;
	login(&seluser);
	// User is authenticated
	cout << "\tWelcome " << seluser.employeename << "!" << endl << endl;

	// PRIVILEGE LEVEL ASSIGNMENT
	// Allows for write-access in the Inventory File
	// Determine if user is a system admin or manager, assign a privilege level accordingly

	if (seluser.userid == 0) { // root account
		seluser.privlev = 3; // 3 - root access
	}
	else if ((seluser.userid == 1) || (seluser.userid == 2)) { // manager and system administator's personal accounts
		seluser.privlev = 2; // 2 - admin access (manager)
	}
	else {
		seluser.privlev = 1; // read/edit access, cannot edit price or SKU
		// 0 - no access
	}

	// END OF PRIVILEGE LEVEL ASSIGNMENT

	// Set max size of items in datafile
	unsigned const int InvSize = 25;

	//InventoryData items[inv_size];

	// Initialize the inventory in parallel arrays
	int SKU_arr[InvSize] = { 0 };
	string type_arr[InvSize];
	string name_arr[InvSize];
	int numInStock_arr[InvSize] = { 0 };
	float price_arr[InvSize] = { 0.00f };


	// Load our inventory into parallel arrays
	loadInventory(&seluser, SKU_arr, type_arr, name_arr, numInStock_arr, price_arr, InvSize);

	// Create a vector for each structure member of InventoryData to hold the parallel arrays
	std::vector<InventoryData> items;

	// Load parallel arrays into vector
	for (unsigned int i = 0; i < InvSize;) {

		// Set default in case of error
		items.push_back(InventoryData());

		items[i].itemSKU = SKU_arr[i];
		items[i].itemtype = type_arr[i];
		items[i].itemname = name_arr[i];
		items[i].numInStock = numInStock_arr[i];
		items[i].price = price_arr[i];

		i++;
	}



	// ************************************************************************************
	// ****** MAIN MENU ******

	// Bring input in as string in case input is longer than expected
	string confirmQuit = "n";
	string selection = "n";

	// Loop through menu until user CONFIRMS quitting
	while (toupper(confirmQuit[0]) != 'Y') {

		//MAIN MENU TABLE
		cout << "********************************************************************************" << endl;
		cout << "\t" << store.name << " Inventory Management System" << endl << endl;
		cout << "\tChoose a Selection : " << endl
			<< "\t\t 1) Display all inventory items" << endl
			<< "\t\t 2) Find item by Amount In Stock" << endl
			<< "\t\t 3) Find item by SKU Number" << endl
			<< "\t\t 4) Find item by Category" << endl // category = item type
			<< "\t\t 5) Edit inventory items" << endl // no bulk-changes allowed for simplicity
			<< "\t\t Q) Quit the program" << endl << endl << "\t\t > ";
		cin >> selection;
		cout << endl;
		cout << "********************************************************************************" << endl << endl;
		// END OF MAIN MENU TABLE


		// **** DISPLAY ALL ITEMS **** 
		if (selection[0] == '1') {
			// Set target lines minimum-maximum range to output
			int rangemin = 0;
			int rangemax = InvSize;

			// Control amount of data to output, don't allow too many lines
			if (InvSize > 1000) {
				rangemax = 1000;
				cout << endl << "Some lines omitted" << endl;
			}
			
			// INVENTORY DISPLAY HEADER
			cout << "______________________________________________________________________________________________" << endl; // Start of table
			cout << "SKU\tCategory\t\tItem Name\n #\tNumber in Stock\t\tPrice" << endl;

			// Display the inventory data for the selected indices
			displayInventory(items, InvSize, rangemin, rangemax);

			// INVENTORY DISPLAY FOOTER
			cout << "______________________________________________________________________________________________" << endl; // End of table
		}
		// **** END OF DISPLAY ALL ITEMS ****


		//  **** DISPLAY ITEMS BY AMOUNT IN STOCK **** 
		else if (selection[0] == '2') {
			display_byStock(items, InvSize);

		}
		//  **** END OF DISPLAY ITEMS BY AMOUNT IN STOCK **** 


		// **** DISPLAY ITEM BY SKU NUMBER **** 
		else if (selection[0] == '3') {
			display_bySKU(items, InvSize);

		}
		// **** END OF DISPLAY ITEM BY SKU NUMBER **** 


		// **** DISPLAY ITEM BY CATEGORY / ITEM TYPE ****

		else if (selection[0] == '4') {
			string categories[InvSize];
			display_byType(items, InvSize, categories);
		}
		// **** END OF DISPLAY ITEM BY CATEGORY / ITEM TYPE ****



		// **** EDIT INVENTORY ITEM BY SKU  ****
		else if (selection[0] == '5') {
			edit_inventoryData(items, &seluser, InvSize);

		}
		// **** END OF EDIT INVENTORY ITEM BY SKU  ****


		// **** USER SELECTED QUIT ****
		else if (toupper(selection[0]) == 'Q') {
			// Confirm the user wishes to quit before leaving
			cout << "\tAre you sure you want to quit?" << endl << "\t> ";
			cin >> confirmQuit;
			if ((toupper(confirmQuit[0]) == 'Y')) {
				exit(0); // Exit the program
			}
		}
		// **** END OF USER SELECTED QUIT ****

		// UNKNOWN INPUT
		else {
			cout << endl << "Sorry, your input was not recognized. Try again" << endl;
		}



		// **** MENU RETURN ****
		// Occurs once every loop, allows user to pause without the menu clogging up screen real estate
		// Do not return to menu screen until user is finished reading
		string pause = "n";
		do {
			cout << endl << "\tEnter 'q' to return to menu" << endl << "\t\t > ";
			cin >> pause;
		} while (pause[0] != 'q');

		// Clear the screen
		for (unsigned int i = 0; i < 50; i++) {
			cout << endl;
		}

		// **** END OF MENU RETURN ****

	} // End of menu loop

	// If the user hits here, they have successfully logged-out and the program will terminate
}




//************************STOREINFO FILE INPUT FUNCTION********************************//
void loadStoreInfo(StoreInfo* store)
{
	ifstream storeFile;
	//open a file in read mode
	storeFile.open("storeInfo.dat");

	cout << "Reading store information..." << endl;

	// Quit if the file cannot be parsed
	if (storeFile.fail()) {
		cout << "\aFatal Error, could not load store info!" << endl << "Please contact your system administrator." << endl;
		//ERROR	 -- file input error for storeInfo.dat
		appendLogFile("ERROR", "E3004", "READ_STORE", "GUEST", "FATAL: storeInfo.dat not found"); // User has not attempted login, considered a guest user (priv: 0)
		exit(0);
	}

	//bring the data in as strings to avoid an abnormal end
	string storename = "", storenumber = "", storeaddress = "", storetown = "", storestate = "", storezip = "", storephone = "",
		storemanager = "", managerID = "", managerExt = "", managerEmail = "", sysadmin = "", sysadminEmail = "", header = "";

	int i = 0; //line index of the file



	//loop through the file to get the single-record
	while (!storeFile.eof())
	{
		getline(storeFile, header, '\n'); // Discard the first line (header)
		getline(storeFile, storename, ',');
		getline(storeFile, storenumber, ',');
		getline(storeFile, storeaddress, ',');
		getline(storeFile, storetown, ',');
		getline(storeFile, storestate, ',');
		getline(storeFile, storezip, ',');
		getline(storeFile, storephone, ',');
		getline(storeFile, storemanager, ',');
		getline(storeFile, managerID, ',');
		getline(storeFile, managerExt, ',');
		getline(storeFile, managerEmail, ',');
		getline(storeFile, sysadmin, ',');
		getline(storeFile, sysadminEmail, '\n');
		i++;

	}
	// Load store's info into structure values to reflect the given input file
	store->name = storename;
	store->storenumber = stoi(storenumber);
	store->address = storeaddress;
	store->town = storetown;
	store->state = storestate;
	store->zip = storezip;
	store->phone = storephone;
	store->manager = storemanager;
	store->managerID = stoi(managerID);
	store->managerExt = stoi(managerExt);
	store->managerEmail = managerEmail;
	store->sysadmin = sysadmin;
	store->sysadminEmail = sysadminEmail;


	// Close the file
	storeFile.close();
	cout << "Store information loaded!" << endl << endl;

}
//************************END OF STOREINFO FILE INPUT FUNCTION********************************//

//************************USER AUTHENTICATION FUNCTION********************************//
void login(UserInfo* seluser)
{
	// **** USER INPUT ****
	// Take a user's login info from standard input
	string cinUser, cinPW;
	string validPW = "n"; // use string in case input is larger than expected

	do {
		cout << "\tEnter your username :" << endl << "\t> "; // indent the columns to make it easier to read
		cin >> cinUser;
		cout << "\tEnter your password :" << endl << "\t> ";
		cin >> cinPW;
		cout << endl << "\tIs the above information correct? (Y/N)" << endl << "\t> ";
		cin >> validPW;
		cout << endl;

	} while (toupper(validPW[0]) != 'Y'); // If the first item in the string is "Y" ("Y" for (y*)es), break.
	// **** END OF USER INPUT ****


	//	**** FILE INPUT	****
	ifstream usersFile;
	//opens the stored user credentials file in read mode
	usersFile.open("users.dat");
	cout << endl << "********************************************************************************" << endl;
	cout << "Checking user information..." << endl;

	// Check if the file can be opened
	if (usersFile.fail()) {
		cout << "\aFatal Error, could not find account info from file!" << endl << "Please contact your system administrator." << endl;
		// Error -- file input error for Users.dat, <ACCOUNT> is the username credentials the user tried to use -- consider the user still a guest
		appendLogFile("ERROR", "E2003", "READ_USERS", "Guest", "FATAL: Users.dat not found");
		exit(0);
	}

	//bring the data in as strings to avoid an abnormal end
	string userid = "", employeename = "", username = "", password = "", header = "";

	bool userauth = 0; // bool to keep track of if the user is authenticated or not
	// Check username-password combos from file input - stop parsing and return current line if data matches input

	getline(usersFile, header, '\n'); // Discard the first line (header)
	while (!usersFile.eof())
	{
		getline(usersFile, userid, ',');
		getline(usersFile, employeename, ',');
		getline(usersFile, username, ',');
		getline(usersFile, password, '\n');

		if ((username == cinUser) && (password == cinPW)) {
			// Load selected user's info into structure values to reflect the given input file
			seluser->userid = stoi(userid);
			seluser->employeename = employeename;
			seluser->username = username;
			seluser->password = password;

			// User is authenticated, exit the loop
			userauth = 1;
			break; // stop parsing if user is logged in
		}
	}
	// Close the file
	usersFile.close();
	//	**** END OF FILE INPUT ****


	// If no matching data was found from the user's input, quit the program
	if (userauth == 0) {
		// Play an audible alert tone to spook- I mean- alert the exiting user
		cout << "\a";
		cout << "\tSorry, your account credentials could not be verified." << endl << "\tProgram exiting...\n\n\n";

		// Log Warn -- user unsuccessful login attempt, with the username credentials the user attempted to use
		appendLogFile("WARN", "W6001", "FAILED_LOGIN", cinUser, "User login unsuccessful");
		exit(0);
	}
	else {
		cout << "Credentials Verified!" << endl << endl;
		// Log Info -- user successful login
		appendLogFile("INFO", "I5001", "AUTH_LOGIN", seluser->username, "User login successful");
	}
}
//*************************END OF USER AUTHENTICATION FUNCTION********************************//


//************************INVENTORY FILE INPUT FUNCTION********************************//
void loadInventory(UserInfo* seluser, int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], unsigned const int InvSize)
{
	ifstream inventoryFile;
	//open a file in read mode
	inventoryFile.open("inventory.dat");

	cout << "Reading inventory..." << endl;

	// Quit if the file cannot be parsed
	if (inventoryFile.fail()) {
		cout << "\aFatal Error, could not load inventory!" << endl << "Please contact your system administrator." << endl;
		// Error -- file input error for inventory.dat
		appendLogFile("ERROR", "E1001", "READ_INV", seluser->username, "FATAL : Inventory.dat not found");
		exit(0);
	}

	//bring the data in as strings to avoid an abnormal end
	string itemSKU = "", itemtype = "", itemname = "", numInStock = "", price = "", header = "";

	unsigned int n = 0; //line index of the file

	//loop through the file to get the single-record
	getline(inventoryFile, header, '\n'); // Discard the first line (header)
	while (!inventoryFile.eof())
	{
		getline(inventoryFile, itemSKU, ',');
		getline(inventoryFile, itemtype, ',');
		getline(inventoryFile, itemname, ',');
		getline(inventoryFile, numInStock, ',');
		getline(inventoryFile, price, '\n');

		// If we are not out of the parallel array index range and there's data on the line
		if ((n < InvSize) && (itemSKU != "")) {
			// Load inventory system info into structure values to reflect the given input file
			SKU_arr[n] = stoi(itemSKU);
			type_arr[n] = itemtype;
			name_arr[n] = itemname;
			numInStock_arr[n] = stoi(numInStock);
			price_arr[n] = stof(price);
		}
		else {
			// IN CASE our constant size of our array is out-of-bounds (too low!),
			// If there's data on the line,  inform the user there's potential data loss in the file
			if (itemSKU != "") {
				cout << "[!]\t<line omitted error>\t Item SKU: " << itemSKU << "\t...\t <- Potential loss of data on line " << n << endl;
				// Error -- file input error at given line index, potential data loss
				// Get error Message for line number
				string errMsg = "ERR : Inventory.dat - potential data loss on line " + to_string(n);
				appendLogFile("ERROR", "E1002", "READ_INV_LINE", seluser->username, errMsg);
			}
		}

		// Go to next line / next inventory item
		n++;
	}

	// Close the file
	inventoryFile.close();
	cout << "Inventory loaded!" << endl << endl;
	// Log Info-- user successfully loaded inventory file
	appendLogFile("INFO", "I4001", "LOAD_INV", seluser->username, "User loaded inventory.dat");

}
//************************END OF INVENTORY FILE INPUT FUNCTION********************************//



//************************DISPLAY DATA FUNCTION********************************//
// Reads inventory from memory, formats it into a table
void displayInventory(vector<InventoryData> &items, unsigned const int InvSize, int rangemin, int rangemax) {

	// Print every item in a table given the range of indices to print
	for (int n = rangemin; n < rangemax;) {
		// IF an item has no listed price, do NOT put it in the system -- Avoids accidentally adding free items (that could be exploited by customers!)
		if (!items[n].price == 0.00f) {
			cout << "______________________________________________________________________________________________" << endl;
			cout << " " << items[n].itemSKU << "\t" << items[n].itemtype << "\t\t" << items[n].itemname << endl 
				<< "  \tStock: " << items[n].numInStock << "\t\tPrice: $" << fixed << setprecision(2) << items[n].price << endl;
		}
		n++;
	}
}
//************************END OF DISPLAY DATA FUNCTION********************************//



//************************DISPLAY DATA BY AMOUNT IN STOCK FUNCTION********************************//
// MENU OPTION 2
void display_byStock(vector<InventoryData> &items, unsigned const int InvSize) {
	int stockMin = 0;
	int stockMax = 1000;

	bool inputIsValid = 0; // Keep track of if the input is valid

	do {
		// Set target lines minimum-maximum range to output
		cout << "\tSelect minimum range in stock" << "(Min: 0)" << endl << "\t> ";
		cin >> stockMin;

		cout << "\tSelect maximum range in stock" << endl << "\t> ";
		cin >> stockMax;

		// Retry if stock minimum or maximum is less than 0, or if minimum input is greater than the maximum input value.
		if ((stockMin < 0) || (stockMin > stockMax) || (stockMax < 0)) {
			cout << "\tInput range is not valid! Please try again" << endl << endl;
		}
		else {
			inputIsValid = 1; // Break out of loop
		}
	} while (!inputIsValid);

	// INVENTORY DISPLAY HEADER
	cout << "______________________________________________________________________________________________" << endl; // Start of table
	cout << "SKU\tCategory\t\tItem Name\n #\tNumber in Stock\t\tPrice" << endl;

	// Set default search range sizes
	int rangemin = 0;
	int rangemax = InvSize;

	// Control the amount to output, don't allow too many lines
	if (InvSize > 1000) {
		rangemax = 1000;
		cout << endl << "Some lines omitted" << endl;
	}

	// Boolean to return "NOT FOUND" if search criteria has 0 results
	bool infoFound = 0;

	// Display items if in given range
	for (unsigned int i = 0; i < InvSize;) {
		// Check if the item at each index fits the search criteria
		if ((items[i].numInStock > stockMin) && (items[i].numInStock <= stockMax)) {
			rangemin = i;
			rangemax = i + 1;
			// Display the inventory data for the selected index
			displayInventory(items, InvSize, rangemin, rangemax);
			infoFound = 1;
		}
		i++;
	}
	cout << "______________________________________________________________________________________________" << endl; // End of table

	// If no matches from search, display the search keys
	if (!infoFound) {
		cout << endl << "\tNo found matches for items with:" << endl << "\t" << stockMin << " in stock to " << stockMax << " in stock" << endl << endl
			<< "\tTry refining your search" << endl << endl;
	}

}
//************************END OF DISPLAY DATA BY AMOUNT IN STOCK FUNCTION********************************//


//************************DISPLAY DATA BY SKU FUNCTION********************************//
void display_bySKU(vector<InventoryData> &items, unsigned const int InvSize) {
	int selSKU = 0;
	cout << "\tEnter a SKU number to search for:" << endl << "\t> ";
	cin >> selSKU;

	// INVENTORY DISPLAY HEADER
	cout << endl << endl << "SELECTED SKU : " << selSKU << endl;
	cout << "______________________________________________________________________________________________" << endl; // Start of table
	cout << "SKU\tCategory\t\tItem Name\n #\tNumber in Stock\t\tPrice" << endl;

	// Set default search range sizes
	int rangemin = 0;
	int rangemax = 1; // initialize at 1 since we're looping through each item

	// Boolean to return "NOT FOUND" if search criteria has 0 results
	bool infoFound = 0;

	// Display all matching SKUs
	for (unsigned int i = 0; i < InvSize;) {
		if (items[i].itemSKU == selSKU) {
			rangemin = i;
			rangemax = i + 1;
			displayInventory(items, InvSize, rangemin, rangemax);
			infoFound = 1;
		}
		i++;
	}

	cout << "______________________________________________________________________________________________" << endl; // End of table

	// If no matches from search, display the search key
	if (!infoFound) {
		cout << endl << "\tNo found matches for items with:" << endl << "\tSKU: " << selSKU << endl << endl
			<< "\tTry refining your search" << endl << endl;
	}
}
//************************END OF DISPLAY DATA BY SKU FUNCTION********************************//


//************************DISPLAY DATA BY TYPE FUNCTION********************************//
void display_byType(vector<InventoryData> &items, unsigned int InvSize, string categories[]) {
	string cat = "";

	unsigned int index = 0; // Keep track of categories index

	// REVERSE SORTING ALGORITHM / DUPLICATE REMOVAL - Loops through all items in the inventory and captures matches (itemtype) only once
	for (unsigned int i = 0; i < InvSize;) {
		// Set a variable for the itemtype category name
		cat = items[i].itemtype;

		if (cat == "") { // In case of blank data, don't bother counting it
			i++;
		}
		else {
			int occ = 0; // Get number of times the category occurs from current index until the end of the array


			// OCCURENCE COUNTER - Loops through the *remaining* inventory items to see how many times the category type occurs
			// Include our given item (at index 'i') as the first occurence of the category (so we don't count items we already did previously!)
			for (unsigned int b = i; b < InvSize;) {
				// If the category type occurs at the 'b' index, add one to the occurence counter
				if (cat == items[b].itemtype) {
					occ += 1;
				}
				b++; // Get the next one of our remaining categories
			}
			// END OF OCCURENCE COUNTER

			// If the occurence counter only counted only ONE item in the remaining indices, the only occuring item is at index 'i'- also known as "cat"
			if (occ == 1) {
				// Add it to our categories list, and increase our categories index by 1 for the next item!
				categories[index] = cat;
				index++;
			}

			i++; // Get the next category name from the inventory
		}
	}
	// END OF REVERSE SORTING ALGORITHM

	// MAKE QUERY BY CATEGORY
	cout << "\tPick a category type to display (Select by option number) :" << endl;
	// Print all categories - index variable is now the size of the categories array
	for (unsigned int i = 0; i < index;) {
		if (categories[i] != "") {
			cout << "\t\t " << i + 1 << ")  " << categories[i] << endl; // i+1 to convert index to position (index = n-1)
		}
		i++;
	}
	unsigned int selcatnum = 0;
	do {
		cout << endl << "\t\t > ";
		cin >> selcatnum;

		if ((selcatnum < 1) || (selcatnum > index)) {
			cout << endl << "\t\t Input not recognized, select a category from 1 to " << index;
		}
	} while ((selcatnum < 1) || (selcatnum > index));

	// Get selected category name
	string selcat = categories[selcatnum - 1]; // -1 to convert position back to index

	// INVENTORY DISPLAY HEADER
	cout << endl << endl << "SELECTED CATEGORY : " << selcat << endl;
	cout << "______________________________________________________________________________________________" << endl; // Start of table
	cout << "SKU\tCategory\t\tItem Name\n #\tNumber in Stock\t\tPrice" << endl;


	// Set default minimum/maximum range values
	int rangemin = 0;
	int rangemax = InvSize;

	// Print every item with the same category name
	for (unsigned int i = 0; i < InvSize;) {
		if (items[i].itemtype == selcat) {
			rangemin = i;
			rangemax = i + 1;
			displayInventory(items, InvSize, rangemin, rangemax);
		}
		i++; // Get next item by category
	}
	cout << "______________________________________________________________________________________________" << endl; // End of table
	// END OF MAKE QUERY BY CATEGORY
}
//************************END OF DISPLAY DATA BY TYPE FUNCTION********************************//



//************************EDIT INVENTORY ITEM FUNCTION********************************//
void edit_inventoryData(vector<InventoryData> &items, UserInfo* seluser, unsigned const int InvSize){

	// Boolean to see if user 
	bool selectNewItem = 1;
	int selSKU = 0; // selected SKU number
	
	
	while (selectNewItem) {
		// Boolean to return "NOT FOUND" if search criteria has 0 results
		bool infoFound = 0;

		// ** SEARCH BY SKU USER INPUT **
		do {
			// SELECTED SKU NUMBER USER INPUT
			cout << "\tEnter the item SKU number:" << endl << "\t> ";
			cin >> selSKU;

			// Find all matching SKUs
			for (unsigned int i = 0; i < InvSize;) {
				if ((items[i].itemSKU == selSKU) && (items[i].itemname != "")) { // Look for index with given SKU, exclude blank data
					infoFound = 1;
				}
				i++;
			}
			// If no matches from search, display the search key
			if (!infoFound) {
				cout << endl << "\tNo found matches for items with:" << endl << "\tSKU: " << selSKU << endl << endl
					<< "\tTry refining your search" << endl << endl;
			}
		} while (!infoFound);
		// ** END OF SELECTED SKU NUMBER USER INPUT **


		// ** OUTPUT ITEM TO MENU SCREEN **
		// INVENTORY DISPLAY HEADER
		cout << "\t\t Item SKU Found!" << endl;
		cout << "______________________________________________________________________________________________" << endl; // Start of table
		cout << "SKU\tCategory\t\tItem Name\n #\tNumber in Stock\t\tPrice" << endl;


		// Set default minimum/maximum range values
		int rangemin = 0;
		int rangemax = InvSize;

		// Capture item's index
		unsigned int selindex = 0;

		for (unsigned int i = 0; i < InvSize;) {
			if (items[i].itemSKU == selSKU) {
				rangemin = i;
				rangemax = i + 1;
				displayInventory(items, InvSize, rangemin, rangemax);
				selindex = i;
			}
			i++;
		}
		cout << "______________________________________________________________________________________________" << endl; // End of table
		// ** END OF OUTPUT ITEM TO MENU SCREEN **


		// CHANGE ITEM INFORMATION SELECTION
		bool moreChanges = 1; // Keep track of if the user wants more changes to be made
		bool saveChanges = 0; // Keep track of if the user has exited, save the data if true
		bool discardChanges = 0; // Keep track of if user has exited, erase changes if true
		string selCategory = ""; // Confirm changes input


		// Load default data in from structures
		string newName = items[selindex].itemname;
		string newType = items[selindex].itemtype;
		int newSKU = items[selindex].itemSKU;
		int newAmtInStock = items[selindex].numInStock;
		float newPrice = items[selindex].price;

		// ** APPEND CHANGES MENU **
		do { // Until user exits menu, stay in menu
			cout << endl << "\tEnter item to change: " << endl <<
				"\t1) Item Name" << endl <<
				"\t2) Item SKU" << endl <<
				"\t3) Item Category" << endl <<
				"\t4) Amount in stock" << endl <<
				"\t5) Item Price" << endl <<
				"\t6) Delete item" << endl << endl <<
				"\tQ) Quit" << endl << endl << "\t> ";
			cin >> selCategory;
			cout << endl;

			// Change category by selected type

			// USER CHANGES ITEM NAME
			if (selCategory[0] == '1') {
				if (seluser->privlev >= 1) { // Checks user privileges -- See definition 
					cout << "\tEnter the new item name" << endl << "\t> ";
					cin.ignore();
					getline(cin, newName);
				}
				else {
					cout << "\a\tError : Unauthorized command!" << endl;
					// Log Warn-- user unsucessfully tried to change an item without correct privilege level
					string warnMsg = "User with privilege level " + to_string(seluser->privlev) + " attempted to change SKU " + to_string(items[selindex].itemSKU) + " name";
					appendLogFile("WARN", "W6002", "UNAUTH_PRIV", seluser->username, warnMsg);
				}
			}

			// USER CHANGES ITEM SKU
			else if (selCategory[0] == '2') {
				if (seluser->privlev >= 2) { // Checks user privileges
					cout << "\tEnter the new item SKU" << endl << "\t> ";
					cin >> newSKU;
				}
				else {
					cout << endl << "\a\tError : Unauthorized command!" << endl;
					string warnMsg = "User with privilege level " + to_string(seluser->privlev) + " attempted to change SKU " + to_string(items[selindex].itemSKU) + " SKU";
					appendLogFile("WARN", "W6002", "UNAUTH_PRIV", seluser->username, warnMsg);
				}
			}

			// USER CHANGES ITEM TYPE
			else if (selCategory[0] == '3') {
				if (seluser->privlev >= 1) { // Checks user privileges
					cout << "\tEnter the new category" << endl << "\t> ";
					cin.ignore();
					getline(cin, newType);
				}
				else {
					cout << endl << "\a\tError : Unauthorized command!" << endl;
					string warnMsg = "User with privilege level " + to_string(seluser->privlev) + " attempted to change SKU " + to_string(items[selindex].itemSKU) + " item type";
					appendLogFile("WARN", "W6002", "UNAUTH_PRIV", seluser->username, warnMsg);
				}
			}

			// USER CHANGES AMOUNT OF ITEM IN STOCK
			else if (selCategory[0] == '4') {
				if (seluser->privlev >= 1) { // Checks user privileges
					cout << "\tEnter the current amount in stock" << endl << "\t> ";
					cin >> newAmtInStock;
				}
				else {
					cout << endl << "\a\tError : Unauthorized command!" << endl;
					string warnMsg = "User with privilege level " + to_string(seluser->privlev) + " attempted to change SKU " + to_string(items[selindex].itemSKU) + " amount in stock";
					appendLogFile("WARN", "W6002", "UNAUTH_PRIV", seluser->username, warnMsg);
				}
			}

			// USER CHANGES PRICE
			else if (selCategory[0] == '5') {
				if (seluser->privlev >= 2) { // Checks user privileges -- Only allow managers+ to edit item's price
					cout << "\tEnter the new price" << endl << "\t> $";
					cin >> newPrice;

					if (newPrice <= 0.00) {
						cout << "\a\tError : Item cost cannot be negative or 0!" << endl;
						newPrice = items[selindex].price; // re-initialize the newPrice
						string warnMsg = "User with privilege level " + to_string(seluser->privlev) + " attempted to change SKU " + to_string(items[selindex].itemSKU) + " price to a bad value";
						appendLogFile("WARN", "W6002", "UNAUTH_PRIV", seluser->username, warnMsg);
					}
				}
				else {
					cout << endl << "\a\tError : Unauthorized command!" << endl;
					string warnMsg = "User with privilege level " + to_string(seluser->privlev) + " attempted to change SKU " + to_string(items[selindex].itemSKU) + " price";
					appendLogFile("WARN", "W6002", "UNAUTH_PRIV", seluser->username, warnMsg);
				}
			}

			// USER DELETES ITEM
			else if (selCategory[0] == '6') {
				if (seluser->privlev >= 2) { // Checks user privileges -- Only allow managers+ to delete item

					// Confirm deletion
					string confirmDelete;
					cout << "\tAre you sure you want to delete " << newName << " from the inventory? (Y/N)" << endl << "\t> ";
					cin >> confirmDelete;

					if (toupper(confirmDelete[0]) == 'Y') {
						// Wipe all data on selected item
						newPrice = 0.00; // Since price listed as "free", it wont appear in the system (NO FREE STUFF!)
						newAmtInStock = 0;
						newType = "";
						newName = "";
						newSKU = 0;
					}
				}
				else {
					cout << endl << "\a\tError : Unauthorized command!" << endl;
					string warnMsg = "User with privilege level " + to_string(seluser->privlev) + "attempted to delete SKU " + to_string(items[selindex].itemSKU);
					appendLogFile("WARN", "W6002", "UNAUTH_PRIV", seluser->username, warnMsg);
				}
			}

			// USER QUIT SCREEN
			else if (toupper(selCategory[0]) == 'Q') {
				cout << endl << "\tAre you sure you wish to quit (Y/N)?" << endl << "\t> ";

				string confirmQuit;
				cin >> confirmQuit;

				cout << endl << endl;
				if ((toupper(confirmQuit[0]) == 'Y')) { // If yes, bring into save loop, if no, bring back to menu

					// CONFIRM SAVE LOOP
					do { // don't allow the user to exit without confirming save/discard
						cout << "\tWould you like to save (Y/N)?" << endl << "\t> ";
						string confirmSave;
						cin >> confirmSave;
						if (toupper(confirmSave[0]) == 'Y') {
							saveChanges = 1; // Exit the save loop - save changes
						}
						else if (toupper(confirmSave[0]) == 'N') {
							discardChanges = 1; // Exit the save loop - discard changes
						}
						else {
							cout << endl << "\tInput not recognized. Please select Yes or No." << endl << endl;
						}
					} while ((!saveChanges) && (!discardChanges)); // Keep in menu until we know if we are saving or discarding changes
					moreChanges = 0; // Exit the menu
					// END OF CONFIRM SAVE LOOP
				}
			}
			// END OF USER QUIT SCREEN

			// USER ENTERED BAD INPUT
			else {
				cout << endl << "Input not recognized, choose a selection from 1 to 5" << endl << endl;
			}


			// Display appended item information before changes wrote
			if (newName != "") {
				cout << endl << "ITEM INFORMATION :" << endl;
				cout << "______________________________________________________________________________________________" << endl; // Start of table
				cout << " " << newSKU << "\t" << newType << "\t\t" << newName << "\n  \tStock: " << newAmtInStock << "\t\tPrice: $" << fixed << setprecision(2) << newPrice << endl;
			}
			else { // items with no name are deleted
				cout << endl << "\tITEM DELETED!" << endl;
			}
			cout << "______________________________________________________________________________________________" << endl; // End of table
		} while (moreChanges);
		// ** END OF APPEND CHANGES MENU **



		// DONE appending changes, write to file

		// ** WRITE CHANGES TO FILE **

		// Keep track of if the file can be written
		bool writeSuccess = 0;


		// APPLY APPENDED CHANGES TO FILE
		if (saveChanges == 1) {
			do {
				// APPLY CHANGES IN PROGRAM MEMORY
				items[selindex] = { newSKU, newType, newName, newAmtInStock, newPrice };

				// WRITE CHANGES TO FILE
				writeSuccess = writechanges(seluser, items, InvSize, selindex);
				if (!writeSuccess) {
					string confirmDiscard;
					cout << endl << endl << "\a\tError writing to inventory file! Contact your system administrator." << endl << endl; // Already wrote error to log file (Error Code E1003)
					cout << "\tWould you like to discard your changes? (Y/N)" << endl << endl << "\t> ";
					cin >> confirmDiscard;

					if (toupper(confirmDiscard[0]) == 'N') {
						discardChanges = 1;
					}

				}
			} while ((!writeSuccess) && (!discardChanges)); // Do not allow user to break from loop until changes discarded or write success
		}
		else {
			cout << endl << "\t Above changes discarded!" << endl;
		}
		// ** END OF WRITE CHANGES TO FILE **

		// ** EDIT NEW ITEM SELECTION **
		// Ask if user wishes to edit another item by SKU
		cout << endl << "\tDo you wish to edit another inventory item? (Y/N)" << endl << "\t> ";
		string editMoreItems;
		cin >> editMoreItems;

		// If no more changes necessary, quit
		if (toupper(editMoreItems[0]) == 'N') {
			selectNewItem = 0;
		}
		// ** END OF EDIT NEW ITEM SELECTION **
	}
}
//************************END OF EDIT INVENTORY ITEM FUNCTION********************************//










//************************WRITE INVENTORY CHANGES FUNCTION********************************//
bool writechanges(UserInfo* seluser, vector<InventoryData> &items, unsigned const int InvSize, unsigned int itemIndex) {
	
	//open a file in write mode
	ofstream inventoryFile;

	cout << "Writing changes to inventory... Do not exit." << endl;

	inventoryFile.open("inventory.dat"); //write to the inventory file

	// Quit if the file cannot be parsed
	if (inventoryFile.fail()) {
		cout << "\aError, could not write to inventory!" << endl << "Please contact your system administrator." << endl;
		// Error -- file output error for inventory.dat // does not exit program
		appendLogFile("ERROR", "E1003", "WRITE_INV", seluser->username, "ERR: Cannot write to inventory.dat");
		return 0; // Return false, allows user to re-try
	}


	// Parse the file, write each inventory item on each line
	string header = "SKU, ITEM TYPE, ITEM NAME, ITEMS IN STOCK, PRICE\n";
	inventoryFile << header;
	for (unsigned int i = 0; i < InvSize;) //loop through the inventory - skip the header
	{
		if (items[i].price != 0.00f) { // Do not include blank inventory items
			inventoryFile << items[i].itemSKU << "," << items[i].itemtype << "," << items[i].itemname << "," <<
				items[i].numInStock << "," << items[i].price << endl;
		}
		i++;
	}

	// Close the file
	inventoryFile.close();
	cout << "Inventory data file updated!" << endl;
	// Log Info -- user successfully wrote changes to inventory file
	string msgInfo = "User changed item at index: " + to_string(itemIndex) + " SKU: " + to_string(items[itemIndex].itemSKU);
	appendLogFile("INFO", "I4002", "WRITE_INV", seluser->username, msgInfo);

	return 1; // Return true, user was able to write to file
}

//************************WRITE INVENTORY CHANGES FUNCTION********************************//



//************************GET SYSTEM DATE FUNCTION********************************//
char* getSystemDate()
{
	//SYSTEM DATE AND TIME
	time_t now = time(0); // current date/time based on current system
	char* todaysdate = ctime(&now); // convert to string form
	return todaysdate;
}

//************************END OF GET SYSTEM DATE FUNCTION********************************//


//************************APPEND LOG FUNCTION********************************//
// Logs correct/incorrect password attempts, user privilege errors, file i/o errors
// See below function for declarations of ERROR, INFO, and WARN
void appendLogFile(string msgType, string msgCode, string msgName, string username, string msgInfo ) { // uses itemIndex as more of a kwarg
	// Takes generic event info to print to the log, and the date/time of the event
	string dateTime = getSystemDate();
	string monDD = dateTime.substr(4, 6);
	string timeHMS = dateTime.substr(11, 8); // Includes an extra whitespace
	string YYYY = dateTime.substr(20, 4); // Do not include newline character

	//open a file in write mode
	ofstream logFile;

	// Silently write to the file
	logFile.open("log.dat",ios::app); //append to the log file

	// Quit if the file cannot be parsed
	if (logFile.fail()) {
		cout << "\aFatal Error, could not write to system log!" << endl << "Please contact your system administrator." << endl;
		// No log file I/O error listed, (<-- because how would we be able to write it the log file!)
		exit(0); // DO NOT allow any further usage of the program if the log file cannot be opened
	}
	else {
		// Write the given log output in specified format to the next line in the file

		// TIME, MESSAGE TYPE, MESSAGE/ERROR CODE, MESSAGE/ERROR, USER/ACCOUNT, MESSAGE INFO
		logFile << monDD << " " << YYYY << " " << timeHMS << ", " << msgType << ", " << msgCode << ", " << msgName << ", " << username << ", " << msgInfo << "\n";
	}
	
	

	// Close the file
	logFile.close();
}
/*
// LOG FILE SYNTAX
// TIME, MESSAGE TYPE, MESSAGE/ERROR CODE, MESSAGE/ERROR, USER/ACCOUNT, MESSAGE INFO

	***********************											*** LOG FILE MESSAGES ***																	     ***********************
MESSAGE TYPES	MSG CODE	MSG NAME		USERNAME	MSG INFO 															|   Description
																															| * critical component of the program not loaded, program may exit *
	ERROR		E1001		READ_INV		USER		FATAL: Inventory.dat not found										|	-- file input error for inventory.dat
	ERROR		E1002		READ_INV_LINE	USER		ERR: Inventory.dat - potential data loss on line <index>			|	-- file input error at given line index, potential data loss
	ERROR		E1003		WRITE_INV		USER		ERR: Cannot write to inventory.dat									|	-- file output error for inventory.dat // does not exit program
	ERROR		E2003		READ_USERS		<ACCOUNT>	FATAL: Users.dat not found											|	-- file input error for Users.dat, <ACCOUNT> is the username credentials the user tried to use
	ERROR		E3004		READ_STORE		GUEST		FATAL: storeInfo.dat not found										|	-- file input error for storeInfo.dat, "GUEST" because the user has not prompted to be logged in
																															| * user changed item or logged in successfully *
	INFO		I4001		LOAD_INV		USER		User loaded inventory.dat											|	-- user successfully loaded inventory file
	INFO		I4002		WRITE_INV		USER		User changed item at <index>, to <new SKU>							|	-- user successfully wrote changes to inventory file, log changed item
	INFO		I5001		AUTH_LOGIN 		USER		User login successful												|	-- user successful login
																															| * user tried doing something they're not allowed to do, warn *
	WARN		W6001		FAILED_LOGIN	<ACCOUNT>	User login unsuccessful												|	-- user unsuccessful login attempt, <ACCOUNT> is the username credentials the user tried to use
	WARN		W6002		UNAUTH_PRIV		USER		User with priv <privlev> attempted to change SKU <SKU>(...)		 	|	-- user unsucessfully tried to change an item <SKU> number without correct privilege level


	// No log file I/O error listed, (<-- because how would we be able to write it the log file!)
	// <ACCOUNT> is ambigous, it does NOT mean the user credentials have been verified
	// LOCAL SYSTEM TIME is the output to the first column of the log file

	****************************************************************************************************************************************************************************************
*/
//************************END OF APPEND LOG FUNCTION********************************//
