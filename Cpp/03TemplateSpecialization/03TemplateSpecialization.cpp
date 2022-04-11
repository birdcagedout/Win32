// 03TemplateSpecialization.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

using namespace std;


// template의 문제점 ==> 상속이 안 된다.
// 기본 template의 일부(또는 전부)를 구체화하여 별도의 클래스로 구현 ==> 상속과 유사한 기능 구현
//
// 예를 들어, MyContainer class에서는 숫자(int, double 등)를 받아서 GetBigger()함수로 1증가한 값을 리턴함
// 그런데 문자(char)를 입력을 받은 경우에는 그 문자의 대문자를 리턴하는 Uppercase() 함수를 만들고자 함.
// ==> 별도의 클래스 이름으로 만들기보다, 원래 클래스 이름을 똑같이 사용하되
//     char인 경우만을 special한 case로 취급하여 새로운 template으로 재정의한 것 = Template Specialization(템플릿 특수화)


// <유의>
// 1. 특수화할 때 기본 template의 모든 멤버변수를 새로 정의해야 한다.
// 2. 템플릿이 완전 또는 부분특수화되었을 때 어느 함수가 호출될지 모호해진다. 
//    ==> 항상 비템플릿함수가 최우선, 특수화된 템플릿이 2순위, 기본 템플릿은 나중 순위
// 3. 특수화된 template은 오버로딩 되지 않는다.(문제 발생)
//    특수화 하면 안 되는 이유 : https://wikidocs.net/652


// 기본 template
template <class T>
class MyContainer {
	T m_element;

public:
	MyContainer(T a_element)
	{
		m_element = a_element;
	}
	T GetBigger()
	{
		return ++m_element;
	}
};


// template specialization
template <>
class MyContainer<char> {
	char m_element;

public:
	MyContainer(char a_element)
	{
		m_element = a_element;
	}
	char Uppercase()
	{
		int offset = 'A' - 'a';
		return (m_element + offset);
	}
};


// 선언 타입 비교

/* 기본형(generic template)
template <class T>
class MyContainer {}
*/

/* 특수화(generic template)
template <>
class MyContainer<char> {}
*/



int main()
{
	MyContainer<int> myconInt(3);
	MyContainer<char> myconChar('b');

	cout << "myconInt: " << myconInt.GetBigger() << endl;
	cout << "myconChar: " << myconChar.Uppercase() << endl;
}

