#pragma once
#ifndef STARDUST_AXIS_TYPE_H
#define STARDUST_AXIS_TYPE_H

namespace stardust
{
	enum class AxisType
	{
		WSKeys,
		ADKeys,
		UpDownKeys,
		LeftRightKeys,
		MouseX,
		MouseY,
		MouseScroll,
		ControllerLeftX,
		ControllerLeftY,
		ControllerRightX,
		ControllerRightY,
		ControllerLeftTrigger,
		ControllerRightTrigger,
		ControllerGyroX,
		ControllerGyroY,
		ControllerGyroZ,
		ControllerAccelX,
		ControllerAccelY,
		ControllerAccelZ,
	};
}

#endif