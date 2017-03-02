#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

class UniformRandom {
public:
	int nextInt(int i) const {
		return rand() + i;
	}
	
	int nextInt() const {
		return rand();
	}
};

template <typename AnyType>
class CuckooHashFamily {
public:
	size_t hash(const AnyType& x, int which) const;
	int getNumberOfFunctions();
	void generateNewFunctons();
};

template <int count>
class StringHashFamily {
public:
	StringHashFamily()
	:MULTIPLIERS(count)
	{
		generateNewFunctions();
	}
	
	int getNumberOfFunctions() const {
		return count;
	}
	
	void generateNewFunctions() {
		for(auto& mult: MULTIPLIERS) {
			mult = r.nextInt();
		}
	}
	
	size_t hash(const std::string& x, int which) const {
		const int multiplier = MULTIPLIERS[which];
		
		size_t hashVal = 0;
		for (char ch: x) {
			hashVal = 37 * hashVal + ch;
		}
		
		return hashVal;
	}

private:
	std::vector<int> MULTIPLIERS;
	UniformRandom r;
};

template <typename AnyType, typename HashFamily>
class CuckooHashTable {
public:
	explicit CuckooHashTable(int size = 101)
	:array(nextPrime(size)) {
		numHashFunctions = hashFunctions.getNumberOfFunctions();
		rehashes = 0;
		makeEmpty();
	}
	
	void makeEmpty() {
		currentSize = 0;
		for(auto& entry: array) {
			entry.isActive = false;
		}
	}
	
	bool contains(const AnyType& x) const {
		return findPos(x) != -1;
	}
	
	bool remove(const AnyType& x) {
		int currentPos = findPos(x);
		if(!isActive(currentPos)) {
			return false;
		}
		
		array[currentPos].isActive = false;
		--currentSize;
		return true;
	}
	
	bool insert(const AnyType& x) {
		if (contains(x)) {
			return false;
		}
		
		if (currentSize >= array.size() * MAX_LOAD) {
			expand();
		}
		
		return insertHelper1(x);
	}
	
	bool insert(AnyType&& x) {
		if (contains(x)) {
			return false;
		}
		
		if (currentSize >= array.size() * MAX_LOAD) {
			expand();
		}
		
		return insertHelper1(std::move(x));
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
	struct HashEntry {
		AnyType element;
		bool isActive;
		
		HashEntry(const AnyType& e = AnyType{}, bool a = false)
		:element{e}, isActive{a}
		{}
		HashEntry(AnyType&& e, bool a = false)
		:element{std::move(e)}, isActive{a}
		{}
	};
	
	bool insertHelper1(const AnyType& xx) {
		const int COUNT_LIMIT = 100;
		AnyType x = xx;
		
		while(true) {
			int lastPos = -1;
			int pos;
			for (int count = 0; count < COUNT_LIMIT; ++count) {
				for (int i = 0; i < numHashFunctions; i++) {
					pos = myhash(x, i);
					
					if (!isActive(pos)) {
						array[pos] = std::move(HashEntry{std::move(x), true});
						++currentSize;
						return true;
					}
				}
				
				int i = 0;
				do {
					pos = myhash(x, r.nextInt(numHashFunctions));
				} while(pos == lastPos && i++ < 5);
				
				lastPos = pos;
				std::swap(x, array[pos].element);
			}
			
			if (++rehashes > ALLOWED_REHASHES) {
				expand();
				rehashes = 0;
			} else {
				rehash();
			}
		}
	}
	
	bool insertHelper1(AnyType&& xx) {
		const int COUNT_LIMIT = 100;
		AnyType x = std::move(xx);
		
		while(true) {
			int lastPos = -1;
			int pos;
			for (int count = 0; count < COUNT_LIMIT; ++count) {
				for (int i = 0; i < numHashFunctions; i++) {
					pos = myhash(x, i);
					
					if (!isActive(pos)) {
						array[pos] = std::move(HashEntry{std::move(x), true});
						++currentSize;
						return true;
					}
				}
				
				int i = 0;
				do {
					pos = myhash(x, r.nextInt(numHashFunctions));
				} while(pos == lastPos && i++ < 5);
				
				lastPos = pos;
				std::swap(x, array[pos].element);
			}
			
			if (++rehashes > ALLOWED_REHASHES) {
				expand();
				rehashes = 0;
			} else {
				rehash();
			}
		}
	}
	
	bool isActive(int currentPos) const {
		return array[currentPos].isActive == true;
	}
	
	int myhash(const AnyType& x, int which) const {
		return hashFunctions.hash(x, which) % array.size();
	}
	
	int findPos(const AnyType& x) const {
		for (int i = 0; i < numHashFunctions; ++i) {
			int pos = myhash(x, i);
			
			if (isActive(pos) && array[pos].element == x) {
				return pos;
			}
		}
		
		return -1;
	}
	
	void expand() {
		rehash(static_cast<int>(array.size() / MAX_LOAD));
	}
	void rehash() {
		hashFunctions.generateNewFunctions();
		rehash(array.size());
	}
	void rehash(int newSize) {
		std::vector<HashEntry> oldArray = array;
		array.resize(nextPrime(newSize));
		
		for(auto& entry: array) {
			entry.isActive = false;
		}
		
		for(auto& entry: oldArray) {
			if(isActive(entry.isActive)) {
				insert(std::move(entry.element));
			}
		}
	}
	
	static constexpr double MAX_LOAD = 0.40;
	static constexpr int ALLOWED_REHASHES = 5;
	
	std::vector<HashEntry> array;
	int currentSize;
	int numHashFunctions;
	int rehashes;
	UniformRandom r;
	HashFamily hashFunctions;
};

int main() {
	CuckooHashTable<std::string, StringHashFamily<101>> cuckoo;
	cuckoo.insert("aaa");
}