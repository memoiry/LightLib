
#include <vector>
#include <list>
#include <string>

template <typename Container>
auto begin(Container& c) -> decltype(c.begin()) {
	return c.begin();
}

template <typename Container>
auto begin(const Container& c) -> decltype(c.begin()) {
	return c.begin();
}

template <typename Container>
auto end(Container& c) -> decltype(c.end()) {
	return c.end();
}

template <typename Container>
auto end(const Container& c) -> decltype(c.end()) {
	return c.end();
}

template <typename Key>
class hash {
public:
	size_t operator()(const Key& k) const;
};

template <typename HashedObj>
class HashTable {
public:
	explicit HashTable(int size = 101)
	:theLists(nextPrime(size))
	{
		makeEmpty();
	}
	
	bool contains(const HashedObj& x) const {
		auto& whichList = theLists[myhash(x)];
		return std::find(begin(whichList), end(whichList), x) != end(whichList);
	}
	
	void makeEmpty() {
		currentSize = 0;
		for (auto& thisList: theLists) {
			thisList.clear();
		}
	}
	
	bool insert(const HashedObj& x) {
		auto& whichList = theLists[myhash(x)];
		if (std::find(begin(whichList), end(whichList), x) != end(whichList)) {
			return false;
		}
		whichList.push_back(x);
		
		if (++currentSize > theLists.size()) {
			rehash();
		}
		
		return true;
	}
	
	bool insert(HashedObj&& x) {
		auto& whichList = theLists[myhash(x)];
		if (std::find(begin(whichList), end(whichList), x) != end(whichList)) {
			return false;
		}
		whichList.push_back(std::move(x));
		
		if (++currentSize > theLists.size()) {
			rehash();
		}
		
		return true;
	}
	
	bool remove(const HashedObj& x) {
		auto& whichList = theLists[myhash(x)];
		auto itr = std::find(begin(whichList), end(whichList), x);
		
		if (itr == end(whichList)) {
			return false;
		}
		
		whichList.erase(itr);
		--currentSize;
		return true;
	}
protected:
	unsigned long long nextPrime(unsigned long long i) {
		int flag = 0;
		while(true) {
			for (int j = 2; j < i; j++) {
				if (i % j == 0) {
					flag = 1;
					break;
				}
			}
			if (flag) {
				flag = 0;
				i++;
			} else {
				break;
			}
		}
		return i;
	}
	
private:
	std::vector<std::list<HashedObj>> theLists;
	int currentSize;
	
	void rehash() {
		std::vector<std::list<HashedObj>> oldLists = theLists;
		theLists.resize(nextPrime(2 * theLists.size()));
		for (auto& thisList: theLists) {
			thisList.clear();
		}
		
		for (auto& thisList: oldLists) {
			for (auto& x: thisList) {
				insert(std::move(x));
			}
		}
	}
	
	size_t myhash(const HashedObj& x) const {
		static hash<HashedObj> hf;
		return hf(x) % theLists.size();
	}
};

class Employee {
public:
	const std::string& getName() const {
		return this->name;
	}
	
	bool operator==(const Employee& rhs) const {
		return getName() == rhs.getName();
	}
	
	bool operator!=(const Employee& rhs) const {
		return !(*this == rhs);
	}
	
private:
	std::string name;
	double salary;
	int seniority;
};

template <>
class hash<std::string> {
public:
	size_t operator()(const std::string& key) {
		size_t hashVal = 0;
		
		for (char ch: key) {
			hashVal = 37 * hashVal + ch;
		}
		
		return hashVal;
	}
};

template <>
class hash<Employee> {
public:
	size_t operator()(const Employee& item) {
		static hash<std::string> hf;
		return hf(item.getName());
	}
};

int main() {
	HashTable<std::string> h;
}