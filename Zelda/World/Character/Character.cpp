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

#include "Character.hpp"
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <string>
#include "CharacterConstructionInfo.hpp"
#include "CharacterController.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Physics/BtOgreExtras.hpp"
#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreSkeletonInstance.h>
#include "../Items/CharacterItem.hpp"
#include "../../Common/Util/Assert.hpp"

// map will be set on enter map, in construction, m_pMap is nullptr
CCharacter::CCharacter(CAbstractWorldEntity *pParent,
                       const CCharacterConstructionInfo &info)
    : CWorldEntity(pParent, info),
      mAttitude(info.getAttitude()),
  mAnimationDataList(info.getAnimationDataList()),
  m_pCharacterController(nullptr),
  m_uiAnimationCount(info.getAnimationDataList().size()),
    m_fTimer(0),
    m_fAnimSpeed(1) {
  m_Anims.resize(m_uiAnimationCount);
  m_uiAnimID = m_uiAnimationCount;

  constructor_impl();
}

CCharacter::CCharacter(const std::string &sID,
                       CAbstractWorldEntity *pParent,
                       const CCharacterConstructionInfo &info)
    : CWorldEntity(sID, pParent, info),
      mAttitude(info.getAttitude()),
  mAnimationDataList(info.getAnimationDataList()),
  m_pCharacterController(nullptr),
  m_uiAnimationCount(info.getAnimationDataList().size()),
    m_fTimer(0),
    m_fAnimSpeed(1) {
  m_Anims.resize(m_uiAnimationCount);
  m_uiAnimID = m_uiAnimationCount;

  constructor_impl();
}

void CCharacter::constructor_impl() {
  switch (getAttitude()) {
  case ATTITUDE_ENEMY:
    mCollisionMask = MASK_PLAYER_N_COLLIDES_WITH;
    mCollisionGroup = COL_CHARACTER_N;
    break;
  case ATTITUDE_FRIENDLY:
    mCollisionMask = MASK_PLAYER_P_COLLIDES_WITH;
    mCollisionGroup = COL_CHARACTER_P;
    break;
  default:
    throw Ogre::Exception(0, "Unknown collision mask", __FUNCTION__);
    break;
  }

  mCCPhysics = NULL;
  m_bMoving = false;
  m_fYaw = 0;

  mAnimationProperty.resize(m_uiAnimationCount);
  for (uint8_t i = 0; i < m_uiAnimationCount; ++i) {
    mAnimationProperty[i].mAnimationData = &mAnimationDataList[i];
  }
}

CCharacter::~CCharacter() {
}

void CCharacter::setupAnimations() {
  ASSERT(m_pBodyEntity);
  // this is very important due to the nature of the exported animations
  m_pBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
 
  preSetupAnimations();
  for (SAnimationProperty &animProp : mAnimationProperty) {
    ASSERT(animProp.mAnimationData);
    Ogre::AnimationState *animState
        = m_pBodyEntity->getAnimationState(animProp.mAnimationData->mName);
    animProp.mState = animState;
    animProp.mFadeState = FADE_NONE;
 
    animState->setLoop(animProp.mAnimationData->mLoop);
    animState->setEnabled(false);
    animState->setWeight(0);
 
    m_Anims[animProp.mAnimationData->mId] = animState;
  }
 
  setAnimation(0, true, true);

  postSetupAnimations();
}

void CCharacter::exit() {
  if (m_pCharacterController) {
    delete m_pCharacterController;
    m_pCharacterController = nullptr;
  }
  
  for (int i = 0; i < CIS_COUNT; i++) {
    mCurrentItems[i].reset();
  }
  CWorldEntity::exit();
  destroyPhysics();
}

void CCharacter::enterMap(CAbstractMap *pMap, const Ogre::Vector3 &vInitPosition) {
  // switch map only, if map an scene node are existing
  bool bSwitchMapOnly = m_pMap && m_pSceneNode;

  // destroy old physics
  destroyPhysics();

  // delete tools, will be recreated, but store their current state
  EItemVariantTypes currentItemType[CIS_COUNT];
  std::string weaponBone[CIS_COUNT];
  for (int i = 0; i < CIS_COUNT; i++) {
    currentItemType[i] = ITEM_VARIANT_COUNT;
    if (mCurrentItems[i]) {
      currentItemType[i] = mCurrentItems[i]->getItemVariantType();
      weaponBone[i] = mCurrentItems[i]->getBoneToAttach();
      mCurrentItems[i]->exit();
      mCurrentItems[i].reset();
    }
  }

  m_pMap = pMap;

  if (!bSwitchMapOnly) {
    // use atlas scene node
    initBody(m_pMap->getParent()->getSceneNode());
    setupAnimations();
  }
  createPhysics();
  if (m_pCharacterController) {delete m_pCharacterController;}
  m_pCharacterController = createCharacterController();
  setupInternal();

  if (bSwitchMapOnly) {
    m_pCharacterController->moveToTarget(vInitPosition,
                                         0.02f, Ogre::Degree(5),
                                         false,
                                         5.f);
  } else {
    setPosition(vInitPosition);
    m_pCharacterController->start();
  }

  for (int i = 0; i < CIS_COUNT; i++) {
    if (currentItemType[i] != ITEM_VARIANT_COUNT) {
      changeItem(static_cast<ECharacterItemSlots>(i),
                 weaponBone[i], currentItemType[i]);
    }
  }
}

bool CCharacter::createDamage(const Ogre::Ray &ray, const CDamage &dmg) {
  // try to interact with the world. So detect an object to interact with
  btCollisionWorld::ClosestRayResultCallback rayCallback(
      BtOgre::Convert::toBullet(ray.getOrigin()),
      BtOgre::Convert::toBullet(ray.getPoint(1)));
  if (getAttitude() == ATTITUDE_FRIENDLY) {
    rayCallback.m_collisionFilterGroup = COL_DAMAGE_P;
    rayCallback.m_collisionFilterMask = MASK_DAMAGE_P_COLLIDES_WITH;
  } else {
    rayCallback.m_collisionFilterGroup = COL_DAMAGE_N;
    rayCallback.m_collisionFilterMask = MASK_DAMAGE_N_COLLIDES_WITH;
  }
  m_pMap->getPhysicsManager()->getWorld()->rayTest(
      BtOgre::Convert::toBullet(ray.getOrigin()),
      BtOgre::Convert::toBullet(ray.getPoint(1)), rayCallback);
  if (rayCallback.hasHit()) {
    CAbstractWorldEntity *pWE = CAbstractWorldEntity::getFromUserPointer(
        rayCallback.m_collisionObject);
    if (pWE) {
      EReceiveDamageResult res = attack(dmg, pWE);
      if (res == RDR_BLOCKED) {
      }
      return true;
    }
  }
  return false;
}

void CCharacter::destroy() {
  destroyPhysics();
  // destroySceneNode(m_pSceneNode);
  m_pBodyEntity = NULL;
  if (m_pCharacterController) {
    delete m_pCharacterController;
  }
}

void CCharacter::setPosition(const Ogre::Vector3 &vPos) {
  if (mCCPhysics) {
    mCCPhysics->warp(BtOgre::Convert::toBullet(vPos));
    m_pSceneNode->setPosition(BtOgre::Convert::toOgre(
        m_pCollisionObject->getWorldTransform().getOrigin()));
  } else {
    m_pCharacterController->setPosition(vPos);
  }
}

void CCharacter::setOrientation(const Ogre::Quaternion &vRotation) {
    m_pCharacterController->setOrientation(vRotation);
}

void CCharacter::update(Ogre::Real fTime) {
  CWorldEntity::update(fTime);

  m_pCharacterController->updateCharacter(fTime);
  preUpdateBoundsCallback(fTime);
  preAnimationUpdateCallback(fTime);
  updateAnimations(fTime);

  // update item/weapon
}

void CCharacter::updateAnimations(Ogre::Real fTime) {
  if (m_uiAnimID >= m_uiAnimationCount) {return;}

  m_fTimer += fTime;
  m_fAnimSpeed = 1;

  updateAnimationsCallback(fTime);

  // increment the current animation times
  if (m_uiAnimID != m_Anims.size()) {
    m_Anims[m_uiAnimID]->addTime(fTime * m_fAnimSpeed);
  }

  // apply smooth transitioning between our animations
  fadeAnimations(fTime);

  postUpdateAnimationsCallback(fTime);
}

void CCharacter::updateAnimationsCallback(const Ogre::Real fTime) {
  if (m_uiAnimID == ANIM_SLICE_HORIZONTAL) {
    if (m_fTimer >= m_Anims[ANIM_SLICE_HORIZONTAL]->getLength()) {
      animAttackEnd();
    }
  }
  else if (m_uiAnimID == ANIM_USE_ITEM) {
    if (m_fTimer >= m_Anims[ANIM_USE_ITEM]->getLength()) {
      animUseToolEnd();
    }
  }
}

void CCharacter::setAnimation(unsigned int id, bool reset, bool force) {
  if (force) {
    // no fading
    for (unsigned int i = 0; i < mAnimationProperty.size(); i++) {
      mAnimationProperty[i].mFadeState = FADE_OUT;
      m_Anims[i]->setEnabled(false);
      m_Anims[i]->setWeight(0);
    }

    // only show current
    m_uiAnimID = id;

    if (id != m_uiAnimationCount) {
      // if we have a new animation, enable it and fade it in
      m_Anims[id]->setEnabled(true);
      m_Anims[id]->setWeight(1);
      mAnimationProperty[id].mFadeState = FADE_IN;
      if (reset) m_Anims[id]->setTimePosition(0);
    }
  } else {
    // fading

    // disable all animations
    if (m_uiAnimID < mAnimationProperty.size()) {
      // if we have an old animation, fade it out
      mAnimationProperty[m_uiAnimID].mFadeState = FADE_OUT;
    }

    m_uiAnimID = id;

    if (id != m_uiAnimationCount) {
      // if we have a new animation, enable it and fade it in
      m_Anims[id]->setEnabled(true);
      // m_Anims[id]->setWeight(0);
      mAnimationProperty[m_uiAnimID].mFadeState = FADE_IN;
      if (reset) m_Anims[id]->setTimePosition(0);
    }
  }
}


const Ogre::AnimationState *CCharacter::getAnimation(unsigned int id) const {
  return m_Anims[id];
}

unsigned int CCharacter::getAnimationIdFromString(const std::string &id) const {
  for (auto &anim : mAnimationProperty) {
    if (anim.mAnimationData->mName == id) {
      return anim.mAnimationData->mId;
    }
  }
  
  LOGW("Animation id '%s' is unknown.", id.c_str());
  return 0;
}

void CCharacter::fadeAnimations(const Ogre::Real deltaTime) {
  for (unsigned int i = 0; i < m_uiAnimationCount; i++) {
    if (mAnimationProperty[i].mFadeState == FADE_NONE) {
      continue;
    } else if (mAnimationProperty[i].mFadeState == FADE_IN) {
      // slowly fade this animation in until it has full weight
      Ogre::Real newWeight = m_Anims[i]->getWeight()
          + deltaTime * ANIM_FADE_SPEED;
      m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
      if (newWeight >= 1) mAnimationProperty[i].mFadeState = FADE_NONE;
    } else if (mAnimationProperty[i].mFadeState == FADE_OUT) {
      // slowly fade this animation out until it has no weight,
      // and then disable it
      Ogre::Real newWeight = m_Anims[i]->getWeight()
          - deltaTime * ANIM_FADE_SPEED;
      m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
      if (newWeight <= 0) {
        m_Anims[i]->setEnabled(false);
        mAnimationProperty[i].mFadeState = FADE_NONE;
      }
    }
  }
}

void CCharacter::animRunStart()
{
    /*if (m_uiTopAnimID ==)
	setTopAnimation(ANIM_RUN_TOP);
	setBaseAnimation(ANIM_RUN_BASE);*/
}
void CCharacter::animRunEnd()
{
	/*setTopAnimation(ANIM_IDLE_TOP);
	setBaseAnimation(ANIM_IDLE_BASE, true);*/
}
void CCharacter::animJumpStart()
{
	//setAnimation(ANIM_JUMP_START, true);
	m_fTimer = 0;
}
void CCharacter::animJumpLoop()
{
	//setAnimation(ANIM_JUMP_LOOP, true);
}
void CCharacter::animJumpEnd() {
  //setAnimation(ANIM_IDLE);
  m_fTimer = 0;
}
void CCharacter::animAttack() {
  if (!isReadyForNewAction()) {return;}
  setAnimation(ANIM_SLICE_HORIZONTAL, true);
  mCurrentItems[CIS_WEAPON]->startDamage();
  m_fTimer = 0;
}
void CCharacter::animAttackEnd() {
  setAnimation(ANIM_IDLE);
  m_fTimer = 0;
}

void CCharacter::animUseToolStart() {
  if (!isReadyForNewAction()) {return;}
  setAnimation(ANIM_USE_ITEM, true);
  mCurrentItems[CIS_TOOL]->show();
  mCurrentItems[CIS_TOOL]->startDamage();
  mCurrentItems[CIS_WEAPON]->hide();
  m_fTimer = 0;
}

void CCharacter::animUseToolEnd() {
  setAnimation(ANIM_IDLE);
  mCurrentItems[CIS_TOOL]->hide();
  mCurrentItems[CIS_WEAPON]->show();
  m_fTimer = 0;
}


void CCharacter::setIsMoving(bool isMoving) {
  m_bMoving = isMoving;
}

bool CCharacter::isReadyForNewAction() {
  return m_uiAnimID == ANIM_NONE || m_uiAnimID == ANIM_IDLE || m_uiAnimID == ANIM_WALK;
}

void CCharacter::useCurrentItem() {
  ASSERT(mCurrentItems[CIS_TOOL]);
  useItem(mCurrentItems[CIS_TOOL]->getItemVariantType());
}

void CCharacter::useCurrentWeapon() {
  ASSERT(mCurrentItems[CIS_TOOL]);
  if (isReadyForNewAction()) {
    animAttack();
  }
}

void CCharacter::changeItem(ECharacterItemSlots slot,
                            const std::string &bone,
                            EItemVariantTypes item) {
  if (mCurrentItems[slot]) {
    // exit old item, it will be deleted by the shared_ptr
    mCurrentItems[slot]->exit();
  }
  mCurrentItems[slot] = std::make_shared<CCharacterItem>(this, bone, item);
  switch (slot) {
    case CIS_TOOL:
      mCurrentItems[slot]->hide();
      break;
    default:
      mCurrentItems[slot]->show();
      break;
  }
}

void CCharacter::useItem(EItemVariantTypes item) {
  if (isReadyForNewAction()) {
    animUseToolStart();
  }
}













