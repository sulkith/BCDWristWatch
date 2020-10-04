/*
 * STM32L4_DisplayManager.hpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */

#ifndef STM32L4_BCDDISPLAYMANAGER_HPP_
#define STM32L4_BCDDISPLAYMANAGER_HPP_
#include "DisplayManager.hpp"
#include "SleepSubscriber.hpp"

class STM32L4_BCDDisplayManager: public DisplayManager, public SleepSubscriber
{
	public:
		virtual void show();
		virtual void init();
		virtual void lockPorts();
		virtual void executeSleepSubscription();
};

#endif /* STM32L4_BCDDISPLAYMANAGER_HPP_ */
