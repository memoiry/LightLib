#include <iostream>
#include <set>
#include <string>
#include <unordered_set>

class CaseInsensitiveStringHash {
public:
	size_t operator()(const std::string& s) const {
		static std::hash<std::string> hf;
		return hf(toLower(s));
	}
	
	bool operator()(const std::string& lhs, const std::string& rhs) const {
		return equalsIgnoreCase(lhs, rhs);
	}
protected:
	std::string toLower(const std::string& rhs) const {
		std::string str = rhs;
		for (auto& ch: str) {
			ch = tolower(ch);
		}
		
		return str;
	}
	
	bool equalsIgnoreCase(const std::string& lhs, const std::string& rhs) const {
		return toLower(lhs) == toLower(rhs);
	}
};

using namespace std;
int main(int argc, char *argv[]) {
	std::unordered_set<std::string, CaseInsensitiveStringHash, CaseInsensitiveStringHash> s;
}