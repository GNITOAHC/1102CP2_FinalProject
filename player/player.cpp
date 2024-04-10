#include <iostream>
#include <vector>
#include <math.h>

#define int_min -2147483647 
using namespace std;

typedef struct Position {
	int x;
	int y;
} Position;

int curround;
char character, foe_character;

Position find_position(const vector<vector<char> > &map, const char character) {
	Position return_position;
	for (int i = 0; i < map.size(); ++i) {
		for (int j = 0; j < map[0].size(); ++j) {
			if (map[i][j] == character) {
				return_position.x = i;
				return_position.y = j;
				break;
			}
		}
	}
	return return_position;
}

bool valid_position(vector<vector<char> > map, const int x, const int y) {
	if (map[x][y] == 'x' || map[x][y] == 'A' || map[x][y] == 'B')
		return false;
	else if (x < 0 || y < 0)
		return false;
	else if (x >= map.size() - 1 || y >= map[0].size() - 1)
		return false;
	return true;
}

Position count_mushrooms_and_stars(vector<vector<char> > map) { // .x -> mush, .y -> star
	Position ans;
	ans.x = 0;
	ans.y = 0;
	for (int i=0; i<map.size(); ++i) {
		for (int j=0; j<map[0].size(); ++j) {
			if (map[i][j] == 'm')
				++ans.x;
			else if (map[i][j] == 's')
				++ans.y;
		}
	}
	return ans;
}

int alpha_beta(const int _myScore, const int _foeScore, const int &_round, vector<vector<char> > _map, Position _countMashroomsAndStar, const Position myPos, const Position foePos, const Position newMe, const Position newFoe);

void decide(const vector<vector<char> > &map, const int myScore, const int foeScore, const char me, const int _round) {
	char notme = 'A';
	if (me == 'A')
		notme = 'B';
	Position myPos = find_position(map, me);
	Position foePos = find_position(map, notme);
	Position newMe, newFoe;
	int result[16] = {0};
	for (int i=0; i<16; ++i)
		result[i] = int_min;
	const Position MushroomsAndStars = count_mushrooms_and_stars(map);

	const int newMeXdir[16] = {1, 1, -1, -1, 0, 0, 0, 0, 1, 1, -1, -1, 0, 0, 0, 0};
	const int newMeYdir[16] = {0, 0, 0, 0, 1, 1, -1, -1, 0, 0, 0, 0, 1, 1, -1, -1};
	const int newFoeXdir[16] = {1, 0, 1, 0, 1, 0, 1, 0, -1, 0, -1, 0, -1, 0, -1, 0};
	const int newFoeYdir[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, -1, 0, -1, 0, -1, 0, -1};
	for (int i = 0; i < 16; ++i) {
		if (valid_position(map, myPos.x + newMeXdir[i], myPos.y + newMeYdir[i]) && valid_position(map, foePos.x + newFoeXdir[i], foePos.y + newFoeYdir[i])) {
			/* printf("Valid Position Condition, myScore = %d, foeScore = %d\n", myScore, foeScore); */
			newMe = myPos;
			newFoe = foePos;
			newMe.x += newMeXdir[i];
			newMe.y += newMeYdir[i];
			newFoe.x += newFoeXdir[i];
			newFoe.y += newFoeYdir[i];
			result[i] = alpha_beta(myScore, foeScore, _round + 1, map, MushroomsAndStars, myPos, foePos, newMe, newFoe);
		}
		cout << "DEBUG" << endl;
	}

	Position printans;
	printans.x = int_min; // current max win 
	printans.y = -1; // direction
	/* cout << printans.y << endl; */
	for (int i=0; i<16; ++i) {
		if (result[i] > printans.x) {
			printans.x = result[i];
			printans.y = i;
		}
	}

	if (newMeXdir[printans.y] == 0) {
		if (newMeYdir[printans.y] == -1)
			cout << "LEFT\n";
		else 
			cout << "RIGHT\n";
	} else {
		if (newMeXdir[printans.y] == 1)
			cout << "DOWN\n";
		else 
			cout << "UP\n";
	}
	if (printans.y == -1) {
		if (valid_position(map, myPos.x - 1, myPos.y)) {
			cout <<"UP\n";
			return;
		}
		if (valid_position(map, myPos.x + 1, myPos.y)) {
			cout <<"DOWN\n";
			return;
		}
		if (valid_position(map, myPos.x, myPos.y + 1)) {
			cout <<"RIGHT\n";
			return;
		}
		if (valid_position(map, myPos.x, myPos.y - 1)) {
			cout <<"LEFT\n";
			return;
		}

	}
	return;
}

int newscore_alphabeta(const char &newPlaceChar, const int &curScore) {
	if (newPlaceChar == '.')
		return curScore;
	if (newPlaceChar == 'b')
		return curScore - 10;
	if (newPlaceChar == 'm')
		return curScore + 1;
	if (newPlaceChar == 'n')
		return curScore - 1;
	if (newPlaceChar == 's')
		return curScore << 1;
	if (newPlaceChar == 't')
		return curScore >> 1;
	return curScore;
}
int alpha_beta(const int _myScore, const int _foeScore, const int &_round, vector<vector<char> > _map, Position _countMashroomsAndStar, const Position myPos, const Position foePos, const Position newMe, const Position newFoe) {
	/* cout << _round << endl; */
	/* if (_round > round + 6) { */
	if (_round > curround) {
		/* printf("Current round = %d, RUN OUT\n", _round); */
		/* cout << "Run Out" << endl; */
		if (_myScore == _foeScore)
			return 0;
		return _myScore > _foeScore ? 1 : -1;
	}
	// change old map with new Pos
	const int myNewScore = newscore_alphabeta(_map[newMe.x][newMe.y], _myScore);
	const int foeNewScore = newscore_alphabeta(_map[newFoe.x][newFoe.y], _foeScore);
	if (_map[newMe.x][newMe.y] == 'm')
		--_countMashroomsAndStar.x;
	if (_map[newFoe.x][newFoe.y] == 'm')
		--_countMashroomsAndStar.x;
	if (_map[newMe.x][newMe.y] == 's')
		--_countMashroomsAndStar.y;
	if (_map[newFoe.x][newFoe.y] == 's')
		--_countMashroomsAndStar.y;
	_map[newMe.x][newMe.y] = character;
	_map[newFoe.x][newFoe.y] = foe_character;
	_map[myPos.x][myPos.y] = '.';
	_map[foePos.x][foePos.y] = '.';
	if (_countMashroomsAndStar.y == 0) {
		if (myNewScore > _countMashroomsAndStar.x + foeNewScore) {
			/* cout << "I win\n"; */
			return 1;
		}
		if (myNewScore + _countMashroomsAndStar.x < foeNewScore) {
			/* cout << "Foe win\n"; */
			return -1;
		}
	}
	if (myNewScore > (_countMashroomsAndStar.x + foeNewScore) * 2 * _countMashroomsAndStar.y) {
		/* cout << "I win\n"; */
		return 1;
	}
	if ((_countMashroomsAndStar.x + myNewScore) * 2 * _countMashroomsAndStar.y < foeNewScore) {
		/* cout << "Foe win\n"; */
		return -1;
	}
	if (_countMashroomsAndStar.x == 0 && _countMashroomsAndStar.y == 0) {
		/* cout << "All done\n"; */
		if (myNewScore == foeNewScore)
			return 0;
		return myNewScore > foeNewScore ? 1 : -1;
	}
	// Pruning Start
	// 16 if condition here
	int result = 0;
	Position nextNewMe, nextNewFoe;
	const int newMeXdir[16] = {1, 1, -1, -1, 0, 0, 0, 0, 1, 1, -1, -1, 0, 0, 0, 0};
	const int newMeYdir[16] = {0, 0, 0, 0, 1, 1, -1, -1, 0, 0, 0, 0, 1, 1, -1, -1};
	const int newFoeXdir[16] = {1, 0, 1, 0, 1, 0, 1, 0, -1, 0, -1, 0, -1, 0, -1, 0};
	const int newFoeYdir[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, -1, 0, -1, 0, -1, 0, -1};
	for (int i = 0; i < 16; ++i) {
		if (valid_position(_map, newMe.x + newMeXdir[i], newMe.y + newMeYdir[i]) && valid_position(_map, newFoe.x + newFoeXdir[i], newFoe.y + newFoeYdir[i])) {
			nextNewMe = newMe;
			nextNewFoe = newFoe;
			nextNewMe.x += newMeXdir[i];
			nextNewMe.y += newMeYdir[i];
			nextNewFoe.x += newFoeXdir[i];
			nextNewFoe.y += newFoeYdir[i];
			result += alpha_beta(myNewScore, foeNewScore, _round + 1, _map, _countMashroomsAndStar, newMe, newFoe, nextNewMe, nextNewFoe);
			/* cout << "result = " << result << endl; */
		}
	}
	// if condition stop
	/* cout << "Result = " << result << endl; */
	return result;
}
int main(void) {

	/* int round, M, N; */
	int M, N;
	vector<vector<char> > map;
	int P, Q;
	char me;

	cin >> curround >> M >> N;
	map.resize(M);
	for (int i = 0; i < M; ++i) {
		map[i].resize(N);
		for (int j = 0; j < N; ++j)
			cin >> map[i][j];
	}
	cin >> P >> Q >> me;

	// check person
	character = me;
	foe_character = 'A';
	if (character == 'A')
		foe_character = 'B';
	// check point
	int myScore, foeScore;
	if (character == 'A') {
		myScore = P;
		foeScore = Q;
	} else {
		myScore = Q;
		foeScore = P;
	}
	decide(map, P, Q, me, curround);
	/* Bot bot(map, P, Q, me); */
	/* bot.decide(round); */

	return 0;
}
