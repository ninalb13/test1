#pragma once
#include "Enemy.h"
#include "Animation.h"
#include "Path.h"

class EnemyMoto :
	public Enemy
{
private:
	Path movement;
	SDL_Texture* graphics = nullptr;

	Animation riding;
	Animation throwing_grenade;

public:
	
	EnemyMoto(int x, int y, int angle, int sub_type, int isglobal);
	void Move();
	virtual ~EnemyMoto();
};

