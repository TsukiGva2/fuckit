#include "game.h"
#include "game_internal.h"

static GAME* _GAME_Get_Self(GAME* game) {
	static GAME* self = NULL;

	if (game != NULL) {
		self = game;
	}

	return self;
}

static GAME* GAME_Get_Self(void) {
	return _GAME_Get_Self(NULL);
}

static HAN_Status* GAME_Create_SDL_Window_And_Screen(void) {
	GAME* self = GAME_Get_Self();

	self->win = SDL_CreateWindow(
									GAME_NAME,
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									WIN_W, WIN_H,
									SDL_WINDOW_SHOWN);

	if (self->win == NULL) {
		return HAN_RETURN(HAN_CREATE_WINDOW_ERR,
											"self->win is NULL");
	}

	self->scr = SDL_GetWindowSurface(self->win);
	if (self->scr == NULL) {
		return HAN_RETURN(HAN_CREATE_WINDOW_ERR,
							"couldnt get window surface");
	}

	return HAN_RETURN_OK;
}

HAN_Status* GAME_Create(
		GAME* game,
		LCD* lcd,
		HAN_Status* handler) {

	/* self */
	_GAME_Get_Self(game); // set self
	
	/* handler */
	HAN_Create(handler);

	/* lcd */
	lcd->attr.rows = ROWS;
	lcd->attr.cols = COLS;
	lcd->attr.size = COLS*ROWS;
	lcd->attr.gap  = GAP;

	lcd->char_data.chars = NULL;
	lcd->dc_head         = NULL;

	game->lcd = lcd;

	/* sdl */
	game->win = NULL;
	game->scr = NULL;

	/* fps */
	game->fps.min_ft = 16;
	game->fps.curr   =  0;
	game->fps.ft     =  0;

	/* bg */
	game->bg.r    = (SDL_Rect){0, 0, WIN_W, WIN_H};
	game->bg.img  = NULL;

	/* game_objects */
	game->objs_head.prev = NULL;
	game->objs_head.next = NULL;

	/* misc */
	game->running = true;
	game->update  = true;

	/* functions */
	FAIL (GAME_Create_SDL_Window_And_Screen());
	FAIL (GAME_Load_BG());

	FAIL (LCD_Char_Data_Initialize(lcd));
	FAIL (LCD_Displayed_Character_Create_All(lcd));
	FAIL (LCD_Displayed_Character_Update_All(lcd));

	return HAN_RETURN_OK;
}

void GAME_Loop_Begin(void) {
	GAME* self = GAME_Get_Self();

	_GAME_Check_Update_Condition();

	self->fps.ft = SDL_GetTicks();
}

void _GAME_Check_Update_Condition(void) {
	GAME* self = GAME_Get_Self();

	// XXX: hardcoded stuff
	if (!self->update) {
		if (self->fps.frames % 30 == 0) self->update = true;
	}
}

HAN_Status* GAME_Loop_Update(void) {
	GAME* self = GAME_Get_Self();

	if (self->update) {
		FAIL (LCD_Out_Clear(self->lcd));

		//FAIL (GAME_OBJECT_Update_All());
		/* call update functions */

		/*
		FAIL (LCD_State_Set_Cursor(
					self->lcd, (self->fps.frames/30)%15, 0));
		FAIL (LCD_Out_Text(self->lcd, "BY TSUKIGVA"));
		*/

		FAIL (GAME_Game_Object_Update_All());
	}

	GAME_Loop_Process_Input();

	return HAN_RETURN_OK;
}

HAN_Status* GAME_Loop_Draw(void) {
	GAME* self = GAME_Get_Self();

	SDL_BlitScaled(
			self->bg.img, NULL, self->scr, &self->bg.r);
	
	LCD_Displayed_Character_Draw_All(self->lcd, self->scr);

	return HAN_RETURN_OK;
}

void GAME_Loop_Finish(void) {
	GAME* self = GAME_Get_Self();

	SDL_UpdateWindowSurface(self->win);
	
	FPS* fps = &self->fps;

	fps->curr = SDL_GetTicks() - fps->ft;
	if (fps->curr < fps->min_ft)
		SDL_Delay(fps->min_ft - fps->curr);

	self->fps.frames++;
	
	self->update = false;
}

/* input */
static void GAME_Loop_Process_Input(void) {
	GAME* self = GAME_Get_Self();

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			self->running = false;
		if (e.type == SDL_KEYDOWN)
			if (e.key.keysym.sym
					== SDLK_ESCAPE)
				self->running = false;
	}
}

/* misc */
void GAME_Free(GAME* game) {
	/* just making sure */
	game->running = false;
	game->update  = false;

	/* sdl/bg */
	if (game->bg.img != NULL)
		SDL_FreeSurface(game->bg.img);
	if (game->win != NULL)
		SDL_DestroyWindow(game->win);

	game->bg.img = NULL;
	game->win    = NULL;

	/* lcd */
	LCD_Free(game->lcd);

	/* finishing */
	SDL_Quit();
}

HAN_Status* GAME_Load_BG(void) {
	GAME* self = GAME_Get_Self();

	self->bg.img = SDL_LoadBMP(LCD_IMG_FILE);

	if (self->bg.img == NULL) {
		return HAN_RETURN(HAN_LOAD_IMG_ERR,
											"couldnt load bg");
	}

	return HAN_RETURN_OK;
}

/* game objects (...) */
HAN_Status* GAME_Add_Game_Object(GAME_OBJECT* go) {
	GAME* self = GAME_Get_Self();

	GAME_OBJECT* it = &self->objs_head;
	size_t i = 0;

	for (it, i; it->next != NULL; it = it->next, i++);
	it->next = go;

	go->prev = it;
	go->next = NULL;

	go->go.id = i;

	return HAN_RETURN_OK;
}

HAN_Status* GAME_Game_Object_Update_All(void) {
	GAME* self = GAME_Get_Self();

	GAME_OBJECT* it = self->objs_head.next;

	for (it; it != NULL; it = it->next) {
		FAIL (LCD_State_Set_Cursor(self->lcd, it->go.x, it->go.y));
		FAIL (LCD_Out_Custom_Char(
					self->lcd, it->go.sprite_ids[it->go.sprite]));
	}

	return HAN_RETURN_OK;
}

