#include "GeoService.h"
#include "Creature.h"
#include "HuntingZone.h"
#include "WorldPosition.h"
#include "Area.h"

#include <stdlib.h>

WorldPosition * GeoService::GetRandom(HuntingZone * zone)
{
	WorldPosition * out = new WorldPosition();

	out->_X = zone->_left + rand() %  zone->_right;
	out->_Y = zone->_top + rand() % zone->_bottom;
	out->_Y = 0;

	out->_heading = 0;
	out->_areaId = zone->_area->_startPosition->_areaId;
	

	return out;
}
