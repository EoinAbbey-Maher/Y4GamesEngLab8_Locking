#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

#include <iostream>
#include <string>
#include <array>

const static int n = 10;

	int num, next = 1;
	static std::array<int, n> m_entering;
	static	std::array<int, n> m_ticket;

	static std::mutex mtx;

	class criticalProcess : public std::thread
	{
		void lock(int t_pid)
		{
			m_entering[t_pid] = 1;

			int max = 0;

			for (int i = 0; i < n; i++)
			{
				int current = m_ticket[i];

				if (current > max)
				{
					max = current;
					std::cout << "Max: " + std::to_string(max) << std::endl;;
				}
			}
			m_ticket[t_pid ] = 1 + max;
			m_entering[t_pid] = 0;

			for (int  i = 0; i < m_ticket.size(); i++)
			{
				if (i != t_pid)
				{
					while (m_entering[i] == 1)
					{
						std::this_thread::yield();
					}

					while (m_ticket[i] != 0 && (m_ticket[t_pid] > m_ticket[i] || (m_ticket[t_pid] == m_ticket[i] && t_pid > i)))
					{
						std::cout << "Thread Yield... " + std::to_string(m_entering[i]) << std::endl;;
						std::this_thread::yield();
					}
				}
			}
		}

		void unlock(int t_pid)
		{
			m_ticket[t_pid] = 0;
		}

		//was trying to get lock to work
		void run()
		{
			mtx.lock();
			try
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				std::cout << "Critical Section" << std::endl;;
			}
			catch (const std::exception&){}

			for (int i	= 0; i < m_ticket.size(); i++)
			{
				std::cout << "[" + std::to_string(m_ticket[i]) + "]" << std::endl;
			}

		}
	};



int main(void)
{

	std::thread ticket(criticalProcess);


	return 0;
}




