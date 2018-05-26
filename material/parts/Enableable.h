#ifndef ENABLEABLE_H
#define ENABLEABLE_H

class Enableable {

private:

	bool enabled;

public:

	void setEnabled(const bool enabled) {this->enabled = enabled;}

	bool isEnabled() const {return enabled;}

};

#endif // ENABLEABLE_H
