#include"kws_cir_buff.hpp"
#include<iostream>
/*
 * compile：
g++ kws_lock.cpp  kws_cir_buf_test.cc -o main -lpthread
./main 

It will print as follows：

print should be: 1.1 2 3
print is:        1.1 2 3 

print should be: 2 3
print is:        2 3 

print should be: 2 3
print is:        2 3 

print should be: 2 3 5 6 7
print is:        2 3 5 6 7 

print should be: 5 6 7 3 4 5
print is:        5 6 7 3 4 5 

print should be: 5 6 7 3
print is:        5 6 7 3 

print should be: 4 5
print is:        4 5

*/

void print(float* p,int size){
  for(int i=0;i<size;++i){
    std::cout<<p[i]<<" ";
  }
  std::cout<<std::endl;
}
void famat_print(const char* s){
  std::cout<<std::endl;
  std::cout<<"print should be: "<< s <<std::endl;
  std::cout<<"print is:        ";
}
int main(){
  kws_cir_buf<float> pbuf(7);
  float arr[10]={1.1,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};
  float tmp[4];
  pbuf.write_buf(arr,3);

  famat_print("1.1 2 3");
  pbuf.print();

  pbuf.pop_buf(1);
  famat_print("2 3");
  pbuf.print();

  pbuf.read_buf(tmp,2); 
  famat_print("2 3");
  print(tmp, 2);

  pbuf.write_buf(arr+4,3);
  famat_print("2 3 5 6 7");
  pbuf.print();

  pbuf.pop_buf(2);
  pbuf.write_buf(arr+2,3);
  famat_print("5 6 7 3 4 5");
  pbuf.print();

  pbuf.read_buf(tmp,4);
  famat_print("5 6 7 3");
  print(tmp, 4);

  pbuf.pop_buf(4);
  famat_print("4 5");
  pbuf.print();
}
