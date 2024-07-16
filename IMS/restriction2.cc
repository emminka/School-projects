#include "simlib.h"
#include "hotel.h"

restriction applyRestrictions(restriction currentRestriction, int newInfectedCount, int currentInfectedCount, int vaccinatedCount) {
	switch (currentRestriction) {
		case NO_RESTRICTIONS:
			if (currentInfectedCount > 10000) return CLOSED;
			if ((float)vaccinatedCount / POPULATION > 0.1) return IMMUNIZATION_REQUIRED;
			break;
		case CLOSED:
			if (currentInfectedCount < 5000) return NO_RESTRICTIONS;
			if ((float)vaccinatedCount / POPULATION > 0.1) return IMMUNIZATION_REQUIRED;
			break;
	}

	return currentRestriction;
}
