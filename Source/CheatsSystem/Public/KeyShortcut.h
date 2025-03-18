#pragma once

namespace dbg
{
	struct CHEATSSYSTEM_API KeyShortcut
	{
		char key = 0; //Must be uppercase
		bool altDown = true;

		bool operator== (const KeyShortcut&) const = default;
	};
}
