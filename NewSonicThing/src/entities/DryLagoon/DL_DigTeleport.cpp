#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "dldigteleport.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../entities/controllableplayer.hpp"
#include "../../toolbox/maths.hpp"
#include "../camera.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../../particles/particle.hpp"
#include "../../particles/particleresources.hpp"
#include "../../particles/particlemaster.hpp"

#include <list>

std::vector<TexturedModel*> DL_DigTeleport::models;
CollisionModel* DL_DigTeleport::cmOriginal = nullptr;

DL_DigTeleport::DL_DigTeleport(
    float x, float y, float z, 
    float yRot,
    float xTarget, float yTarget, float zTarget,
    float r, float g, float b)
{
    position.set(x, y, z);
    rotY = yRot;

    teleportLocation.set(xTarget, yTarget, zTarget);

    baseColor.set(r, g, b);

    updateTransformationMatrix();

    collideModelOriginal = DL_DigTeleport::cmOriginal;
    collideModelTransformed = DL_DigTeleport::cmOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void DL_DigTeleport::step()
{
    extern float dt;

    float currDiggTimer = Global::gameMainPlayer->getDiggingTimer();

    float prevTimeUntilWarp = timeUntilWarp;
    timeUntilWarp -= dt;

    if (collideModelTransformed->playerIsOn)
    {
        if (prevDiggTimer > 0.7f && currDiggTimer <= 0.7f)
        {
            timeUntilWarp = 0.7f;
            Vector3f vel(0, 0, 0);
            ParticleMaster::createParticle(ParticleResources::textureBlackFadeOutAndIn, &Global::gameCamera->fadePosition1, &vel, 0, 1.4f, 400, 0, true, false, 1.0f, false);
        }
    }

    if (doTeleport)
    {
        doTeleport = false;

        Vector3f newCamDir(1, 0, 0);
        newCamDir = Maths::rotatePoint(&newCamDir, &Y_AXIS, Maths::toRadians(rotY)); //todo: make new variabel for this
        newCamDir.normalize();
        Global::gameMainPlayer->position = teleportLocation;
        Global::gameMainPlayer->camDir = newCamDir;
        Global::gameMainPlayer->camDirSmooth = newCamDir;
        Global::gameMainPlayer->relativeUp.set(0, 1, 0);
        Global::gameMainPlayer->vel = newCamDir.scaleCopy(-10.0f);
    }

    if (prevTimeUntilWarp > 0.0f && timeUntilWarp <= 0.0f)
    {
        doTeleport = true;
        Global::gameMainPlayer->onGround = false;
    }

    prevDiggTimer = currDiggTimer;
}

std::vector<TexturedModel*>* DL_DigTeleport::getModels()
{
    return &DL_DigTeleport::models;
}

void DL_DigTeleport::loadStaticModels()
{
    if (DL_DigTeleport::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading DL_DigTeleport static models...\n");
    #endif

    ModelLoader::loadModel(&DL_DigTeleport::models, "res/Models/Levels/DryLagoon/Objects/", "DigTeleport");

    if (DL_DigTeleport::cmOriginal == nullptr)
    {
        DL_DigTeleport::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/DryLagoon/Objects/", "DigTeleport");
    }
}

void DL_DigTeleport::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DL_DigTeleport static models...\n");
    #endif

    Entity::deleteModels(&DL_DigTeleport::models);
    Entity::deleteCollisionModel(&DL_DigTeleport::cmOriginal);
}
