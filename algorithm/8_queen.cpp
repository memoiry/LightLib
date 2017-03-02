#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <climits>
#include <stack>
#include <queue>
#include <vector>
#include <set>
#include <map>
#include <list>

#define GET_BIT(n, i) (((n) & (1 << ((i)-1))) >> ((i)-1)) // i start from 1
#define SET_BIT(n, i) ((n) | (1 << ((i)-1)))
#define CLR_BIT(n, i) ((n) & ~(1 << ((i)-1)))
#define SHOW_A(x) {cout << #x << " = " << x << endl;}
#define SHOW_B(x, y) {cout << #x << " = " << x << ", " << #y << " = " << y << endl;}
#define SHOW_C(x, y, z) {cout << #x << " = " << x << ", " << #y << " = " << y << ", " << #z << " = " << z << endl;}
#define REACH_HERE {cout << "REACH_HERE! line: " << __LINE__ << endl;}

const double E = 1e-8;
const double PI = acos(-1);

using namespace std;

const int MAXN = 100;
int P[9], hashTable[9];
int res[MAXN];
int count_ = 1;

void generate(int index){
	if (index == 9){
		int temp = 0;
		for(int i = 1; i <= 8; i++){
			temp = temp * 10 +P[i];
		}
		res[count_] = temp;
		count_++;
	}
	
	for(int x = 1; x <= 8; x++){
		if (hashTable[x] == false){
			bool flag = true;
			for(int pre = 1; pre < index;pre ++){
				if( abs(pre - index) == abs(x - P[pre])){
					flag = false;
				}
			}
			if (flag){
				P[index] = x;
				hashTable[x] = true;
				generate(index +1);
				hashTable[x] = false;
			}
		}
	}
}
int main() {
	int n;
	scanf("%d", &n);
	generate(1);
	int temp=0;
	for (int i = 0; i < n;i++){
		scanf("%d", &temp);
		printf("%d\n", res[temp]);
	}
	return 0;
}