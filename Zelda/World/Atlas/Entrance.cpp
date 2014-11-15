/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "Entrance.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include <OgreStringConverter.h>

using namespace XMLHelper;

CEntrance::CEntrance(CWorldEntity *pParent, const tinyxml2::XMLElement *pElem)
  : CRegion(pParent, pElem),
    mPlayerDirection(Ogre::StringConverter::parseVector3(Attribute(pElem, "player_direction", "0 0 0"))),
    mPlayerRelativePosition(Ogre::StringConverter::parseVector3(Attribute(pElem, "player_relative_position")))
    {
}
