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
	int number = 0;
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
	// Unique user id, given real name, their username and (plaintext*) password
	int userid = 0;
	string employeename;
	string username;
	string password;
} seluser;
// **** End of Structure Prototypes ****

// **** Function Prototypes ****
void loadStoreInfo(StoreInfo* store);
void login(UserInfo* seluser);

// **** End of Function Prototypes ****


int main() {
	// Read in Store Info data
	StoreInfo store;
	loadStoreInfo(&store);
	
	// Welcome the customer
	cout << "********************************************************************************" << endl;
	cout << "Welcome to the " << store.name << " Inventory Management System!" << endl << endl;
	cout << "\tWe are located at:\n\t\t" << store.address << ", " << store.town << ", " << store.state << " " << store.zip << endl << endl;
	cout << "\tOur phone number is: \n\t\t" << store.phone << endl << endl;

	cout << "\tManager: \n\t\t" << store.manager << endl << "\t\tID: " << store.managerID << "\n\t\tExt: " << store.managerExt << endl << "\t\tEmail: " << store.managerEmail << endl << endl;
	cout << "\tSystem Admin:\n\t\t" << store.sysadmin << "\n\t\tEmail: " << store.sysadminEmail << endl << endl;
	cout << "********************************************************************************" << endl;

	// Prompt authentication and read in users datafile
	cout << "Please authenticate before continuing\n\n";
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

	//*******************************
	//_____ Suggestions / TODO _____
	
	//void loadInventory();
		// Load the inventory database file
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
		cout << "\aFatal Error, could not load store info!\nPlease contact your system administrator.\n";
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
	store->number = stoi(storenumber);
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

	// Gracefully exit the file
	storeFile.close();
	cout << "\nStore information loaded!\n\n";

}
//************************END OF STOREINFO FILE INPUT FUNCTION********************************//

//************************USER AUTHENTICATION FUNCTION********************************//
void login(UserInfo* seluser)
{
	// **** USER INPUT ****
	// Take a user's login info from standard input
	string cinUser, cinPW;
	char validPW = 'n';
	char c; // keeps track of entered character
	do {
		cout << "\tEnter your username :\n\t"; // indent the columns to make it easier to read
		cin >> cinUser;
		cout << "\tEnter your password :\n\t";
		cin >> cinPW;
		cout << "\n\tIs the above information correct? (Y/N)" << endl << "\t";
		cin >> validPW;
	} while (validPW == 'n' || validPW == 'N');
	// **** END OF USER INPUT ****


	//	**** FILE INPUT	****
	ifstream dbFile;
	//opens the stored user credentials file in read mode
	dbFile.open("users.dat");
	cout << "********************************************************************************\n";
	cout << "Checking user information..." << endl;

	// Check if the file can be opened
	if (dbFile.fail()) {
		cout << "\aFatal Error, could not find account info from file!\nPlease contact your system administrator.\n";
		exit(0);
	}

	//bring the data in as strings to avoid an abnormal end
	string userid = "", employeename = "", username = "", password = "";

	bool userauth = 0; // bool to keep track of if the user is authenticated or not
	// Check username-password combos from file input - stop parsing and return current line if data matches input
	while (!dbFile.eof())
	{
		getline(dbFile, userid, ',');
		getline(dbFile, employeename, ',');
		getline(dbFile, username, ',');
		getline(dbFile, password, '\n');

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
	// Gracefully exit the file
	dbFile.close();
	//	**** END OF FILE INPUT ****


	// If no matching data was found from the user's input, quit the program
	if (userauth == 0) {
		// Play an audible alert tone to spook- I mean- alert the exiting user
		cout << "\a";
		cout << "\tSorry, your account credentials could not be verified.\n\tProgram exiting...\n\n\n";
		exit(0);
	}
	else {
		cout << "\tCredentials Verified!\n";
	}
}
//*************************END OF USER AUTHENTICATION FUNCTION********************************//