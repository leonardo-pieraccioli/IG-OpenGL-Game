#include "Ship.h"
#include "MiniEngine/Game.h"

#define SHIP_INITIAL_HEALTH 100

Ship::Ship(int nProjectiles)
{
	this->nProjectiles = nProjectiles;
    this->tag = "Ship";
    this->health.UpgradeMax(SHIP_INITIAL_HEALTH);
}

void Ship::Shoot()
{
    float baseAngle = 180.f / (nProjectiles + 1);
    float currentAngle;
    for (int i = 1; i <= nProjectiles; i++) {
        currentAngle = -90.f + baseAngle * i;
        std::pair<float, float> pCoords = utilsF::calculateForwardXY(this->transform.rotation.z + currentAngle, 1.0f, this->transform.position.x, this->transform.position.y, 0.8f);
        Game::Instance().InstantiateGameObject(new Projectile(), new Transform(glm::vec3(pCoords.first, pCoords.second, 0.0f), glm::vec3(this->transform.rotation.x, this->transform.rotation.y, this->transform.rotation.z + currentAngle), glm::vec3(0.10f, 0.25f, 0.25f)));
    }
}

void Ship::Damage(int damage)
{
    if (health.Damage(damage) <= 0)
    {
        Die();
    }
	std::cout << "Ship " << GetID() << " health: " << health.healthStatus() << std::endl;
}

void Ship::Die()
{
    this->isActive = false;
}



float Ship::getShipMovementRate()
{
    return movementRate;
}

void Ship::updateTLerp(float deltaTime, int pitchRotVal)
{
    deltaTime = deltaTime / pitchRotationTime;
    switch (pitchRotVal) {
        case -1:
            tLerp = (tLerp - deltaTime < -1.0f) ? -1.0f : tLerp - deltaTime;
            break;
        case 0:
            tLerp = (tLerp < 0.0f) ? (tLerp + deltaTime > 0.0f ? 0.0f : tLerp + deltaTime) : (tLerp - deltaTime < 0.0f ? 0.0f : tLerp - deltaTime);
            break;
        case 1:
            tLerp = (tLerp + deltaTime > 1.0f) ? 1.0f : tLerp + deltaTime;
            break;
    }
}

void Ship::Update(float deltaTime)
{
    
}

void Ship::Draw(Shader shader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    
    model_mat = glm::translate(model_mat, transform.position);
    model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    model_mat = glm::rotate(model_mat, glm::radians(utilsF::lerp(0.0f, 45.0f, tLerp)), glm::vec3(1.0f, 0.0f, 0.0f));
    model_mat = glm::scale(model_mat, transform.getScale());
    shader.SetMatrix4("model", model_mat);
    objectModel.Draw(shader);
}
