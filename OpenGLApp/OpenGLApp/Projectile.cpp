#include "Projectile.h"
#include "Enemy.h"
#include "MiniEngine/Game.h"

static Model projectileModel;
static bool loaded;

Projectile::Projectile(float damage, float speed, float destroyDistance)
{
	this->speed = speed;
	this->destroyDistance = destroyDistance;
	this->damage = damage;
	this->tag = "Projectile";
	if(!loaded)
	{ 
		projectileModel = Model("Assets/Models/laserbeam.obj");
		loaded = true;
	}
}

void Projectile::Update(float deltaTime)
{
	float a_x = this->transform.position.x;
	float a_y = this->transform.position.y;

  	if (utilsF::distance2DSquare(a_x, a_y, 0.0f, 0.0f) > destroyDistance) {
		Game::Instance().DestroyGameObject(this);
	}
	Move(utilsF::calculateForwardXY(this->transform.rotation.z, deltaTime, this->transform.position.x, this->transform.position.y, speed));

	GameObject* hit = Game::Instance().CheckCollision(*this, this->transform.position, this->transform.scale);
	if(hit != nullptr && !hit->CompareTag("Projectile"))
	{
		if (hit->CompareTag("Ship"))
		{
			Ship* shipHit = dynamic_cast<Ship*>(hit);
			shipHit->Damage(damage);
		}

		else if (hit->CompareTag("Planet"))
		{
			Planet* p = dynamic_cast<Planet*>(hit);
			p->Damage(damage);
		}

		else if (hit->CompareTag("Enemy"))
		{
			ShootingEntity* shootingEntityHit = dynamic_cast<ShootingEntity*>(hit);
			if (shootingEntityHit->health.Damage(damage) <= 0)
			{
				shootingEntityHit->Die();
			}
		}

		else
		{
			return;
		}

		Game::Instance().DestroyGameObject(this);
	}
}

void Projectile::Draw(Shader shader)
{
	glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model_mat = glm::translate(model_mat, transform.position);
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
	model_mat = glm::scale(model_mat, transform.getScale());
	shader.SetMatrix4("model", model_mat);
	projectileModel.Draw(shader);
}

void Projectile::Move(std::pair<float, float> newCoords)
{
	this->transform.position.x = newCoords.first;
	this->transform.position.y = newCoords.second;
}