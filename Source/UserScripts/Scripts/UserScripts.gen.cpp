// Auto-generated. Do not edit.
#pragma optimize("", off)
#include "Export.h"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "Object.h"
#include "GameObject.h"
#include "CoreComponents.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

#include "Dongho/Battlestats.h"
#include "Dongho/Building/BuildingPoint.h"
#include "Dongho/Castle/Castle.h"
#include "Dongho/Enemy/Enemy.h"
#include "Dongho/Manager/BattleManager.h"
#include "Dongho/Player/Player.h"
#include "Mingi/EngineLogoStartAnim.h"
#include "Mingi/UI/CameraMove.h"
#include "Mingi/UI/FadeInOutFX.h"
#include "Mingi/UI/MiniMap.h"
#include "Mingi/UI/RotateTrakingUI.h"
#include "Mingi/UI/TitleMenu.h"
#include "Mingi/UI/TitleOpeningSequencer.h"
#include "Mingi/UI/WorldSpaceUI.h"
#include "Mingi/UI/WorldSpaceUISorter.h"
#include "Sunken/AnimResourceManager.h"
#include "Sunken/EnemyAnimController.h"
#include "Sunken/PlayerAnimController.h"
#include "test/CastleManager.h"
#include "test/EnemyMove.h"
#include "test/MeshSize.h"
#include "test/PlayerController.h"
#include "test/PlayerMove.h"
#include "test/SnowBullet.h"
#include "test/SnowCollider.h"
#include "test/SnowTrigger.h"
#include "test/SnowballManager2.h"
#include "test/TileMap.h"

using namespace rttr;
using namespace MMMEngine;

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Battlestats>("Battlestats")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Battlestats>"))
		.property("HP", &Battlestats::HP);

	registration::class_<ObjPtr<Battlestats>>("ObjPtr<Battlestats>")
		.constructor([]() { return Object::NewObject<Battlestats>(); })
		.method("Inject", &ObjPtr<Battlestats>::Inject);

	registration::class_<BuildingPoint>("BuildingPoint")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BuildingPoint>"))
		.property("checkdist", &BuildingPoint::checkdist);

	registration::class_<ObjPtr<BuildingPoint>>("ObjPtr<BuildingPoint>")
		.constructor([]() { return Object::NewObject<BuildingPoint>(); })
		.method("Inject", &ObjPtr<BuildingPoint>::Inject);

	registration::class_<Castle>("Castle")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Castle>"))
		.property("level", &Castle::level)
		.property("maxHP", &Castle::maxHP)
		.property("exp", &Castle::exp)
		.property("point", &Castle::point);

	registration::class_<ObjPtr<Castle>>("ObjPtr<Castle>")
		.constructor([]() { return Object::NewObject<Castle>(); })
		.method("Inject", &ObjPtr<Castle>::Inject);

	registration::class_<Enemy>("Enemy")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Enemy>"))
		.property("atk", &Enemy::atk)
		.property("velocity", &Enemy::velocity)
		.property("attackDelay", &Enemy::attackDelay)
		.property("battledist", &Enemy::battledist)
		.property("checkdist", &Enemy::checkdist);

	registration::class_<ObjPtr<Enemy>>("ObjPtr<Enemy>")
		.constructor([]() { return Object::NewObject<Enemy>(); })
		.method("Inject", &ObjPtr<Enemy>::Inject);

	registration::class_<BattleManager>("BattleManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BattleManager>"));

	registration::class_<ObjPtr<BattleManager>>("ObjPtr<BattleManager>")
		.constructor([]() { return Object::NewObject<BattleManager>(); })
		.method("Inject", &ObjPtr<BattleManager>::Inject);

	registration::class_<Player>("Player")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Player>"))
		.property("level", &Player::level)
		.property("maxHP", &Player::maxHP)
		.property("battledist", &Player::battledist)
		.property("atk", &Player::atk)
		.property("maxpoint", &Player::maxpoint);

	registration::class_<ObjPtr<Player>>("ObjPtr<Player>")
		.constructor([]() { return Object::NewObject<Player>(); })
		.method("Inject", &ObjPtr<Player>::Inject);

	registration::class_<EngineLogoStartAnim>("EngineLogoStartAnim")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EngineLogoStartAnim>"));

	registration::class_<ObjPtr<EngineLogoStartAnim>>("ObjPtr<EngineLogoStartAnim>")
		.constructor([]() { return Object::NewObject<EngineLogoStartAnim>(); })
		.method("Inject", &ObjPtr<EngineLogoStartAnim>::Inject);

	registration::class_<CameraMove>("CameraMove")
		(rttr::metadata("wrapper_type_name", "ObjPtr<CameraMove>"))
		.property("Offset", &CameraMove::Offset)
		.property("Target", &CameraMove::Target)
		.property("ChasingSpeed", &CameraMove::ChasingSpeed)
		.property("XClamp", &CameraMove::XClamp)
		.property("ZClamp", &CameraMove::ZClamp);

	registration::class_<ObjPtr<CameraMove>>("ObjPtr<CameraMove>")
		.constructor([]() { return Object::NewObject<CameraMove>(); })
		.method("Inject", &ObjPtr<CameraMove>::Inject);

	registration::class_<FadeInOutFX>("FadeInOutFX")
		(rttr::metadata("wrapper_type_name", "ObjPtr<FadeInOutFX>"))
		.property("FadeImage", &FadeInOutFX::FadeImage)
		.property("FadeDuration", &FadeInOutFX::FadeDuration)
		.property("FadeCurve", &FadeInOutFX::FadeCurve);

	registration::class_<ObjPtr<FadeInOutFX>>("ObjPtr<FadeInOutFX>")
		.constructor([]() { return Object::NewObject<FadeInOutFX>(); })
		.method("Inject", &ObjPtr<FadeInOutFX>::Inject);

	registration::class_<MiniMap>("MiniMap")
		(rttr::metadata("wrapper_type_name", "ObjPtr<MiniMap>"))
		.property("MinimapUnitScale", &MiniMap::MinimapUnitScale)
		.property("Player", &MiniMap::Player)
		.property("PlayerRectUI", &MiniMap::PlayerRectUI);

	registration::class_<ObjPtr<MiniMap>>("ObjPtr<MiniMap>")
		.constructor([]() { return Object::NewObject<MiniMap>(); })
		.method("Inject", &ObjPtr<MiniMap>::Inject);

	registration::class_<RotateTrakingUI>("RotateTrakingUI")
		(rttr::metadata("wrapper_type_name", "ObjPtr<RotateTrakingUI>"))
		.property("WorldSpaceUI", &RotateTrakingUI::WorldSpaceUI);

	registration::class_<ObjPtr<RotateTrakingUI>>("ObjPtr<RotateTrakingUI>")
		.constructor([]() { return Object::NewObject<RotateTrakingUI>(); })
		.method("Inject", &ObjPtr<RotateTrakingUI>::Inject);

	registration::class_<TitleMenu>("TitleMenu")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TitleMenu>"))
		.property("playButton", &TitleMenu::playButton)
		.property("creditButton", &TitleMenu::creditButton)
		.property("exitButton", &TitleMenu::exitButton)
		.property("creditPopup", &TitleMenu::creditPopup)
		.property("IsControllAble", &TitleMenu::IsControllAble)
		.property("ButtonOnScaleX", &TitleMenu::ButtonOnScaleX)
		.property("ButtonOnScaleY", &TitleMenu::ButtonOnScaleY)
		.property("ButtonExecuteScaleX", &TitleMenu::ButtonExecuteScaleX)
		.property("ButtonExecuteScaleY", &TitleMenu::ButtonExecuteScaleY)
		.property("ButtonExecuteRotZ", &TitleMenu::ButtonExecuteRotZ)
		.property("PopUpScaleXY", &TitleMenu::PopUpScaleXY)
		.property("PopUpOffScaleXY", &TitleMenu::PopUpOffScaleXY)
		.property("popupFadeDuration", &TitleMenu::popupFadeDuration);

	registration::class_<ObjPtr<TitleMenu>>("ObjPtr<TitleMenu>")
		.constructor([]() { return Object::NewObject<TitleMenu>(); })
		.method("Inject", &ObjPtr<TitleMenu>::Inject);

	registration::class_<TitleOpeningSequencer>("TitleOpeningSequencer")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TitleOpeningSequencer>"))
		.property("titleFadeStartTime", &TitleOpeningSequencer::titleFadeStartTime)
		.property("titleLoadFadeTime", &TitleOpeningSequencer::titleLoadFadeTime)
		.property("buttonLoadFadeTime", &TitleOpeningSequencer::buttonLoadFadeTime)
		.property("buttonAnimOffsetTime", &TitleOpeningSequencer::buttonAnimOffsetTime)
		.property("buttonAnimYDiff", &TitleOpeningSequencer::buttonAnimYDiff)
		.property("TitleImage", &TitleOpeningSequencer::TitleImage)
		.property("PlayImage", &TitleOpeningSequencer::PlayImage)
		.property("CreditImage", &TitleOpeningSequencer::CreditImage)
		.property("ExitImage", &TitleOpeningSequencer::ExitImage)
		.property("TitleRotate", &TitleOpeningSequencer::TitleRotate)
		.property("TitleScaleX", &TitleOpeningSequencer::TitleScaleX)
		.property("TitleScaleY", &TitleOpeningSequencer::TitleScaleY)
		.property("ButtonScaleX", &TitleOpeningSequencer::ButtonScaleX)
		.property("ButtonScaleY", &TitleOpeningSequencer::ButtonScaleY)
		.property("TitleMenu", &TitleOpeningSequencer::TitleMenu);

	registration::class_<ObjPtr<TitleOpeningSequencer>>("ObjPtr<TitleOpeningSequencer>")
		.constructor([]() { return Object::NewObject<TitleOpeningSequencer>(); })
		.method("Inject", &ObjPtr<TitleOpeningSequencer>::Inject);

	registration::class_<WorldSpaceUI>("WorldSpaceUI")
		(rttr::metadata("wrapper_type_name", "ObjPtr<WorldSpaceUI>"))
		.property("SelfRect", &WorldSpaceUI::SelfRect)
		.property("TargetTransform", &WorldSpaceUI::TargetTransform)
		.property("DontSortByDistance", &WorldSpaceUI::DontSortByDistance)
		.property("ClampToScreenEdge", &WorldSpaceUI::ClampToScreenEdge)
		.property("EdgeYOffset", &WorldSpaceUI::EdgeYOffset)
		.property("EdgeXOffset", &WorldSpaceUI::EdgeXOffset)
		.property("HideWhenOutsideScreen", &WorldSpaceUI::HideWhenOutsideScreen)
		.property("ScaleWithDistance", &WorldSpaceUI::ScaleWithDistance)
		.property("ReferenceDistance", &WorldSpaceUI::ReferenceDistance)
		.property("MinScale", &WorldSpaceUI::MinScale)
		.property("MaxScale", &WorldSpaceUI::MaxScale);

	registration::class_<ObjPtr<WorldSpaceUI>>("ObjPtr<WorldSpaceUI>")
		.constructor([]() { return Object::NewObject<WorldSpaceUI>(); })
		.method("Inject", &ObjPtr<WorldSpaceUI>::Inject);

	registration::class_<WorldSpaceUISorter>("WorldSpaceUISorter")
		(rttr::metadata("wrapper_type_name", "ObjPtr<WorldSpaceUISorter>"))
		.property("GlobalOrder", &WorldSpaceUISorter::GlobalOrder);

	registration::class_<ObjPtr<WorldSpaceUISorter>>("ObjPtr<WorldSpaceUISorter>")
		.constructor([]() { return Object::NewObject<WorldSpaceUISorter>(); })
		.method("Inject", &ObjPtr<WorldSpaceUISorter>::Inject);

	registration::class_<AnimResourceManager>("AnimResourceManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<AnimResourceManager>"))
		.property("animSize", &AnimResourceManager::animSize);

	registration::class_<ObjPtr<AnimResourceManager>>("ObjPtr<AnimResourceManager>")
		.constructor([]() { return Object::NewObject<AnimResourceManager>(); })
		.method("Inject", &ObjPtr<AnimResourceManager>::Inject);

	registration::class_<EnemyAnimController>("EnemyAnimController")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EnemyAnimController>"))
		.property("mAnimator", &EnemyAnimController::mAnimator)
		.property("mEnemy", &EnemyAnimController::mEnemy)
		.property("mAnimSpeed", &EnemyAnimController::mAnimSpeed)
		.property("AnimSize", &EnemyAnimController::AnimSize);

	registration::class_<ObjPtr<EnemyAnimController>>("ObjPtr<EnemyAnimController>")
		.constructor([]() { return Object::NewObject<EnemyAnimController>(); })
		.method("Inject", &ObjPtr<EnemyAnimController>::Inject);

	registration::class_<PlayerAnimController>("PlayerAnimController")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PlayerAnimController>"))
		.property("mIdleSpeed", &PlayerAnimController::mIdleSpeed)
		.property("mAnimSpeed", &PlayerAnimController::mAnimSpeed)
		.property("mMoveSpeed", &PlayerAnimController::mMoveSpeed)
		.property("mAttacking", &PlayerAnimController::mAttacking)
		.property("mScooping", &PlayerAnimController::mScooping)
		.property("mAnimSize", &PlayerAnimController::mAnimSize)
		.property("mAnimator", &PlayerAnimController::mAnimator)
		.property("mAnimController", &PlayerAnimController::mAnimController);

	registration::class_<ObjPtr<PlayerAnimController>>("ObjPtr<PlayerAnimController>")
		.constructor([]() { return Object::NewObject<PlayerAnimController>(); })
		.method("Inject", &ObjPtr<PlayerAnimController>::Inject);

	registration::class_<CastleManager>("CastleManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<CastleManager>"));

	registration::class_<ObjPtr<CastleManager>>("ObjPtr<CastleManager>")
		.constructor([]() { return Object::NewObject<CastleManager>(); })
		.method("Inject", &ObjPtr<CastleManager>::Inject);

	registration::class_<EnemyMove>("EnemyMove")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EnemyMove>"))
		.property("movespeed", &EnemyMove::movespeed)
		.property("Obj_target", &EnemyMove::Obj_target)
		.property("sweepRadius", &EnemyMove::sweepRadius)
		.property("sweepAhead", &EnemyMove::sweepAhead)
		.property("avoidGain", &EnemyMove::avoidGain);

	registration::class_<ObjPtr<EnemyMove>>("ObjPtr<EnemyMove>")
		.constructor([]() { return Object::NewObject<EnemyMove>(); })
		.method("Inject", &ObjPtr<EnemyMove>::Inject);

	registration::class_<MeshSize>("MeshSize")
		(rttr::metadata("wrapper_type_name", "ObjPtr<MeshSize>"));

	registration::class_<ObjPtr<MeshSize>>("ObjPtr<MeshSize>")
		.constructor([]() { return Object::NewObject<MeshSize>(); })
		.method("Inject", &ObjPtr<MeshSize>::Inject);

	registration::class_<PlayerController>("PlayerController")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PlayerController>"))
		.property("m_TileMap", &PlayerController::m_TileMap)
		.property("m_SnowManager", &PlayerController::m_SnowManager);

	registration::class_<ObjPtr<PlayerController>>("ObjPtr<PlayerController>")
		.constructor([]() { return Object::NewObject<PlayerController>(); })
		.method("Inject", &ObjPtr<PlayerController>::Inject);

	registration::class_<PlayerMove>("PlayerMove")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PlayerMove>"))
		.property("isSlow", &PlayerMove::isSlow)
		.property("turnSpeed", &PlayerMove::turnSpeed)
		.property("is_Scoop", &PlayerMove::is_Scoop)
		.property("DefaultSpeed", &PlayerMove::DefaultSpeed)
		.property("OnSnowSpeed", &PlayerMove::OnSnowSpeed)
		.property("MinusSpeed", &PlayerMove::MinusSpeed)
		.property("MinSpeed", &PlayerMove::MinSpeed);

	registration::class_<ObjPtr<PlayerMove>>("ObjPtr<PlayerMove>")
		.constructor([]() { return Object::NewObject<PlayerMove>(); })
		.method("Inject", &ObjPtr<PlayerMove>::Inject);

	registration::class_<SnowBullet>("SnowBullet")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowBullet>"))
		.property("speed", &SnowBullet::speed)
		.property("target", &SnowBullet::target);

	registration::class_<ObjPtr<SnowBullet>>("ObjPtr<SnowBullet>")
		.constructor([]() { return Object::NewObject<SnowBullet>(); })
		.method("Inject", &ObjPtr<SnowBullet>::Inject);

	registration::class_<SnowCollider>("SnowCollider")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowCollider>"))
		.property("m_Rolesmooth", &SnowCollider::m_Rolesmooth)
		.property("TriggerCollider", &SnowCollider::TriggerCollider)
		.property("SnowManager", &SnowCollider::SnowManager)
		.property("m_holdDistance", &SnowCollider::m_holdDistance)
		.property("m_rollSpeed", &SnowCollider::m_rollSpeed);

	registration::class_<ObjPtr<SnowCollider>>("ObjPtr<SnowCollider>")
		.constructor([]() { return Object::NewObject<SnowCollider>(); })
		.method("Inject", &ObjPtr<SnowCollider>::Inject);

	registration::class_<SnowTrigger>("SnowTrigger")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowTrigger>"));

	registration::class_<ObjPtr<SnowTrigger>>("ObjPtr<SnowTrigger>")
		.constructor([]() { return Object::NewObject<SnowTrigger>(); })
		.method("Inject", &ObjPtr<SnowTrigger>::Inject);

	registration::class_<SnowballManager2>("SnowballManager2")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowballManager2>"))
		.property("m_Player", &SnowballManager2::m_Player)
		.property("Pre_Snow", &SnowballManager2::Pre_Snow);

	registration::class_<ObjPtr<SnowballManager2>>("ObjPtr<SnowballManager2>")
		.constructor([]() { return Object::NewObject<SnowballManager2>(); })
		.method("Inject", &ObjPtr<SnowballManager2>::Inject);

	registration::class_<TileMap>("TileMap")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TileMap>"))
		.property("P_trans", &TileMap::P_trans)
		.property("threshold", &TileMap::threshold)
		.property("box", &TileMap::box);

	registration::class_<ObjPtr<TileMap>>("ObjPtr<TileMap>")
		.constructor([]() { return Object::NewObject<TileMap>(); })
		.method("Inject", &ObjPtr<TileMap>::Inject);

}
