// Auto-generated. Do not edit.
#pragma optimize("", off)
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
#include "Dongho/Manager/SnowballManager.h"
#include "Dongho/Player/Player.h"
#include "test/EnemyMove.h"
#include "test/PlayerMove.h"
#include "test/SnowBullet.h"
#include "test/SnowCollider.h"
#include "test/SnowTrigger.h"
#include "test/TileMap.h"

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

	registration::class_<Castle>("Castle")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Castle>"))
		.property("level", &Castle::level)
		.property("HP", &Castle::HP);

	registration::class_<ObjPtr<Castle>>("ObjPtr<Castle>")
		.constructor([]() { return Object::NewObject<Castle>(); })
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
		.constructor([]() { return Object::NewObject<Enemy>(); })
		.method("Inject", &ObjPtr<Enemy>::Inject);

	registration::class_<BattleManager>("BattleManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BattleManager>"));

	registration::class_<ObjPtr<BattleManager>>("ObjPtr<BattleManager>")
		.constructor([]() { return Object::NewObject<BattleManager>(); })
		.method("Inject", &ObjPtr<BattleManager>::Inject);

	registration::class_<SnowballManager>("SnowballManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowballManager>"))
		.property("m_Player", &SnowballManager::m_Player);

	registration::class_<ObjPtr<SnowballManager>>("ObjPtr<SnowballManager>")
		.constructor([]() { return Object::NewObject<SnowballManager>(); })
		.method("Inject", &ObjPtr<SnowballManager>::Inject);

	registration::class_<Player>("Player")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Player>"))
		.property("level", &Player::level)
		.property("HP", &Player::HP)
		.property("maxHP", &Player::maxHP)
		.property("battledist", &Player::battledist)
		.property("atk", &Player::atk);

	registration::class_<ObjPtr<Player>>("ObjPtr<Player>")
		.constructor([]() { return Object::NewObject<Player>(); })
		.method("Inject", &ObjPtr<Player>::Inject);

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

	registration::class_<PlayerMove>("PlayerMove")
		(rttr::metadata("wrapper_type_name", "ObjPtr<PlayerMove>"))
		.property("basespeed", &PlayerMove::basespeed)
		.property("isSlow", &PlayerMove::isSlow)
		.property("turnSpeed", &PlayerMove::turnSpeed)
		.property("is_Scoop", &PlayerMove::is_Scoop);

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
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowCollider>"));

	registration::class_<ObjPtr<SnowCollider>>("ObjPtr<SnowCollider>")
		.constructor([]() { return Object::NewObject<SnowCollider>(); })
		.method("Inject", &ObjPtr<SnowCollider>::Inject);

	registration::class_<SnowTrigger>("SnowTrigger")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowTrigger>"));

	registration::class_<ObjPtr<SnowTrigger>>("ObjPtr<SnowTrigger>")
		.constructor([]() { return Object::NewObject<SnowTrigger>(); })
		.method("Inject", &ObjPtr<SnowTrigger>::Inject);

	registration::class_<TileMap>("TileMap")
		(rttr::metadata("wrapper_type_name", "ObjPtr<TileMap>"))
		.property("trans", &TileMap::trans)
		.property("threshold", &TileMap::threshold)
		.property("box", &TileMap::box);

	registration::class_<ObjPtr<TileMap>>("ObjPtr<TileMap>")
		.constructor([]() { return Object::NewObject<TileMap>(); })
		.method("Inject", &ObjPtr<TileMap>::Inject);

}
