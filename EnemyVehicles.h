#pragma once
#include "Enemy.h"
#include "Animation.h"
#include "Path.h"
class EnemyVehicles :
	public Enemy
{

private:
		Path movement;

		Animation truck_riding;
		Animation moto_rtol;
		Animation moto_ltor;
		Animation vertical_truck;
		Animation car_enemies_ltor;
		Animation car_enemies_rtol;


	public:
		EnemyVehicles(int x, int y, int angle, int sub_type);
		void Move();
		virtual ~EnemyVehicles();

	};



