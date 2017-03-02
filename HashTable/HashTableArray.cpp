
#include <vector>
#include <string>

template <typename Key>
class hash {
public:
	size_t operator()(const Key& k) const;
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

template <typename HashedObj>
class HashTable {
public:
	explicit HashTable(int size = 101)
	:array(nextPrime(size)) {
		makeEmpty();
	}
	
	bool contains(const HashedObj& x) const {
		return isActive(findPos(x));
	}
	
	void makeEmpty() {
		currentSize = 0;
		for (auto& entry: array) {
			entry.info = EntryType::EMPTY;
		}
	}
	
	bool insert(const HashedObj& x) const {
		int currentPos = findPos(x);
		if (isActive(currentPos)) {
			return false;
		}
		
		array[currentPos].element = x;
		array[currentPos].info = EntryType::ACTIVE;
		
		if (++currentSize > array.size()/2) {
			rehash();
		}
		
		return true;
	}
	
	bool insert(HashedObj&& x) const {
		int currentPos = findPos(x);
		if (isActive(currentPos)) {
			return false;
		}
		
		array[currentPos].element = std::move(x);
		array[currentPos].info = EntryType::ACTIVE;
		
		if (++currentSize > array.size()/2) {
			rehash();
		}
		
		return true;
	}
	
	bool remove(const HashedObj& x) const {
		int currentPos = findPos(x);
		if (!isActive(currentPos)) {
			return false;
		}
		
		array[currentPos].info = EntryType::DELETED;
		return true;
	}
	
	enum EntryType {ACTIVE, EMPTY, DELETED};

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
	struct HashEntry {
		HashedObj element;
		EntryType info;
		
		HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EntryType::EMPTY)
		:element{e}, info{i}
		{}
		
		HashEntry(HashedObj&& e, EntryType i = EntryType::EMPTY)
		:element{std::move(e)}, info{i}
		{}
	};
	
	std::vector<HashEntry> array;
	int currentSize;
	
	bool isActive(int currentPos) const {
		return array[currentPos].info == EntryType::ACTIVE;
	}
	int findPos(const HashedObj& x) const {
		int offset = 1;
		int currentPos = myhash(x);
		
		while(array[currentPos].info != EntryType::EMPTY &&
			array[currentPos].element != x) {
			currentPos += offset;
			offset += 2;
			if(currentPos >= array.size()) {
				currentPos -= array.size();
			}		
		}
		
		return currentPos;
	}
	
	void rehash() {
		std::vector<HashEntry> oldArray = array;
		array.resize(next(2 * oldArray.size()));
		
		for (auto& entry: array) {
			entry.info = EntryType::EMPTY;
		}
		
		for (auto& entry: oldArray) {
			if (entry.info == EntryType::ACTIVE) {
				insert(std::move(entry.element));
			}
		}
	}
	
	size_t myhash(const HashedObj& x) const {
		static hash<HashedObj> hf;
		return hf(x) % array.size();
	}
	
};

int main() {
	HashTable<int> h;
}