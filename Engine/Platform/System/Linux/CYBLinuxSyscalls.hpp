#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				class Sys {
				public:
					enum CallNumber : unsigned long long {
						EXIT = 60,
					};
				};
			};
		};
	};
};