#ifndef __DEF_GAME_H__
#define __DEF_GAME_H__

#include <SDL.h>
#include <SDL_ttf.h>

class Game
{
	enum class Result {playerWin, computerWin, draw};
	enum class TerrainType {playerCoin, computerCoin, emptyPlace};
public :


    Game();
    ~Game();
	
	void close() {m_done = true;}
    int exec();
    inline bool isRunning() {return !m_done;}

private :
    void init();
    void render();
    void cleanUp();
	
	void clearTerrain();
	void drawTerrain();
	bool clicTerrain();
	bool checkResult(Result res);
	void updateScoreBoard();
	void showState(Result res);
	
	void computerPlay();
	bool winFound();
	bool loseFound();
	bool playCenter();
	void playRandom();

private :
    bool m_done = false;
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Event m_event;
	SDL_Rect m_systemScreenSize;
	bool m_touch = false;
	bool m_renderScreen = true;
	TerrainType m_terrain[3][3];
	
	
	int m_playerScore = 0;
	int m_computScore = 0;
	
	SDL_Point m_posTouch;
	
	int m_terrainSize;
	int m_terrainX;
	int m_terrainY;
	int m_terrainTileSize;
	
	SDL_Texture* m_background = nullptr;
	SDL_Texture* m_playerCoin = nullptr;
	SDL_Texture* m_computCoin = nullptr;
	
	SDL_Texture* m_scoreBoard = nullptr;
	SDL_Rect m_scoreBoardPos;
	
	SDL_Texture* m_bar = nullptr;
	
	TTF_Font* m_font = nullptr;
	TTF_Font *m_resFont = nullptr;

};

#endif //__DEF_GAME_H__