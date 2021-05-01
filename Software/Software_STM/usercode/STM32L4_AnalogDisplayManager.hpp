/*
 * STM32L4_AnalogDisplayManager.hpp
 *
 *  Created on: Oct 04, 2020
 *      Author: sulkith
 */

#ifndef STM32L4_ANALOGDISPLAYMANAGER_HPP_
#define STM32L4_ANALOGDISPLAYMANAGER_HPP_
#include "DisplayManager.hpp"
#include "SleepSubscriber.hpp"

class STM32L4_AnalogDisplayManager: public DisplayManager, public SleepSubscriber
{
	public:
		virtual void show();
		virtual void init();
		virtual void lockPorts();
		virtual void executeSleepSubscription();
};

#endif /* STM32L4_ANALOGDISPLAYMANAGER_HPP_ */
