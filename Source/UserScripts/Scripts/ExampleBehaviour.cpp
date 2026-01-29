#include "Export.h"
#include "ScriptBehaviour.h"
#include "ExampleBehaviour.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<ExampleBehaviour>("ExampleBehaviour")
        (rttr::metadata("wrapper_type_name", "ObjPtr<ExampleBehaviour>"))
        .property("IsCustomBool",&ExampleBehaviour::isCustomBool)
        .property("CustomFloat",&ExampleBehaviour::customFloat)
        .property("CustomInt",&ExampleBehaviour::customInt)
        .property("CustomString",&ExampleBehaviour::customString);

	registration::class_<ObjPtr<ExampleBehaviour>>("ObjPtr<ExampleBehaviour>")
		.constructor(
			[]() {
				return Object::NewObject<ExampleBehaviour>();
			})
        .method("Inject", &ObjPtr<ExampleBehaviour>::Inject);
}

void MMMEngine::ExampleBehaviour::Start()
{
}

void MMMEngine::ExampleBehaviour::Update()
{
}
