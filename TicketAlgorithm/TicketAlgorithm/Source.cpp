#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

#include <iostream>
#include <string>
#include <array>



static const int n = 5;
int number, next = 1;
std::array<int, n> turn;

int numOfThreads = 0;
std::mutex mtx;

void ticketAlgorithm()
{
	int i = numOfThreads++;

	mtx.lock();
	std::cout << "Thread" << i << std::endl;
	mtx.unlock();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (true)
	{
		turn[i] = number;
		number++;

		mtx.lock();
		std::cout << "t" << i << "\t Turn: " << turn[i] << std::endl;
		mtx.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		while(turn[i] != next)
		{
			continue;
		}

		mtx.lock();
		std::cout << "t" << i << "\t +Critical Section " << std::endl;
		mtx.unlock();

		std::cout << "Critical Section" + i << std::endl;

		next++;

		mtx.lock();
		std::cout << "t" << i << "\t Next: " << next << std::endl;	
		mtx.unlock();

	}
}

int main(void)
{
	for (int i = 0; i < turn.size(); i++)
	{
		turn[i] = 0;
	}

	std::thread ticket1(ticketAlgorithm);
	std::thread ticket2(ticketAlgorithm);
	
	ticket1.join();
	ticket2.join();
	
	std::cin.get();
	return 0;
}




