//This is a new program designed to take in a user's stats for their birding.
//It takes in their location, the duration of their trip, the distance they traveled, how they birded, how many birds they saw, and the hourly average.

// ********** DOCTEST STATEMENTS **********
#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

// ********** INCLUDE STATEMENTS **********
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <windows.h>

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//********** Enum To Determine What Style of Birding **********
enum _Type {
	TRAIL,
	CAR,
	PARK
};

//********** PRINT TABLE FUNCTION (NOT USED CURRENTLY) **********
//void printTable(const string& location, int time, double distance, _Type type, double birdsObserved, double hourlyAverage);

//********** Base Class To Determine Time, Distance, Location, and Type of Birding **********
class Distancetime {
private:
	_Type type;
	int _time;
	double _distance;
protected:
	string _location;
public:
	Distancetime() {
		_time = 0;
		_distance = 0.0;
		type = TRAIL;
	}

	virtual void Abstractfunction() = 0;

	void setLocation(const string& location) {
		_location = location;
	}
	void setTime(int time) {
		_time = time;
	}
	void setDistance(double distance) {
		_distance = distance;
	}
	void setType(_Type birdingtype) {
		type = birdingtype;
	}

	string getLocation() const {
		return _location;
	}
	int getTime() const {
		return _time;
	}
	double getDistance() const {
		return _distance;
	}
	_Type getType() const {
		return type;
	}

	virtual void Print() {
		cout << "Type of Birding: ";
		switch (type) {
		case _Type::TRAIL: cout << "Trail Birding\n"; break;
		case _Type::CAR: cout << "Car Birding\n"; break;
		case _Type::PARK: cout << "Park Birding\n"; break;
		}
		cout << "Location Birding: " << _location << endl;
		cout << "Minutes Spent Birding: " << _time << endl;
		cout << "Miles Traveled: " << _distance << endl;
	}

	virtual ~Distancetime() {

	};
};

//********** Composition Class 1 To Determine Bird Average **********
class Hourlybirds {
private:
	double _hourlybirds;
protected:

public:
	void Division(double birdsseen, int time) {
		_hourlybirds = birdsseen / (time / 60.0);
	}

	double getDivision() const {
		return _hourlybirds;
	}

	void printDivision() const {
		cout << "Birds Seen per Hour: " << _hourlybirds << endl;
	}
};

//********** Derived Class 1 To Determine How Many Birds Seen **********
class Birdsseen : public Distancetime {
private:
	double _birdsseen;
protected:

public:
	Hourlybirds hourlybirds;

	void Abstractfunction() override {
		cout << "Birdsseen overrides this function!" << endl;
	}

	void setBirdsseen(double birdsseen) {
		_birdsseen = birdsseen;
		hourlybirds.Division(_birdsseen, getTime());
	}
	void setTime(int time) {
		Distancetime::setTime(time);
		hourlybirds.Division(_birdsseen, getTime());
	}
	double getBirdsseen() const {
		return _birdsseen;
	}

	virtual void Print() {
		Distancetime::Print();
		cout << "Birds Observed: " << _birdsseen << endl;
		hourlybirds.printDivision();
	}
};


//********** Derived Class 2 To Determine if Birding Was Fun Despite No Birds**********
class Nobirds : public Distancetime {
private:
	string _fun;
protected:

public:

	void Abstractfunction() override {
		cout << "Nobirds overrides this function!" << endl;
	}

	void setFun(const string& fun) {
		_fun = fun;
	}

	string getFun() const {
		return _fun;
	}

	virtual void Print() {
		Distancetime::Print();
		cout << "Birds Observed: 0" << endl;
		cout << "Did You Have Fun Regardless: " << _fun << endl;
	}
};

class Manager {
private:
	Distancetime** items;
	int size;
	int capacity;
protected:

public:
	Manager(int initialCapacity = 10) {
		size = 0;
		capacity = initialCapacity;
		items = new Distancetime * [capacity];
	}

	~Manager() {
		for (int i = 0; i < size; i++) {
			delete items[i];
		}
		delete[] items;
	}

	void addTrip(Distancetime* trip) {
		if (size >= capacity) {
			capacity *= 2;
			Distancetime** newItems = new Distancetime * [capacity];
			for (int i = 0; i < size; i++)
				newItems[i] = items[i];
			delete[] items;
			items = newItems;
		}
		items[size++] = trip;
	}

	void removeTrip(int index) {
		if (index < 0 || index >= size) {
			cout << "Removed Trip" << endl;
			return;
		}
		delete items[index];

		for (int i = index; i < size - 1; i++) {
			items[i] = items[i + 1];
		}
		size--;
	}

	void printTrips() const {
		for (int i = 0; i < size; i++) {
			SetConsoleTextAttribute(hConsole, 13);
			cout << "Birding Trip #" << (i + 1) << endl;
			SetConsoleTextAttribute(hConsole, 7);
			items[i]->Print();
		}
	}

	int getSize() const {
		return size;
	}
};

//********** DOCTEST UNIT TESTS **********
#ifdef _DEBUG

TEST_CASE("Division Tests") {
	Hourlybirds hourlybirds;
	hourlybirds.Division(15, 60);
	CHECK(hourlybirds.getDivision() == doctest::Approx(15.0));

	hourlybirds.Division(5, 120);
	CHECK(hourlybirds.getDivision() == doctest::Approx(2.5));
}

TEST_CASE("Birdsseen Test Using Hourlybirds") {
	Birdsseen birdsseen;
	birdsseen.setLocation("Lake St. Clair");
	birdsseen.setTime(120);
	birdsseen.setBirdsseen(20);

	CHECK(birdsseen.getLocation() == "Lake St. Clair");
	CHECK(birdsseen.getTime() == 120);
	CHECK(birdsseen.getBirdsseen() == doctest::Approx(20.0));
	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(10.0));
}

TEST_CASE("Birdsseen/Hourlybirds Test When Time Changes") {
	Birdsseen birdsseen;
	birdsseen.setBirdsseen(12);
	birdsseen.setTime(60);

	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(12.0));

	birdsseen.setTime(120);

	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(6.0));
}

TEST_CASE("Nobirds Tests") {
	Nobirds nobirds;
	nobirds.setLocation("Belle Isle");
	nobirds.setTime(45);
	nobirds.setDistance(1.2);
	nobirds.setFun("Yes");

	CHECK(nobirds.getLocation() == "Belle Isle");
	CHECK(nobirds.getTime() == 45);
	CHECK(nobirds.getFun() == "Yes");
}

TEST_CASE("Print Tests") {
	Birdsseen birdsseen;
	birdsseen.setLocation("Gallup Park");
	birdsseen.setTime(30);
	birdsseen.setDistance(0.5);
	birdsseen.setType(PARK);
	birdsseen.setBirdsseen(5);

	CHECK(birdsseen.getLocation() == "Gallup Park");
	CHECK(birdsseen.getTime() == 30);
	CHECK(birdsseen.getDistance() == doctest::Approx(0.5));
	CHECK(birdsseen.getType() == PARK);
	CHECK(birdsseen.getBirdsseen() == doctest::Approx(5));
	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(10.));
}

TEST_CASE("Addition/Deletion Tests") {
	Manager manager(2);

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Lake A"); b1->setTime(60); b1->setBirdsseen(5);
	manager.addTrip(b1);

	Nobirds* n1 = new Nobirds();
	n1->setLocation("Park B"); n1->setTime(30); n1->setFun("Yes");
	manager.addTrip(n1);

	CHECK(manager.getSize() == 2);

	// Add another to force resize
	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake C"); b2->setTime(45); b2->setBirdsseen(3);
	manager.addTrip(b2);

	CHECK(manager.getSize() == 3);

	// Remove middle trip
	manager.removeTrip(1);
	CHECK(manager.getSize() == 2);
}

TEST_CASE("Virtual Function Test") {
	Distancetime* ptr1 = new Birdsseen();
	Distancetime* ptr2 = new Nobirds();

	ptr1->Abstractfunction();
	ptr2->Abstractfunction();

	ptr1->Print();
	ptr2->Print();

	delete ptr1;
	delete ptr2;
}

#else

//********** MAIN FUNCTION **********

int main() {
	string location;
	int time;
	double distance;
	int typechoice;
	double birdsobserved;
	string fun;
	char moreTrips = 'y';
	char removeTrips = 'y';
	int wantedTrip;

	Manager manager;

	SetConsoleTextAttribute(hConsole, 13);
	cout << "Welcome to the Bird Stat Tracker" << endl;
	SetConsoleTextAttribute(hConsole, 7);

	do {
		cout << "Where did you go birding?" << endl;
		getline(cin, location);

		cout << "How many minutes did you go birding?" << endl;
		cin >> time;

		do {
			if (time <= 0) {
				cout << "Please input a number larger than 0:" << endl;
				cin >> time;
			}
		} while (time <= 0);

		cout << "How many miles did you travel while birding?" << endl;
		cin >> distance;

		do {
			if (distance <= 0) {
				cout << "Please input a number larger than 0:" << endl;
				cin >> distance;
			}
		} while (distance <= 0);

		cout << "What type of birding did you do? (0 = Trail, 1 = Car, 2 = Park): " << endl;
		cin >> typechoice;

		cout << "How many birds did you see?" << endl;
		cin >> birdsobserved;
		cin.ignore();

		_Type birdingtype;
		switch (typechoice) {
		case 0: birdingtype = TRAIL; break;
		case 1: birdingtype = CAR; break;
		case 2: birdingtype = PARK; break;
		default: birdingtype = TRAIL; break;
		}

		if (birdsobserved > 0) {
			Birdsseen* trip = new Birdsseen();
			trip->setLocation(location);
			trip->setTime(time);
			trip->setDistance(distance);
			trip->setType(birdingtype);
			trip->setBirdsseen(birdsobserved);

			manager.addTrip(trip);
		}
		else {
			cout << "Did you still have fun and enjoy the nature? (Yes/No): " << endl;
			getline(cin, fun);

			Nobirds* trip = new Nobirds();
			trip->setLocation(location);
			trip->setTime(time);
			trip->setDistance(distance);
			trip->setType(birdingtype);
			trip->setFun(fun);

			manager.addTrip(trip);
		}

		cout << "Would you like to add another birding trip? (Y/N)" << endl;
		cin >> moreTrips;
		cin.ignore();

	} while (moreTrips == 'y' || moreTrips == 'Y');

	cout << "\nAll Birding Trips:" << endl;
	manager.printTrips();

	do {
		cout << "Would you like to remove a birding trip? (Y/N)" << endl;
		cin >> removeTrips;
		cin.ignore();

		if (removeTrips == 'y' || removeTrips == 'Y') {
			cout << "Which trip would you like to remove?" << endl;
			cin >> wantedTrip;
			cin.ignore();

			manager.removeTrip(wantedTrip - 1);
		}

	} while (removeTrips == 'y' || removeTrips == 'Y');

	cout << "\nUpdated Birding Trips:" << endl;
	manager.printTrips();

	return 0;
}
#endif
/*
void printTable(const string& location, int time, double distance, _Type type, double birdsObserved, double hourlyAverage) {
	cout << fixed << setprecision(2);

	SetConsoleTextAttribute(hConsole, 13);
	cout << setw(32) << setfill('*') << "" << setfill(' ') << endl;
	SetConsoleTextAttribute(hConsole, 7);

	cout << "   Location: " << setw(16) << location << endl;
	cout << "   Minutes Birding: " << setw(9) << time << endl;
	cout << "   Miles Traveled: " << setw(10) << distance << endl;
	cout << "   Birds Seen: " << setw(14) << birdsObserved << endl;
	cout << "   Average Birds: " << setw(11) << hourlyAverage << endl;

	SetConsoleTextAttribute(hConsole, 13);
	cout << setw(32) << setfill('*') << "" << endl;
	SetConsoleTextAttribute(hConsole, 7);

	ofstream outFile("report.txt");
	outFile << setw(30) << setfill('*') << "" << setfill(' ') << endl;
	outFile << "   Location: " << location << endl;
	outFile << "   Minutes Birding: " << time << endl;
	outFile << "   Miles Traveled: " << distance << endl;
	outFile << "   Birds Seen: " << birdsObserved << endl;
	outFile << "   Avg Birds/Hour: " << hourlyAverage << endl;
	outFile << setw(30) << setfill('*') << "" << endl;
	outFile.close();
}
*/