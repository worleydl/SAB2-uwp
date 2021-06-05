#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "ghfloatingplatform.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> GH_FloatingPlatform::models;
CollisionModel* GH_FloatingPlatform::cmOriginal = nullptr;

GH_FloatingPlatform::GH_FloatingPlatform()
{
	
}

GH_FloatingPlatform::GH_FloatingPlatform(float x, float y, float z, float rotY, float scale)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	this->rotY = rotY;
	rotZ = 0;
	this->scale = scale;
	visible = true;
	updateTransformationMatrix();

	collideModelOriginal = GH_FloatingPlatform::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void GH_FloatingPlatform::step()
{

}

std::list<TexturedModel*>* GH_FloatingPlatform::getModels()
{
	return &GH_FloatingPlatform::models;
}

void GH_FloatingPlatform::loadStaticModels()
{
	if (GH_FloatingPlatform::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading GH_FloatingPlatform static models...\n");
	#endif

	ObjLoader::loadModel(&GH_FloatingPlatform::models, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillFallingPlatform");

	if (GH_FloatingPlatform::cmOriginal == nullptr)
	{
		GH_FloatingPlatform::cmOriginal = ObjLoader::loadCollisionModel("Models/Levels/GreenHillZone/Objects/", "GreenHillFallingPlatform");
	}
}

void GH_FloatingPlatform::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting GH_FloatingPlatform static models...\n");
	#endif

	Entity::deleteModels(&GH_FloatingPlatform::models);
	Entity::deleteCollisionModel(&GH_FloatingPlatform::cmOriginal);
}
