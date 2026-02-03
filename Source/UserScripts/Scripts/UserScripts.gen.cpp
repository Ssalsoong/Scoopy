// Auto-generated. Do not edit.
#include "Export.h"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "Object.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

#include "Dongho/Building/BuildingPoint.h"
#include "Dongho/Player/Player.h"

using namespace rttr;
using namespace MMMEngine;

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<BuildingPoint>("BuildingPoint")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BuildingPoint>"))
		.property("checkdist", &BuildingPoint::checkdist);

	registration::class_<ObjPtr<BuildingPoint>>("ObjPtr<BuildingPoint>")
		.constructor([]() { return Object::NewObject<BuildingPoint>(); })
		.method("Inject", &ObjPtr<BuildingPoint>::Inject);

	registration::class_<Player>("Player")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Player>"))
		.property("HP", &Player::HP)
		.property("maxHP", &Player::maxHP)
		.property("battledist", &Player::battledist)
		.property("atk", &Player::atk);

	registration::class_<ObjPtr<Player>>("ObjPtr<Player>")
		.constructor([]() { return Object::NewObject<Player>(); })
		.method("Inject", &ObjPtr<Player>::Inject);

}
