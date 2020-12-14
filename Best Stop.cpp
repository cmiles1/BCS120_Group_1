// Best Stop.cpp : This file contains the 'main' function for an inventory management system for an example company, called "Best Stop".
//				   This project was created for a Group Project in a C++ programming course

/*
Program Authors	:
					Christopher Miles
					**Put Your Names Up Here If You Edit This File**

Other Collaborators :
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
int main();
// Loads store information from file
void loadStoreInfo(StoreInfo* store); 
// Loads user data file, prompts login and authenticates them
void login(UserInfo* seluser);
// Load the inventory from file
void loadInventory(int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], int InvSize);
void displayInventory(vector<InventoryData> items, int InvSize, int rangemin, int rangemax);
bool writechanges(vector<InventoryData> items, int InvSize, unsigned int selindex);

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

	// Determine if user is a system admin or manager, assign a privilege level accordingly
	int privlev = 1; // 1 - user access
	if (seluser.userid == 0) { // root account
		privlev = 3; // 3 - root access
	}
	else if ((seluser.userid == 1) || (seluser.userid == 2)) { // manager and system administator's personal accounts
		privlev = 2; // 2 - admin access (manager)
	}

	// END OF PRIVILEGE LEVEL ASSIGNMENT

	// Set max size of items in datafile
	const int InvSize = 20;

	//InventoryData items[inv_size];

	// Initialize the inventory in parallel arrays
	int SKU_arr[InvSize] = { 0 };
	string type_arr[InvSize];
	string name_arr[InvSize];
	int numInStock_arr[InvSize] = { 0 };
	float price_arr[InvSize] = { 0.00f };


	// Load our inventory into parallel arrays
	loadInventory(SKU_arr, type_arr, name_arr, numInStock_arr, price_arr, InvSize);

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
			<< "\t\t 2) Query inventory by Amount In Stock" << endl
			<< "\t\t 3) Query inventory by SKU Number" << endl
			<< "\t\t 4) Query inventory by Category" << endl // category = item type
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

			// INVENTORY DISPLAY HEADER
			cout << "______________________________________________________________________________________________" << endl; // Start of table
			cout << "SKU\tCategory\t\tItem Name\n #\tNumber in Stock\t\tPrice" << endl;

			// Display the inventory data for the selected indices
			displayInventory(items, InvSize, rangemin, rangemax);
			cout << "______________________________________________________________________________________________" << endl; // End of table
		}
		// **** END OF DISPLAY ALL ITEMS ****






		//  **** DISPLAY ITEMS BY AMOUNT IN STOCK **** 
		else if (selection[0] == '2') {
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
		//  **** END OF DISPLAY ITEMS BY AMOUNT IN STOCK **** 






		// **** DISPLAY ITEM BY SKU NUMBER **** 
		else if (selection[0] == '3') {
			int selSKU = 0;
			cout << "\tEnter a SKU number to search for:" << endl << "\t> ";
			cin >> selSKU;

			// INVENTORY DISPLAY HEADER
			cout << endl << endl << "SELECTED SKU : " << selSKU << endl;
			cout << "______________________________________________________________________________________________" << endl; // Start of table
			cout << "SKU\tCategory\t\tItem Name\n #\tNumber in Stock\t\tPrice" << endl;

			// Set default search range sizes
			int rangemin = 0;
			int rangemax = InvSize;

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
		// **** END OF DISPLAY ITEM BY SKU NUMBER **** 






		// **** DISPLAY ITEM BY CATEGORY / ITEM TYPE ****

		else if (selection[0] == '4') {

			string cat = "";
			string categories[InvSize];

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
				cout << endl << endl << "\t\t > ";
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
		// **** END OF DISPLAY ITEM BY CATEGORY / ITEM TYPE ****









		// **** EDIT INVENTORY ITEM BY SKU  ****
		else if (selection[0] == '5') {
			// Boolean to return "NOT FOUND" if search criteria has 0 results
			bool infoFound = 0;

			int selSKU = 0; // selected SKU number

			// SELECTED SKU NUMBER USER INPUT
			do {
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
			// END OF SELECTED SKU NUMBER USER INPUT

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

			// APPEND CHANGES MENU
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
					if (privlev >= 1) { // Checks user privileges -- See definition 
						cout << "\tEnter the new item name" << endl << "\t> ";
						cin >> newName;
					}
					else {
						cout << "\a\tError : Unauthorized command!" << endl;
					}
				}
				// USER CHANGES ITEM SKU
				else if (selCategory[0] == '2') {
					if (privlev >= 1) { // Checks user privileges
						cout << "\tEnter the new item SKU" << endl << "\t> ";
						cin >> newSKU;
					}
					else {
						cout << endl << "\a\tError : Unauthorized command!" << endl;
					}
				}
				// USER CHANGES ITEM TYPE
				else if (selCategory[0] == '3') {
					if (privlev >= 1) { // Checks user privileges
						cout << "\tEnter the new category" << endl << "\t> ";
						cin >> newType;
					}
					else {
						cout << endl << "\a\tError : Unauthorized command!" << endl;
					}
				}
				// USER CHANGES AMOUNT OF ITEM IN STOCK
				else if (selCategory[0] == '4') {
					if (privlev >= 1) { // Checks user privileges
						cout << "\tEnter the current amount in stock" << endl << "\t> ";
						cin >> newAmtInStock;
					}
					else {
						cout << endl << "\a\tError : Unauthorized command!" << endl;
					}
				}
				// USER CHANGES PRICE
				else if (selCategory[0] == '5') {
					if (privlev >= 2) { // Checks user privileges -- Only allow managers+ to edit item's price
						cout << "\tEnter the new price" << endl << "\t> $";
						cin >> newPrice;

						if (newPrice <= 0.00) {
							cout << "\a\tError : Item cost cannot be negative or 0!" << endl;
							newPrice = items[selindex].price; // re-initialize the newPrice
						}
					}
					else {
						cout << endl << "\a\tError : Unauthorized command!" << endl;
					}
				}
				else if (selCategory[0] == '6') {
					if (privlev > -2) { // Checks user privileges -- Only allow managers+ to delete item

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
					}
				}
				// USER QUITS
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
				// USER ENTERED BAD INPUT
				else {
					cout << endl << "Input not recognized, choose a selection from 1 to 5" << endl << endl;
				}

				// Display appended item information after changes made
				if (newName != "") { // items with no name are deleted
					cout << endl << "ITEM INFORMATION :" << endl;
					cout << "______________________________________________________________________________________________" << endl; // Start of table
					cout << " " << newSKU << "\t" << newType << "\t\t" << newName << "\n  \tStock: " << newAmtInStock << "\t\tPrice: $" << fixed << setprecision(2) << newPrice << endl;
				}
				else {
					cout << endl << "\tITEM DELETED!" << endl;
				}
				cout << "______________________________________________________________________________________________" << endl; // End of table
			} while (moreChanges);
			// END OF APPEND CHANGES MENU

			// Keep track of if the file can be written
			bool writeSuccess = 0;
			 
				// APPLY APPENDED CHANGES
			if (saveChanges == 1) {
				do {

					// APPLY CHANGES IN PROGRAM
					items[selindex] = { newSKU, newType, newName, newAmtInStock, newPrice };
					SKU_arr[selindex] = newSKU;
					type_arr[selindex] = newType;
					name_arr[selindex] = newName;
					numInStock_arr[selindex] = newAmtInStock;
					price_arr[selindex] = newPrice;



					// WRITE CHANGES TO FILE
					writeSuccess = writechanges(items, InvSize, selindex);
					if (!writeSuccess) {
						string confirmDiscard;
						cout << endl << endl << "\a\tError writing to file! Contact your system administrator." << endl << endl;
						cout << "\tWould you like to discard your changes? (Y/N)" << endl << endl << "\t> ";
						cin >> confirmDiscard;

						if (toupper(confirmDiscard[0]) == 'N') {
							cout << "N";
						}

					}
				} while ((!writeSuccess) && (!discardChanges)); // Do not allow user to break from loop until changes discarded or write success
			}

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

		else {
			cout << endl << "Sorry, your input was not recognized. Try again" << endl;
		}
		// **** END OF USER SELECTED QUIT ****



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


	// ****** END OF MAIN MENU ******
	// ************************************************************************************


	//*******************************
	//_____ Suggestions / TODO _____

	//.... getsystemDate() function -- used for log output
	// Log all inventory changes to a log output file

	//*******************************
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
		exit(0);
	}
	else {
		cout << "Credentials Verified!" << endl << endl;
	}
}
//*************************END OF USER AUTHENTICATION FUNCTION********************************//


//************************INVENTORY FILE INPUT FUNCTION********************************//
void loadInventory(int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], int InvSize)
{
	ifstream inventoryFile;
	//open a file in read mode
	inventoryFile.open("inventory.dat");

	cout << "Reading inventory..." << endl;

	// Quit if the file cannot be parsed
	if (inventoryFile.fail()) {
		cout << "\aFatal Error, could not load inventory!" << endl << "Please contact your system administrator." << endl;
		exit(0);
	}

	//bring the data in as strings to avoid an abnormal end
	string itemSKU = "", itemtype = "", itemname = "", numInStock = "", price = "", header = "";

	int n = 0; //line index of the file

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
			}
		}

		// Go to next line / next inventory item
		n++;
	}

	// Close the file
	inventoryFile.close();
	cout << "Inventory loaded!" << endl << endl;

}
//************************END OF INVENTORY FILE INPUT FUNCTION********************************//



//************************DISPLAY DATA FUNCTION********************************//
void displayInventory(vector<InventoryData> items, int InvSize, int rangemin, int rangemax) {

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



//************************WRITE INVENTORY CHANGES FUNCTION********************************//
bool writechanges(vector<InventoryData> items, int InvSize, unsigned int itemIndex) {
	
	//open a file in read mode
	ofstream inventoryFile;

	cout << "Writing changes to inventory... Do not exit." << endl;

	inventoryFile.open("inventory.dat"); //append to the transaction file

	// Quit if the file cannot be parsed
	if (inventoryFile.fail()) {
		cout << "\aFatal Error, could not write to inventory!" << endl << "Please contact your system administrator." << endl;
		return 0;
	}


	// Parse the file

	string header = "SKU, ITEM TYPE, ITEM NAME, ITEMS IN STOCK, PRICE\n";
	inventoryFile << header;
	for (int i = 0; i < InvSize;) //loop through the inventory - skip the header
	{
		if (items[i].price != 0) {
			inventoryFile << items[i].itemSKU << "," << items[i].itemtype << "," << items[i].itemname << "," <<
				items[i].numInStock << "," << items[i].price << endl;
		}
		i++;
	}
	inventoryFile.close();
	cout << "Inventory data file updated!" << endl;

	return 1;
}

//************************WRITE INVENTORY CHANGES FUNCTION********************************//