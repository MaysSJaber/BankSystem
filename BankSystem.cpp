#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

const string ClientFilePath = "ClientFile.txt";
const string UserFilePath = "UserFile.txt";

struct stClientData {

	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

struct stUser {

	string UserName = "";
	string Password = "";
	int Permissions = 0;
	bool MarkForDelete = false;

};

stUser CurrentUser;

enum enMainMenueScreen {
	eShowAllClients = 1,
	eAddNewClients = 2,
	eUpdateClients = 3,
	eDeleteClients = 4,
	eFindClients = 5,
	eTransactionMenu = 6,
	eManegeUserMenu = 7,
	eLogOut = 8,
	Permissions = 9
};


enum enTransactionType {
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
	eMainMenue = 4
};

enum enManegeUserScreen {

	eShowAllUser = 1,
	eAddUser = 2,
	eUpdateUser = 3,
	eDeleteUser = 4,
	eMainMenu = 5

};

enum enMainMenuePermissions {
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
	pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};



bool CheckAccessPermission(enMainMenuePermissions Permission)
{
	if (CurrentUser.Permissions == enMainMenuePermissions
		::eAll)
		return true;
	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;
}

int ReadPermissionsToSet()
{

	int Permissions = 0;
	char Answer = 'n';


	cout << "\nDo you want to give full access? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		return -1;
	}

	cout << "\nDo you want to give access to : \n ";

	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{


		Permissions += enMainMenuePermissions::pListClients;
	}


	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pAddNewClient;
	}

	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pDeleteClient;
	}

	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pUpdateClients;
	}

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pFindClient;
	}

	cout << "\nTransactions? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pTranactions;
	}

	cout << "\nManage Users? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pManageUsers;
	}


	return Permissions;
}


void PrintMainMenu();
void PerformMainMenueScreen(enMainMenueScreen Option);
void PrintTransactionMenu();
void PerformTransactionMenu(enTransactionType TransactionOption);
void GoBackToManegUserScreen();
void ShowManegeUserScreen();
void ShowAccessDeniedMessage();
void PerformManegeMenueScreen(enManegeUserScreen Option);
void Login();

void PrintClientCard(stClientData Client) {
	cout << "\nThe following are the client details:\n";
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
}

void PrintClientRecord(stClientData Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector <stClientData> vClients)
{
	cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client(s)";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stClientData Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

vector <string> SplitString(string St, string Delim) {

	vector <string> vWords;
	short pos = 0;

	string sWord;

	while ((pos = St.find(Delim)) != std::string::npos) {

		sWord = St.substr(0, pos);

		if (sWord != "") {
			vWords.push_back(sWord);
		}


		St.erase(0, pos + Delim.length());

	}

	if (St != "")vWords.push_back(St);

	return vWords;

}

string ReadString() {
	cout << "Enter a String :";
	string St;
	getline(cin, St);
	return St;
}

string ConvertClientRecordToLine(stClientData Client, string Delim = "#//#") {

	string RecordLine = "";
	RecordLine += Client.AccountNumber + Delim;
	RecordLine += Client.PinCode + Delim;
	RecordLine += Client.Name + Delim;
	RecordLine += Client.Phone + Delim;
	RecordLine += to_string(Client.AccountBalance);

	return RecordLine;
}

stClientData ConvertClientLineToRecord(string sDataLine, string Delim = "#//#") {
	vector <string> vLine = SplitString(sDataLine, Delim);
	stClientData Client;
	Client.AccountNumber = vLine[0];

	Client.PinCode = vLine[1];
	Client.Name = vLine[2];
	Client.Phone = vLine[3];
	Client.AccountBalance = stod(vLine[4]);

	return Client;
}

vector <stClientData> LodeClientDataFromFile(string ClientsFile) {

	vector <stClientData> vClients;
	fstream MyFile;
	MyFile.open(ClientsFile, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line = "";
		stClientData Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertClientLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();


	}
	return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, stClientData&
	Client)
{
	vector <stClientData> vClients =
		LodeClientDataFromFile(ClientFilePath);
	for (stClientData C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

vector <stClientData> SaveClientsDataToFile(string ClientsFileName, vector <stClientData> vClient) {

	fstream MyFile;

	MyFile.open(ClientsFileName, ios::out);

	string DataLine;

	if (MyFile.is_open()) {


		for (stClientData& Client : vClient) {


			if (Client.MarkForDelete == false) {

				DataLine = ConvertClientRecordToLine(Client);
				MyFile << DataLine << endl;

			}


		}

		MyFile.close();
	}

	return vClient;
}

bool MarkClientForDeleteByAccountNumber(string ClientsFileName, string AccountNumber, vector <stClientData>& vClient) {




	for (stClientData& Client : vClient) {


		if (AccountNumber == Client.AccountNumber) {


			Client.MarkForDelete = true;


			return true;

		}



	}
	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber,
	vector <stClientData>& vClient) {

	char Answer = 'N';
	stClientData Client;

	if (FindClientByAccountNumber(AccountNumber, Client)) {

		PrintClientCard(Client);
		cout << "\nAre You Sure You Want To Delete This Client ? Y / N ? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y') {

			MarkClientForDeleteByAccountNumber(ClientFilePath, AccountNumber, vClient);
			vClient = SaveClientsDataToFile(ClientFilePath, vClient);
			return true;


		}
		else return false;
	}
	else {

		cout << "\nClient With Account Number (" << AccountNumber << ") Not Found." << endl;
		return false;

	}
}

stClientData ChangeClientRecord(string AccountNumber)
{
	stClientData Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector
	<stClientData>& vClients)
{
	stClientData Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber,
		Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n?";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (stClientData& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientFilePath, vClients);
			cout << "\n\nClient Updated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber
			<< ") is Not Found!";
		return false;
	}
}

void AddClientDataLineToFile(string ClientFile, string sDataLine) {


	fstream MyFile;

	MyFile.open(ClientFile, ios::out | ios::app);

	if (MyFile.is_open()) {

		MyFile << sDataLine << endl;

		MyFile.close();

	}

}

bool ClientExistsByAccountNumber(string AccountNumber, string
	FileName)
{
	vector <stClientData> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stClientData Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertClientLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
	}
	return false;
}

stClientData ReadNewClient(stClientData Client) {

	cout << "Enter Account Number :";

	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientFilePath)) {
		cout << "\nAccount Number already exists, please enter another account number :";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter Pin Code :";
	getline(cin, Client.PinCode);

	cout << "Enter Name :";
	getline(cin, Client.Name);

	cout << "Enter Phone :";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance :";
	cin >> Client.AccountBalance;

	return Client;
}

void AddClient() {

	stClientData Client;
	Client = ReadNewClient(Client);
	string Line = ConvertClientRecordToLine(Client, "#//#");
	AddClientDataLineToFile(ClientFilePath, Line);

}

void AddClients() {

	stClientData Client;

	char AddMore = 'Y';

	do {

		system("cls");
		cout << "Adding New Client:\n\n";
		AddClient();
		cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');

}

string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

string Tabs(int N) {


	string Tab = "";
	for (int i = 0; i < N; i++) {
		Tab += "\t";
	}
	return Tab;
}


void GoBackToMainMenu() {
	cout << "\nPress any key to go back to main menu...";
	system("pause>0");
	system("cls");
	PrintMainMenu();
}


void ShowAllClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector <stClientData> vClients = LodeClientDataFromFile(ClientFilePath);

	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tShow All Clients\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	PrintAllClientsData(vClients);

}

void ShowAddNewClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tAdd New Client\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	AddClients();

}

void ShowUpdateClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector <stClientData> vClient = LodeClientDataFromFile(ClientFilePath);
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tUpdate Client\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClient);

}

void ShowDeleteClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector <stClientData> vClient = LodeClientDataFromFile(ClientFilePath);
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tDelete Client\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClient);

}

void ShowFindClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector <stClientData> vClient = LodeClientDataFromFile(ClientFilePath);
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tFind Client\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	string AccountNumber = ReadClientAccountNumber();
	stClientData Client;
	if (FindClientByAccountNumber(AccountNumber, Client)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\nClient With Account Number (" << AccountNumber << ") Not Found.\n";
	}

}

void ShowExitScreen() {


	system("cls");
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tEnd Program :) --\t\t";
	cout << "\n\t\t_______________________________________________________\n";

}

short ReadOption() {
	short Option;
	cout << "\nPlease choose an Operation? ";
	cin >> Option;
	return Option;
}


void PrintMainMenu() {
	system("cls");
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t_______________________________________________________\n";
	cout << Tabs(5) << "Main Menu\t\t\n";
	cout << Tabs(2) << "_______________________________________________________\n";
	cout << Tabs(2) << "_______________________________________________________\n\n";
	cout << Tabs(2) << "[1] Show All Clients\n";
	cout << Tabs(2) << "[2] Add New Client\n";
	cout << Tabs(2) << "[3] Update Client\n";
	cout << Tabs(2) << "[4] Delete Client\n";
	cout << Tabs(2) << "[5] Find Client\n";
	cout << Tabs(2) << "[6]Transaction Menu\n";
	cout << Tabs(2) << "[7] Manege User Menu\n";
	cout << Tabs(2) << "[8] Logout\n";
	PerformMainMenueScreen((enMainMenueScreen)ReadOption());
}



void PrintClientBalance(stClientData Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}




void PrintAllClientsBalances(vector <stClientData> vClients)
{
	cout << "\n\t\t\t\tClient Balances(" << vClients.size() << ") Client(s)";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stClientData Client : vClients)
	{
		PrintClientBalance(Client);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}





bool DepositBalanceToClientByAccountNumber(string AccountNumber,
	double Amount, vector <stClientData>& vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction?y / n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (stClientData& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientFilePath, vClients);
				cout << "\n\nDone Successfully. New balance is: "
					<< C.AccountBalance;
				return true;
			}
		}
		return false;
	}
}


void TotalBalance() {

	double TotalBalance = 0;
	vector <stClientData> vClient = LodeClientDataFromFile(ClientFilePath);

	for (stClientData& Client : vClient) {

		TotalBalance += Client.AccountBalance;

	}
	PrintAllClientsBalances(vClient);
	cout << "Total Balance : " << TotalBalance << endl;

}

short ReadTransactionOption() {
	short Option;
	cout << "\nPlease choose a Transaction? ";
	cin >> Option;
	return Option;
}


void GoBackToTransactionMenu() {
	cout << "\nPress any key to go back to transaction menu...";
	system("pause>0");
	PrintTransactionMenu();
}


void ShowDepositScreen() {


	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";
	stClientData Client;
	vector <stClientData> vClients =
		LodeClientDataFromFile(ClientFilePath);
	string AccountNumber = ReadClientAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does notexist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount,
		vClients);

}


void ShowWithdrawScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";
	stClientData Client;
	vector <stClientData> vClients =
		LodeClientDataFromFile(ClientFilePath);
	string AccountNumber = ReadClientAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does notexist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw upto : " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Amount;
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount *
		-1, vClients);
}

void ShowTotalBalanceScreen() {

	TotalBalance();
}

void ShowMainMenuScreen() {

	PrintMainMenu();
}


void PrintTransactionMenu() {

	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	system("cls");
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\n\t\t_______________________________________________________\n";
	cout << Tabs(4) << "Transaction Menu\t\t\n";
	cout << Tabs(2) << "_______________________________________________________\n";
	cout << Tabs(2) << "_______________________________________________________\n\n";
	cout << Tabs(2) << "[1] Deposit\n";
	cout << Tabs(2) << "[2] Withdraw\n";
	cout << Tabs(2) << "[3] Total Balance\n";
	cout << Tabs(2) << "[4] Main Menu\n";
	PerformTransactionMenu((enTransactionType)ReadTransactionOption());
}


void PerformTransactionMenu(enTransactionType TransactionOption) {

	switch (TransactionOption) {
	case eDeposit:
		ShowDepositScreen();
		GoBackToTransactionMenu();
		break;
	case eWithdraw:
		ShowWithdrawScreen();
		GoBackToTransactionMenu();
		break;
	case eTotalBalances:
		ShowTotalBalanceScreen();
		GoBackToTransactionMenu();
		break;
	case eMainMenue:
		ShowMainMenuScreen();
		break;

	default:cout << "\nInvalid Option. Please choose a valid option.\n";
		GoBackToTransactionMenu();
		break;
	}


}


void PerformMainMenueScreen(enMainMenueScreen Option) {

	switch ((enMainMenueScreen)Option) {
	case enMainMenueScreen::eShowAllClients:
		ShowAllClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenueScreen::eAddNewClients:
		ShowAddNewClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenueScreen::eUpdateClients:
		ShowUpdateClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenueScreen::eDeleteClients:
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenueScreen::eFindClients:
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenueScreen::eTransactionMenu:PrintTransactionMenu();
		break;
	case enMainMenueScreen::eManegeUserMenu:
		ShowManegeUserScreen();
		break;
	case enMainMenueScreen::eLogOut:
		ShowExitScreen();
		Login();
		break;
	default:
		cout << "\nInvalid Option. Please choose a valid option.\n";
		GoBackToMainMenu();
		break;
	}

}



stUser ReadUser() {

	stUser User;

	cout << "\nEnter User Name : \n";
	getline(cin >> ws, User.UserName);

	cout << "\nEnter User Password : \n";
	getline(cin, User.Password);

	cout << "\nEnter User Permission : \n";

	User.Permissions = ReadPermissionsToSet();

	return User;

}

void ShowAccessDeniedMessage()
{
	system("cls");

	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}

string ConvertUserRecordToLine(stUser User, string Delime = "#//#") {

	string Line = "";
	Line += User.UserName + Delime;
	Line += User.Password + Delime;
	Line += to_string(User.Permissions);

	return Line;
}

void AddUserDataToFile(string UserFilePath, string sLineData) {

	fstream MyFile;

	MyFile.open(UserFilePath, ios::out | ios::app);

	if (MyFile.is_open()) {

		MyFile << sLineData << endl;

		MyFile.close();

	}

}

void AddUser() {

	stUser UserDataRecord = ReadUser();

	string UserDataLine = ConvertUserRecordToLine(UserDataRecord, "#//#");

	AddUserDataToFile(UserFilePath, UserDataLine);

}

void AddUsers() {

	char AddMore = 'Y';

	do {

		cout << "Adding New User:\n\n";
		AddUser();
		cout << "\nUser Added Successfully, do you want to add more User ? Y / N ? ";
		cin >> AddMore;

	} while (tolower(AddMore) == 'y');

	system("cls");

}



void ShowAddNewUserScreen() {

	cout << "\n\n\t\t-------------------------------------------------\n\n";
	cout << Tabs(5) << "Add New User Screen ";
	cout << "\n\n\t\t-------------------------------------------------\n\n";
	AddUsers();

}

stUser ConvertUserLineToRecord(string UserLineData, string Delime = "#//#") {

	vector <string>vUserData = SplitString(UserLineData, Delime);

	stUser User;

	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	User.Permissions = stoi(vUserData[2]);

	return User;
}

bool UserExistsByUsername(string Username, string FileName)
{


	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{

		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{

			User = ConvertUserLineToRecord(Line);
			if (User.UserName == Username)
			{
				MyFile.close();
				return true;
			}

		}

		MyFile.close();

	}

	return false;


}
vector <stUser> LoadUserDataFromFile(string UserFilePath) {

	vector <stUser> vUser;

	fstream MyFile;

	MyFile.open(UserFilePath, ios::in);

	if (MyFile.is_open()) {

		stUser User;

		string LineData = "";

		while (getline(MyFile, LineData)) {

			User = ConvertUserLineToRecord(LineData);

			vUser.push_back(User);
		}
		MyFile.close();

	}

	return vUser;
};

void SaveUserDataToFile(vector <stUser> vUser, string UserFilePath) {


	fstream MyFile;

	MyFile.open(UserFilePath, ios::out);

	if (MyFile.is_open()) {

		string Line = " ";
		for (stUser& User : vUser) {

			if (User.MarkForDelete == false) {

				Line = ConvertUserRecordToLine(User, "#//#");

				MyFile << Line << endl;

			}

		}

		MyFile.close();
	}


}

void PrintUserData(stUser User) {
	cout << "\nThe following are the user details:\n";
	cout << "\nUser Name: " << User.UserName;
	cout << "\nPassword : " << User.Password;
	cout << "\nPermission : " << User.Permissions;
}

stUser ChangeUserDataRecord(string UserName) {


	stUser User;
	User.UserName = UserName;

	cout << "\n\nEnter The Password : ";
	getline(cin >> ws, User.Password);

	//cout << "\nEnter The Perrmision : ";

	User.Permissions = ReadPermissionsToSet();

	return User;
}
bool FindUserByUsername(string Username, vector <stUser> vUsers,
	stUser& User)
{
	for (stUser U : vUsers)
	{
		if (U.UserName == Username)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUserByUsernameAndPassword(string Username, string
	Password, stUser& User)
{
	vector <stUser> vUsers = LoadUserDataFromFile(UserFilePath);
	for (stUser U : vUsers)
	{
		if (U.UserName == Username && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUserByName(string FileUserPath, stUser& User, string UserName) {

	vector <stUser> vUser = LoadUserDataFromFile(FileUserPath);

	for (stUser U : vUser) {

		if (U.UserName == UserName) {

			User = U;
			return true;

		}



	}

	return false;

}

string ReadUserName() {

	string UserName;

	cout << "\nEnter User Name Pleas :";

	cin >> UserName;

	return UserName;
}

bool UpdateUserDataToFile(string FileUserPath, string UserName) {

	stUser UserToUpdate;
	vector <stUser> vUser = LoadUserDataFromFile(FileUserPath);

	if (UserName == "Admin")
	{
		cout << "\n\nYou cannot Update This User.";
		return false;

	}


	char Answer = 'Y';
	if (FindUserByName(FileUserPath, UserToUpdate, UserName)) {

		for (stUser& User : vUser) {


			if (User.UserName == UserToUpdate.UserName) {

				cout << "\nAre You Sure You Want To Update This User ? Y / N ?";
				cin >> Answer;
				if (tolower(Answer) == 'y') {

					User = ChangeUserDataRecord(UserName);
					SaveUserDataToFile(vUser, FileUserPath);

					cout << "\nUser Update Successfully\n";

					return true;
				}

			}

		}


	}
	else {

		cout << "\nUser with Name (" << UserName << ") is Not Found!\n";
		return false;
	}
}

void ShowUpdateUserDataScreen() {

	cout << "\n\n\t\t-------------------------------------------------\n\n";
	cout << Tabs(5) << "Update User Screen ";
	cout << "\n\n\t\t-------------------------------------------------\n\n";

	UpdateUserDataToFile(UserFilePath, ReadUserName());

}

void MarkUserForDelete(vector <stUser>& vUser, string UserName) {

	stUser User;


	if (FindUserByName(UserFilePath, User, UserName)) {

		for (stUser& User : vUser) {

			if (User.UserName == UserName) {


				User.MarkForDelete = true;
			}

		}

	}


}

bool DeleteUserDataFromFile(string UserFilePath, string UserName) {


	if (UserName == "Admin")
	{
		cout << "\n\nYou cannot Delete This User.";
		return false;

	}

	vector <stUser> vUser = LoadUserDataFromFile(UserFilePath);
	stUser User;

	MarkUserForDelete(vUser, UserName);

	fstream MyFile;
	char Answer = 'Y';

	if (FindUserByName(UserFilePath, User, UserName)) {

		string Line = " ";

		cout << "\nAre You Sure You Want To Delete This User ? Y / N ?";

		cin >> Answer;

		if (tolower(Answer) == 'y')

			for (stUser User : vUser) {

				Line = ConvertUserRecordToLine(User, "#//#");
				SaveUserDataToFile(vUser, UserFilePath);
			}
		cout << "\nUser Delete Successfully\n";
		vUser = LoadUserDataFromFile(UserFilePath);
		return true;

	}
	else {
		cout << "\nDelete operation cancelled.\n";
		return false;
	}


	cout << "\nUser with Name (" << UserName << ") is Not Found!\n";
	return false;

}

void ShowDeleteUserScreen() {

	cout << "\n\n\t\t-------------------------------------------------\n\n";
	cout << Tabs(5) << "Delete User Screen ";
	cout << "\n\n\t\t-------------------------------------------------\n\n";

	DeleteUserDataFromFile(UserFilePath, ReadUserName());

}


void PrintUserRecord(stUser User)
{
	cout << "| " << setw(20) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(5) << left << User.Permissions;

}



void PrintAllUserData(vector <stUser> vUser)
{
	cout << "\n\t\t\t\tUser List (" << vUser.size() << ") User(s)";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(20) << "User Name :";
	cout << "| " << left << setw(10) << "Password :";
	cout << "| " << left << setw(5) << "Permission :";

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stUser User : vUser)
	{
		PrintUserRecord(User);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void ShowAllUserScreen() {


	vector <stUser> vUser = LoadUserDataFromFile(UserFilePath);

	PrintAllUserData(vUser);

}

void PrintManegeUserMenu() {

	system("cls");
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\n\t\t_______________________________________________________\n";
	cout << Tabs(4) << "Manege User Menu\t\t\n";
	cout << Tabs(2) << "_______________________________________________________\n";
	cout << Tabs(2) << "_______________________________________________________\n\n";
	cout << Tabs(2) << "[1] Show All User \n";
	cout << Tabs(2) << "[2] Add User\n";
	cout << Tabs(2) << "[3] Update User\n";
	cout << Tabs(2) << "[4] Delete User\n";
	cout << Tabs(2) << "[5] Main Menu\n";
	PerformManegeMenueScreen((enManegeUserScreen)ReadOption());

}

void PerformManegeMenueScreen(enManegeUserScreen Option) {

	switch ((enManegeUserScreen)Option) {
	case enManegeUserScreen::eShowAllUser:
		ShowAllUserScreen();
		GoBackToManegUserScreen();
		break;
	case enManegeUserScreen::eAddUser:

		ShowAddNewUserScreen();
		GoBackToManegUserScreen();
		break;
	case enManegeUserScreen::eUpdateUser:
		ShowUpdateUserDataScreen();
		GoBackToManegUserScreen();
		break;
	case enManegeUserScreen::eDeleteUser:
		ShowDeleteUserScreen();
		GoBackToManegUserScreen();
		break;

	case enManegeUserScreen::eMainMenu:
		GoBackToMainMenu();
		break;
	default:
		cout << "\nInvalid Option. Please choose a valid option.\n";
		GoBackToManegUserScreen();
		break;
	}

}

enManegeUserScreen ReadOparetorOnUser() {

	cout << "\n\nEnter Opertor Pleas :";
	short Option;
	cin >> Option;

	return (enManegeUserScreen)Option;

}

void GoBackToManegUserScreen() {

	cout << "\nPress any key to go back to Manege User menu...";
	system("pause>0");
	PrintManegeUserMenu();

}

void ShowManegeUserScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	PrintManegeUserMenu();

}


bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByUsernameAndPassword(Username, Password,
		CurrentUser))
		return true;
	else
		return false;
}


void Login()
{
	bool LoginFaild = false;
	string Username, Password;
	do
	{
		system("cls");
		cout << "\n";
		cout << "\t\t=========================================\n";
		cout << "\t\t             Login Screen\n";
		cout << "\t\t=========================================\n\n";
		if (LoginFaild)
		{
			cout << "Invlaid Username/Password!\n";
		}
		cout << "Enter Username? ";
		cin >> Username;
		cout << "Enter Password? ";
		cin >> Password;
		LoginFaild = !LoadUserInfo(Username, Password);
	} while (LoginFaild);
	ShowMainMenuScreen();
}

int main() {

	//Eng.Mays Jaber .. 

	Login();

	system("pause>0");
	return 0;
}
