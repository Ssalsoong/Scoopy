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
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowCollider>"))
		.property("m_Rolesmooth", &SnowCollider::m_Rolesmooth)
		.property("SnowManager", &SnowCollider::SnowManager)
		.property("m_holdDistance", &SnowCollider::m_holdDistance);

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
		.property("trans", &TileMap::trans)
		.property("threshold", &TileMap::threshold)
		.property("box", &TileMap::box);

	registration::class_<ObjPtr<TileMap>>("ObjPtr<TileMap>")
		.constructor([]() { return Object::NewObject<TileMap>(); })
		.method("Inject", &ObjPtr<TileMap>::Inject);

}
