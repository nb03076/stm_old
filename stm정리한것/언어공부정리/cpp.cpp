여기서 인덱스는 인덱스 0번부터 시작하는것을 말함
str[i] = value에서 str초기화 안해주면 값못넣어...
===================================================
2주차

getline(cin,str,'a') a문자 앞까지만 str에 넣음

string method
empty size length capacity erase(index start, index end) 
clear swap(스왑대상) toupper tolower(글자 하나씩만 변환가능)
isdigit isalpha
front back   str.front() = '2'; 처럼 치환가능  문자 하나 반환
pop_back() push_back('a')  스택이라고 생각하면됨
append("bbb") 문자열만 사용가능. string class +오퍼레이터와 다른점은 char 삽입가능 여부
insert(index, "삽입할 문자열")  문자 삽입함
replace(index,넣을 갯수,"삽입할 문자열")  문자 교체함
substr(index) index부터 끝까지 반환
substr(index,size) index부터 size갯수만큼만 반환
find("문자열") ex) "fff"면 fff가 처음나오는 index반환. 없다면 -1 반환
find("문자열",시작 index) 어느 index부터 찾을지 정할 수 있음

index가 필요하면 첫번째 인자에 index부터...

stoi(str)
string str = "2000";
int a = stoi(str);

to_string
int a = 12;
string str;
str = to_string(a);


c=str[1] 이랑 c=str.at(1) 동일



===================================================
3주차

위임생성자 : 
Circle::Circle() : Circle(1) { } // 위임 생성자
Circle::Circle(int r) { // 타겟 생성자
radius = r;
cout << "반지름 " << radius << " 원 생성" << endl;
}

여기서 중요한게 내가 Circle::Circle() 해야하는데
그냥 Circle::Circle로 해버림... 무조건 argument도 신경써줘야함!


초기화 방법
Point::Point(int a, int b) { x = a; y = b; }
Point::Point(int a, int b) : x(a), y(b) {}

기본생성자 : 생성자 하나라도 안만들었을 경우 기본생성자 만들어짐


생성자 생성했던 거랑 반대로 소멸자 호출(스택개념)


생성자 하나라도 만들면 기본생성자 안만들어지기 때문에 이부분 조심...

파생클래스에서 부모꺼의 생성자는 default가 기본생성자를 불러와.
위임 생성자를 호출하게 하고싶다면
B(int x):A(x+3) 이런식으로 직접 호출해야함
꼭 이렇게 해야한다. 조심하자

====================================================


learncpp정리


static_cast<int> 명시적 변환

using Distance = double; c++에서는 typedef 대신사용
using FcnType = int(*)(double, char);

오버로딩 인자는 int랑 const int랑 같은취급하는듯

T,U,V...이렇게 명명함
template <typename T> // this is the template parameter declaration
T max(T x, T y) // this is the function template definition for max<T>
{
    return (x > y) ? x : y;
}




display period 480	270
back porch 43	12
front 8	4
pulse width(hsync) 1	10 