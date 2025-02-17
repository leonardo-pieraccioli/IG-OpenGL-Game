#include "Ship.h"
#include "MiniEngine/Game.h"

#define SHIP_INITIAL_HEALTH 50
static float actualMovementRate;
static Model shipModel;

Ship::Ship(int nProjectiles)
{
	actualMovementRate = movementRate;
	this->nProjectiles = nProjectiles;
    this->tag = "Ship";
    this->health.UpgradeMax(SHIP_INITIAL_HEALTH);
}

void Ship::Shoot()
{
	// must be a number divisible by 2 and which division by 2 is divisible by 3 (180,120,90,60,30)
    float baseAngle = 120.f / (nProjectiles + 1);
    float currentAngle;
    for (int i = 1; i <= nProjectiles; i++) {
        currentAngle = -60.f + baseAngle * i;
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
}

void Ship::Die()
{
    int random = rand() % 7 + 1;
    std::string path = "Assets/Sounds/explosion/explosion" + to_string(random) + ".mp3";
    SoundManager::Instance().playSound(path.c_str(), false);
    this->isActive = false;
}

float Ship::getActualMovementRate()
{
    return actualMovementRate;
}

void Ship::setActualMovementRate(float movementRate)
{
	actualMovementRate = movementRate;
}



float Ship::getShipMovementRate()
{
    return movementRate;
}

void Ship::setShipMovementRate(float movementRate)
{
	this->movementRate = movementRate;
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

void Ship::upgrade(UpgradeIndex upgradeIndex)
{
    int intUpgradeIndex = static_cast<int>(upgradeIndex);

    switch (intUpgradeIndex) {
        case 2:
            nProjectiles = static_cast<int>(UpgradeManager::Instance().getGenericCurrentValue(upgradeIndex));
            break;
        case 3:
            health.UpgradeMax(static_cast<int>(UpgradeManager::Instance().getGenericCurrentValue(upgradeIndex)));
            break;
        case 4:
            damage = UpgradeManager::Instance().getGenericCurrentValue(upgradeIndex);
            break;
        default:
            cout << "Errore, upgradeIndex fuori dal range accettabile dalla classe Ship" << endl;
    }
}

void Ship::resetUpgrades()
{
    nProjectiles = UpgradeManager::Instance().getInitialValue(UpgradeIndex::BulletsNumber);
    health.UpgradeMax(UpgradeManager::Instance().getInitialValue(UpgradeIndex::MaxShipsHealth));
    damage = UpgradeManager::Instance().getInitialValue(UpgradeIndex::Damage);
}

static bool loaded = false;
void Ship::setShipModel()
{
	if (!loaded)
	{
		loaded = true;
		shipModel = Model("Assets/Models/spaceship.obj");
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
    shipModel.Draw(shader);
}
