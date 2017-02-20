#pragma once
#include <string>
#include "Bullets.h"
#include <vector>
#include <glm/glm.hpp>
class Gun
{
public:
	Gun(std::string name,int fireRate,int bulletPerShot, float spread,float bulletDamage,float bulletSpeed);
	~Gun();

	void update(bool isMouseDown,const glm::vec2& position, const glm::vec2& direction, std::vector<Bullets>& bullets);

private:
	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullets>& bullets);

	std::string _name;

	int _fireRate;
	
	int _bulletsPerShot;

	float _spread;

	float _bulletSpeed;

	int _bulletDamage;

	int _frameCounter;
};

