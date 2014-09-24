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

#ifndef _MESSAGE_SHOW_TEXT_HPP_
#define _MESSAGE_SHOW_TEXT_HPP_

#include "../../Common/Message/Message.hpp"
#include "../../Common/Util/EnumIdMap.hpp"


class CMessageShowText: public CMessage {
public:
  enum EStatus {
    REQUEST,    //!< when a text box shall be shown
    FINISHED,   //!< when a text box has closed
    FINISHED_RESULT_REPEAT, //!< when a text box has closed with a question result: repeat
    FINISHED_RESULT_CONTINUE, //!< when a text box has closed with a question result: continue
  };
  class CStatusIDMap : public CEnumIdMap<EStatus> {
  public:
    CStatusIDMap();
  };
  const static CStatusIDMap STATUS_ID_MAP;
protected:
  const std::string m_sLanguageString;
  const EStatus mStatus;
public:
  CMessageShowText(const std::string &sLanguageString, EStatus status = REQUEST);
  CMessageShowText(const tinyxml2::XMLElement *pElem);

  const std::string &getLanguageString() const {return m_sLanguageString;}
};

#endif // _MESSAGE_SHOW_TEXT_HPP_
