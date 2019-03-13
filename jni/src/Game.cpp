#include "Game.h"
#include "GameError.h"
#include <SDL_image.h>
#include <time.h>
#include <stdio.h>




Game::Game()
{

}

Game::~Game()
{

}

void
Game::clearTerrain()
{
    for (auto i = 0; i <=2; i++)
        for (auto j = 0; j<=2; j++)
            m_terrain[i][j] = TerrainType::emptyPlace;
}

void
Game::updateScoreBoard()
{
	SDL_Color color={0,0,0};
	
	char text[40];
	sprintf(text, "You: %d - %d : Opponent", m_playerScore, m_computScore);
	
	SDL_Surface* surf = TTF_RenderText_Blended(m_font, text, color);
	m_scoreBoard = SDL_CreateTextureFromSurface(m_renderer, surf);
	SDL_FreeSurface(surf);
	SDL_QueryTexture(m_scoreBoard, nullptr, nullptr, &m_scoreBoardPos.w, &m_scoreBoardPos.h);
}


bool
Game::clicTerrain()
{
    int i, j;
    if (m_posTouch.x < m_terrainX || m_posTouch.y < m_terrainY)
        return false;

    if (m_posTouch.x > m_terrainX + 3*m_terrainTileSize || m_posTouch.y > m_terrainY + 3* m_terrainTileSize)
        return false;

    i = (m_posTouch.x - m_terrainX) / m_terrainTileSize;
    j = (m_posTouch.y - m_terrainY) / m_terrainTileSize;

    if (m_terrain[i][j] != TerrainType::emptyPlace)
        return false;
    m_terrain[i][j] = TerrainType::playerCoin;
    return true;
}

void
Game::showState(Result res)
{

	SDL_Surface* srf = nullptr;
	SDL_Texture* text = nullptr;
    SDL_Rect position;
    SDL_Color black = {0, 0, 0};

    switch(res) // On afficher un texte différent selon le test indiqué.
    {
        case Result::playerWin :
        srf = TTF_RenderText_Blended(m_resFont, "You Win !", black);
        break;

        case Result::computerWin :
        srf = TTF_RenderText_Blended(m_resFont, "You Lose !", black);
        break;

        case Result::draw :
        srf = TTF_RenderText_Blended(m_resFont, "Draw!", black);
        break;
    }
	
	text = SDL_CreateTextureFromSurface(m_renderer, srf);
	SDL_FreeSurface(srf);
	
    position.x = 0;
	position.w = m_systemScreenSize.w;
	position.h = (int) (position.w / 3.38983f);
	position.y = (m_systemScreenSize.h / 2) - (position.h / 2);
	
    SDL_RenderCopy(m_renderer, m_bar, nullptr, &position);

	
	SDL_QueryTexture(text, nullptr, nullptr, &position.w, &position.h);
	position.x = (m_systemScreenSize.w / 2) - (position.w / 2);
    position.y = (m_systemScreenSize.h / 2) - (position.h / 2);
	
	SDL_RenderCopy(m_renderer, text, nullptr, &position);
	SDL_RenderPresent(m_renderer);
	SDL_DestroyTexture(text);

}

void
Game::drawTerrain()
{
	SDL_Rect dest = {m_terrainX, m_terrainY, m_terrainSize, m_terrainSize};
	SDL_RenderCopy(m_renderer, m_background, nullptr, &dest);
	
	dest.w = m_terrainTileSize;
	dest.h = m_terrainTileSize;
	
	m_scoreBoardPos.x = m_systemScreenSize.w / 2 - m_scoreBoardPos.w / 2;
	m_scoreBoardPos.y = (m_systemScreenSize.h - m_terrainSize) / 4 - m_scoreBoardPos.h / 2;

	
	SDL_RenderCopy(m_renderer, m_scoreBoard, nullptr, &m_scoreBoardPos);
	
	
	for (auto i = 0; i<=2; i++)
        for (auto j = 0; j<=2; j++)
        {
            dest.x = (m_terrainX + (m_terrainTileSize * i));
            dest.y = (m_terrainY + (m_terrainTileSize * j));
			
            if (m_terrain[i][j] == TerrainType::playerCoin)
                SDL_RenderCopy(m_renderer, m_playerCoin, nullptr, &dest);
            else if (m_terrain[i][j] == TerrainType::computerCoin)
                SDL_RenderCopy(m_renderer, m_computCoin, nullptr, &dest);

        }
}

bool
Game::checkResult(Result res)
{
    if (res == Result::playerWin) {
		
        //vertically
        if (m_terrain[0][0] == TerrainType::playerCoin && m_terrain[0][1] == TerrainType::playerCoin && m_terrain[0][2] == TerrainType::playerCoin)  return true;
        if (m_terrain[1][0] == TerrainType::playerCoin && m_terrain[1][1] == TerrainType::playerCoin && m_terrain[1][2] == TerrainType::playerCoin)  return true;
        if (m_terrain[2][0] == TerrainType::playerCoin && m_terrain[2][1] == TerrainType::playerCoin && m_terrain[2][2] == TerrainType::playerCoin)  return true;
        //horizontally
        if (m_terrain[0][0] == TerrainType::playerCoin && m_terrain[1][0] == TerrainType::playerCoin && m_terrain[2][0] == TerrainType::playerCoin)  return true;
        if (m_terrain[0][1] == TerrainType::playerCoin && m_terrain[1][1] == TerrainType::playerCoin && m_terrain[2][1] == TerrainType::playerCoin)  return true;
        if (m_terrain[0][2] == TerrainType::playerCoin && m_terrain[1][2] == TerrainType::playerCoin && m_terrain[2][2] == TerrainType::playerCoin)  return true;
        //diagonally
        if (m_terrain[0][0] == TerrainType::playerCoin && m_terrain[1][1] == TerrainType::playerCoin && m_terrain[2][2] == TerrainType::playerCoin)  return true;
        if (m_terrain[2][0] == TerrainType::playerCoin && m_terrain[1][1] == TerrainType::playerCoin && m_terrain[0][2] == TerrainType::playerCoin)  return true;

        return 0;
    
	} else if (res == Result::computerWin) {
		
        //vertically
        if (m_terrain[0][0] == TerrainType::computerCoin && m_terrain[0][1] == TerrainType::computerCoin && m_terrain[0][2] == TerrainType::computerCoin)  return true;
        if (m_terrain[1][0] == TerrainType::computerCoin && m_terrain[1][1] == TerrainType::computerCoin && m_terrain[1][2] == TerrainType::computerCoin)  return true;
        if (m_terrain[2][0] == TerrainType::computerCoin && m_terrain[2][1] == TerrainType::computerCoin && m_terrain[2][2] == TerrainType::computerCoin)  return true;
        //horizontally
        if (m_terrain[0][0] == TerrainType::computerCoin && m_terrain[1][0] == TerrainType::computerCoin && m_terrain[2][0] == TerrainType::computerCoin)  return true;
        if (m_terrain[0][1] == TerrainType::computerCoin && m_terrain[1][1] == TerrainType::computerCoin && m_terrain[2][1] == TerrainType::computerCoin)  return true;
        if (m_terrain[0][2] == TerrainType::computerCoin && m_terrain[1][2] == TerrainType::computerCoin && m_terrain[2][2] == TerrainType::computerCoin)  return true;
        //diagonally
        if (m_terrain[0][0] == TerrainType::computerCoin && m_terrain[1][1] == TerrainType::computerCoin && m_terrain[2][2] == TerrainType::computerCoin)  return true;
        if (m_terrain[2][0] == TerrainType::computerCoin && m_terrain[1][1] == TerrainType::computerCoin && m_terrain[0][2] == TerrainType::computerCoin)  return true;

        return 0;
    } else if (res == Result::draw) {

        for (auto i = 0; i<=2; i++)
            for (auto j = 0; j<=2; j++)
                if (m_terrain[i][j] == TerrainType::emptyPlace)
                    return false;
        return true;
    }
}


void
Game::computerPlay()
{
    if (winFound())
        return;
    if (loseFound())
        return;
    if (playCenter())
        return;
    playRandom();
    return;
}

bool
Game::winFound()
{
    for (auto i = 0; i<=2 ; i++)
        for (auto j = 0; j<=2; j++)
            if (m_terrain[i][j] == TerrainType::emptyPlace)
            {
                m_terrain[i][j] = TerrainType::computerCoin;
                if (checkResult(Result::computerWin))
                    return true;
                m_terrain[i][j] = TerrainType::emptyPlace;
            }
    return false;
}


bool
Game::loseFound()
{
    for (auto i = 0; i<=2 ; i++)
        for (auto j = 0; j<=2; j++)
            if (m_terrain[i][j] == TerrainType::emptyPlace) 
			{
                m_terrain[i][j] = TerrainType::playerCoin;
                if (checkResult(Result::playerWin))
				{
                    m_terrain[i][j] = TerrainType::computerCoin;
					return true;
				}
                m_terrain[i][j] = TerrainType::emptyPlace;
            }
    return false;
}


bool
Game::playCenter()
{
    srand(time(NULL));
    if (m_terrain[1][1] != TerrainType::emptyPlace)
        return false;
    if ((rand() % 2) == 0)
        return false;
    m_terrain[1][1] = TerrainType::computerCoin;
    return true;
}

void
Game::playRandom()
{
    srand(time(NULL));
    while (true)
    {
        auto i = (rand() % 3);
        auto j = (rand() % 3);
        if (m_terrain[i][j] == TerrainType::emptyPlace)
        {
            m_terrain[i][j] = TerrainType::computerCoin;
            return;
        }
    }
}

void
Game::init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw Game_init_fail(std::string("ERROR : Unable to initialize SDL") + SDL_GetError());
		
		SDL_DisplayMode displayMode;
		
        if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
        {
            m_systemScreenSize.w = displayMode.w;
            m_systemScreenSize.h = displayMode.h;
        }
		
	
 	
	if(TTF_Init()==-1)
		throw Game_init_fail(std::string("ERROR : Unable to initialize SDL_ttf") + TTF_GetError());


	m_window = SDL_CreateWindow("XO Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_systemScreenSize.w, m_systemScreenSize.h,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	
	if(m_window == nullptr)
	    throw Game_init_fail(std::string("FATAL_ERROR : Cannot create Game window, ") + SDL_GetError());
	
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	
    if(m_renderer == nullptr)
        throw Game_init_fail(std::string("FATAL_ERROR : Cannot create renderer, ") + SDL_GetError());
	
	clearTerrain();
	
	m_background = IMG_LoadTexture(m_renderer, "background.png");
	if(m_background == nullptr)
		throw Game_init_fail(std::string("FATAL_ERROR : Unable to load texture \"background.png\", ") + IMG_GetError());
	
	m_playerCoin = IMG_LoadTexture(m_renderer, "playerCoin.png");
	if(m_playerCoin == nullptr)
		throw Game_init_fail(std::string("FATAL_ERROR : Unable to load texture \"playerCoin.png\", ") + IMG_GetError());
	
	m_computCoin = IMG_LoadTexture(m_renderer, "computCoin.png");
	if(m_computCoin == nullptr)
		throw Game_init_fail(std::string("FATAL_ERROR : Unable to load texture \"computCoin.png\", ") + IMG_GetError());
	
	m_terrainSize = ((m_systemScreenSize.w * 3) / 4);
	m_terrainX = (m_systemScreenSize.w - m_terrainSize) / 2;
	m_terrainY = (m_systemScreenSize.h - m_terrainSize) / 2;
	m_terrainTileSize = m_terrainSize / 3;
	
	int ptsize = (m_systemScreenSize.h - m_terrainSize) / 10;
	m_font = TTF_OpenFont("Megrim.ttf", ptsize);
	if(m_font == nullptr)
		throw Game_init_fail(std::string("TTF_OpenFont") + TTF_GetError());
	
	m_bar = IMG_LoadTexture(m_renderer, "bar.png");
	if (m_bar == nullptr)
		throw Game_init_fail(std::string("FATAL_ERROR : Unable to load texture \"bar.png\", ") + IMG_GetError());
	
	ptsize = (m_systemScreenSize.w / 5);
	m_resFont = TTF_OpenFont("Averia-Regular.ttf", ptsize);
	if(m_resFont == nullptr)
		throw Game_init_fail(std::string("TTF_OpenFont") + TTF_GetError());
	


	updateScoreBoard();
	
	render();
	
}

void
Game::render()
{
	//Initialize renderer color
	SDL_SetRenderDrawColor( m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	//Clear screen
	SDL_RenderClear(m_renderer);

	drawTerrain();	
				
	//Update screen
	SDL_RenderPresent( m_renderer );
}

void
Game::cleanUp()
{
	
    if (m_computCoin != nullptr)
		SDL_DestroyTexture(m_computCoin);
	
	if (m_playerCoin != nullptr)
		SDL_DestroyTexture(m_playerCoin);
	
	if (m_background != nullptr)
		SDL_DestroyTexture(m_background);
	
	if (m_scoreBoard != nullptr)
		SDL_DestroyTexture(m_scoreBoard);
	
	if (m_bar != nullptr)
		SDL_DestroyTexture(m_bar);
	
	if(m_renderer != nullptr)
        SDL_DestroyRenderer(m_renderer);

    if (m_window != nullptr)
        SDL_DestroyWindow(m_window);
	
	if (m_font != nullptr)
		TTF_CloseFont(m_font);
	
	if (m_resFont != nullptr)
		TTF_CloseFont(m_resFont);
	
	if (TTF_WasInit())
		TTF_Quit();

    if (SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Quit();
}

int
Game::exec()
{
    try {
		init();

		while(isRunning()) {
		
		SDL_WaitEvent(&m_event);
		
		switch(m_event.type) {
		
		case SDL_QUIT :
			close();
			break;
		
		
		case SDL_WINDOWEVENT :
			render();
			break;

		case SDL_FINGERDOWN :
			m_posTouch.x = m_event.tfinger.x * m_systemScreenSize.w;
			m_posTouch.y = m_event.tfinger.y * m_systemScreenSize.h;
		
			if (clicTerrain()) {
				render();
				SDL_Delay(500);
				if (checkResult(Result::playerWin)) {
					
                        m_playerScore++;
						showState(Result::playerWin);
                        SDL_Delay(2000);
                        clearTerrain();
						updateScoreBoard();
                        render();
                    }
				if (checkResult(Result::draw)) {
					
                    showState(Result::draw);
					SDL_Delay(2000);
                    clearTerrain();
                    render();
                }
				
				computerPlay();
                render();
                if (checkResult(Result::computerWin)) {
					
                    m_computScore++;
					showState(Result::computerWin);
                    SDL_Delay(2000);
                    clearTerrain();
					updateScoreBoard();
					render();
                }
				
                if (checkResult(Result::draw)) {
					
                    showState(Result::draw);
					SDL_Delay(2000);
                    clearTerrain();
                    render();
                }
			}
			break;
		
		}
		}
	} catch (std::exception& e) {
		
        SDL_Log("%s\n", e.what());
        cleanUp();
        return 1;
    }

    cleanUp();
    return 0;
}
