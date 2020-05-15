#pragma once

#include <memory>

// ---------------------------------------
// ----- Scope (unique_ptr) --------------
// ---------------------------------------

template<class T>
using Scope = std::unique_ptr<T>;

// ---------------------------------------
// ----- Ref (shared_ptr) ----------------
// ---------------------------------------

template<class T>
using Ref = std::shared_ptr<T>;

// ---------------------------------------
// ----- Weak (weak_ptr) -----------------
// ---------------------------------------

template<class T>
using Weak = std::weak_ptr<T>;

namespace Capp {
	
	namespace Memory {
		
		template<class T, typename ...Args>
		static constexpr Scope<T> createScope(Args&& ...args) {
			return std::make_unique<T>(std::forward<Args>(args)...);
		}


		template<class T, typename ...Args>
		static constexpr Ref<T> createRef(Args&& ...args) {
			return std::make_shared<T>(std::forward<Args>(args)...);
		}
		
	}
	
}