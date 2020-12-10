
// AppleCard.cpp: This program processes transactions for the Apple Card credit card.
// Author: C. Miles
// Date: December 9th, 2020

#pragma warning(disable : 4996)
#include <string>
#include <iomanip>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

// **** Structure Prototypes ****
struct Customer {
	int cardNumber = 0;
	string custName;
	int secCode = 0;
	string expDate;
	float maxAmt = 0.00f;
	float balOwed = 0.00f;
	int billDD = 0;
} seluser;

// "Store" structure -> Transaction structure - Uses loaded store info in the transaction info
struct Transaction {
	// Use store variables in each transaction for the display functions
	int storeType = 0;
	int storeNum = 0;
	string storeName;

	string transCode;
	string transDate;
	string cardNum;
	float transAmount = 0;
} trans;
// **** End of Structure Prototypes ****


// **** Function Prototypes ****
int validateCard(int securitycodes[], const int CARDSIZE, string custNames[], int cardNumbers[]);
void processTrans(Customer* seluser, Transaction* trans, int& ti, string transcodes[], string cards[], string transamounts[],
	string transdates[], int storeTypes[], string storeNames[], int storeNums[], const int storesAmt);
void loadCustomers(int secCodes[], int cardNumbers[], string custNames[], float maxAmounts[], float balances[],
	int billDDs[], string expDates[]);
void loadStores(int storeTypes[], int storeNums[], string storeNames[]);
void displayReceipt(string cardNum, string transCode, float transAmount, string transDate);
void displaySummary(Customer seluser, int ti, string transcodes[], string cards[], string transamounts[], string transdates[]);
char* getSystemDate(); // function for getting the system date

//Receive all transactions. tsize is the amount of transactions, not the array size
void analyzeTrans(Customer seluser, int tsize, string transcodes[], string transamounts[], int storeTypes[]);
void writeTransFile(Customer* seluser, int tsize, string transcodes[], string transamounts[], int storeTypes[], int storeNums[], string transdates[]);
// **** End of Function Prototypes ****


int main()
{
	//working variables
	int ci = -1; //card index
	char moretrans = 'x'; //get multiple transactions
	string transCode = "";
	const int CARDSIZE = 4;//size of the card arrays
	const int TRANSSIZE = 10;//size of the transaction arrays - limited to 10 transactions
	const int storesAmt = 8; // size of the store arrays - limited to 8 different stores
	//PARALLEL ARRAYS
	int secCodes[CARDSIZE] = { 0 };
	int cardNumbers[CARDSIZE] = { 0 };
	string custNames[CARDSIZE];
	float maxAmounts[CARDSIZE] = { 0.00f };
	float balances[CARDSIZE] = { 0.00f }; 
	int billDDs[CARDSIZE] = { 0 };
	string expDates[CARDSIZE];
	//MULTIPLE TRANSACTIONS ARRAYS
	string transcodes[TRANSSIZE];
	string cards[TRANSSIZE];
	string transamounts[TRANSSIZE];
	string transdates[TRANSSIZE];
	int ti = 0;
	//Company Variables
	string bizName = "Apple";
	//Welcome the user
	cout << "Welcome to " << bizName << " Credit Card Processing System!" << endl;
	// STORE ARRAYS
	int storeTypes[storesAmt];
	int storeNums[storesAmt];
	string storeNames[storesAmt];
	//Load store data
	loadStores(storeTypes, storeNums, storeNames);
	//Load the card data
	loadCustomers(secCodes, cardNumbers, custNames, maxAmounts, balances, billDDs, expDates); //pass the entire array
	// Card validation loop
	do {
		ci = validateCard(secCodes, CARDSIZE, custNames, cardNumbers); //capture the index of the card being used
	} while (ci == -1);
	
	// Get the selected customer's card data
	Customer seluser;
	seluser.cardNumber = cardNumbers[ci];
	seluser.custName = custNames[ci];
	seluser.secCode = secCodes[ci];
	seluser.expDate = expDates[ci];
	seluser.maxAmt = maxAmounts[ci];
	seluser.balOwed = balances[ci];
	seluser.billDD = billDDs[ci];

	//Get multiple transactions from user
	do
	{
		Transaction trans;
		// Process the transaction
		processTrans(&seluser, &trans, ti, transcodes, cards, transamounts, transdates, storeTypes, storeNames, storeNums, storesAmt);

		cout << "Do you have another transaction to enter? (Y/N): ";
		cin >> moretrans;
	} while (toupper(moretrans) == 'Y');

	// Display the card summary, analyze the transactions, write the transactions to an output file
	displaySummary(seluser, ti, transcodes, cards, transamounts, transdates);
	analyzeTrans(seluser, ti, transcodes, transamounts, storeTypes);
	writeTransFile(&seluser, ti, transcodes, transamounts, storeTypes, storeNums, transdates);

	// Gracefully exit the program
	cout << "Thank you for using the " << bizName << " Credit Card System, Program Ended!" << endl;
	return 0;
}
/************************************************Load Stores Function**************************** ************/
void loadStores(int storeTypes[], int storeNums[], string storeNames[])
{
	// Load store info from file

	ifstream storefile;
	//open a file in read modeifstream customerfile;
	storefile.open("stores.dat");
	cout << "Reading...store data" << endl;
	//bring the data in as strings to avoid an abnormal end
	string storeType;
	string storeNum;
	string storeName;

	int si = 0; // store index for the arrays
	//loop through the file to get each record
	while (!storefile.eof())
	{
		getline(storefile, storeType, ',');
		getline(storefile, storeNum, ',');
		getline(storefile, storeName, '\n');

		//add data to the parallel arrays

		storeTypes[si] = stoi(storeType);
		storeNums[si] = stoi(storeNum);
		storeNames[si] = storeName;

		si++;
	}
	storefile.close();
	cout << "Store data loaded!" << endl;

}



/************************************************Load Customers Function**************************** ************/
void loadCustomers(int secCodes[], int cardNumbers[], string custNames[], float maxAmounts[], float balances[],
	int billDDs[], string expDates[])
{
	// Load customer data from file

	ifstream customerfile;
	//open a file in read modeifstream customerfile;
	customerfile.open("customers.dat");
	cout << "Reading...customer data" << endl;
	//bring the data in as strings to avoid an abnormal end

	string custName = "", billDD = "", secCode = "", dob = "", phoneNumber = "", cardNumber = "", expDate = "", maxAmt = "",
		balOwed = "";
	int ci = 0; //an index for the array
	//loop through the file to get each record
	while (!customerfile.eof())
	{
		getline(customerfile, cardNumber, ',');
		getline(customerfile, secCode, ',');
		getline(customerfile, expDate, ',');
		getline(customerfile, billDD, ',');
		getline(customerfile, custName, ',');
		getline(customerfile, dob, ',');
		getline(customerfile, phoneNumber, ',');
		getline(customerfile, maxAmt, ',');
		getline(customerfile, balOwed, '\n');
		//add data to the parallel arrays
		
		cardNumbers[ci] = stoi(cardNumber);
		secCodes[ci] = stoi(secCode);
		expDates[ci] = expDate;
		custNames[ci] = custName;
		maxAmounts[ci] = stof(maxAmt);
		balances[ci] = stof(balOwed);
		billDDs[ci] = stoi(billDD);
		ci++;
	}
	customerfile.close();
	cout << "Customer data loaded!" << endl;
}
/************************************************Validate Card Function***************************** ***********/
int validateCard(int secCodes[], const int CARDSIZE, string custNames[], int cardNumbers[])
{
	// VALIDATE the user's card info, check if expired, return the card index

	int eCode = 0;
	char validInfo;
	cout << "Please enter your security code: ";
	cin >> eCode;

	for (int i = 0; i < CARDSIZE; i++)
	{
		if (eCode == secCodes[3]) //test to see if card is expired
		{
			cout << endl << "Card Number :  ****-****-****-" << to_string(cardNumbers[3]).substr(3,-1) << endl; // STRIP the card number to the last 4 digits
			cout << "Card holder : " << custNames[3] << endl;
			cout << "Is this correct? (Y/N)" << endl;
			cin >> validInfo;
			if (validInfo == 'N' || validInfo == 'n') {
				return -1;
			}
			else {
				cout << "Card Expired, Program Terminated!" << endl;
				exit(0);
			}
			
		}
		else if (eCode == secCodes[i]) //test to see if card is valid
		{
			cout << endl << "Card Number : ****-****-****-" << to_string(cardNumbers[i]).substr(3, -1) << endl; // STRIP the card number to the last 4 digits
			cout << "Card holder : " << custNames[i] << endl;
			cout << "Is this correct? (Y/N)" << endl;
			cin >> validInfo;
			if (validInfo == 'Y' || validInfo == 'y') {
				return i;
			}
			else {
				// False condition, restarts the loop
				return -1;
			}
		}
	}
	cout << "Invalid card, Program Terminated!" << endl; //Invalid card because it was not found in the loop
	exit(0);
	return 0;
}
/****************************************End Of Validate Card Function****************************** ********/
/****************************************Process Transactions*************************************** ********/
void processTrans(Customer* seluser, Transaction* trans, int& ti, string transcodes[], string cards[],
	string transamounts[], string transdates[], int storeTypes[], string storeNames[], int storeNums[], const int storesAmt)
{
	// Process and validate the transaction, update transaction arrays, then display the receipt for the transaction

	// ***********************************************************************************************************************************************************
	// In this function, for some reason, I throw an exception error when I iterate too many times. It's a stack error, revolving around the variable "SelStores"
	// This variable appears a lot in here, so I'm unsure as to which line causes the exception.
	// I assume it's due to maybe a variable that is not re-initialized right. It does not break first-time-around.
	// I spent a decent time looking into this error and was unable to correct it

	// Steps to reproduce :
	// 1. Enter store type, enter store number, run through the transaction
	// 2. Enter "Y" when prompted to make a new transaction
	// 3. Enter the same store type and name as the original
	// *exception* - Run-Time Check Failure #2 - Stack around the variable 'selStores' was corrupted.
	// Workaround : Just don't enter the same stores info 2x in a row

	// *** I hope acknowledging this and showing how to reproduce the exception will be better than staying silent and hoping you miss it! ***
	// ***********************************************************************************************************************************************************

	//Create an approval and denial code
	char app = 'A';
	char den = 'D';
	static int codenum = 100; //static variables only update once - need it to create new #'s each t ime function runs
	string appCode = "";//app + codenum
	string denCode = "";//den + codenum
	// Data entry
	trans->cardNum = to_string(seluser->cardNumber);
	int enteredST = -1;
	trans->transAmount = 0.00f;
	float availBal = seluser->maxAmt - seluser->balOwed;
	trans->transDate = getSystemDate();
	//get store type
	do {
		cout << endl;
		cout << "Select your store" << endl;
		cout << "1. SUPERMARKET" << endl;
		cout << "2. RESTAURANT" << endl;
		cout << "3. SALON" << endl;
		cout << "4. DEPARTMENT STORE" << endl;
		cin >> enteredST;
		if (enteredST < 1 || enteredST > 4)
		{
			cout << "Invalid store type!" << endl;
		}
		cin.ignore();
	} while (enteredST < 1 || enteredST > 4);
	trans->storeType = enteredST; // update trans.StoreType to reflect the entered store type
	int selStores[2] = { 0 }; // Capture store indices that are the same as the entered StoreType (enteredST) (Max stores per type in data: n=2, index: 1 (=n-1))
	
	
					  
	// ****************************** VALIDATE TRANSACTION DATA ************************************************************

	// Loop through the store types, capture the index of each matching storeType in the array
	int indx = 0; // selected store index
	for (int i = 0; i < storesAmt;) {
		if (storeTypes[i] == enteredST) {
			selStores[indx] = i; // Capture the index
			//cout << "found match at index: " << tsi << endl; // debug- check index
			indx++;
		}
		i++; // the store's index
	}
	
	//	************* STORE VALIDATION LOOP *************
	// (Logic : Since these arrays are parallel, the selected indices will be at the same index in storeNames and storeNums)
	int validStoreInfo = 0;
	char validStoreNum = 'n';
	int storeNum = 0;
	string storeName;
	do {
		cout << endl << "Please enter store name." << endl;
		getline(cin, storeName);

		// Loop through matching indices, check for storeName matches
		for (unsigned int i = 0; i < (sizeof(selStores)/sizeof(selStores[0]));) {
			//cout << i << endl << storeNames[selStores[i]] << endl << "---" << endl; // Debug - shows matching store names for StoreType
			if (storeName == storeNames[selStores[i]]) {
				// Validate the store name and number
				do {
					cout << endl << "Matching store number for " << storeName << " found!" << endl;
					cout << "Store number is " << storeNums[selStores[i]] << endl;
					cout << "Is this correct? (Y/N)" << endl;
					cin >> validStoreNum;
				} while (validStoreNum == 'n' || validStoreNum == 'N');

				// Capture the data into the transaction structure
				storeNum = storeNums[i];
				validStoreInfo = 1; // Break if the store name is valid
			}
			i++;
		}
		if (validStoreInfo == 0) { // If no valid matches were found, restart the store validation loop
			cout << "Invalid store name!" << endl;
		}
	} while (validStoreInfo != 1);
	//	************* END OF STORE VALIDATION LOOP *************
	trans->storeNum = storeNum;
	trans->storeName = storeName;

	//	************* TRANSACTION VALIDATION LOOP *************
	// Get the transaction amount + Validate it is correct and not negative (we wouldn't want any thieves, would we?)
	char validTransAmt = 'n';
	float transAmount = 0.00f;
	do {
		cout << endl << "Please enter the transaction amount.\n$";
		cin >> transAmount;
		
		if (transAmount <= 0) { // IF 0 or NEGATIVE, display warning and restart the loop
			cout << "The transaction amount cannot be negative.";
		}
		else { // IF POSITIVE, validate
			cout << endl << "Transaction amount is: $" << fixed << setprecision(2) << transAmount << endl;
			cout << "Is this correct? (Y/N)" << endl;
			cin >> validTransAmt;
		}
	} while (validTransAmt == 'n' || validTransAmt == 'N');
	trans->transAmount = transAmount;
	//	************* END OF TRANSACTION VALIDATION LOOP *************
	// ****************************** END OF VALIDATE TRANSACTION DATA ************************************************************

	// All input validated, run the transaction!

	// Approve or Deny the transaction
	if (trans->transAmount > availBal)
	{
		cout << "Insufficient funds.. Transaction denied.\n\n";
		denCode = den + to_string(codenum); //create the denial code
		trans->transCode = denCode; //set the transaction code for the receipt
	}
	else
	{
		cout << "Transaction Approved." << endl;
		seluser->balOwed += trans->transAmount; //update original structure item
		appCode = app + to_string(codenum); //create the approval code
		trans->transCode = appCode; //set the transaction code for the receip
	}
	codenum++; //update the codenumber for the next transaction

	//add to transactions arrays
	transcodes[ti] = trans->transCode;
	cards[ti] = to_string(seluser->cardNumber);
	transamounts[ti] = to_string(trans->transAmount);
	transdates[ti] = trans->transDate;
	storeTypes[ti] = trans->storeType;


	//cout << transcodes[ti] << " " << cards[ti] << " " << transamounts[ti] << " " << transdates[ti];
	displayReceipt(trans->cardNum, trans->transCode, trans->transAmount, trans->transDate);
	ti++; //update the count for the transaction array
}
//***********************END OF PROCESS TRANSACTION FUNCTION********************************/
//************************DISPLAY RECEIPT FUNCTION********************************//
void displayReceipt(string cardNum, string transCode, float transAmount, string transDate)
{
	cout << "**********RECEIPT**********" << endl;
	cout << "App/Den Code:\t\t" << transCode << endl;
	cout << "Card Number:\t\t" << "****-****-****-" << cardNum.substr((cardNum.length()-4), -1) << endl; // STRIP the first 3 characters (card number is constant length 7)
	cout << "Transaction Amount:\t" << "$" << fixed << setprecision(2) << transAmount << endl;
	cout << "Transaction Date:\t" << transDate << endl;
	cout << "*******END OF RECEIPT******" << endl;
	
}
//************************END OF DISPLAY RECEIPT FUNCTION********************************//
//************************DISPLAY SUMMARY FUNCTION********************************//
void displaySummary(Customer seluser, int tsize, string transcodes[], string cards[], string transamounts[], string transdates[])
{

	// The original output in this showed the entire card number in a field that was made for the last 4 digits of the card
	// Obviously, this would have been a huge security risk


	int todayday = 20;
	string todaysdate = getSystemDate();

	// I think you'll like my string iteration here!
	// I wish we had learned more about string formatting in C++, b/c this section would've been really good practice imo  (in my opinion)
	// It's definitely not as simple as Python imo

	string date_stripped = todaysdate.substr('\t', -1); // Get everything after the first tab (WEEKDAY MONTH(\t) DD)
	todayday = stoi(date_stripped.substr(0, 2)); // Get the number and remove any blank spaces (for 1-digit dates)

	//due day variables
	string billDM = "12";
	string billDD = to_string(seluser.billDD);
	string billDY = "2020";
	if (todayday > seluser.billDD) // set the bill due date until next month if the day has past
	{
		billDM = "01";
		billDY = "2021";
	}
	string billdue = billDM + "/" + billDD + "/" + billDY;
	string card = to_string(seluser.cardNumber); // Makes cardnum a string so we can strip it
	float Owedbal = seluser.balOwed;
	float maxAm = seluser.maxAmt;
	cout << "\n\n";
	cout << "************************CARD SUMMARY***************************" << endl;
	cout << "Today's date and time is: " << todaysdate << endl;

	cout << "Customer Name:\t\t" << seluser.custName << endl;
	cout << "Card Number:\t\t" << "****-****-****-" << card.substr(3, -1) << endl; // STRIP the first 3 characters (card number is constant length 7)
	cout << "Maximum Amount:\t\t" << "$" << fixed << setprecision(2) << maxAm << endl;
	cout << "Balance Owed:\t\t" << "$" << fixed << setprecision(2) << Owedbal << endl;
	cout << "Bill Due Date:\t\t" << billdue << endl << endl;
	cout << "TRANSACTIONS" << endl;
	cout << "App/Den Code\t" << "Transaction Amount\t" << "Transaction Date " << endl;
	for (int i = 0; i < tsize; i++) //tsize is the amount of transaction - TRANSSIZE is the size the array can hold
	{
		float tranAmt = stof(transamounts[i]);

		cout << transcodes[i] << "\t\t" << "$" << fixed << setprecision(2) << tranAmt << "\t\t" << transdates[i] <<
			endl;
	}
	cout << "************************END OF CARD SUMMARY*************************" << endl << endl;
}
//************************END OF DISPLAY SUMMARY FUNCTION********************************//

//************************GET SYSTEM DATE FUNCTION********************************//
char* getSystemDate()
{
	//SYSTEM DATE AND TIME
	time_t now = time(0); // current date/time based on current system
	char* todaysdate = ctime(&now); // convert now to string form
	return todaysdate;
}
//************************END OF GET SYSTEM DATE FUNCTION********************************//

//************************ANALYZE TRANSACTION FUNCTION********************************//
void analyzeTrans(Customer seluser, int tsize, string transcodes[], string transamounts[], int storeTypes[])
{
	// It saddens me to say, that I could not understand this function from a beginner perspective
	// If we had more time, I'm sure this would've been a cool function to play with.
	// I can see that you wanted trans[i], which means my Transactions (trans) structure would have been changed drastically
	// I couldn't impliment it without thoroughly revising the entire project with too little time to spare. I hope loading from the arrays instead is okay.

	enum storetype { SUPERMARKET = 1, RESTAURANT, SALON, DEPARTMENT}; //enum storetype & start at 1; i.e. 1-5 vs 0-4
	int storetypenum = -1; //switch statement needs an int to check
	float totalTransAmt = 0, totalSu = 0, totalR = 0, totalSa = 0, totalD = 0; //grand total and totals by category
	float pctSu = 0, pctR = 0, pctSa = 0, pctD = 0; //used to convert percentage to a # instead of fraction
	string codetype = "x"; //used to capture code type (A or D) - exclude "denied"
	for (int i = 0; i < tsize; i++)
	{
		storetypenum = storeTypes[i]; //capture the store type for each purchase to analyze percentage spent
		codetype = transcodes[i].at(0); //capture the first letter of transaction code, if D do not calculate
		cout << codetype;
		if (codetype == "A")
		{
			totalTransAmt += stof(transamounts[i]); //calculate the grand total for all approved transactions
			switch (storetypenum) //update the totals based on store type categories
			{
			case SUPERMARKET:
			{ totalSu += stof(transamounts[i]); break; }
			case RESTAURANT:
			{ totalR += stof(transamounts[i]); break; }
			case SALON:
			{ totalSa += stof(transamounts[i]); break; }
			case DEPARTMENT:
			{ totalD += stof(transamounts[i]); break; }
			}//end of switch
		} //end of if statement
	} //end of for loop

	//Calculate and display percentages
	cout << "***************Transactions Analysis for Customer " << seluser.custName << "************" << endl;
	if (totalSu > 0)
	{
		pctSu = (totalSu / totalTransAmt) * 100; cout << pctSu << "%\t" << "Supermarket" << endl;
	}
	if (totalR > 0)
	{
		pctR = (totalR / totalTransAmt) * 100; cout << pctR << "%\t" << "Restaurant" << endl;
	}
	if (totalSa > 0)
	{
		pctSa = (totalSa / totalTransAmt) * 100; cout << pctSa << "%\t" << "Salon" << endl;
	}
	if (totalD > 0)
	{
		pctD = (totalD / totalTransAmt) * 100; cout << pctD << "%\t" << "Department Store" << endl;
	}
	cout << "Monthly Total Spent : $" << fixed << setprecision(2) << totalTransAmt << endl;
	cout << "***********************END OF TRANSACTION ANALYSIS REPORT***********************" << endl << endl;

}
//************************END OF ANALYZE TRANSACTION FUNCTION********************************//

//************************FILE OUTPUT FUNCTION********************************//
void writeTransFile(Customer* seluser, int tsize, string transcodes[], string transamounts[], int storeTypes[], int storeNums[], string transdates[])
{

	// WARNING : THE ORIGINAL OUTPUT FOR THIS FUNCTION INCLUDED A LINE BREAK AFTER THE DATE! *** OUR BEAUTIFUL CSV FORMAT WOULD HAVE BEEN RUINED!   :(
	// Thankfully, I stripped it out!


	//open a file in read mode
	ofstream transfile;
	transfile.open("appletrans.dat", ios::app); //append to the transaction file
	cout << "Writing... transaction data" << endl;
	for (int i = 0; i < tsize; i++) //loop through the transaction array
	{ //write to the file using the file variable instead of cout

		// Strip out the unneeded line terminator!
		string date = transdates[i].substr(0, transdates[i].length() - 1);
		transfile << date << "," << transcodes[i] << "," << seluser->cardNumber << "," << fixed << setprecision(2) << stof(transamounts[i]) << ","
			<< storeNums[i] << "," << storeTypes[i] << "\n";
	}
	transfile.close();
	cout << "Transactions data file written!" << endl;
}
//************************END OF FILE OUTPUT FUNCTION********************************//
