#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Human::Human() : _frames(0)
{
}


Human::~Human()
{
}
void Human::init(float speed, glm::vec2 pos) {

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-5.0f, 5.0f);

	_health = 20;

	_color.r = 200;
	_color.g = 0;
	_color.b = 200;
	_color.a = 255;

	_speed = speed;
	_position = pos;
	//get random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	//make sure direction isn't zero
	if (_direction.length() == 0)
		_direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies) {

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-5.0f, 5.0f);
	_position += _direction * _speed;

	//randomly change direction every 20 frame
	if (_frames == 20) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else {
		_frames++;
	}

	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}

}