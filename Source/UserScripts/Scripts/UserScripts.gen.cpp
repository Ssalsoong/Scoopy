// Auto-generated. Do not edit.
#include "Export.h"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "Object.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

#include "Dongho/Building/BuildingPoint.h"
#include "Dongho/Castle/Castle.h"
#include "Dongho/Enemy/Enemy.h"
#include "Dongho/Manager/BattleManager.h"
#include "Dongho/Player/Player.h"

using namespace rttr;
using namespace MMMEngine;

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<BuildingPoint>("BuildingPoint")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BuildingPoint>"))
		.property("checkdist", &BuildingPoint::checkdist);

	registration::class_<ObjPtr<BuildingPoint>>("ObjPtr<BuildingPoint>")
		.constructor(&Object::NewObject<BuildingPoint>)
		.method("Inject", &ObjPtr<BuildingPoint>::Inject);

	registration::class_<Castle>("Castle")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Castle>"))
		.property("level", &Castle::level)
		.property("HP", &Castle::HP);

	registration::class_<ObjPtr<Castle>>("ObjPtr<Castle>")
		.constructor(&Object::NewObject<Castle>)
		.method("Inject", &ObjPtr<Castle>::Inject);

	registration::class_<Enemy>("Enemy")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Enemy>"))
		.property("HP", &Enemy::HP)
		.property("atk", &Enemy::atk)
		.property("velocity", &Enemy::velocity)
		.property("attackDelay", &Enemy::attackDelay)
		.property("battledist", &Enemy::battledist)
		.property("checkdist", &Enemy::checkdist);

	registration::class_<ObjPtr<Enemy>>("ObjPtr<Enemy>")
		.constructor(&Object::NewObject<Enemy>)
		.method("Inject", &ObjPtr<Enemy>::Inject);

	registration::class_<BattleManager>("BattleManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BattleManager>"));

	registration::class_<ObjPtr<BattleManager>>("ObjPtr<BattleManager>")
		.constructor(&Object::NewObject<BattleManager>)
		.method("Inject", &ObjPtr<BattleManager>::Inject);

	registration::class_<Player>("Player")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Player>"))
		.property("level", &Player::level)
		.property("HP", &Player::HP)
		.property("maxHP", &Player::maxHP)
		.property("battledist", &Player::battledist)
		.property("atk", &Player::atk);

	registration::class_<ObjPtr<Player>>("ObjPtr<Player>")
		.constructor(&Object::NewObject<Player>)
		.method("Inject", &ObjPtr<Player>::Inject);

}
