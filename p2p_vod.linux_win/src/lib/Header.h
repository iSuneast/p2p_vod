#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include <queue>
#include <stack>
#include <cassert>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#define POW2(k) (1U<<(k))
#define POW2L(k) (1ULL<<(k))
#define BIT_AND(a, b) (bool)(POW2(a) & (b))
#define BIT_OR(a, b) (bool)(POW2(a) | (b))
#define INF 0x3f3f3f3f
#define EPS 1e-10

using namespace std;

const double PI = acos(-1.0);

typedef pair<int, int> pii;
typedef long long LL;
typedef unsigned long long ULL;

template<class T1, class T2> bool ChkMax(T1 &a, const T2 &b) { if (a < b) { a = b; return true; } return false; }
template<class T1, class T2> bool ChkMin(T1 &a, const T2 &b) { if (a > b) { a = b; return true; } return false; }
