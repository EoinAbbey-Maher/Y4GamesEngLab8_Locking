#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <array>


static const int g_N = 5;
int g_number, g_next = 0;
std::array<int, g_N> g_turn;

int g_numOfThreads = 0;
std::mutex g_mtx;


/// <summary>
///  --------------------------- Player Class For Global Creation ------------------------------
/// </summary>
class Player
{
public:
	Player() {};
	~Player() {};

	void updatePosition(sf::Vector2f t_pos)
	{
		std::cout <<  "Updating Player position "<< std::endl << std::endl;
		m_position += t_pos;
	};

	void getPosition()
	{
		std::cout << "position X: " << std::to_string(m_position.x) << "position Y: " << std::to_string(m_position.y) << std::endl << std::endl;
	};
	void renderPlayer()
	{
		std::cout << "Rendering Phase" << std::endl << std::endl;
	};
	;

	sf::Vector2f m_position{ 0,0 };

private:

};

/// <summary>
/// --------------------------- Global Player ----------------------------------
/// </summary>
Player g_player;

/// <summary>
/// -------------------------------- Render System Thread Function -----------------------------
/// </summary>
void renderSystem()
{
	//increasing number of threads
	int i = g_numOfThreads++;

	//locking the mutex to stop other threads
	g_mtx.lock();
	std::cout << "Render Thread" << std::endl;
	//unlocking mutex for other threads	
	g_mtx.unlock();

	//sleep system to make the output easier to read
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (true)
	{
		//increase the turn number
		g_turn[i] = g_number;

		g_number++;

		while (g_turn[i] != g_next)
		{
			continue;
		}

		// Carry out the Critical Section
		g_mtx.lock();
		std::cout << "----------------- Rendering Critical Section -----------------" << std::endl;
		g_player.renderPlayer();
		g_mtx.unlock();

		//increase ticket number
		g_next++;

	}

}

/// <summary>
/// ------------------------------ Input System Thread Function ---------------------------------
/// </summary>
void inputSystem()
{
	int i = g_numOfThreads++;

	g_mtx.lock();
	std::cout << "Input Thread" << std::endl;
	g_mtx.unlock();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (true)
	{
		g_turn[i] = g_number;
		g_number++;

		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		while (g_turn[i] != g_next)
		{
			continue;
		}

		g_mtx.lock();
		std::cout << "----------------- Update Position -----------------" << std::endl;
		g_player.updatePosition(sf::Vector2f(1, 1));
		g_mtx.unlock();

		g_next++;

	}

}

/// <summary>
/// ------------------------------ Physics System Thread Function ---------------------------------
/// </summary>

void physicsSystem()
{
	int i = g_numOfThreads++;

	g_mtx.lock();
	std::cout << "Physics System Thread" << std::endl;
	g_mtx.unlock();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (true)
	{
		g_turn[i] = g_number;
		g_number++;

		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		while (g_turn[i] != g_next)
		{
			continue;
		}

		g_mtx.lock();
		std::cout << "t" << i << "----------------- Get Position Critical Section -----------------"   << std::endl;
		g_player.getPosition();
		g_mtx.unlock();

		g_next++;
		
	}
}



int main(void)
{
	for (int i = 0; i < g_turn.size(); i++)
	{
		g_turn[i] = 0;
	}

	//Setting Up threads
	std::thread posreturn(physicsSystem);
	std::thread posUpdate(inputSystem);
	std::thread posreturn2(physicsSystem);
	std::thread render(renderSystem);
	
	
	posreturn.join();
	posUpdate.join();
	posreturn2.join();
	render.join();
	
	std::cin.get();
	return 0;
}

