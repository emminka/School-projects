// IMS projekt 2021/22
// Simulovanie vplyvu rôznych COVID-19 opatrení na hotelové zariadenia
// Autori: Emma Krompaščíková (xkromp00), Samuel Olekšáḱ (xoleks00)

#include "simlib.h"

#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <unistd.h>

#include "hotel.h"
#include "restriction3.cc"

char *OUTPUT_FILE = (char*)"hotel.dat";

int DEFAULT_CAPACITY = 100;
int UNIT_PRICE = 1000;
int DAILY_EXPENSE = 5000;
int DAILY_EXPENSE_PER_GUEST = 200;
int BANKRUPCY_AMOUNT = -1000000;
int DAYS_TO_RUN = 650;
int APPLY_RESTRICTIONS = true;

int ballance = 0;

restriction currentRestriction = NO_RESTRICTIONS;

std::string infectedCount;
std::string vaccinatedCount;
std::string occupants2019;

std::fstream fileInfected;
std::fstream fileVaccinated;
std::fstream occupancy2019;

int infectedArray[INFECTED_DAYS] = {0};
int stayingGuests[MAX_STAY_DAYS] = {0};

int recovered = 0;
float incomingGuestsMultiplier = 1.0;
float immunizationMultiplier = 1.0;

TStat DailyRejectedGuests("Daily rejected guests", 0);
TStat DailyMoney("Daily sales", 0);
TStat Occupancy("Occupancy", 0);

void printHelp() {
	printf("Hotel with COVID restrictions simulator\n");
	printf("Usage ./hotel [-c <capacity>] [-p <price>] [-b <amount>] [-e <amount>] [-d <days>] [-o <file>] [-f]\n");
	printf("Options:\n");
	printf("  -c <capacity>    max hotel capacity\n");
	printf("  -p <price>       average price per guest per night\n");
	printf("  -b <amount>      bankrupcy amount, below how much money is the hotel bankrupt\n");
	printf("  -e <amount>      daily expenses of a hotel\n");
	printf("  -d <days>        how many days since 2020-03-01 should the simulation run\n");
	printf("  -g <amount>      daily expenses per guest\n");
	printf("  -o <file>        file for simulation output (existing file will be overwritten)\n");
	printf("  -f               run without any restrictions (simulate times before COVID-19)\n");
}

void HotelDay() {
	// check for bankrupcy
	if (ballance < BANKRUPCY_AMOUNT) {
		Print("Hotel bankrupted\n");
		Stop();
		return;
	}

	// load stats from files
	std::getline(fileInfected, infectedCount);
	std::getline(fileVaccinated, vaccinatedCount);
	std::getline(occupancy2019, occupants2019);

	if (!APPLY_RESTRICTIONS) {
		incomingGuestsMultiplier = atof(occupants2019.c_str());
	}

	recovered += infectedArray[(int)Time % INFECTED_DAYS];
	infectedArray[(int)Time % INFECTED_DAYS] = atoi(infectedCount.c_str());

	int infectedCurrently = 0;

	for (int i = 0; i < INFECTED_DAYS; i++) {
		infectedCurrently += infectedArray[i];
	}

	// update restrictions if not disabled by -f flag
	int currentCapacity = DEFAULT_CAPACITY;

	if (APPLY_RESTRICTIONS) {
		currentRestriction = applyRestrictions(currentRestriction, atoi(infectedCount.c_str()), infectedCurrently, atoi(vaccinatedCount.c_str()));

		if (currentRestriction == HALF_CAPACITY || currentRestriction == HALF_CAPACITY_IMMUNIZATION_REQUIRED) {
			currentCapacity = DEFAULT_CAPACITY * 0.5;
		}
		else if (currentRestriction == CLOSED) {
			currentCapacity = 0;
		}

		if (currentRestriction == IMMUNIZATION_REQUIRED || currentRestriction == HALF_CAPACITY_IMMUNIZATION_REQUIRED) {
			immunizationMultiplier = (recovered + atof(vaccinatedCount.c_str())) / POPULATION;
		}
		else {
			immunizationMultiplier = 1.0;
		}
	}

	// remove leaving guests
	for (int i = 0; i < MAX_STAY_DAYS - 1; i++) {
		stayingGuests[i] = stayingGuests[i + 1];
	}

	stayingGuests[MAX_STAY_DAYS - 1] = 0;

	// add new incoming guests
	int incomingGroups = (int)Lim((int)Normal(4 * incomingGuestsMultiplier, 0.5), 0, 100).Value();

	int todayRejectedGuestCount = 0;
	int todayMoney = 0;

	// check if group is not over capacity
	int currentOccupancy = 0;
	for (int j = 0; j < MAX_STAY_DAYS; j++) {
		currentOccupancy += stayingGuests[j];
	}

	for (int i = 0; i < incomingGroups; i++) {
		int groupSize = (int)Exponential(3);
		int groupStay = (int)Lim(Exponential(5), 1, 14).Value();

		if (currentOccupancy + groupSize > currentCapacity) {
			// Print("Rejected a group of %d persons\n", groupSize);
			todayRejectedGuestCount += groupSize;
		}
		else {
			//Print("Accepted a group of %d persons for %d days\n", groupSize, groupStay);
			stayingGuests[groupStay - 1] += groupSize;
			currentOccupancy += groupSize;
			todayMoney = groupSize * groupStay * UNIT_PRICE;
		}
	}

	ballance += todayMoney;
	ballance -= DAILY_EXPENSE;
	ballance -= DAILY_EXPENSE_PER_GUEST * currentOccupancy;

	float occupancyPercentage = currentCapacity == 0 ? 0 : (float)currentOccupancy / currentCapacity;

	DailyRejectedGuests(todayRejectedGuestCount);
	DailyMoney(todayMoney);
	Occupancy(occupancyPercentage);

	if (APPLY_RESTRICTIONS) {
		Print("Day %3d - %20s: current guests: %5d/%5d, occupancy: %f, newly infected: %7s, currently infected: %7d, total fully vaccined: %7d, recovered: %7d, ballance: %8d\n", (int)Time, getRestriction[currentRestriction], currentOccupancy, currentCapacity, occupancyPercentage, infectedCount.c_str(), infectedCurrently, atoi(vaccinatedCount.c_str()), recovered, ballance);
	}
	else {
		Print("Day %3d: current guests: %5d/%5d, occupancy: %1.2f, ballance: %8d\n", (int)Time, currentOccupancy, currentCapacity, occupancyPercentage, ballance);
	}
}
Sampler S(HotelDay, 1); // one sample per day

int main(int argc, char *argv[]) {
  	int opt;

    while((opt = getopt(argc, argv, ":hc:p:b:e:d:g:o:f")) != -1) { 
        switch(opt) { 
            case 'h': 
                printHelp();
                return 0;
            case 'c': 
				DEFAULT_CAPACITY = atoi(optarg);
              	break;
            case 'p': 
                UNIT_PRICE = atoi(optarg);
              	break;
            case 'b': 
                BANKRUPCY_AMOUNT = atoi(optarg);
              	break;
            case 'e': 
                DAILY_EXPENSE = atoi(optarg);
              	break;
			case 'd': 
				DAYS_TO_RUN = atoi(optarg);
				break;
			case 'g': 
				DAILY_EXPENSE_PER_GUEST = atoi(optarg);
				break;
			case 'o': 
				OUTPUT_FILE = optarg;
				break;
			case 'f':
				APPLY_RESTRICTIONS = false;
				break;
            case ':': 
                fprintf(stderr, "Option needs a value\n"); 
                break; 
            case '?': 
                fprintf(stderr, "Unknown option: %c\n", optopt);
                break; 
        } 
    }
      
    for(; optind < argc; optind++){     
        fprintf(stderr, "Extra arguments: %s\n", argv[optind]); 
    }

	if (DEFAULT_CAPACITY <= 0) {
		fprintf(stderr, "Invalid hotel capacity\n");
		return -1;
	}

	if (UNIT_PRICE <= 0) {
		fprintf(stderr, "Invalid price per guest per night\n");
		return -1;
	}

	#ifdef ahoj
	printf("ahoj");
	#endif

	fileInfected.open("infected.csv");
	fileVaccinated.open("vaccinated.csv");
	occupancy2019.open("occupancy2019.csv");

	RandomSeed(time(NULL));

	SetOutput(OUTPUT_FILE);
	Init(0, DAYS_TO_RUN - 1);
	Run();
	Print("Výnos: %d\n", ballance);

	DailyRejectedGuests.Output();
	DailyMoney.Output();
	Occupancy.Output();

	SIMLIB_statistics.Output();
}
