#pragma once
#include <GameEngine/Components/BaseComponents/BaseComponent.h>

class SkillTreeComponent : public BaseComponent
{
public:
	GAME_COMPONENT(SkillTreeComponent)

	SkillTreeComponent();
	~SkillTreeComponent();

	void update(float dt);

	void AddPoints(int p) { this->points += p; }
	int GetPoints() { return this->points; }

	void SetSkillTreeView(bool opened) { this->opened = opened; }
	bool GetSkillTreeView() { return this->opened; }

	bool GetExtraDamage10() { return this->extraDmg10; }
	bool GetExtraDamage20_1() { return this->extraDmg20_1; }
	bool GetDoT() { return this->doT; }
	bool GetExtraDamage20_2() { return this->extraDmg20_2; }
	bool GetDirectionalShoot2() { return this->shootDir2; }
	bool GetDirectionalShoot3() { return this->shootDir3; }
	bool GetExtraArrow() { return this->shoot2; }

	void AddSkill(int type);

private:

	int points;
	bool extraDmg10;	// 0
	bool extraDmg20_1;	// 1
	bool doT;			// 2
	bool extraDmg20_2;	// 3
	bool shootDir2;		// 4
	bool shootDir3;		// 5
	bool shoot2;		// 6
	bool extraDmg10Added;
	bool extraDmg20_1Added;
	bool extraDmg20_2Added;

	bool opened;
};