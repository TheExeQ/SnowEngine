#pragma once

#include <stdint.h>

namespace Snow
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			MOUSELBUTTON = 0x01,
			MOUSERBUTTON = 0x02,
			CANCEL = 0x03,
			MBUTTON = 0x04,

			MOUSEXBUTTON1 = 0x05,
			MOUSEXBUTTON2 = 0x06,
		};
	}
}