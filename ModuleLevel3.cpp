﻿#include "ModuleLevel3.h"
#include "ModuleSceneGame.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneCongrats.h"
#include "ModuleSound.h"
#include "ModuleCollision.h"
#include "ModuleEnemies.h"
#include <string>
#include "ModuleObjects.h"
#include "ModuleFonts.h"
#include "SDL/include/SDL_timer.h"
#include "EnemyVehicles.h"
#include "ModuleLevelTransition.h"




ModuleLevel3::ModuleLevel3() : ModuleSceneGame()
{
	intro = true;
	next = this;
}


ModuleLevel3::~ModuleLevel3()
{
}


bool ModuleLevel3::Start() {
	screen_enemies = 0;
	bool ret = true;
	restart = false;
	global = false;
	global_on = true;
	App->level = 3;
	App->current_scene = this;
	intro = false;
	background_graphics = App->textures->Load("Images/Mapa3.png", &level_dimensions);
	//App->scene_game->setLevelDimensions(level_dimensions);
	App->render->camera.x = -20 * SCREEN_SIZE;
	App->render->camera.y = (-level_dimensions.y + SCREEN_HEIGHT) * SCREEN_SIZE;

	//ADD ENEMIES
	if (App->level == 3) 
	{
		App->enemies->AddEnemy(MOTO_TYPE, SCREEN_WIDTH, 940, 270, 1);
		App->enemies->AddEnemy(ENEMY_GRENADE, 10, 940, 0, 4);

		App->enemies->AddEnemy(ENEMY_GRENADE, SCREEN_WIDTH, 940, 0, 5);

		App->enemies->AddEnemy(MOTOLEVEL3, 0-46, 740, 270, 1);

		App->enemies->AddEnemy(MOTOLEVEL3, 0 - 46, 640, 270, 1);

		App->enemies->AddEnemy(MOTOLEVEL3, 0 -46, 540, 270, 1);

		App->enemies->AddEnemy(MOTOLEVEL3, 0 - 46, 519, 270, 1);

		App->enemies->AddEnemy(MOTOLEVEL3, SCREEN_WIDTH + 45 , 440, 270, 2);

		App->enemies->AddEnemy(MOTOLEVEL3, SCREEN_WIDTH + 64 , 770, 270, 3);

		App->enemies->AddEnemy(MOTOLEVEL3, SCREEN_WIDTH + 64 , 670, 270, 3);

		App->enemies->AddEnemy(MOTOLEVEL3, 0 - 47, 615, 270, 4);

		App->enemies->AddEnemy(MOTOLEVEL3, SCREEN_WIDTH + 47, 495, 270, 5);

		App->enemies->AddEnemy(MOTOLEVEL3, SCREEN_WIDTH + 47, 410, 270, 5);

		App->enemies->AddEnemy(MOTOLEVEL3, SCREEN_WIDTH/2, 700, 270, 6);

		//App->enemies->AddSpawner(LEFT_WEAPON, 0, 0, 0, 100, 1, true);

		//App->enemies->AddSpawner(ENEMY_GRENADE, 0, 0, 0, 100, 2, true);
		
		App->enemies->AddEnemy(COMMANDER, SCREEN_WIDTH / 2, 5, 0, 1);

		App->enemies->AddEnemy(ENEMY_MORTAR, 210, 60, 0, 2);

		App->enemies->AddEnemy(ENEMY_MORTAR, 30, 60, 0, 1);

		App->enemies->AddSpawner(LEFT_WEAPON, 0, 915, 0, 200, 3,0);
		App->enemies->AddSpawner(LEFT_WEAPON, SCREEN_WIDTH, 915, 0, 200, 4, 0);
		App->enemies->AddSpawner(ENEMY_GRENADE, SCREEN_WIDTH, 1455, 270, 300,1,0);
		App->enemies->AddSpawner(ENEMY_GRENADE, 0,1950, 90, 75,1,0, nullptr, 0 , 50);
		App->enemies->AddSpawner(ENEMY_GRENADE, SCREEN_WIDTH, 1800, 270, 80,1,0,nullptr,0, 60);
		App->enemies->AddSpawner(ENEMY_GRENADE, SCREEN_WIDTH, 1325, 270, 220,1,0,nullptr, 0 , 25);
		App->enemies->AddSpawner(ENEMY_GRENADE, SCREEN_WIDTH, 825, 270, 200,1,0 ,nullptr,0, 75);
		App->enemies->AddSpawner(ENEMY_GRENADE, 0, 825, 90,50, 1,0,nullptr,0, 75);

	}

	font_red = App->fonts->Load("Images/Fuentes_small_red.png", "0123456789ABCDEF\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1           K;�.,0123456789=      ABCDEFGHIJKLMNOPQRSTUVWXYZ.\1\1   abcdefghijklmnopqrstuvwxyz    |                                ", 5, 0, 1);
	font_white = App->fonts->Load("Images/Fuentes_small_grey.png", "0123456789ABCDEF\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1           K;�.,0123456789=      ABCDEFGHIJKLMNOPQRSTUVWXYZ.\1\1   abcdefghijklmnopqrstuvwxyz    |                                ", 5, 0, 1);

	App->interfac->AddLabel(font_red, "1up", 15, 0);
	App->interfac->AddLabel(font_red, "top score", SCREEN_WIDTH / 2 - 30, 0);
	App->interfac->AddLabel(font_white, "50000", SCREEN_WIDTH / 2 - 15, 8);
	grenade_label = App->interfac->getLabel(App->interfac->AddLabel(font_white, "", SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT - 15));
	score_label = App->interfac->getLabel(App->interfac->AddLabel(font_white, "", SCREEN_WIDTH / 2 - 103, 8));

	//Enabling modules
	App->player->Enable();
	App->collision->Enable();
	App->objects->Enable();
	App->particles->Enable();
	App->enemies->Enable();
	App->fonts->Enable();
	App->interfac->Enable();

	spawning = false;
	counter = 0;

	/*if (App->sound->LoadMusic("Soundtrack/3.Hintergrundmusik 1.wav") == nullptr)
		ret = false;*/
	if (background_graphics == nullptr)
		ret = false;


	/*if (!App->sound->isPlaying()) {
		App->sound->PlayMusic();
	}*/
	
	/*App->sound->PlaySound(newstart, 0);*/

	return ret;
}

update_status ModuleLevel3::PreUpdate() {

	if (restart) {
		App->fade->FadeToBlack(this, next, 0.0f);
		screen_enemies = 0;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleLevel3::Update() {
	bool ret = true;

	ret = App->render->Blit(background_graphics, 0, 0, nullptr);

	sprintf_s(score_text, 10, "%7d", score);

	if (App->player->position.y <= 675)
	{
		global = true;
	}
	if (global == true && global_on == true)
	{
		//Global = App->enemies->AddSpawner(LEFT_WEAPON, 0, 0, 0, 80, 1, 1);
		global_on = false;
	}

	if (App->player->position.y <= SCREEN_HEIGHT - 100)
	{
		if (spawning == false)
		{
			//Global->alive = false;
			timer = SDL_GetTicks();
			spawning = true;
			App->objects->final_door.speed = 0.05f;
		}
		else if (timer + 1500 < SDL_GetTicks() && counter < 5)
		{
			App->enemies->AddEnemy(ENEMY_GRENADE, ((rand() % 148) + (SCREEN_WIDTH - 37))/2, 5, 0, 2);		
			timer = SDL_GetTicks();
			counter++;
		}
		else if (timer + 2000 < SDL_GetTicks() && App->player->final_anim == 0)
		{
			App->player->final_anim = 1;
			App->enemies->Disable();
		}
	}




	if (score > top_score)
	{
		top_score = score;
	}
	grenade_str = "= ";
	grenade_str.append(std::to_string(App->player->grenades));

	grenade_label->setString(grenade_str.c_str());
	score_label->setString(score_text);

	

	return ret ? update_status::UPDATE_CONTINUE : update_status::UPDATE_ERROR;
}

update_status ModuleLevel3::PostUpdate() {
	if (App->player->final_anim == 3)
	{
		App->fade->FadeToBlack(this, App->transition, 1.0f);
		App->player->final_anim = 0;
	}

	return UPDATE_CONTINUE;
}

bool ModuleLevel3::CleanUp() {
	bool ret = true;
	restart = false;
	App->sound->StopAll();
	App->player->Disable();
	App->collision->Disable();
	App->objects->Disable();
	App->particles->Disable();
	App->fonts->Disable();
	App->interfac->Disable();
	ret = App->textures->Unload(background_graphics);

	return ret;
}

void ModuleLevel3::onFadeInEnd() {
	restart = false;
}