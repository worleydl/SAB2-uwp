#ifndef ITEMBALLOON_H
#define ITEMBALLOON_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

class ItemBalloon : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    static constexpr float BOMB_RADIUS = 400.0f;

    int itemType = 0;

public:
    ItemBalloon();
    ItemBalloon(float x, float y, float z, int itemType);

    void step();

    void die();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;
};
#endif
