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
#include "Dongho/Manager/BuildingManager.h"
#include "Dongho/Player/Player.h"
#include "Mingi/ClearEmitTrail.h"
#include "Mingi/EngineLogoStartAnim.h"
#include "Mingi/ExplosionParticles.h"
#include "Mingi/ExplosionPool.h"
#include "Mingi/FXSnowFall.h"
#include "Mingi/Manager/SoundManager.h"
#include "Mingi/RedLine.h"
#include "Mingi/UI/CameraMove.h"
#include "Mingi/UI/FadeInOutFX.h"
#include "Mingi/UI/MiniMap.h"
#include "Mingi/UI/PauseUI.h"
#include "Mingi/UI/RotateTrakingUI.h"
#include "Mingi/UI/ScoopParticleCheck.h"
#include "Mingi/UI/SwitchSceneFX.h"
#include "Mingi/UI/TimerUI.h"
#include "Mingi/UI/TitleMenu.h"
#include "Mingi/UI/TitleOpeningSequencer.h"
#include "Mingi/UI/WorldSpaceUI.h"
#include "Mingi/UI/WorldSpaceUISorter.h"
#include "Sunken/AnimResourceManager.h"
#include "Sunken/BuildingLevelController.h"
#include "Sunken/CastleLevelController.h"
#include "Sunken/ControlManager.h"
#include "Sunken/EnemyAnimController.h"
#include "Sunken/LevelUpBubble.h"
#include "Sunken/LevelUpManager.h"
#include "Sunken/PlayerAnimController.h"
#include "Sunken/PlayerHpGage.h"
#include "Sunken/PrefabTest.h"
#include "Sunken/TimerGageScript.h"
#include "Sunken/WaveUIScript.h"
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
#include "Dongho/Building/Building.h"
#include "Dongho/Enemy/ArrowEnemy.h"
#include "Dongho/Enemy/NormalEnemy.h"
#include "Dongho/Enemy/ThiefEnemy.h"

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
		.property("atk", &Castle::atk)
		.property("point", &Castle::point)
		.property("pre_bullet", &Castle::pre_bullet)
		.property("bulletSpeed", &Castle::bulletSpeed);

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

	registration::class_<BuildingManager>("BuildingManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BuildingManager>"))
		.property("pre_building", &BuildingManager::pre_building);

	registration::class_<ObjPtr<BuildingManager>>("ObjPtr<BuildingManager>")
		.constructor([]() { return Object::NewObject<BuildingManager>(); })
		.method("Inject", &ObjPtr<BuildingManager>::Inject);

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

	registration::class_<ClearEmitTrail>("ClearEmitTrail")
		(rttr::metadata("wrapper_type_name", "ObjPtr<ClearEmitTrail>"));

	registration::class_<ObjPtr<ClearEmitTrail>>("ObjPtr<ClearEmitTrail>")
		.constructor([]() { return Object::NewObject<ClearEmitTrail>(); })
		.method("Inject", &ObjPtr<ClearEmitTrail>::Inject);

	registration::class_<EngineLogoStartAnim>("EngineLogoStartAnim")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EngineLogoStartAnim>"));

	registration::class_<ObjPtr<EngineLogoStartAnim>>("ObjPtr<EngineLogoStartAnim>")
		.constructor([]() { return Object::NewObject<EngineLogoStartAnim>(); })
		.method("Inject", &ObjPtr<EngineLogoStartAnim>::Inject);

	registration::class_<ExplosionParticles>("ExplosionParticles")
		(rttr::metadata("wrapper_type_name", "ObjPtr<ExplosionParticles>"))
		.property("Particle1", &ExplosionParticles::Particle1)
		.property("Particle2", &ExplosionParticles::Particle2)
		.property("Particle3", &ExplosionParticles::Particle3)
		.property("Particle4", &ExplosionParticles::Particle4);

	registration::class_<ObjPtr<ExplosionParticles>>("ObjPtr<ExplosionParticles>")
		.constructor([]() { return Object::NewObject<ExplosionParticles>(); })
		.method("Inject", &ObjPtr<ExplosionParticles>::Inject);

	registration::class_<ExplosionPool>("ExplosionPool")
		(rttr::metadata("wrapper_type_name", "ObjPtr<ExplosionPool>"))
		.property("ExplosionPrefab", &ExplosionPool::ExplosionPrefab);

	registration::class_<ObjPtr<ExplosionPool>>("ObjPtr<ExplosionPool>")
		.constructor([]() { return Object::NewObject<ExplosionPool>(); })
		.method("Inject", &ObjPtr<ExplosionPool>::Inject);

	registration::class_<FXSnowFall>("FXSnowFall")
		(rttr::metadata("wrapper_type_name", "ObjPtr<FXSnowFall>"));

	registration::class_<ObjPtr<FXSnowFall>>("ObjPtr<FXSnowFall>")
		.constructor([]() { return Object::NewObject<FXSnowFall>(); })
		.method("Inject", &ObjPtr<FXSnowFall>::Inject);

	registration::class_<SoundManager>("SoundManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SoundManager>"));

	registration::class_<ObjPtr<SoundManager>>("ObjPtr<SoundManager>")
		.constructor([]() { return Object::NewObject<SoundManager>(); })
		.method("Inject", &ObjPtr<SoundManager>::Inject);

	registration::class_<RedLine>("RedLine")
		(rttr::metadata("wrapper_type_name", "ObjPtr<RedLine>"))
		.property("CheckDistance", &RedLine::CheckDistance)
		.property("CheckZPos", &RedLine::CheckZPos)
		.property("CheckXPos", &RedLine::CheckXPos)
		.property("PlayerTr", &RedLine::PlayerTr)
		.property("ReddoLine", &RedLine::ReddoLine);

	registration::class_<ObjPtr<RedLine>>("ObjPtr<RedLine>")
		.constructor([]() { return Object::NewObject<RedLine>(); })
		.method("Inject", &ObjPtr<RedLine>::Inject);

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
		.property("PlayerRectUI", &MiniMap::PlayerRectUI)
		.property("EnemyTrackerUI", &MiniMap::EnemyTrackerUI)
		.property("BuildingTrackerUI", &MiniMap::BuildingTrackerUI)
		.property("InitialPoolSize", &MiniMap::InitialPoolSize);

	registration::class_<ObjPtr<MiniMap>>("ObjPtr<MiniMap>")
		.constructor([]() { return Object::NewObject<MiniMap>(); })
		.method("Inject", &ObjPtr<MiniMap>::Inject);

	registration::class_<PauseUI>("PauseUI")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PauseUI>"))
		.property("PanelGO", &PauseUI::PanelGO)
		.property("P_rect", &PauseUI::P_rect)
		.property("a_rect", &PauseUI::a_rect)
		.property("u_rect", &PauseUI::u_rect)
		.property("s_rect", &PauseUI::s_rect)
		.property("e_rect", &PauseUI::e_rect)
		.property("Resume_rect", &PauseUI::Resume_rect)
		.property("ToTitle_rect", &PauseUI::ToTitle_rect)
		.property("PosYCurve", &PauseUI::PosYCurve)
		.property("ButtonYCurve", &PauseUI::ButtonYCurve)
		.property("FadeDuration", &PauseUI::FadeDuration)
		.property("SceneChangeTime", &PauseUI::SceneChangeTime)
		.property("AnimOffestTime", &PauseUI::AnimOffestTime)
		.property("buttonAnimOffset", &PauseUI::buttonAnimOffset)
		.property("FocusFadeSpeed", &PauseUI::FocusFadeSpeed)
		.property("ButtonScaleCurve", &PauseUI::ButtonScaleCurve)
		.property("ButtonSelectTimeOffset", &PauseUI::ButtonSelectTimeOffset)
		.property("TitleSceneName", &PauseUI::TitleSceneName);

	registration::class_<ObjPtr<PauseUI>>("ObjPtr<PauseUI>")
		.constructor([]() { return Object::NewObject<PauseUI>(); })
		.method("Inject", &ObjPtr<PauseUI>::Inject);

	registration::class_<RotateTrakingUI>("RotateTrakingUI")
		(rttr::metadata("wrapper_type_name", "ObjPtr<RotateTrakingUI>"))
		.property("WorldSpaceUI", &RotateTrakingUI::WorldSpaceUI);

	registration::class_<ObjPtr<RotateTrakingUI>>("ObjPtr<RotateTrakingUI>")
		.constructor([]() { return Object::NewObject<RotateTrakingUI>(); })
		.method("Inject", &ObjPtr<RotateTrakingUI>::Inject);

	registration::class_<ScoopParticleCheck>("ScoopParticleCheck")
		(rttr::metadata("wrapper_type_name", "ObjPtr<ScoopParticleCheck>"))
		.property("pc", &ScoopParticleCheck::pc)
		.property("pt", &ScoopParticleCheck::pt);

	registration::class_<ObjPtr<ScoopParticleCheck>>("ObjPtr<ScoopParticleCheck>")
		.constructor([]() { return Object::NewObject<ScoopParticleCheck>(); })
		.method("Inject", &ObjPtr<ScoopParticleCheck>::Inject);

	registration::class_<SwitchSceneFX>("SwitchSceneFX")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SwitchSceneFX>"))
		.property("FXImage", &SwitchSceneFX::FXImage)
		.property("FXCurve", &SwitchSceneFX::FXCurve);

	registration::class_<ObjPtr<SwitchSceneFX>>("ObjPtr<SwitchSceneFX>")
		.constructor([]() { return Object::NewObject<SwitchSceneFX>(); })
		.method("Inject", &ObjPtr<SwitchSceneFX>::Inject);

	registration::class_<TimerUI>("TimerUI")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TimerUI>"))
		.property("WaveText", &TimerUI::WaveText)
		.property("TimerGage", &TimerUI::TimerGage)
		.property("WaveBack", &TimerUI::WaveBack)
		.property("changeTextTime", &TimerUI::changeTextTime)
		.property("scaleCurve", &TimerUI::scaleCurve)
		.property("rotCurve", &TimerUI::rotCurve)
		.property("posCurve", &TimerUI::posCurve)
		.property("alphaCurve", &TimerUI::alphaCurve)
		.property("gageCurve", &TimerUI::gageCurve);

	registration::class_<ObjPtr<TimerUI>>("ObjPtr<TimerUI>")
		.constructor([]() { return Object::NewObject<TimerUI>(); })
		.method("Inject", &ObjPtr<TimerUI>::Inject);

	registration::class_<TitleMenu>("TitleMenu")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TitleMenu>"))
		.property("playButton", &TitleMenu::playButton)
		.property("creditButton", &TitleMenu::creditButton)
		.property("exitButton", &TitleMenu::exitButton)
		.property("creditPopup", &TitleMenu::creditPopup)
		.property("IsControllAble", &TitleMenu::IsControllAble)
		.property("PlaySceneName", &TitleMenu::PlaySceneName)
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
		.property("DontMoveUI", &WorldSpaceUI::DontMoveUI)
		.property("DontSortByDistance", &WorldSpaceUI::DontSortByDistance)
		.property("ClampToScreenEdge", &WorldSpaceUI::ClampToScreenEdge)
		.property("EdgeYOffset", &WorldSpaceUI::EdgeYOffset)
		.property("EdgeXOffset", &WorldSpaceUI::EdgeXOffset)
		.property("HideWhenOutsideScreen", &WorldSpaceUI::HideWhenOutsideScreen)
		.property("HideWhenInsideScreen", &WorldSpaceUI::HideWhenInsideScreen)
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

	registration::class_<BuildingLevelController>("BuildingLevelController")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BuildingLevelController>"))
		.property("mBuilding", &BuildingLevelController::mBuilding)
		.property("mPlayer", &BuildingLevelController::mPlayer)
		.property("mBattleStat", &BuildingLevelController::mBattleStat)
		.property("mGagePosOffset", &BuildingLevelController::mGagePosOffset)
		.property("mReadyPosOffset", &BuildingLevelController::mReadyPosOffset)
		.property("mCountPosOffset", &BuildingLevelController::mCountPosOffset)
		.property("mUIScale", &BuildingLevelController::mUIScale)
		.property("mPadding", &BuildingLevelController::mPadding)
		.property("mSelectPadding", &BuildingLevelController::mSelectPadding)
		.property("mDistanceFactor", &BuildingLevelController::mDistanceFactor);

	registration::class_<ObjPtr<BuildingLevelController>>("ObjPtr<BuildingLevelController>")
		.constructor([]() { return Object::NewObject<BuildingLevelController>(); })
		.method("Inject", &ObjPtr<BuildingLevelController>::Inject);

	registration::class_<CastleLevelController>("CastleLevelController")
		(rttr::metadata("wrapper_type_name", "ObjPtr<CastleLevelController>"))
		.property("mGagePosOffset", &CastleLevelController::mGagePosOffset)
		.property("mReadyPosOffset", &CastleLevelController::mReadyPosOffset)
		.property("mCountPosOffset", &CastleLevelController::mCountPosOffset)
		.property("mUIScale", &CastleLevelController::mUIScale)
		.property("mPadding", &CastleLevelController::mPadding)
		.property("mSelectPadding", &CastleLevelController::mSelectPadding)
		.property("mDistanceFactor", &CastleLevelController::mDistanceFactor);

	registration::class_<ObjPtr<CastleLevelController>>("ObjPtr<CastleLevelController>")
		.constructor([]() { return Object::NewObject<CastleLevelController>(); })
		.method("Inject", &ObjPtr<CastleLevelController>::Inject);

	registration::class_<ControlManager>("ControlManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<ControlManager>"));

	registration::class_<ObjPtr<ControlManager>>("ObjPtr<ControlManager>")
		.constructor([]() { return Object::NewObject<ControlManager>(); })
		.method("Inject", &ObjPtr<ControlManager>::Inject);

	registration::class_<EnemyAnimController>("EnemyAnimController")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EnemyAnimController>"))
		.property("mAnimator", &EnemyAnimController::mAnimator)
		.property("mEnemy", &EnemyAnimController::mEnemy)
		.property("mAnimSpeed", &EnemyAnimController::mAnimSpeed)
		.property("AnimSize", &EnemyAnimController::AnimSize);

	registration::class_<ObjPtr<EnemyAnimController>>("ObjPtr<EnemyAnimController>")
		.constructor([]() { return Object::NewObject<EnemyAnimController>(); })
		.method("Inject", &ObjPtr<EnemyAnimController>::Inject);

	registration::class_<LevelUpBubble>("LevelUpBubble")
		(rttr::metadata("wrapper_type_name", "ObjPtr<LevelUpBubble>"))
		.property("mAnimCurve", &LevelUpBubble::mAnimCurve)
		.property("mUIScale", &LevelUpBubble::mUIScale)
		.property("mDistanceFactor", &LevelUpBubble::mDistanceFactor)
		.property("mSpeechOffset", &LevelUpBubble::mSpeechOffset)
		.property("mIconOffset", &LevelUpBubble::mIconOffset)
		.property("mIconPadding", &LevelUpBubble::mIconPadding)
		.property("mHeadlineOffset", &LevelUpBubble::mHeadlineOffset)
		.property("mScriptOffset", &LevelUpBubble::mScriptOffset)
		.property("mSelectIconSize", &LevelUpBubble::mSelectIconSize)
		.property("mDeselectIconSize", &LevelUpBubble::mDeselectIconSize);

	registration::class_<ObjPtr<LevelUpBubble>>("ObjPtr<LevelUpBubble>")
		.constructor([]() { return Object::NewObject<LevelUpBubble>(); })
		.method("Inject", &ObjPtr<LevelUpBubble>::Inject);

	registration::class_<LevelUpManager>("LevelUpManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<LevelUpManager>"))
		.property("mCanvas", &LevelUpManager::mCanvas)
		.property("mPlayer", &LevelUpManager::mPlayer)
		.property("mCastle", &LevelUpManager::mCastle)
		.property("mLevelUpBubble", &LevelUpManager::mLevelUpBubble)
		.property("mExpGage", &LevelUpManager::mExpGage)
		.property("mHpGage", &LevelUpManager::mHpGage)
		.property("mReadyIcon", &LevelUpManager::mReadyIcon)
		.property("mCastleIcon", &LevelUpManager::mCastleIcon)
		.property("mScoopIcon", &LevelUpManager::mScoopIcon)
		.property("mHPIcon", &LevelUpManager::mHPIcon)
		.property("mBuffIcon", &LevelUpManager::mBuffIcon)
		.property("mDeBuffIcon", &LevelUpManager::mDeBuffIcon)
		.property("mSnowIcon", &LevelUpManager::mSnowIcon)
		.property("mSpeechBubbleIcon", &LevelUpManager::mSpeechBubbleIcon)
		.property("mHeadlineText", &LevelUpManager::mHeadlineText)
		.property("mScriptText", &LevelUpManager::mScriptText)
		.property("mReadyPrefab", &LevelUpManager::mReadyPrefab)
		.property("mCountPrefab", &LevelUpManager::mCountPrefab)
		.property("mGagePrefab", &LevelUpManager::mGagePrefab);

	registration::class_<ObjPtr<LevelUpManager>>("ObjPtr<LevelUpManager>")
		.constructor([]() { return Object::NewObject<LevelUpManager>(); })
		.method("Inject", &ObjPtr<LevelUpManager>::Inject);

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

	registration::class_<PlayerHpGage>("PlayerHpGage")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PlayerHpGage>"))
		.property("mPlayer", &PlayerHpGage::mPlayer);

	registration::class_<ObjPtr<PlayerHpGage>>("ObjPtr<PlayerHpGage>")
		.constructor([]() { return Object::NewObject<PlayerHpGage>(); })
		.method("Inject", &ObjPtr<PlayerHpGage>::Inject);

	registration::class_<PrefabTest>("PrefabTest")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PrefabTest>"));

	registration::class_<ObjPtr<PrefabTest>>("ObjPtr<PrefabTest>")
		.constructor([]() { return Object::NewObject<PrefabTest>(); })
		.method("Inject", &ObjPtr<PrefabTest>::Inject);

	registration::class_<TimerGageScript>("TimerGageScript")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TimerGageScript>"));

	registration::class_<ObjPtr<TimerGageScript>>("ObjPtr<TimerGageScript>")
		.constructor([]() { return Object::NewObject<TimerGageScript>(); })
		.method("Inject", &ObjPtr<TimerGageScript>::Inject);

	registration::class_<WaveUIScript>("WaveUIScript")
		(rttr::metadata("wrapper_type_name", "ObjPtr<WaveUIScript>"))
		.property("mWaveText", &WaveUIScript::mWaveText);

	registration::class_<ObjPtr<WaveUIScript>>("ObjPtr<WaveUIScript>")
		.constructor([]() { return Object::NewObject<WaveUIScript>(); })
		.method("Inject", &ObjPtr<WaveUIScript>::Inject);

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
		.property("mControlLayout", &PlayerController::mControlLayout)
		.property("m_TileMap", &PlayerController::m_TileMap)
		.property("m_SnowManager", &PlayerController::m_SnowManager);

	registration::class_<ObjPtr<PlayerController>>("ObjPtr<PlayerController>")
		.constructor([]() { return Object::NewObject<PlayerController>(); })
		.method("Inject", &ObjPtr<PlayerController>::Inject);

	registration::class_<PlayerMove>("PlayerMove")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PlayerMove>"))
		.property("mPAController", &PlayerMove::mPAController)
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
		.property("m_speed", &SnowBullet::m_speed)
		.property("target", &SnowBullet::target);

	registration::class_<ObjPtr<SnowBullet>>("ObjPtr<SnowBullet>")
		.constructor([]() { return Object::NewObject<SnowBullet>(); })
		.method("Inject", &ObjPtr<SnowBullet>::Inject);

	registration::class_<SnowCollider>("SnowCollider")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowCollider>"))
		.property("m_Rolesmooth", &SnowCollider::m_Rolesmooth)
		.property("TriggerCollider", &SnowCollider::TriggerCollider)
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
		.property("Pre_Snow", &SnowballManager2::Pre_Snow)
		.property("m_Castle", &SnowballManager2::m_Castle);

	registration::class_<ObjPtr<SnowballManager2>>("ObjPtr<SnowballManager2>")
		.constructor([]() { return Object::NewObject<SnowballManager2>(); })
		.method("Inject", &ObjPtr<SnowballManager2>::Inject);

	registration::class_<TileMap>("TileMap")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TileMap>"))
		.property("P_trans", &TileMap::P_trans)
		.property("threshold", &TileMap::threshold)
		.property("box", &TileMap::box)
		.property("RESPAWN_TIME", &TileMap::RESPAWN_TIME);

	registration::class_<ObjPtr<TileMap>>("ObjPtr<TileMap>")
		.constructor([]() { return Object::NewObject<TileMap>(); })
		.method("Inject", &ObjPtr<TileMap>::Inject);

}
