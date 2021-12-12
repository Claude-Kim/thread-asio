// quick_sort

#include <list>
#include <iostream>

template<typename T>
void print_sort(T data)
{
    for(auto num : data) 
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
}

template<typename T>
std::list<T> quick_sort(std::list<T> input)
{
    
    if(input.empty()) // 재귀호출의 끝단
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin()); // input의 첫번째 항목을 잘라서 result의 첫번째에 옮김

    T const& pivot=*result.begin();  // result의 첫번째 항목의 값

    auto divide_point=std::partition(input.begin(), input.end(),
        [&](T const& t){return t<pivot;}); // result의 첫번째 항목의 값보다 작으면 앞으로, 크면 뒤로 재배열
                                           // return으로 앞 파트와 뒤 파트의 경계점 이터레이터  
    
    std::list<T> lower_part;    
    lower_part.splice(lower_part.end(),input,input.begin(),divide_point); // 경계선으로 lower파트 잘라감

    auto new_lower(quick_sort(std::move(lower_part))); // 재귀호출 작은 부분
    auto new_higher(quick_sort(std::move(input))); // 재귀호출 큰 부분

    result.splice(result.end(),new_higher) ; // 개속 갈라져서 잘라지면서 정렬된 것들을 회수
    result.splice(result.begin(),new_lower); 

    return result;
    
}


int main()
{
    std::list<int> data{5,8,7,10,1,3,4,2,6,9};

    //std::list<int> result=
    print_sort(quick_sort(data));

    //print_sort(result);
}