/*************************************************************************
    > File Name: asan_demo.cpp
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 14 May 2024 03:13:53 PM CST
 ************************************************************************/

#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = {1, 2, 3};

    // 故意越界访问
    std::cout << "Element at index 3: " << vec[3] << std::endl;

    // 创建一个野指针
    int *ptr = new int[3];
    delete[] ptr;
    std::cout << "Dereferencing freed memory: " << *ptr << std::endl; // 这将是一个错误

    return 0;
}
