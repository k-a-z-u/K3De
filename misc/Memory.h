#ifndef MEMORY_H
#define MEMORY_H

#include <memory>

namespace std {

	template<typename T, typename ...Args>
	std::unique_ptr<T> make_unique( Args&& ...args ){
		return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
	}

	template<typename T>
	std::unique_ptr<T> make_unique(T* ptr) {
		return std::unique_ptr<T>(ptr);
	}

//		template<typename T>
//		std::unique_ptr<T> make_unique( T ){
//			return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
//		}


//	template<class T> struct _Unique_if {
//		typedef std::unique_ptr<T> _Single_object;
//	};

}

#endif // MEMORY_H
