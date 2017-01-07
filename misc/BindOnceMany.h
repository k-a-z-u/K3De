// helper methods to ensure the bind() method for one type [shader, texture, vbo, ...]
// is not called too often as those ones are quite costly

#ifdef WITH_ASSERTIONS

inline std::set<unsigned int>& bound() const {
	static std::set<unsigned int> set;
	return set;
}

inline void setBound(const unsigned int id) const {
	bound().insert(id);
}

inline void setUnbound(const unsigned int id) const {
	bound().erase(id);
}

inline bool isBound(const unsigned int id) const {
	return bound().find(id) != bound().end();
}

#else

inline void setBound(const unsigned int) const {;}
inline void setUnbound(const unsigned int) const {;}

#endif
