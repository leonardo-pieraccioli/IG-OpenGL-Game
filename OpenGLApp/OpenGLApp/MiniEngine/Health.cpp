#include "Health.h"

Health::Health(int startingHealth)
{
	max = current = startingHealth;
}

/// <summary>
/// Damage the health by the amount specified
/// </summary>
/// <param name="amount"> The amount of damage to be dealt </param>
/// <returns> The current health after the damage </returns>
int Health::Damage(int amount)
{
	current = current - amount <= 0 ? 0 : current - amount;
	return current;
}

/// <summary>
/// Heal the health by the amount specified
/// </summary>
/// <param name="amount"> The amount of health to be healed </param>
/// <returns> The current health after the healing </returns>
int Health::Heal(int amount)
{
	current = current + amount >= max ? max : current + amount;
	return current;
}

/// <summary>
/// Upgrade the max health to the new value. If the new value is less than the current health, the current health is set to the new max
/// </summary>
/// <param name="newMax"> The new max health value </param>
/// <returns> The current health after the upgrade </returns>
int Health::UpgradeMax(int newMax)
{
	if (current == max)
	{
		current = newMax;
	}
	else if (newMax < current)
	{
		current = newMax;
	}
	max = newMax;
	return newMax = current;
}

/// <summary>
/// Check if the health is zero, meaning the object is dead
/// </summary>
/// <returns> True if the object is dead, false otherwise </returns>
bool Health::isDead()
{
	return current <= 0;
}

/// <summary>
/// Get the current health value
/// </summary>
int Health::healthStatus()
{
	return current;
}

void Health::resetHealth()
{
	current = max;
}
