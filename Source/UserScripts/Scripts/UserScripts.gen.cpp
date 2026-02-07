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

#include "ChangeScreen.h"
#include "Dongho/Player/Player.h"
#include "Mingi/EngineLogoStartAnim.h"
#include "Mingi/UI/FadeInOutFX.h"
#include "Mingi/UI/MiniMap.h"
#include "Mingi/UI/RotateTrakingUI.h"
#include "Mingi/UI/TitleMenu.h"
#include "Mingi/UI/TitleOpeningSequencer.h"
#include "Mingi/UI/WorldSpaceUI.h"
#include "Mingi/UI/WorldSpaceUISorter.h"

using namespace rttr;
using namespace MMMEngine;

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<ChangeScreen>("ChangeScreen")
		(rttr::metadata("wrapper_type_name", "ObjPtr<ChangeScreen>"));

	registration::class_<ObjPtr<ChangeScreen>>("ObjPtr<ChangeScreen>")
		.constructor([]() { return Object::NewObject<ChangeScreen>(); })
		.method("Inject", &ObjPtr<ChangeScreen>::Inject);

	registration::class_<Player>("Player")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Player>"))
		.property("velocity", &Player::velocity)
		.property("healHP", &Player::healHP)
		.property("healDelay", &Player::healDelay);

	registration::class_<ObjPtr<Player>>("ObjPtr<Player>")
		.constructor([]() { return Object::NewObject<Player>(); })
		.method("Inject", &ObjPtr<Player>::Inject);

	registration::class_<EngineLogoStartAnim>("EngineLogoStartAnim")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EngineLogoStartAnim>"));

	registration::class_<ObjPtr<EngineLogoStartAnim>>("ObjPtr<EngineLogoStartAnim>")
		.constructor([]() { return Object::NewObject<EngineLogoStartAnim>(); })
		.method("Inject", &ObjPtr<EngineLogoStartAnim>::Inject);

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

}
