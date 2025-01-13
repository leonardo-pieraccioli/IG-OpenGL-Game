#pragma once
class Health
{
public:
	Health(int startingHealth);
	~Health() = default;

	int Damage(int amount);
	int Heal(int amount);

	int UpgradeMax(int newMax);

	bool isDead();
	int healthStatus();

private:
	int max;
	int current;

};
