#ifndef AETHER_TIMER_HPP
#define AETHER_TIMER_HPP

#include <chrono>

namespace Aether {
    /**
     * @brief Small utility class to measure time.
     */
    class Timer {
        private:
            /** @brief Point in time timer was started */
            std::chrono::time_point<std::chrono::steady_clock> startTime;

            /** @brief Point in time the timer was started */
            std::chrono::time_point<std::chrono::steady_clock> endTime;

            /** @brief Used to mark if started */
            bool isRunning;

        public:
            /**
             * @brief Create a new timer. Doesn't start the timer. See \ref start().
             */
            Timer();

            /**
             * @brief Starts the timer. Does nothing if already running.
             */
            void start();

            /**
             * @brief Stops the timer. Does nothing if not running.
             */
            void stop();

            /**
             * @brief Returns the number of milliseconds elapsed since the timer was started.
             */
            double elapsedMillis();

            /**
             * @brief Returns the number of seconds elapsed since the timer was started.
             */
            double elapsedSeconds();
    };
};

#endif