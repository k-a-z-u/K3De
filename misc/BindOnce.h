// helper methods to ensure the bind() method for one type [shader, texture, vbo, ...]
// is not called too often as those ones are quite costly

#ifdef WITH_ASSERTIONS

inline unsigned int& bound() const {
	static unsigned int id = -1;
	return id;
}

inline void setBound(const unsigned int id) const {
	bound() = id;
}

inline void setUnbound(const unsigned int id) const {
	bound() = -1;
}

inline bool isBound(const unsigned int id) const {
	return bound() == id;
}

#else

inline void setBound(const unsigned int) const {;}
inline void setUnbound(const unsigned int) const {;}

#endif
