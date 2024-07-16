// IMS projekt 2021/22
// Simulovanie vplyvu rôznych COVID-19 opatrení na hotelové zariadenia
// Autori: Emma Krompaščíková (xkromp00), Samuel Olekšáḱ (xoleks00)

#ifndef HOTEL_H
#define HOTEL_H

enum restriction {
	NO_RESTRICTIONS,
	HALF_CAPACITY,
	IMMUNIZATION_REQUIRED,
	HALF_CAPACITY_IMMUNIZATION_REQUIRED,
	CLOSED
};

char *getRestriction[] = {(char*)"no restrictions", (char*)"half capacity", (char*)"vaccination required", (char*)"half capacity and vaccination required",  (char*)"closed"};

const int INFECTED_DAYS = 14;
const int MAX_STAY_DAYS = 14;
const int POPULATION = 10.7e6; // Czech republic population

#endif
