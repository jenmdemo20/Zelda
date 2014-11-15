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

#include "UserMessageCreator.hpp"
#include "UserMessageTypes.hpp"
#include "MessageShowText.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using namespace XMLHelper;

CMessage *CUserMessageCreator::createMessage(const tinyxml2::XMLElement *pElem, const Ogre::Any &any) {
  unsigned int type(USER_MESSAGE_TYPES_MAP.parseString(Attribute(pElem, m_sMessageTypeAttributeName.c_str())));
  switch (type) {
  case MSG_SHOW_TEXT:
    return new CMessageShowText(pElem, any.get<std::shared_ptr<CGUITextBox::SResult> >());
    break;
  }

  return CMessageCreator::createMessage(pElem, any);
}
