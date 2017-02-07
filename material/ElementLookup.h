#ifndef ELEMENTLOOKUP_H
#define ELEMENTLOOKUP_H

#include <vector>
#include <string>
#include <unordered_set>

/**
 * add anything with the method getID()
 * for later look-ups using this ID
 */
template <typename T> class ElementLookup {

private:

	/** set containing all used IDs */
	std::unordered_set<std::string> usedIDs;

	/** all added elements */
	std::vector<T> elements;

public:

	void add(T element) {
		elements.push_back(element);
		usedIDs.insert(IDProvider<T>::getID(element));
	}

	const std::vector<T>& all() const {
		return elements;
	}

	size_t size() const {
		return elements.size();
	}

	T byID(const std::string& id) const {
		for(const T& elem : elements) {
			const std::string& elemID = IDProvider<T>::getID(elem);
			if (id == elemID) {return elem;}
		}
		throw Exception("element not found: " + id);
	}

private:

	template <typename Elem> struct IDProvider {
		static std::string getID(Elem e) {return e.getID();}
	};

	template <typename Elem> struct IDProvider<Elem*> {
		static std::string getID(Elem* e) {return e->getID();}
	};

};

#endif // ELEMENTLOOKUP_H
