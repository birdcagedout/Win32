// 01TemplateFunc.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
// C++ 템플릿 참고사이트 : https://www.cplusplus.com/doc/oldtutorial/templates/

#include "pch.h"
#include <iostream>

using namespace std;

// 템플릿 함수
// 선언부는 간단

template <class T>
T GetMax(T a, T b)
{
	T result;
	result = (a >= b) ? a : b;
	return result;
}

int main()
{
	int r1;
	double r2;

	// 템플릿함수가 구체적으로 실행될 때까지 만들어지지 않음
	// 특정 자료형의 템플릿함수를 호출할 때 형태도 기억할 것.
	r1 = GetMax <int> (10, 20);
	r2 = GetMax <double> (1.5, 5 / 3);

	cout << "GetMax <int>    (10, 20)   : " << r1 << endl;
	cout << "GetMax <double> (1.5, 5/3) : " << r2 << endl;
}
