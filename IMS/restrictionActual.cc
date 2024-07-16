#include "simlib.h"
#include "hotel.h"

restriction applyRestrictions(restriction currentRestriction, int newInfectedCount, int currentInfectedCount, int vaccinatedCount) {
	if (Time < 11) {            // 2020-03-01 - 2020-03-11
        return NO_RESTRICTIONS;
    }
    else if (Time < 78) {       // 2020-03-12 - 2020-05-17
        return CLOSED;
    }
    else if (Time < 85) {       // 2020-05-18 - 2020-05-24
        return CLOSED;
    }
    else if (Time < 218) {      // 2020-05-25 - 2020-10-04
        return NO_RESTRICTIONS;
    }
    else if (Time < 406) {      // 2020-10-05 - 2021-04-10
        return CLOSED;
    }
    else if (Time < 631) {      // 2021-04-11 - 2021-11-21
        return NO_RESTRICTIONS;
    }
    else {                      // 2021-11-22 - 2021-12-09
        return IMMUNIZATION_REQUIRED;
    }
}
