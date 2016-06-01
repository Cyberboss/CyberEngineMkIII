#pragma once
namespace CYB {
	namespace Platform {
		namespace Modules {
			namespace Implementation {
				class Module {
				protected:
					void* FModule;
				protected:
					Module(const API::String::CStyle& AModuleName);
					Module(const Module&) = delete;
					Module(Module&& AMove);
					Module& operator=(Module&& AMove);
					~Module();
				};
			};
		};
	};
};

