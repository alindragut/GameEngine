#pragma once
#include <GameEngine/Components/BaseComponents/BaseComponent.h>

class CombatComponent : public BaseComponent
{
public:
	GAME_COMPONENT(CombatComponent)

	CombatComponent();
	CombatComponent(int hp, int stamina, int ammo, int damage);
	~CombatComponent();

	int GetHP() { return this->hp; }
	int GetStamina() { return this->stamina; }
	int GetAmmo() { return this->ammo; }
	int AddAmmo(int bonusAmmo) { this->ammo += bonusAmmo; }
	int GetDamage() { return this->damage; }

	void DealDamage(int damage) { this->hp -= damage; }
	void RegenerateStamina() { if(this->stamina < this->maxStamina)this->stamina++; }
	void RegenerateHP() { if (this->hp < this->maxHp)this->hp++; }

	void update(float deltaTimeSeconds) override;

private:

	int maxStamina;
	int maxHp;
	int hp;
	int stamina;
	int ammo;
	int damage;
};