#pragma once
#include <vector>
#include <functional>

#include "../defines.h"

namespace Brainstorm {
	class BS_API Timer {
	private:
		int64_t lastTime;
		float delta, time, realTime, scale;
	public:
		Timer();

		void update();
		void setScale(float scale);
		
		float getDelta() const;
		float getRealDelta() const;

		float getTime() const;
		float getRealTime() const;
	};
}