#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

const string FileName = "ClientFile.txt";

struct stClientData {

	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

enum enMainMenueScreen {
	eShowAllClients = 1,
	eAddNewClients = 2,
	eUpdateClients = 3,
	eDeleteClients = 4,
	eFindClients = 5,
	eTransactionMenu = 6,
	eExit = 7
};


enum enTransactionType {
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
	eMainMenue = 4
};


void PrintMainMenu();
void PerformMainMenueScreen(enMainMenueScreen Option);
void PrintTransactionMenu();
void PerformTransactionMenu(enTransactionType TransactionOption);


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

string ConvertRecordToLine(stClientData Client, string Delim = "#//#") {

	string RecordLine = "";
	RecordLine += Client.AccountNumber + Delim;
	RecordLine += Client.PinCode + Delim;
	RecordLine += Client.Name + Delim;
	RecordLine += Client.Phone + Delim;
	RecordLine += to_string(Client.AccountBalance);

	return RecordLine;
}

stClientData ConvertLineToRecord(string sDataLine, string Delim = "#//#") {
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
			Client = ConvertLineToRecord(Line);
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
		LodeClientDataFromFile(FileName);
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

				DataLine = ConvertRecordToLine(Client);
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

			MarkClientForDeleteByAccountNumber(FileName, AccountNumber, vClient);
			vClient = SaveClientsDataToFile(FileName, vClient);
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
			SaveClientsDataToFile(FileName, vClients);
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
			Client = ConvertLineToRecord(Line);
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

	while (ClientExistsByAccountNumber(Client.AccountNumber, FileName)) {
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
	string Line = ConvertRecordToLine(Client, "#//#");
	AddClientDataLineToFile(FileName, Line);

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

	vector <stClientData> vClients = LodeClientDataFromFile(FileName);

	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tShow All Clients\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	PrintAllClientsData(vClients);

}

void ShowAddNewClientScreen() {

	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tAdd New Client\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	AddClients();

}

void ShowUpdateClientScreen() {
	vector <stClientData> vClient = LodeClientDataFromFile(FileName);
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tUpdate Client\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClient);

}

void ShowDeleteClientScreen() {

	vector <stClientData> vClient = LodeClientDataFromFile(FileName);
	cout << "\n\t\t_______________________________________________________\n";
	cout << "\t\t\t\tDelete Client\t\t";
	cout << "\n\t\t_______________________________________________________\n";
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClient);

}

void ShowFindClientScreen() {

	vector <stClientData> vClient = LodeClientDataFromFile(FileName);
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
	cout << Tabs(2) << "[7] Exit\n";
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
				SaveClientsDataToFile(FileName, vClients);
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
	vector <stClientData> vClient = LodeClientDataFromFile(FileName);

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
		LodeClientDataFromFile(FileName);
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
		LodeClientDataFromFile(FileName);
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
	case enMainMenueScreen::eExit:
		ShowExitScreen();
		break;
	default:
		cout << "\nInvalid Option. Please choose a valid option.\n";
		GoBackToMainMenu();
		break;
	}

}


int main() {

	//Eng.Mays Jaber .. 

	PrintMainMenu();
	system("pause>0");
	return 0;


}