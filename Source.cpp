#include <iostream>
#include <time.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <Windows.h>
using namespace std;

void inputDetails(int);
string listArray[10][4];
string receiptArray[10][5];
int j = 0;

class Account;
class Business;
class Item;
class Staff;
class Admin;
void logoDisplay();
void main();

class Account
{
protected:
	string name, password;
	static int attempts;
	ifstream readLoginDetailsFromDatabase;
	ofstream writeLoginDetailsToDatabase;
	Business* businessPtr;
	Item* itemPtr;
	int numOfOrders;
	bool businessCreated = false;
public:
	void checkDetails(char, Account*);
	virtual void setStreams(char) = 0;
	void setDetails(string, string);
	virtual void mainScreen() = 0;
	void checkForOrders();
	void addItemsScreen();
	void removeItemsFromList();
	void displayList();
	void completeCheckout();
	void itemByID();
	void searchDirectory();
	void showReceipt();
	friend void logoDisplay();
};

class Staff :public Account
{
public:
	void setStreams(char);
	void mainScreen();
};

class Business
{
private:
	string businessName;
public:
	string getBusinessName();
	void getRandom();
};

class Admin :public Account
{
public:
	void setStreams(char);
	void mainScreen();
	void removeStaffAccount();
	void createStaffAccount();
	friend void logoDisplay();
};

class Item
{
protected:
	string itemName;
	int quantity;
public:
	void getRandom();
	string getItemName();
	string getQuantity();
};

void Account::checkDetails(char choice, Account *ptr)
{
	string fileData;
	int nameCorrect = 0, passwordCorrect = 0, count = 0;
	while (getline(readLoginDetailsFromDatabase, fileData))
	{
		if (name.compare(fileData) == 0 && count == 0)
		{
			nameCorrect = 1;
		}
		if (password.compare(fileData) == 0 && count == 1)
		{
			passwordCorrect = 1;
		}
		count++;
		if (nameCorrect == 1 && passwordCorrect == 1)
		{
			break;
		}
		if (fileData == "`" && count == 3)
		{
			count = 0;
			nameCorrect = 0;
			passwordCorrect = 0;
		}
	}
	readLoginDetailsFromDatabase.close();

	if (nameCorrect == 1 && passwordCorrect == 1)
	{
		mainScreen();
	}
	else
	{
		cout << endl << "Username or password incorrect!" << endl;
		attempts++;
		delete ptr;
		time_t tryAgainTime = time(NULL);

		if (attempts > 2)
		{
			for (int i = 14; i > -1; i--)
			{
				system("CLS");
				cout << "Username:\nPassword:\n";
				cout << "Try again in " << i+1 << " seconds...";
				while (time(NULL) - tryAgainTime < 1) {}
				tryAgainTime = time(NULL);
			}
		}
		Sleep(1000);
		inputDetails(choice);
	}
}

void Account::setDetails(string n, string p)
{
	name = n;
	password = p;
}

void Admin::mainScreen()
{
	char x[7] = { "     " }, y = '>';
	int opt;
	int cpos = 0, c;
	bool chosen = false;
	x[0] = y;

	while (1)
	{
		system("cls");
		logoDisplay();
		cout << " " << x[0] << x[0];
		cout << " Create Employee Account" << endl;
		cout << " " << x[1] << x[1];
		cout << " Remove Employee Account" << endl;
		cout << " " << x[2] << x[2];
		cout << " Switch to Employee Mode" << endl;
		cout << " " << x[3] << x[3];

		opt = _getch();

		if (opt == 0 || opt == 224)
		{
			opt = _getch();
			if (opt == 72)
			{
				if (cpos == 0)
				{
					cpos = 3;
				}
				cpos = cpos - 1;

				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}

				x[cpos] = y;
			}
			else if (opt == 80)
			{
				if (cpos == 2)
				{
					cpos = -1;
				}
				cpos = cpos + 1;
				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}
				x[cpos] = y;
			}
		}
		else if (opt == 13)
		{
			switch (cpos)
			{
			case 0:
			{
				createStaffAccount();
				break;
			}
			case 1:
			{
				removeStaffAccount();
				break;
			}
			case 2:
			{
				Account* ptr;
				ptr = new Staff();
				ptr->mainScreen();
				break;
			}
			default:
			{
				cout << endl << endl << "Incorrect Option!";
				Sleep(1000);
				mainScreen();
			}
			}

		}
	}
}

void Admin::removeStaffAccount()
{
	system("CLS");
	logoDisplay();

	ifstream toReadFromStaffDatabase;
	ofstream toWriteToStaffDatabase;
	string username, text, array[3];
	char option;

	cout << " Staff username: ";
	cin >> username;

	cout << " Are you sure you want to remove the account " << username << "? (Y/N): ";
	cin >> option;

	if (option == 'Y' || option == 'y')
	{
		toReadFromStaffDatabase.open("staffDatabase.txt");
		toWriteToStaffDatabase.open("tempStaff.txt");
		while (!toReadFromStaffDatabase.eof())
		{
			for (int i = 0; i < 3; i++)
			{
				getline(toReadFromStaffDatabase, array[i]);
			}
			if (array[0] == "")
			{
				break;
			}
			if (username != array[0])
			{
				for (int l = 0; l < 3; l++)
				{
					toWriteToStaffDatabase << array[l] << endl;
				}
			}
		}
		toReadFromStaffDatabase.close();
		toWriteToStaffDatabase.close();

		toReadFromStaffDatabase.open("tempStaff.txt");
		toWriteToStaffDatabase.open("staffDatabase.txt");

		while (getline(toReadFromStaffDatabase, text))
		{
			toWriteToStaffDatabase << text << endl;
		}

		toReadFromStaffDatabase.close();
		toWriteToStaffDatabase.close();

		cout << " Account Removed!";
		Sleep(1000);
		mainScreen();
	}
	
}

void Admin::createStaffAccount()
{
	system("CLS");
	logoDisplay();

	string name, password;
	string tempInfo;
	string eAccountsDirectory[3];
	ifstream toReadDirectory;
	ofstream toWriteDirectory;

	toReadDirectory.open("staffDatabase.txt");

	cout << endl;
	cout << " Enter employee name: ";
	cin >> name;
	cout << " Enter employee password: ";
	cin >> password;
	int k = 0;

	while (getline(toReadDirectory, tempInfo))
	{
		if (k == 0 && name == eAccountsDirectory[k])
		{
			cout << " Username already exists!";
			Sleep(1000);
			toReadDirectory.close();
			createStaffAccount();
		}
		eAccountsDirectory[k] = tempInfo;
		k++;
		if (eAccountsDirectory[k - 1] == "`")
		{
			eAccountsDirectory[0] = "";
			eAccountsDirectory[1] = "";
			eAccountsDirectory[2] = "";
			k = 0;
		}
	}

	toReadDirectory.close();

	toWriteDirectory.open("staffDatabase.txt", ofstream::app);

	toWriteDirectory << name << endl << password << endl << "`" << endl;

	toWriteDirectory.close();
	cout << " Account added!";
	Sleep(1000);
	mainScreen();
}

void Admin::setStreams(char choice)
{
	readLoginDetailsFromDatabase.open("adminDatabase.txt");
	writeLoginDetailsToDatabase.open("adminDatabase.txt", ios::app);
}

void Account::checkForOrders()
{
	
	if (businessCreated == false)
	{
		srand(time(NULL));
		numOfOrders = rand() % 10 + 1;
		businessPtr = new Business[numOfOrders];
		itemPtr = new Item[numOfOrders];
		businessCreated = true;
	}

	for (int i = 0; i < numOfOrders; i++)
	{
		if (businessPtr[i].getBusinessName() == "" && itemPtr[i].getItemName() == "")
		{
			businessPtr[i].getRandom();
			itemPtr[i].getRandom();
		}
		listArray[i][0] = to_string(i + 1);
		listArray[i][1] = businessPtr[i].getBusinessName();
		listArray[i][2] = itemPtr[i].getItemName();
		listArray[i][3] = itemPtr[i].getQuantity();
	}
}

void Account::addItemsScreen()
{
	char x[7] = { "     " }, y = '>';
	int opt;
	int cpos = 0, c;
	bool chosen = false;
	x[0] = y;

	while (1)
	{
		system("cls");
		logoDisplay();
		cout << " " << x[0] << x[0];
		cout << " Enter Item By ID" << endl;
		cout << " " << x[1] << x[1];
		cout << " Search Item From Directory" << endl;
		cout << " " << x[2] << x[2];
		cout << " Exit" << endl;
		cout << " " << x[3] << x[3];

		opt = _getch();

		if (opt == 0 || opt == 224)
		{
			opt = _getch();
			if (opt == 72)
			{
				if (cpos == 0)
				{
					cpos = 3;
				}
				cpos = cpos - 1;

				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}

				x[cpos] = y;
			}
			else if (opt == 80)
			{
				if (cpos == 2)
				{
					cpos = -1;
				}
				cpos = cpos + 1;
				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}
				x[cpos] = y;
			}
		}
		else if (opt == 13)
		{
			switch (cpos)
			{
			case 0:
			{
				system("CLS");
				logoDisplay();
				showReceipt();
				itemByID();
				break;
			}
			case 1:
			{
				searchDirectory();
				break;
			}
			case 2:
			{
				mainScreen();
				break;
			}
			default:
			{
				cout << endl << endl << " Incorrect Option!";
				Sleep(1000);
				mainScreen();
			}
			}

		}
	}
}

void Account::searchDirectory()
{
	system("CLS");
	logoDisplay();
	showReceipt();

	ifstream toSearchDirectory;
	string word, tempInfo, tempWord = "";
	string itemPrice, itemID;
	string itemWord, itemInfo[4];
	int count = 0;
	int i = 0;
	bool found = 0;

	toSearchDirectory.open("itemDirectory.txt");

	cout << endl;
	cout << endl << " Enter a keyword: ";
	cin >> word;

	system("CLS");
	logoDisplay();

	while (getline(toSearchDirectory, tempInfo))
	{
		if (count == 0)
		{
			for (i = 0; i < tempInfo.length(); i++)
			{
				tempWord += tempInfo[i];

				if (tempWord == word)
				{
					itemInfo[0] = tempInfo;
					cout << " ------------------------------------" << endl << " Name\t\t|" << itemInfo[0] << endl;
					tempWord = "";
					count++;
					break;
				}
			}
			tempWord = "";
			continue;
		}

		else if (count == 1)
		{
			if (itemInfo[0] != "")
			{
				itemInfo[1] = tempInfo;
				cout << " Supplier\t|" << itemInfo[1] << endl;
				count++;
				continue;
			}
		}

		else if (count == 2)
		{
			if (itemInfo[0] != "")
			{
				itemInfo[2] = tempInfo;
				cout << " Price Per Item\t|" << itemInfo[2] << endl;
				count++;
				continue;
			}
		}
		else if (count == 3)
		{
			if (itemInfo[0] != "")
			{
				itemInfo[3] = tempInfo;
				cout << " ID\t\t|" << itemInfo[3] << endl << " ------------------------------------" << endl;
				count++;
				continue;
			}
		}
		else if (count == 4)
		{
			if (itemInfo[0] != "")
			{
				count++;
				continue;
			}
		}
		else if (count == 5 && tempInfo == "`")
		{
			tempWord = "";
			count = 0;
			continue;
		}
		else
		{
			continue;
		}
	}

	toSearchDirectory.close();

	itemByID();
}

void Account::itemByID()
{

	int i = 0;
	int quantity;
	char check;
	string ID, tempInfo, itemInfo[5];
	bool IDcorrect = 0;

	ifstream toReadID;
	toReadID.open("itemDirectory.txt");

	cout << endl << " ID: ";
	cin >> ID;
	while (getline(toReadID, tempInfo))
	{
		if (tempInfo != "`")
		{
			itemInfo[i] = tempInfo;
			i++;
			if (ID == itemInfo[3])
			{
				IDcorrect = 1;
				break;
			}
		}
		else if (tempInfo == "`")
		{
			i = 0;
		}
	}

	if (IDcorrect == 0)
	{
		cout << " Invalid ID!";
		Sleep(1000);
		addItemsScreen();
	}

	toReadID.close();

	system("CLS");
	logoDisplay();
	showReceipt();

	cout << " Item Name: " << itemInfo[0] << endl;
	cout << " Supplier Name: " << itemInfo[1] << endl;
	cout << " Price per item: " << itemInfo[2] << endl;
	cout << " Quantity: ";
	cin >> quantity;

	itemInfo[2] = to_string(stoi(itemInfo[2]) * quantity);
	string strQuantity;
	strQuantity = to_string(quantity);

	receiptArray[j][0] = itemInfo[3];
	receiptArray[j][1] = itemInfo[0];
	receiptArray[j][2] = strQuantity;
	receiptArray[j][3] = itemInfo[2];
	receiptArray[j][4] = itemInfo[1];

	j++;

	showReceipt();

	cout << " Do you want to add more items? (Y/N) : ";
	cin >> check;

	if (check == 'Y' || check == 'y')
	{
		system("CLS");
		logoDisplay();
		addItemsScreen();
	}

	if (check == 'N' || check == 'n')
	{
		mainScreen();
	}
}

void Account::showReceipt()
{
	system("CLS");
	logoDisplay();

	string text;
	int sum = 0;
	int price = 0;

	cout << endl;
	cout << "\t\t    ID\t\tName\t\tQuantity\t\tPrice\t\tSupplier Name" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << "\t\t    ";
		for (int j = 0; j < 5; j++)
		{
			cout << receiptArray[i][j] << "\t\t";
		}
		cout << endl;
	}

	for (int i = 0; i < 10; i++)
	{
		if (receiptArray[i][3] == "")
		{
			break;
		}
		price = stoi(receiptArray[i][3]);
		sum = sum + price;
	}
	cout << "\t\t\t\t\t\t\t\t        Total:\t" << sum;

	cout << endl << endl;
}

void Account::removeItemsFromList()
{
	system("CLS");
	logoDisplay();
	showReceipt();

	string ID;
	bool IDfound = 0;
	int quantity = 0;
	int k = 0;
	char check;

	cout << endl;
	cout << " ID to delete: ";
	cin >> ID;

	for (k = 0; k < 10; k++)
	{
		if (ID == receiptArray[k][0])
		{
			IDfound = 1;
			break;
		}
	}

	if (IDfound == 1)
	{
		cout << " Quantity: ";
		cin >> quantity;

		if (to_string(quantity) == receiptArray[k][2])
		{
			receiptArray[k][0] = "";
			receiptArray[k][1] = "";
			receiptArray[k][2] = "";
			receiptArray[k][3] = "";
			receiptArray[k][4] = "";

			for (int i = k; i < j; i++)
			{
				swap(receiptArray[i], receiptArray[i + 1]);
			}
			j--;
		}

		else if (to_string(quantity) < receiptArray[k][2])
		{
			receiptArray[k][3] = to_string((stoi(receiptArray[k][3]) / stoi(receiptArray[k][2])) * (stoi(receiptArray[k][2]) - quantity));
			receiptArray[k][2] = to_string(stoi(receiptArray[k][2]) - quantity);
		}

		else
		{
			cout << " Invalid Input!";
			Sleep(1000);
			removeItemsFromList();
		}
	}

	system("CLS");
	logoDisplay();
	showReceipt();

	cout << " Do you want to remove more items? (Y/N): ";
	cin >> check;

	if (check == 'Y' || check == 'y')
	{
		removeItemsFromList();
	}

	if (check == 'N' || check == 'n')
	{
		mainScreen();
	}
}

void Account::displayList()
{
	string text;
	int sum = 0;
	int price = 0;

	cout << endl;
	cout << "\t\tID\t\tBusiness Name\t\t\tItem\t\tQuantity" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << "\t\t";
		for (int j = 0; j < 4; j++)
		{
			cout << listArray[i][j] << "\t\t";
		}
		cout << endl;
	}
}

void Account::completeCheckout()
{
	char ch;
	string tempInfo;
	string itemInfo[6];
	bool complete = 0;
	ifstream toReadDirectory;
	ofstream toWriteDirectory;
	ifstream toReadTempDirectory;
	ofstream toWriteTempDirectory;

	system("CLS");
	logoDisplay();
	cout << "\t\t    ------------------------------------------------------------------" << endl;
	showReceipt();
	cout << "\t\t    ------------------------------------------------------------------" << endl;
	cout << "\t\t    \t\t  THIS ORDER WAS APPROVED BY CARGOX ADMINISTRATION\t\t" << endl;
	cout << "\t\t    ------------------------------------------------------------------" << endl;
	cout << "\t\t      Hotline: (555) 654 321 ----------- Email: management@cargox.com" << endl;
	cout << "\t\t    ------------------------------------------------------------------" << endl << endl;

	cout << " Enter Y to send alerts to suppliers: ";
	cin >> ch;

	if (ch == 'Y' || ch == 'y')
	{
		toReadDirectory.open("itemDirectory.txt");
		toWriteTempDirectory.open("tempItemDirectory.txt");

		while (!toReadDirectory.eof())
		{
			for (int l = 0; l < 6; l++)
			{
				getline(toReadDirectory, itemInfo[l]);
			}
			if (itemInfo[0] == "")
			{
				break;
			}
			for (int i = 0; i < j; i++)
			{
				if (receiptArray[i][0] == itemInfo[3])
				{
					itemInfo[4] = to_string(stoi(itemInfo[4]) - stoi(receiptArray[i][2]));
				}
			}
			for (int a = 0; a < 6; a++)
			{
				toWriteTempDirectory << itemInfo[a] << endl;
			}
		}

		toWriteTempDirectory.close();
		toReadDirectory.close();

		toReadTempDirectory.open("tempItemDirectory.txt");
		toWriteDirectory.open("itemDirectory.txt");

		while (getline(toReadTempDirectory, tempInfo))
		{
			toWriteDirectory << tempInfo << endl;
		}

		toReadTempDirectory.close();
		toWriteDirectory.close();

		j = 0;

		for (int i = 0; i < 10; i++)
		{
			for (int k = 0; k < 5; k++)
			{
				receiptArray[i][k] = "";
			}
		}
		mainScreen();
	}
	else
	{
		cout << " Invalid Input!";
		Sleep(1000);
		mainScreen();
	}
}

int Account::attempts = 0;

void Staff::mainScreen()
{
	char x[7] = { "     " }, y = '>';
	int opt;
	int cpos = 0, c;
	bool chosen = false;
	x[0] = y;

	while (1)
	{
		system("cls");
		logoDisplay();
		cout << " " << x[0] << x[0];
		cout << " Check for orders" << endl;
		cout << " " << x[1] << x[1];
		cout << " Search supplier database" << endl;
		cout << " " << x[2] << x[2];
		cout << " Remove item from list" << endl;
		cout << " " << x[3] << x[3];
		cout << " Compile and send list" << endl;
		cout << " " << x[4] << x[4];
		cout << " Logout" << endl;

		opt = _getch();

		if (opt == 0 || opt == 224)
		{
			opt = _getch();
			if (opt == 72)
			{
				if (cpos == 0)
				{
					cpos = 5;
				}
				cpos = cpos - 1;

				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}

				x[cpos] = y;
			}
			else if (opt == 80)
			{
				if (cpos == 4)
				{
					cpos = -1;
				}
				cpos = cpos + 1;
				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}
				x[cpos] = y;
			}
		}
		else if (opt == 13)
		{
			cout << cpos << endl;
			switch (cpos)
			{
			case 0:
			{
				char ch;
				system("CLS");
				logoDisplay();
				checkForOrders();
				displayList();
				cout << "Enter Y to exit: ";
				cin >> ch;
				if (ch == 'y' || ch == 'Y')
				{
					mainScreen();
				}
				break;
			}
			case 1:
			{
				addItemsScreen();
				break;
			}
			case 2:
			{
				removeItemsFromList();
				break;
			}
			case 3:
			{
				completeCheckout();
				break;
			}
			case 4:
			{
				main();
				break;
			}
			default:
			{
				cout << endl << endl << " Incorrect Option!";
				Sleep(1000);
				mainScreen();
			}
			}

		}
	}
}

void Staff::setStreams(char choice)
{
	readLoginDetailsFromDatabase.open("staffDatabase.txt");
	writeLoginDetailsToDatabase.open("staffDatabase.txt", ios::app);
}

string Business::getBusinessName()
{
	return businessName;
}

void Business::getRandom()
{
	srand(time(NULL)+rand());
	ifstream readFromFile;
	int count = 0;
	string line;
	readFromFile.open("businessNames.txt");

	while (!readFromFile.eof())
	{
		getline(readFromFile, line);
		count++;
	}

	readFromFile.close();
	readFromFile.open("businessNames.txt");

	string* array;
	array = new string[count];
	int i = 0;

	while (!readFromFile.eof())
	{
		getline(readFromFile, array[i]);
		i++;
	}


	businessName = array[rand() % count];

	delete[] array;
}

string Item::getItemName()
{
	return itemName;
}

string Item::getQuantity()
{
	return to_string(quantity);
}

void Item::getRandom()
{
	srand(time(NULL)+rand());
	ifstream readFromFile;
	int count = 0;
	string line;
	readFromFile.open("itemDatabase.txt");

	while (!readFromFile.eof())
	{
		getline(readFromFile, line);
		count++;
	}

	readFromFile.close();
	readFromFile.open("itemDatabase.txt");

	string* array;
	array = new string[count];
	int i = 0;

	while (!readFromFile.eof())
	{
		getline(readFromFile, array[i]);
		i++;
	}


	itemName = array[rand() % count];

	delete[] array;

	quantity = rand() % 100 + 1;
}

void main()
{
	char x[7] = { "     " }, y = '>';
	int opt;
	int cpos = 0, c;
	bool chosen = false;
	x[0] = y;

	while (1)
	{
		system("cls");
		logoDisplay();
		cout << " Please select your role: " << endl;
		cout << " " << x[0] << x[0];
		cout << " Admin" << endl;
		cout << " " << x[1] << x[1];
		cout << " Staff" << endl;
		cout << " " << x[2] << x[2];

		opt = _getch();

		if (opt == 0 || opt == 224)
		{
			opt = _getch();
			if (opt == 72)
			{
				if (cpos == 0)
				{
					cpos = 2;
				}
				cpos = cpos - 1;

				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}

				x[cpos] = y;
			}
			else if (opt == 80)
			{
				if (cpos == 1)
				{
					cpos = -1;
				}
				cpos = cpos + 1;
				for (c = 0; c < 7; c++)
				{
					x[c] = ' ';
				}
				x[cpos] = y;
			}
		}
		else if (opt == 13)
		{
			inputDetails(cpos);

		}
	}
}

void inputDetails(int choice)
{
	system("CLS");
	logoDisplay();
	string n, p;
	char ch;
	Account* ptr = nullptr;

	cout << " Username: ";
	cin >> n;
	cout << " Password: ";

	ch = _getch();
	while (ch != 13)
	{
		p.push_back(ch);
		cout << '*';
		ch = _getch();
	}

	if (choice == 0) {

		ptr = new Admin();
	}
	else if (choice == 1) {
		ptr = new Staff();
	}
	else {
		cout << "\n Invalid Input!";
		Sleep(1000);
		main();
	};
	ptr->setStreams(choice);
	ptr->setDetails(n, p);
	ptr->checkDetails(choice, ptr);
}

void logoDisplay()
{
	system("CLS");
	cout << endl;
	cout << "		  _____          _____   _____  ______   __" << endl;
	cout << "		 / ____|   /\\   |  __ \\ / ____|/ __ \\ \\ / /" << endl;
	cout << "		| |       /  \\  | |__) | |  __| |  | \\ V / " << endl;
	cout << "		| |      / /\\ \\ |  _  /| | |_ | |  | |> <  " << endl;
	cout << "		| |____ / ____ \\| | \\ \\| |__| | |__| / . \\ " << endl;
	cout << "		 \\_____/_/    \\_\\_|  \\_\\\\_____|\\____/_/ \\_\\" << endl;
	cout << endl;
}