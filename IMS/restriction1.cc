#include "simlib.h"
#include "hotel.h"

restriction applyRestrictions(restriction currentRestriction, int newInfectedCount, int currentInfectedCount, int vaccinatedCount) {
	switch (currentRestriction) {
		case NO_RESTRICTIONS:
			if (currentInfectedCount > 1000) return HALF_CAPACITY;
			if ((float)vaccinatedCount / POPULATION > 0.3) return IMMUNIZATION_REQUIRED;
			break;
		case HALF_CAPACITY:
			if (currentInfectedCount < 100) return NO_RESTRICTIONS;
			if (currentInfectedCount > 10000) return CLOSED;
			if ((float)vaccinatedCount / POPULATION > 0.3) return IMMUNIZATION_REQUIRED;
			break;
		case CLOSED:
			if (currentInfectedCount < 5000) return HALF_CAPACITY;
			if ((float)vaccinatedCount / POPULATION > 0.3) return IMMUNIZATION_REQUIRED;
			break;
	}

	return currentRestriction;
}
