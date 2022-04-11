// ExTemplate.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 
// C++ 템플릿 클래스 : https://www.cplusplus.com/doc/oldtutorial/templates/


#include "pch.h"
#include <stdio.h>
#include <iostream>

using namespace std;

// 템플릿이 필요한 이유 : 내부 자료형에 따라 별도의 클래스 정의가 필요함

class IntData
{
private:
	int m_data;

public:
	void SetData(int a_data)
	{
		m_data = a_data;
	}

	int GetData()
	{
		return m_data;
	}
};


class DoubleData
{
private:
	double m_data;

public:
	void SetData(double a_data)
	{
		m_data = a_data;
	}

	double GetData()
	{
		return m_data;
	}
};


// Template 1: 정의
// 내부 자료형에 따라 클래스를 따로 만들지 않고
// 자료형이 int일 때나 double일 때 모두 사용 가능

template <class T>
class MyData_1
{
private:
	T m_data1;

public:
	void SetData(T a_data)
	{
		m_data1 = a_data;
	}

	T GetData()
	{
		return m_data1;
	}

	// 외부선언용
	void SetDataExt(T a_data);
	T GetDataExt();
};

MyData_1<int> a;
MyData_1<double> b;



// Template 2: 멤버함수 외부 정의
// 멤버함수를 클래스 외부에서 정의할 때 ==> 형태 기억할 것.
// 특히 클래스이름 뒤 "<템플릿파라미터 이름>"이 붙는다는 것!

template <class T>
void MyData_1<T>::SetDataExt(T a_data)
{
	m_data1 = a_data;
}

template <class T>
T MyData_1<T>::GetDataExt()
{
	return m_data1;
}




// Template 3: 템플릿 파라미터가 2개 이상인 경우 
template <class T, class U>
class MyData
{
private:
	T m_data1;
	U m_data2;

public:
	void SetData1(T a_data)
	{
		m_data1 = a_data;
	}

	T GetData1()
	{
		return m_data1;
	}

	void SetData2(U a_data);
	U GetData2();
};

template <class T, class U>
void MyData<T, U>::SetData2(U a_data)
{
	m_data2 = a_data;
}

template <class T, class U>
U MyData<T, U>::GetData2()
{
	return m_data2;
}


int main()
{
	MyData <int, double> m1;
	MyData <double, int> m2;

	m1.SetData1(5);
	m2.SetData1(6.0);

	printf("m1(int)= %d, m2(double)=%f\n", m1.GetData1(), m2.GetData1());

	m1.SetData2(5.0);
	m2.SetData2(6);

	printf("m1(double)= %f, m2(int)=%d\n", m1.GetData2(), m2.GetData2());
}
