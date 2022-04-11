// 04TemplateClassExample.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 템플릿을 사용하는 유용한 예제

#include "pch.h"
#include <iostream>

using namespace std;


template <class T, int N>
class MyArray {
	T m_arr[N] = {0};
	int m_count = N;			// 배열 초기화를 위해 몇개인지 저장

public:
	MyArray();
	~MyArray();
	void SetValue(int index, T value);
	T GetValue(int index);
};

template <class T, int N>
MyArray<T, N>::MyArray()
{
	memset(m_arr, 0, sizeof(m_arr));
}

template <class T, int N>
MyArray<T, N>::~MyArray()
{
}

template <class T, int N>
void MyArray<T, N>::SetValue(int index, const T value)
{
	m_arr[index] = value;
}

template <class T, int N>
T MyArray<T, N>::GetValue(int index)
{
	return m_arr[index];
}


int main()
{
	MyArray<wchar_t, 10> myArrayWchar;
	MyArray<double, 5> myArrayDouble;

	myArrayWchar.SetValue(1, L'b');
	myArrayDouble.SetValue(1, 3.14);

	setlocale(LC_ALL, "");

	printf("myArrayWchar[0]=%lc,  myArrayWchar[1]=%lc \n", myArrayWchar.GetValue(0), myArrayWchar.GetValue(1));
	printf("myArrayDouble[0]=%f,  myArrayDouble[1]=%f", myArrayDouble.GetValue(0), myArrayDouble.GetValue(1));
}