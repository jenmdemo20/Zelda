#ifndef _HUD_HPP_
#define _HUD_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"

class CGUIHeartsDisplay;
class CGUICounter;

class CHUD : public CGUIOverlay {
private:
  CEGUI::Window *m_pRoot;
  CGUIHeartsDisplay *m_pHeartsDisplay;
  CGUICounter *m_pRupeeCounter;
public:
  CHUD(CEntity *pParentEntity, CEGUI::Window *pParentWindow);
};
#endif // _HUD_HPP_
