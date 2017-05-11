#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleSceneGame.h"
//#include "ModuleParticles.h"
#include <math.h>
#include "ModuleSound.h"
#include "ModuleCollision.h"
#include "SDL/include/SDL_timer.h"
#include "ModuleObjects.h"


ModulePlayer::ModulePlayer()
{
	lives = 3;
	grenades = 6;
	graphics = NULL;
	current_animation = NULL;

	// walk forward animation (arcade sprite sheet)
	
	forward.PushBack({0, 0, 13, 23});
	forward.PushBack({14, 0, 13, 22});
	forward.PushBack({ 0, 0, 13, 23 });
	forward.PushBack({28, 0, 13, 22});
	forward.loop = true;
	forward.speed = 0.15f;

	//walk diagonal down-left

	down_left.PushBack({105,24,15,22});
	down_left.PushBack({120,24,15,22});
	down_left.PushBack({105,24,15,22});
	down_left.PushBack({137,24,15,22});
	down_left.loop = true;
	down_left.speed = 0.15f;

	//walk diagonal down-right

	down_right.PushBack({88,23,15,22});
	down_right.PushBack({56,23,15,22});
	down_right.PushBack({88,23,15,22});
	down_right.PushBack({73,23,15,22});
	down_right.loop = true;
	down_right.speed = 0.15f;

	//walk diagonal up-right

	up_right.PushBack({0,47,15,22});
	up_right.PushBack({ 38,47,18,21 }, { 2,0 });
	up_right.PushBack({0,47,15,22 });
	up_right.PushBack({16,47,21,21}, { 7,0 });
	up_right.loop = true;
	up_right.speed = 0.15f;

	//walk diagonal down-left

	up_left.PushBack({97,47,15,22});
	up_left.PushBack({56,47,18,21});
	up_left.PushBack({ 97,47,15,22 });
	up_left.PushBack({75,47,21,21});
	up_left.loop = true;
	up_left.speed = 0.15f;

	//walk right animation

	
	right.PushBack({ 42,0,19,22 });
	right.PushBack({ 62,0,25,21 }, { 4,0 });
	right.PushBack({ 42,0,19,22 });
	right.PushBack({ 88,0,22,21 }, { 2,0 });
	right.PushBack({ 88,0,22,21 });
	right.loop = true;
	right.speed = 0.15f;
	

	//walk left annimation

	
	left.PushBack({ 160,0,19,22 });
	left.PushBack({ 134,0,25,21 });
	left.PushBack({ 160,0,19,22 });
	left.PushBack({ 111,0,19,22 });
	left.loop = true;
	left.speed = 0.15f;
	
	//walk backward animation

	backward.PushBack({ 28,24,13,22 });
	backward.PushBack({ 0,24,13,22 });
	backward.PushBack({ 28,24,13,22 });
	backward.PushBack({ 13,24,13,22 });
	backward.loop = true;
	backward.speed = 0.15f;
	
	speed = 1;
	shooting = false;

	//death animation
	death.PushBack({170,104, 27, 23});
	death.PushBack({202, 105, 25, 22});
	death.PushBack({235, 104, 15,22});
	death.loop = false;
	death.speed = 0.15f;

	//leaving helicoppter animationn
	leave_heli.PushBack({ 917, 874, 7, 15 });
	leave_heli.PushBack({ 932, 877, 16, 20 });
	leave_heli.PushBack({ 949, 877, 16, 21 });
	leave_heli.PushBack({ 968, 876, 16, 22 });
	//leave_heli.PushBack({ 986, 876, 17, 22 });
	//leave_heli.PushBack({ 1007, 877, 23, 21 });
	
	leave_heli.loop = false;
	leave_heli.speed = 0.10F;

	bye_anim.PushBack({ 1035, 873, 16, 27 });
	bye_anim.PushBack({ 1052, 874, 16, 26 });
	bye_anim.PushBack({ 1035, 873, 16, 27 });
	bye_anim.PushBack({ 1052, 874, 16, 26 });


	//drown animation
	drown.PushBack({ 270, 40, 17, 23 });
	drown.PushBack({ 296, 40, 25, 14 });
	drown.PushBack({ 334, 40, 15, 12 });
	drown.PushBack({ 357, 40, 14, 12 });
	drown.PushBack({ 377, 40, 15, 13 });
	drown.loop = false;
	drown.speed = 0.10f;

	//throw grenade animation
	throw_grenade.PushBack({ 87,70,17,22 });
	throw_grenade.PushBack({ 109,71,23,21 });
	throw_grenade.PushBack({ 136,68,20,24 });
	throw_grenade.loop = true;
	throw_grenade.speed = 3.0f;

	//Bridge animamtion
	bridge.PushBack({ 691, 79, 64, 32 });

	//Other part of the bridge (part below)
	bridge2.PushBack({ 1066, 0, 66, 28 });
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	position.x = (SCREEN_WIDTH / 2) + 20;
	position.y = (float)((ModuleSceneGame*)App->current_scene)->getLevelDimensions().y + 110;
	shooting_angle = { 0.0f, 0.0f };
	direction = 0;
	shooting_position = { 9,1 };
	player_min_y = (int)position.y;
	prev_position = position;
	state = IDLE;
	fire = App->particles->fire_up;
	collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 13, 23 }, COLLIDER_PLAYER, this);
	LOG("Loading player textures");
	graphics = App->textures->Load("Images/sprites.png"); 
	godmode = App->textures->Load("Images/godmode.png");
	current_animation= &forward;

	shoot = App->sound->LoadSound("SoundFX/Commando (shoot)_03.wav");
	grenade_explosion = App->sound->LoadSound("SoundFX/Commando (grenade)_02.wav");

	return true;
}

bool ModulePlayer::CleanUp() {
	LOG("Unloading player");
	App->textures->Unload(graphics);
	App->textures->Unload(godmode);
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (state != DEAD && !((ModuleSceneGame*)App->current_scene)->restart)
	{
		prev_position = position;
		current_animation->speed = 0.15f;
		shooting = false;
		grenade1 = false;

		if (((ModuleSceneGame*)App->current_scene)->intro == true)
		{
			if (intro_state == 0)
			{
				current_animation->speed = 0.05f;
				current_animation = &leave_heli;
				position.x = position.x + 0.5f;;
				position.y = position.y - parabol;
				parabol = parabol - 0.015f ;
				if (current_animation->Finished() == true)
				{
					intro_state = 1;
				}
			}
			else if (intro_state == 1)
			{
				current_animation = &bye_anim;
				if (current_animation->Finished() == true)
				{
					intro_state = 2;
					init_pos = position;
				}
			}
			else if (intro_state == 2)
			{
				position.y = position.y + 0.5f;
				current_animation = &backward;
				if (init_pos.y + 20 <= position.y )
				{
					intro_state = 3;
				}
			}
			else if (intro_state == 3)
			{
				if (position.x >= (SCREEN_WIDTH / 2) + 15)
				{
					position.x = position.x - 0.55f;
					position.y = position.y + 0.6f;

				}
				else
				{
					intro_state = 4;
					current_animation = &forward;
					current_animation->speed = 0;
					App->scene_game->intro = false;
					App->objects->droping = false;

				}
			}
		}
		else 
		{
			if (state != IDLE)
				prev_state = state;
			state = IDLE;
			checkInput();
			processInput();
		}

		rotateShootingAngle();

		if (shooting)
		{
			App->sound->PlaySound(shoot, 0);
			Particle bullet = App->particles->bullet;
			bullet.speed = { PLAYER_BULLET_SPEED * shooting_angle.x, PLAYER_BULLET_SPEED * shooting_angle.y };
			bullet.life = 300;
			//App->particles->AddParticle(fire, (int)position.x + shooting_position.x, (int)position.y + shooting_position.y, EXPLOSION, COLLIDER_NONE);
			App->particles->AddParticle(bullet, (int)position.x + shooting_position.x, (int)position.y + shooting_position.y, BULLET, COLLIDER_PLAYER_SHOT);
			App->particles->AddParticle(bullet, (int)position.x + shooting_position.x, (int)position.y + shooting_position.y, BULLET, COLLIDER_PLAYER_SHOT, nullptr, 50);
		}

		if (grenade_on == false)
		{
			if (grenade1)
			{
				Particle grenade;
				grenade_on = true;
				bthrowing = true;
				grenade = App->particles->grenade;
				grenade.speed = { 0, - grenade_speed };
				App->particles->AddParticle(grenade, (int)position.x + 7, (int)position.y + 1, GRENADE_PLAYER, COLLIDER_NONE, grenade_explosion);
			}
		}
		else if  (bthrowing == true)
		{
			current_animation = &throw_grenade;
			if (current_animation->Finished() == true)
			{
				bthrowing = false;
				current_animation->Reset();
				state = MOVING_RIGHT;
			}
		}
	}
	else if (current_animation->Finished() && !((ModuleSceneGame*)App->current_scene)->restart) {
		((ModuleSceneGame*)App->current_scene)->restart = true;
		state = IDLE;
	}


	// Draw everything --------------------------------------
	AnimationFrame frame = current_animation->GetCurrentFrame();
	SDL_Rect* camera = &App->render->camera;

	if ((int)position.x - frame.pivot.x < 20)
		position.x = (float)frame.pivot.x + 20.0f;
	else if ((int)position.x - frame.pivot.x + frame.rect.w > SCREEN_WIDTH + 20)
		position.x = SCREEN_WIDTH + 20.0f - (float)frame.rect.w + (float)frame.pivot.x;
	if ((int)position.y - frame.pivot.y < 0)
		position.y = (float)frame.pivot.y;
	else if ((int)position.y - frame.pivot.y + frame.rect.h > SCREEN_HEIGHT + (-camera->y / SCREEN_SIZE))
		position.y = SCREEN_HEIGHT + (-camera->y / SCREEN_SIZE) - (float)frame.rect.h + (float)frame.pivot.y;

	collider->rect = { (int)position.x - frame.pivot.x, (int)position.y - frame.pivot.y, frame.rect.w, frame.rect.h };

	int margin = 110; //Must be equal to the player's initial position
	if (camera->y < 0 && !((ModuleSceneGame*)App->current_scene)->intro) {
		player_min_y = MIN(player_min_y, (int)position.y);
		camera->y =(-player_min_y + margin) * SCREEN_SIZE;
		int level_y = (((ModuleSceneGame*)App->current_scene)->getLevelDimensions().y) * SCREEN_SIZE;
		if (camera->y < -level_y)
			camera->y = -level_y;
		
	}

	LOG("Player position.y:: %f", position.y);
	LOG("Player position.x:: %f", position.x);
	LOG("Camera position.y:: %d", camera->y);

	if(b_godmode == false)
		App->render->Blit(graphics, ((int)position.x - frame.pivot.x), ((int)position.y - frame.pivot.y), &frame.rect);
	else 
		App->render->Blit(godmode, ((int)position.x - frame.pivot.x), ((int)position.y - frame.pivot.y), &frame.rect);

	//Bridge sprite
	if (App->level == 1)
	{
		App->render->Blit(graphics, 96, 802, &bridge.GetCurrentFrame().rect);
		App->render->Blit(graphics, 96, 834, &bridge2.GetCurrentFrame().rect);
	}
	else if (App->level == 3) 
	{
		App->render->Blit(graphics, 96, 942, &bridge.GetCurrentFrame().rect);
		App->render->Blit(graphics, 96, 975, &bridge2.GetCurrentFrame().rect);
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::checkInput() {
	if (App->input->controller_connected) {
		GamePad p = App->input->controller_1;
		if (p.left_axis.x - 0.25f > 0) //0.15f is the controller threshold
			state = MOVING_RIGHT | state;
		if (p.left_axis.x + 0.25f < 0)
			state = MOVING_LEFT | state;
		if (p.left_axis.y - 0.25f > 0)
			state = MOVING_DOWN | state;
		if (p.left_axis.y + 0.25f < 0)
			state = MOVING_UP | state;
		if (p.left_trigger && grenades > 0 && current_animation != &throw_grenade) {
			grenade1 = true;
			if (grenade_on == false) {
				grenades--;
			}
		}
		if (p.right_trigger)
			shooting = true;
		if (p.left_bumper)
			speed = 10;
		else speed = 1;
		if (abs(p.left_axis.x) > 0.25f || abs(p.left_axis.y) > 0.25f)
			shooting_angle_delta = { (p.left_axis.x / abs(p.left_axis.x)) * 0.15f, (p.left_axis.y / abs(p.left_axis.y)) * 0.15f };
	}
	else {
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
		{
			state = MOVING_RIGHT | state;
			shooting_angle_delta.x = 0.15f;
		}

		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
		{
			state = MOVING_LEFT | state;
			shooting_angle_delta.x = -0.15f;
		}

		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			state = MOVING_UP | state;
			shooting_angle_delta.y = -0.15f;
		}

		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
		{
			state = MOVING_DOWN | state;
			shooting_angle_delta.y = 0.15f;
		}

		if (App->input->keyboard[SDL_SCANCODE_Z] == KEY_STATE::KEY_DOWN)
		{
			shooting = true;
		}
		if (App->input->keyboard[SDL_SCANCODE_X] == KEY_STATE::KEY_DOWN && grenades > 0 && current_animation != &throw_grenade)
		{
			grenade1 = true;
			if (grenade_on == false) {
				grenades--;
			}
		}
		if (App->input->keyboard[SDL_SCANCODE_G] == KEY_STATE::KEY_DOWN)
		{
			b_godmode = !b_godmode;

			if (collider->type != COLLIDER_NONE)
				collider->type = COLLIDER_NONE;

			else
				collider->type = COLLIDER_PLAYER;
		}
		if (App->input->keyboard[SDL_SCANCODE_L] == KEY_STATE::KEY_DOWN)
		{
			/*lives = 1;
			enemyCollision();*/
		}

		if (App->input->keyboard[SDL_SCANCODE_LSHIFT] == KEY_STATE::KEY_REPEAT)
			speed = 10;
		else speed = 1;
	}
}

void ModulePlayer::processInput() {

	switch (state) {
	case MOVING_DOWN:
		shooting_position = { 2,16 };
		direction = 180;
		current_animation = &backward;
		fire = App->particles->fire_down;
		grenade_speed = 1;
		shooting_angle.y = fmaxf(shooting_angle.y, 0.0f);
		shooting_angle_delta.x = shooting_angle.x > 0 ? -0.15f : 0.15f;
		break;
	case MOVING_UP:
		shooting_position = { 10,2 };
		direction = 0;
		current_animation = &forward;
		fire = App->particles->fire_up;
		grenade_speed = 1 + speed * cosf(direction * (M_PI / 180.0f));
		shooting_angle.y = fminf(shooting_angle.y, 0.0f);
		shooting_angle_delta.x = shooting_angle.x > 0 ? -0.15f : 0.15f;
		break;
	case MOVING_RIGHT:
		shooting_position = { 20,8 };
		direction = 90;
		current_animation = &right;
		fire = App->particles->fire_right;
		grenade_speed = 1;
		shooting_angle.x = fmaxf(shooting_angle.x, 0.0f);
		shooting_angle_delta.y = shooting_angle.y > 0 ? -0.15f : 0.15f;
		break;
	case MOVING_LEFT:
		shooting_position = { 0,8 };
		direction = 270;
		current_animation = &left;
		fire = App->particles->fire_left;
		grenade_speed = 1;
		shooting_angle.x = fminf(shooting_angle.x, 0.0f);
		shooting_angle_delta.y = shooting_angle.y > 0 ? -0.15f : 0.15f;
		break;
	case MOVING_DOWN_RIGHT:
		shooting_position = { 15,17 };
		direction = 135;
		current_animation = &down_right;
		fire = App->particles->fire_downright;
		grenade_speed = 1 + speed * cosf(direction * (M_PI / 180.0f));
		break;
	case MOVING_DOWN_LEFT:
		shooting_position = { 2,15 };
		direction = 225;
		current_animation = &down_left;
		fire = App->particles->fire_downleft;
		grenade_speed = 1 + speed * cosf(direction * (M_PI / 180.0f));
		break;
	case MOVING_UP_LEFT:
		shooting_position = { 0,4 };
		direction = 315;
		current_animation = &up_left;
		fire = App->particles->fire_upleft;
		grenade_speed = 0.3 + speed * cosf(direction * (M_PI / 180.0f));
		break;
	case MOVING_UP_RIGHT:
		shooting_position = { 15,5 };
		direction = 45;
		current_animation = &up_right;
		fire = App->particles->fire_upright;
		grenade_speed = 0.3 + speed * cosf(direction * (M_PI / 180.0f));
		break;
	case IDLE:
		grenade_speed = 1;
		if (!bthrowing) {
			current_animation->speed = 0.0f;
		}
		switch (prev_state)
		{
		case MOVING_UP:
			shooting_angle.y = fminf(shooting_angle.y, 0.0f);
			shooting_angle_delta.x = shooting_angle.x > 0 ? -0.15f : 0.15f;
			break;
		case MOVING_DOWN:
			shooting_angle.y = fmaxf(shooting_angle.y, 0.0f);
			shooting_angle_delta.x = shooting_angle.x > 0 ? -0.15f : 0.15f;
			break;
		case MOVING_RIGHT:
			shooting_angle.x = fmaxf(shooting_angle.x, 0.0f);
			shooting_angle_delta.y = shooting_angle.y > 0 ? -0.15f : 0.15f;
			break;
		case MOVING_LEFT:
			shooting_angle.x = fminf(shooting_angle.x, 0.0f);
			shooting_angle_delta.y = shooting_angle.y > 0 ? -0.15f : 0.15f;
			break;
		default:
			break;
		}
		break;
	}

	/*shooting_angle.x = fmaxf(-1.0f, shooting_angle.x);
	shooting_angle.x = fminf(1.0f, shooting_angle.x);
	shooting_angle.y = fmaxf(-1.0f, shooting_angle.y);
	shooting_angle.y = fminf(1.0f, shooting_angle.y);*/

	if (state != IDLE) {
		position.x += speed * sinf(direction * (M_PI / 180.0f));
		position.y -= speed * cosf(direction * (M_PI / 180.0f));
	}
}

void ModulePlayer::rotateShootingAngle() {
	shooting_angle += shooting_angle_delta;
	float module = sqrtf(shooting_angle.x * shooting_angle.x + shooting_angle.y * shooting_angle.y);
	shooting_angle.x /= module;
	shooting_angle.y /= module;
}

void ModulePlayer::OnCollision(Collider* self, Collider* other) {
	switch (other->type) {
	case COLLIDER_WALL:
		wallCollision();
		break;
	case COLLIDER_WATER:
		waterCollision();
		break;
	case COLLIDER_ENEMY:
	case COLLIDER_ENEMY_SHOT:
		enemyCollision();
		break;
	}
}

void ModulePlayer::wallCollision() {
	position = prev_position;
}

void ModulePlayer::waterCollision() {
	Drown();
}

void ModulePlayer::enemyCollision() {
	if (state != DEAD) {
		state = DEAD;
		lives--;
		if (lives <= 0) {
			((ModuleSceneGame*)App->current_scene)->next = (Module*)App->scene_congrats;
		}
		else ((ModuleSceneGame*)App->current_scene)->next = App->current_scene;

		collider->active = false;
		current_animation = &death;
		//if (current_animation->Finished())
			current_animation->Reset();
	}
}

void ModulePlayer::Drown() {
	if (state != DEAD) {
		state = DEAD;
		lives--;
		if (lives <= 0) {
			((ModuleSceneGame*)App->current_scene)->next = (Module*)App->scene_congrats;
		}
		else ((ModuleSceneGame*)App->current_scene)->next = App->current_scene;
		collider->active = false;
		current_animation = &drown;
		//if (current_animation->Finished())
			current_animation->Reset();
	}
}

PLAYER_STATE operator |(PLAYER_STATE p, PLAYER_STATE s) {
	PLAYER_STATE ret = static_cast<PLAYER_STATE>((int)p | (int)s);
	return ret;
}

iPoint toiPoint(fPoint a) {
	return{ (int)a.x, (int)a.y };
}

fPoint tofPoint(iPoint a) {
	return{ (float)a.x, (float)a.y };
}