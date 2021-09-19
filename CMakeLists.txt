cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Cache_Q2)

add_executable(Cache_Q2 source/Cache.cpp source/Tests.cpp)

enable_testing()

add_test(FirstTest Cache_Q2 ../source/Tests/input_1.txt ../source/Tests/answer_1.txt)
add_test(SecondTest Cache_Q2 ../source/Tests/input_2.txt ../source/Tests/answer_2.txt)
add_test(ThirdTest Cache_Q2 ../source/Tests/input_3.txt ../source/Tests/answer_3.txt)
add_test(FourthTest Cache_Q2 ../source/Tests/input_4.txt ../source/Tests/answer_4.txt)
add_test(FifthTest Cache_Q2 ../source/Tests/input_5.txt ../source/Tests/answer_5.txt)
