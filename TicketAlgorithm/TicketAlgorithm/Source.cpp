#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <array>



static const int g_N = 5;
int g_number, g_next = 1;
std::array<int, g_N> g_turn;

int g_numOfThreads = 0;
std::mutex g_mtx;

void criticalSection();

void ticketAlgorithm()
{
	int i = g_numOfThreads++;

	g_mtx.lock();
	std::cout << "Thread" << i << std::endl;
	g_mtx.unlock();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (true)
	{
		g_turn[i] = g_number;
		g_number++;

		g_mtx.lock();
		std::cout << "t" << i << "\t Turn: " << g_turn[i] << std::endl;
		g_mtx.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		while(g_turn[i] != g_next)
		{
			continue;
		}

		g_mtx.lock();
		std::cout << "t" << i << "\t  + Critical Section " << std::endl;
		g_mtx.unlock();

		std::cout << "Critical Section" + i << std::endl;
		criticalSection();

		g_next++;

		g_mtx.lock();
		std::cout << "t" << i << "\t Next: " << g_next << std::endl;	
		g_mtx.unlock();

	}
}

int main(void)
{
	for (int i = 0; i < g_turn.size(); i++)
	{
		g_turn[i] = 0;
	}

	std::thread ticket1(ticketAlgorithm);
	std::thread ticket2(ticketAlgorithm);
	std::thread ticket3(ticketAlgorithm);
	std::thread ticket4(ticketAlgorithm);
	
	ticket1.join();
	ticket2.join();
	ticket3.join();
	ticket4.join();
	
	std::cin.get();
	return 0;
}

void criticalSection()
{
	
}
