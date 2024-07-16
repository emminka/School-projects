#include "simlib.h"
#include "hotel.h"

restriction applyRestrictions(restriction currentRestriction, int newInfectedCount, int currentInfectedCount, int vaccinatedCount) {
	switch (currentRestriction) {
		case NO_RESTRICTIONS:
			if (currentInfectedCount > 20000) return HALF_CAPACITY;
			break;
		case HALF_CAPACITY:
			if (currentInfectedCount < 10000) return NO_RESTRICTIONS;
			if (currentInfectedCount > 50000) return CLOSED;
			break;
		case CLOSED:
			if (currentInfectedCount < 20000) return HALF_CAPACITY;
			break;
	}

	return currentRestriction;
}
