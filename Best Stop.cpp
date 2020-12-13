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
void loadStoreInfo(StoreInfo* store);
void login(UserInfo* seluser);
void loadInventory(int index_arr[], int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], int arrsize);
void displayInventory(int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], int arrsize, int rangemin, int rangemax);


// **** End of Function Prototypes ****


int main() {

	// Read in Store Info data
	StoreInfo store;
	loadStoreInfo(&store);
	
	// Welcome the customer
	cout << "********************************************************************************" << endl;
	cout << "Welcome to the " << store.name << " Inventory Management System!" << endl << endl;
	cout << "\tWe are located at:" << endl << "\t\t" << store.address << ", " << store.town << ", " << store.state << " " << store.zip << endl << endl;
	cout << "\tOur phone number is:  " << endl << "\t\t" << store.phone << endl << endl;

	cout << "\tManager: " << endl << "\t\t" << store.manager << endl << "\t\tID: " << store.managerID << endl << "\t\tExt: " << store.managerExt << endl << "\t\tEmail: " << store.managerEmail << endl << endl;
	cout << "\tSystem Admin:" << endl << "\t\t" << store.sysadmin << endl << "\t\tEmail: " << store.sysadminEmail << endl << endl;
	cout << "********************************************************************************" << endl;

	// Prompt authentication and read in users datafile
	cout << "Please authenticate before continuing" << endl << endl;
	UserInfo seluser;
	login(&seluser);
	// User is authenticated
	cout << "Welcome " << seluser.employeename << "!" << endl;
	
	// Determine if user is a system admin or manager, assign a privilege level accordingly
	int privlev = 1;
	if (seluser.userid == 0) {
		privlev = 3;
	}
	else if (seluser.userid == 1) {
		privlev = 2;
	}

	const int arrsize = 15;
	//InventoryData items[inv_size];

	// Initialize the inventory in parallel arrays
	int SKU_arr[arrsize] = { 0 };
	string type_arr[arrsize];
	string name_arr[arrsize];
	int numInStock_arr[arrsize] = { 0 };
	float price_arr[arrsize] = { 0.00f };
	int index_arr[arrsize] = { 0 };

	// Load our inventory into parallel arrays
	loadInventory(index_arr, SKU_arr, type_arr, name_arr, numInStock_arr, price_arr, arrsize);


	//*******************************
	//_____ WIP _____


	// Load parallel arrays into structure


	// Bring the user into a menu
	
	// Loop through menu

	// IF user selects to display menu

	// Display the inventory data
	// Set target lines minimum-maximum range to output
	int rangemin = 0; // set to sku index range?
	int rangemax = arrsize;

	// Allow custom ranges

	displayInventory(SKU_arr, type_arr, name_arr, numInStock_arr, price_arr, arrsize, rangemin, rangemax);
	
	

	//*******************************
	//_____ Suggestions / TODO _____

	//.... getsystemDate() function -- used for log output

	// Menu function (main menu loop)
		
		// Append new info as necessary to inventory database
		// Options :
			// 1. View inventory by item ID
			// 2. Edit inventory items in the database
			// 3. (if user is sysAdmin or manager) View log file of all transactions

		// Log all inventory changes to a log output file
		// Append all inventory changes to the inventory datafile

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
		storemanager = "", managerID = "", managerExt = "", managerEmail = "", sysadmin="", sysadminEmail="";
	
	int i = 0; //line index of the file

	//loop through the file to get the single-record
	while (!storeFile.eof())
	{
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
	char validPW = 'n';
	do {
		cout << "\tEnter your username :" << endl << "\t"; // indent the columns to make it easier to read
		cin >> cinUser;
		cout << "\tEnter your password :" << endl << "\t";
		cin >> cinPW;
		cout << endl << "\tIs the above information correct? (Y/N)" << endl << "\t";
		cin >> validPW;
	} while (validPW == 'n' || validPW == 'N');
	// **** END OF USER INPUT ****


	//	**** FILE INPUT	****
	ifstream usersFile;
	//opens the stored user credentials file in read mode
	usersFile.open("users.dat");
	cout << "********************************************************************************" << endl;
	cout << "Checking user information..." << endl;

	// Check if the file can be opened
	if (usersFile.fail()) {
		cout << "\aFatal Error, could not find account info from file!" << endl << "Please contact your system administrator." << endl;
		exit(0);
	}

	//bring the data in as strings to avoid an abnormal end
	string userid = "", employeename = "", username = "", password = "";

	bool userauth = 0; // bool to keep track of if the user is authenticated or not
	// Check username-password combos from file input - stop parsing and return current line if data matches input
	while (!usersFile.eof())
	{
		getline(usersFile, userid, ',');
		getline(usersFile, employeename, ',');
		getline(usersFile, username, ',');
		getline(usersFile, password, '\n');

		if ( (username == cinUser) && (password == cinPW) ) {
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
		cout << "\tCredentials Verified!" << endl;
	}
}
//*************************END OF USER AUTHENTICATION FUNCTION********************************//


//************************INVENTORY FILE INPUT FUNCTION********************************//
void loadInventory(int index_arr[], int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], int arrsize)
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
	string itemSKU = "", itemtype = "", itemname = "", numInStock = "", price = "";

	int n = 0; //line index of the file

	//loop through the file to get the single-record
	while (!inventoryFile.eof())
	{
		getline(inventoryFile, itemSKU, ',');
		getline(inventoryFile, itemtype, ',');
		getline(inventoryFile, itemname, ',');
		getline(inventoryFile, numInStock, ',');
		getline(inventoryFile, price, '\n');

		// If we are not out of the parallel array index range and there's data on the line
		if ((n < arrsize) && (itemSKU != "")) {
			// Load inventory system info into structure values to reflect the given input file
			SKU_arr[n] = stoi(itemSKU);
			type_arr[n] = itemtype;
			name_arr[n] = itemname;
			numInStock_arr[n] = stoi(numInStock);
			price_arr[n] = stof(price);
		}
		else {
			// IN CASE our constant size of our array goes out-of-bounds,
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
void displayInventory(int SKU_arr[], string type_arr[], string name_arr[], int numInStock_arr[], float price_arr[], int arrsize, int rangemin, int rangemax) {
	cout << "______________________________________________________________________________________________" << endl;
	cout << "SKU\tCategory\tItem Name\n #\tNumber in Stock\t  Price" << endl;
	for (unsigned int n = 0; n < arrsize;) {
		cout << "______________________________________________________________________________________________" << endl;
		cout << " " << SKU_arr[n] << "\t" << type_arr[n] << "\t" << name_arr[n] << "\n  \tStock: " << numInStock_arr[n] << "\tPrice: $" << price_arr[n] << endl;
		n++;
	}
}
//************************END OF DISPLAY DATA FUNCTION********************************//