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
#include "Mingi/UI/FadeInOutFX.h"
#include "Mingi/UI/MiniMap.h"
#include "Mingi/UI/RotateTrakingUI.h"
#include "Mingi/UI/TestValidFade.h"
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

	registration::class_<FadeInOutFX>("FadeInOutFX")
		(rttr::metadata("wrapper_type_name", "ObjPtr<FadeInOutFX>"))
		.property("m_internalTime", &FadeInOutFX::m_internalTime)
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

	registration::class_<TestValidFade>("TestValidFade")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TestValidFade>"));

	registration::class_<ObjPtr<TestValidFade>>("ObjPtr<TestValidFade>")
		.constructor([]() { return Object::NewObject<TestValidFade>(); })
		.method("Inject", &ObjPtr<TestValidFade>::Inject);

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
