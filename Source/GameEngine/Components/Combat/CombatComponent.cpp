#include "CombatComponent.h"

CombatComponent::CombatComponent() {
	this->hp = 100;
	this->maxHp = 100;
	this->stamina = 100;
	this->maxStamina = 100;
	this->ammo = 20;
	this->damage = 10;
	this->baseDamage = 10;
}

CombatComponent::CombatComponent(int hp, int stamina, int ammo, int damage) {
	this->hp = hp;
	this->maxHp = hp;
	this->stamina = stamina;
	this->maxStamina = stamina;
	this->ammo = ammo;
	this->damage = damage;
	this->baseDamage = damage;
}

CombatComponent::~CombatComponent() {

}

void CombatComponent::update(float deltaTimeSeconds) {

}