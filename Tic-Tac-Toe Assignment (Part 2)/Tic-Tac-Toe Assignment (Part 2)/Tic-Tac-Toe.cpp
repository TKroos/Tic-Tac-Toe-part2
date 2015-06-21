#include <iostream>
#include <fstream>
#include "string.h"
#include <time.h>
#include <windows.h> 
using namespace std;
int arr[101][101] = { 100 };
int b[101][101] = { 100 };
clock_t times[100];
int t[100],d[101][101];
const char c[26] = { 'X', 'O', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'Y', 'Z' };
int x, y, winner, q, p, number, bonusmode, players;//q个人玩, p*p棋盘
bool nexttype;//true for X, false for O;
char str[3] = {};
void saveandexit(int m, int n, int player){
	int i, j, k;
	ofstream SaveFile("Save&Exit.txt");
	SaveFile << m << endl;
	SaveFile << n << endl;
	SaveFile << number << endl;
	SaveFile << bonusmode << endl;
	SaveFile << player << endl;
	for (i = 0; i < player; i++){
		SaveFile << times[i] << endl;
	}
	for (j = 1; j <= 2 * n; j++)
	{
		for (i = 1; i <= n * 4; i++)
		{
			if (j == 1)//handle the first row
			{
				if ((i + 1) % 4 == 0)//write column number
				{
					SaveFile << (i + 1) / 4;
				}//end if i%3
				else
				{
					SaveFile << " ";
				}//end if i%3
			} // end if j==1
			else
			{
				if ((j + 1) % 2 == 0)//for a row all with -----
				{
					if (i != 1)
					{
						if (i % 4 == 1)
						{
							SaveFile << "+";
						}
						else
						{
							SaveFile << "-";
						}
					}
					else
					{
						SaveFile << " ";
					}

				}//(j+1)%2 ==0 
				else
				{
					if (i == 1) SaveFile << j / 2; //write row number
					else if (i % 4 == 1)
					{
						SaveFile << "|";
					}
					else
					{
						//key place
						if ((i + 1) % 4 == 0)
						{
							if (b[j / 2][(i + 2) / 4] == 100)
							{
								SaveFile << " ";
							}
							else
							{
								for (k = 0; k < m; k++){
									if (b[j / 2][(i + 2) / 4] == k)
									{
										_itoa_s(arr[j / 2][(i + 2) / 4] + 1, str, 10);
										SaveFile << str;
									}
								}
							}
						}
						else
						{
							SaveFile << " ";
						}
					}
				}//(j+1)%2 ==0
			}// end if j==1
		}// for i
		SaveFile << endl;
	}//for j
}
int bonus1(int m, int n, int number){//找到一条横边
	int i, j, k;
	i = m;
	j = n;
	k = 1;
	while ((j > 0) && (b[i - 1][j] == b[i][j])){//column
		i--;
		k++;
	}
	if (k != number - 1) {
		i = m; k = 1;
		while ((b[i + 1][j] == b[i][j])){
			i++;
			k++;
		}
		if (k != number - 1) return 100;//not finish
		else
		{
			k = 1;
			while ((i > 0) && (b[i][j - 1] == b[i][j])){
				j--;
				k++;
			}
			if (k != number - 1) return 100;//not finish
			else {
				k = 1;
				while ((j > 0) && (b[i - 1][j] == b[i][j])){//column
					i--;
					k++;
				}
				if (k != number - 1) return 100;//not finish
				else return (b[i][j]);
			}
		}
	}
	else
	{
		k = 1;
		while ((i > 0) && (b[i][j - 1] == b[i][j])){
			j--;
			k++;
		}
		if (k != number - 1) return 100;//not finish
		else {
			k = 1;
			while ((b[i + 1][j] == b[i][j])){
				i++;
				k++;
			}
			if (k != number - 1) return 100;//not finish
			else return (b[i][j]);
		}
	}
};
int bonus2(int m, int n, int number){//找到一条竖边
	int i, j, k;
	i = m;
	j = n;
	k = 1;
	while ((i > 0) && (b[i][j - 1] == b[i][j])){
		j--;
		k++;
	}
	if (k != number - 1) {
		i = m; k = 1;
		while (b[i][j + 1] == b[i][j]){
			j++;
			k++;
		}
		if (k != number - 1) return 100;//not finish
		else
		{
			k = 1;
			while ((j > 0) && (b[i - 1][j] == b[i][j])){//column
				i--;
				k++;
			}
			if (k != number - 1) return 100;//not finish
			else {
				k = 1;
				while ((i > 0) && (b[i][j - 1] == b[i][j])){
					j--;
					k++;
				}
				if (k != number - 1) return 100;//not finish
				else return (b[i][j]);
			}
		}
	}
	else
	{
		k = 1;
		while ((j > 0) && (b[i - 1][j] == b[i][j])){//column
			i--;
			k++;
		}
		if (k != number - 1) return 100;//not finish
		else {
			k = 1;
			while (b[i][j + 1] == b[i][j]){
				j++;
				k++;
			}
			if (k != number - 1) return 100;//not finish
			else return (b[i][j]);
		}
	}
};
int testend(int m, int n, int number, int bonusmodeswitch)
{
	int i,j,k;
	i = m;
	j = n;
	k = 1;
	while ((i > 0) && (b[i][j - 1] == b[i][j])){//row
		j--;
		k++;
	}
	j = n;
	while (b[i][j + 1] == b[i][j]){
		j++;
		k++;
	}
	if (k >= number)
	{
		winner = b[x][y];
		return winner;
	}
	if ((bonusmodeswitch==1)&&(k == number - 1)&&(bonus1(i, j, number) != 100)) 
	{
		winner = b[x][y];
		return winner;
	}
	i = m;
	j = n;
	k = 1;
	while ((j > 0) && (b[i - 1][j] == b[i][j])){//column
		i--;
		k++;
	}
	i = m;
	while (b[i + 1][j] == b[i][j]){
		i++;
		k++;
	}
	if (k >= number)
	{
		winner = b[x][y];
		return winner;
	}
	if ((bonusmodeswitch == 1) && (k == number - 1) && (bonus2(i, j, number) != 100))
	{
		winner = b[x][y];
		return winner;
	}
	i = m;
	j = n;
	k = 1;
	while ((i > 0) && (j > 0) && (b[i - 1][j - 1] == b[i][j])){//diagonal 1
		j--;
		i--;
		k++;
	}
	i = m;
	j = n;
	while (b[i + 1][j + 1] == b[i][j]){
		j++;
		i++;
		k++;
	}
	if (k >= number)
	{
		winner = b[x][y];
		return winner;
	}
	i = m;
	j = n;
	k = 1;
	while ((i > 0) && (j > 0) && (b[i - 1][j + 1] == b[i][j])){//diagonal 2
		j++;
		i--;
		k++;
	}
	i = m;
	j = n;
	while ((i > 0) && (j > 0) && (b[i + 1][j - 1] == b[i][j])){
		j--;
		i++;
		k++;
	}
	if (k >= number)
	{
		winner = b[x][y];
		return winner;
	}
	return 100;//not finish
}
void replaygame(int m, int n)//m个人玩, n*n棋盘
{
	int i, j, k;
	for (j = 1; j <= 2*n; j++)
	{
			for (i = 1; i <= n * 4; i++)
			{
				if (j == 1)//handle the first row
				{
					if ((i + 1) % 4 == 0)//write column number
					{
						cout << (i + 1) / 4;
					}//end if i%3
					else
					{
						cout << " ";
					}//end if i%3
				} // end if j==1
				else
				{
					if ((j + 1) % 2 == 0)//for a row all with -----
					{
						if (i != 1)
						{
							if (i % 4 == 1)
							{
								cout << "+";
							}
							else
							{
								cout << "-";
							}
						}
						else
						{
							cout << " ";
						}

					}//(j+1)%2 ==0 
					else
					{
						if (i == 1) cout << j / 2; //write row number
						else if (i % 4 == 1)
						{
							cout << "|";
						}
						else
						{
							//key place
							if ((i + 1) % 4 == 0)
							{
								if (d[j / 2][(i + 2) / 4] == 100)
								{
									cout << " ";
								}
								else 
								{
									for (k = 0; k < m; k++){
										if (d[j / 2][(i + 2) / 4] == k) cout << c[k];
									}
								}
							}
							else
							{
								cout << " ";
							}
						}
					}//(j+1)%2 ==0
				}// end if j==1
			}// for i	
		cout << endl;
	}//for j
}//end of function
void paintbg(int m, int n)//m个人玩, n*n棋盘
{
	int i, j, k;
	for (j = 1; j <= 2 * n; j++)
	{
		for (i = 1; i <= n * 4; i++)
		{
			if (j == 1)//handle the first row
			{
				if ((i + 1) % 4 == 0)//write column number
				{
					cout << (i + 1) / 4;
				}//end if i%3
				else
				{
					cout << " ";
				}//end if i%3
			} // end if j==1
			else
			{
				if ((j + 1) % 2 == 0)//for a row all with -----
				{
					if (i != 1)
					{
						if (i % 4 == 1)
						{
							cout << "+";
						}
						else
						{
							cout << "-";
						}
					}
					else
					{
						cout << " ";
					}

				}//(j+1)%2 ==0 
				else
				{
					if (i == 1) cout << j / 2; //write row number
					else if (i % 4 == 1)
					{
						cout << "|";
					}
					else
					{
						//key place
						if ((i + 1) % 4 == 0)
						{
							if (b[j / 2][(i + 2) / 4] == 100)
							{
								cout << " ";
							}
							else
							{
								for (k = 0; k < m; k++){
									if (b[j / 2][(i + 2) / 4] == k) cout << c[k];
								}
							}
						}
						else
						{
							cout << " ";
						}
					}
				}//(j+1)%2 ==0
			}// end if j==1
		}// for i	
		cout << endl;
	}//for j
}//end of function
void move(int m, int n, int player){//m个人玩, n*n棋盘, 走了player步
	int k = player;
	int r;
	do{
		r = k % m;
		do{
			cout << "Player " << r+1 << ": Input the row number." << endl;
			cout << "Enter 100 to Save & Exit!" << endl;
			cin >> x;
			if (x == 100) break;
			while (((x > n) && (x!=100)) || x <= 0 || cin.fail())
			{
				cout << "Wrong row number. Input row number again." << endl;
				cin.clear();
				cin.sync();
				cin >> x;
			}
			cout << "Player " << r+1 << ": Input the column number." << endl;
			cout << "Enter 100 to Save & Exit!" << endl;
			cin >> y;
			if (y == 100) break;
			while (((y > n) && (y != 100)) || y <= 0 || cin.fail())
			{
				cout << "Wrong column number. Input column number again." << endl;
				cin.clear();
				cin.sync();
				cin >> y;
			}
			if (arr[x][y] != 100) cout << "The position has been OCCUPIED, Please enter AGAIN!" << endl;
		} while (arr[x][y] != 100);
		if ((x == 100) || (y == 100))
		{
			saveandexit(q, p, k);
			break;
		}
		else{
			arr[x][y] = k;
			b[x][y] = r;
			system("cls");
			times[k] = clock();
			paintbg(m, n);
			cout << endl;
			if (testend(x, y, number, bonusmode) != 100) break;
			k++;
		}
	} while (k<=n*n);
};
bool checkwinningpossible(int m, int n, int number){
	if ((n*n > (m*(number - 1))) && (number - 1 <= n)) return true;
	else return false;
}
void loadgame(){
	char s[100];
	int i, j;
	ifstream LoadFile("Save&Exit.txt");
	LoadFile.getline(s, ' ');
	sscanf_s(s, "%d", &q);
	LoadFile.getline(s, ' ');
	sscanf_s(s, "%d", &p);
	LoadFile.getline(s, ' ');
	sscanf_s(s, "%d", &number);
	LoadFile.getline(s, ' ');
	sscanf_s(s, "%d", &bonusmode);
	LoadFile.getline(s, ' ');
	sscanf_s(s, "%d", &players);
	for (i = 0; i < players; i++){
		LoadFile.getline(s, ' ');
		sscanf_s(s, "%d", &t[i]);
		times[i] = t[i];
//		cout << times[i] << endl;
	}
	for (i = 1; i <= 2*p; i++){
		if (i % 2 == 1) LoadFile.getline(s, 100);
		else
		{
			LoadFile.ignore(3, ' ');
			for (j = 1; j <= p; j++)
			{
				s[2] = {};
				LoadFile.read(s, 2);
				sscanf_s(s, "%d", &(arr[i / 2][j]));
				if (arr[i / 2][j] != 100) {
					arr[i / 2][j] = arr[i / 2][j] - 1;
					b[i / 2][j] = arr[i / 2][j] % q;
				}
//				cout << arr[i / 2][j] << " ";
				if ((arr[i / 2][j] <= 8) || (arr[i / 2][j] == 100))	LoadFile.seekg(2, ios::cur);
				else LoadFile.seekg(3, ios::cur);
			}
		}
	}
}
void main()
{
	int i, j, k, gamemode, replay;
	for (i = 0; i <= 101; i++){
		for (j = 0; j <= 101; j++){
			arr[i][j] = 100;
			b[i][j] = 100;
			d[i][j] = 100;
		}
	}
	cout << "Welcome to play tic-tac-toe!" << endl;
	cout << "Would you like to start a NEW game or resume a SAVED game?" << endl;
	cout << "Enter 1: for a new game; 2: for a saved game!" << endl;
	cin >> gamemode;
	if (gamemode == 1){
		cout << "How many players?" << endl;
		cin >> q;
		cout << "How large the board should be?" << endl;
		cin >> p;
		cout << "What the win sequence count should be?" << endl;
		cout << "For example: Enter '3' means 3 in a row!" << endl;
		cin >> number;
		while (number > p){
			cout << "Error! Impossible to WIN! Please Enter AGAIN!!" << endl;
			cin >> number;
		}
		if (checkwinningpossible(q, p, number) == false) cout << "Error! Impossible to WIN!" << endl;
		else{
			cout << "Do You want to open BONUS MODE?" << endl;
			cout << "BONUS MODE: Allow winning by a SQUARE whose SIDE DIMENSIONS are n - 1, where n is the win sequence count.(Squares may be HOLLOW)" << endl;
			cout << "1: Open; 2: Close!" << endl;
			cin >> bonusmode;
			system("cls");
			cout << "OK, Let's GO" << endl;
			for (i = 0; i <= 101; i++){
				for (j = 0; j <= 101; j++){
					arr[i][j] = 100;
					b[i][j] = 100;
				}
			}
			paintbg(q, p);
			move(q, p, 0);
			if ((testend(x, y, number, bonusmode) == 100)&&(x!=100)&&(y!=100)) cout << "Tie!" << endl;
			else if (testend(x, y, number, bonusmode) != 100) cout << "Winner is Player " << testend(x, y, number, bonusmode) + 1 << endl;
			else cout << "Save Successful!" << endl;
		}
	}
	else {
		loadgame();
		cout << "Do You want to REPLAY the Game?" << endl;
		cout << "1: Yes; 2: No!" << endl;
		cin >> replay;
		system("cls");
		if (replay == 2) paintbg(q, p);
		else {
			for (i = 0; i <= players - 1; i++){
				for (j = 1; j <= p; j++){
					for (k = 1; k <= p; k++){
						if (arr[j][k] == i)
						{
							d[j][k] = b[j][k];
							break;
						}
					}
				}
				replaygame(q, p);
				cout << "MOVE " << i + 1 << endl;
				if (i > 0) cout << "Time Consumed: " << t[i] - t[i - 1] << endl;
				if (i < players - 1)
				{
					Sleep(t[i + 1] - t[i]);
					system("cls");
				}
			}
		}
		move(q, p, players);
		if ((testend(x, y, number, bonusmode) == 100) && (x != 100) && (y != 100)) cout << "Tie!" << endl;
		else if (testend(x, y, number, bonusmode) != 100) cout << "Winner is Player " << testend(x, y, number, bonusmode) + 1 << endl;
		else cout << "Save Successful!" << endl;
	}
	system("pause");
}