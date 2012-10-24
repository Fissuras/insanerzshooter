#ifndef timer_h
#define timer_h
	#include <SDL/SDL.h>

	/* 
	Classe utilizada para controle de tempo.
	É usada para controlar por exemplo o número de frames por segundo
	*/
	class Timer {
		private:
			//The clock time when the timer started
			int startTicks;
			//The ticks stored when the timer was paused
			int pausedTicks;
			//The timer status
			bool paused;
			bool started;

		public:
			Timer() {
				startTicks = 0;
				pausedTicks = 0;
				paused = false;
				started = false;
			}
			~Timer() {}

			void start();
			void stop();
			void pause();
			void unpause();
			int get_ticks();
			bool is_started();
			bool is_paused();
	};
#endif
