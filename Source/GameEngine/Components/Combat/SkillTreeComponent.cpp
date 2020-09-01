#include "SkillTreeComponent.h"
#include <GameEngine\ImGUI\imgui.h>
#include <GameEngine\Components\Combat\CombatComponent.h>

SkillTreeComponent::SkillTreeComponent() {
	points = 0;
	extraDmg10 = false;
	extraDmg20_1 = false;
	extraDmg20_2 = false;
	doT = false;
	shootDir2 = false;
	shootDir3 = false;
	shoot2 = false;
	opened = false;
	extraDmg10Added = false;
	extraDmg20_1Added = false;
	extraDmg20_2Added = false;
}

SkillTreeComponent::~SkillTreeComponent() {

}

void SkillTreeComponent::AddSkill(int type) {
	switch (type)
	{
	case 0:
		if (points >= 5) {
			extraDmg10 = true;
			points -= 5;
		}
		else {
			printf("Not enough points!\n");
		}
		break;
	case 1:
		if (points >= 10) {
			extraDmg20_1 = true;
			points -= 5;
		}
		else {
			printf("Not enough points!\n");
		}
		break;
	case 2:
		if (points >= 15) {
			doT = true;
			points -= 5;
		}
		else {
			printf("Not enough points!\n");
		}
		break;
	case 3:
		if (points >= 25) {
			extraDmg20_2 = true;
			points -= 5;
		}
		else {
			printf("Not enough points!\n");
		}
		break;
	case 4:
		if (points >= 10) {
			shootDir2 = true;
			points -= 5;
		}
		else {
			printf("Not enough points!\n");
		}
		break;
	case 5:
		if (points >= 15) {
			shootDir3 = true;
			points -= 5;
		}
		else {
			printf("Not enough points!\n");
		}
		break;
	case 6:
		if (points >= 25) {
			shoot2 = true;
			points -= 5;
		}
		else {
			printf("Not enough points!\n");
		}
		break;
	default:
		break;
	}
}

void SkillTreeComponent::update(float dt) {

	bool nextPerk = true;
	points = 50000;
	int localPoints = points;
	if (!opened) {
		return;
	}
	ImGui::SetNextWindowPos(ImVec2(480, 270));
	ImGui::SetNextWindowSize(ImVec2(444, 330));
	ImGui::Begin("Skill Tree");
	if (ImGui::TreeNode("Skill Tree")) {
		if (ImGui::TreeNode("Single target perks")) {
			ImGui::Checkbox("Perk 1 - Extra Damage", &extraDmg20_1);

			if (!extraDmg20_1 || localPoints < 1000) {
				nextPerk = false;
				extraDmg20_1 = false;
				extraDmg20_1Added = false;
				static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->ResetDamage();
			}
			else {
				localPoints -= 1000;
				if (!extraDmg20_1Added) {
					extraDmg20_1Added = true;
					static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->AddDamage(
						static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetDamage() / 2);
				}
			}

			if (ImGui::IsItemHovered()) {
				ImGui::Text("50%% damage increase. Costs 1000SP");
			}

			ImGui::Checkbox("Perk 2 - Damage over Time", &doT);

			if (!doT || !nextPerk || localPoints < 100) {
				nextPerk = false;
				doT = false;
			}
			else {
				localPoints -= 100;
			}

			if (ImGui::IsItemHovered()) {
				ImGui::Text("Add damage over time to arrows. Costs 100SP");
			}

			ImGui::Checkbox("Perk 3 - Extra Damage", &extraDmg20_2);

			if (!extraDmg20_2 || !nextPerk || localPoints < 10) {
				nextPerk = false;
				extraDmg20_2 = false;
				extraDmg20_2Added = false;
				static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->ResetDamage();
				if (extraDmg20_1Added) {
					static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->AddDamage(
						static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetDamage()/2);
				}
			}
			else {
				localPoints -= 10;
				if (!extraDmg20_2Added) {
					extraDmg20_2Added = true;
					static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->AddDamage(
						static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetDamage());
				}
			}

			if (ImGui::IsItemHovered()) {
				ImGui::Text("100%% damage increase. Costs 10SP");
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Area of effect perks")) {
			ImGui::Checkbox("Perk 1 - 2 Directions", &shootDir2);

			if (!shootDir2 || localPoints < 1000) {
				nextPerk = false;
				shootDir2 = false;
			}
			else {
				localPoints -= 1000;
			}

			if (ImGui::IsItemHovered()) {
				ImGui::Text("Shoot arrows in 2 directions. Costs 1000SP");
			}

			ImGui::Checkbox("Perk 2 - 3 Directions", &shootDir3);

			if (!shootDir3 || !nextPerk || localPoints < 100) {
				nextPerk = false;
				shootDir3 = false;
			}
			else {
				localPoints -= 100;
			}

			if (ImGui::IsItemHovered()) {
				ImGui::Text("Shoot arrows in 3 directions. Costs 100SP");
			}

			ImGui::Checkbox("Perk 3 - Consecutive Arrows", &shoot2);

			if (!shoot2 || !nextPerk || localPoints < 10) {
				nextPerk = false;
				shoot2 = false;
			}
			else {
				localPoints -= 10;
			}

			if (ImGui::IsItemHovered()) {
				ImGui::Text("Shoot 2 consecutive arrows. Costs 10SP");
			}

			ImGui::TreePop();
		}
		ImGui::Text("Total skill points: %d", localPoints);
		ImGui::Text("Total damage: %d", static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetDamage());
		ImGui::TreePop();
	}
	ImGui::End();

}