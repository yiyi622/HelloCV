cmake_minimum_required(VERSION 3.8)
project(demo_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#1.找依赖的库
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

#2.添加可执行文件
add_executable(cpp_node src/cpp_node.cpp)
add_executable(person_node src/person_node.cpp)
#后面写的是相对路径，这意味着scr前面不能加/，否则代表的是从根目录开始的绝对路径了

#打印信息
#message(STATUS ${rclcpp_INCLUDE_DIRS})
#message(STATUS ${rclcpp_LIBRARIES})

#3.链接库和可执行文件
#target_include_directories(cpp_node PUBLIC ${rclcpp_INCLUDE_DIRS})
#target_link_libraries(cpp_node ${rclcpp_LIBRARIES})
ament_target_dependencies(cpp_node rclcpp)
ament_target_dependencies(person_node rclcpp)

  #ament是ros2专用的构建工具,就是从上面找包ament_cmake里添加进来的
  #它可以代替上面两个以前用的target指令，效果一样。
  #后续还想加什么东西，上面加找包，ament括号里再往后加个包名字就好了


#4.安装到install目录里
#*****
install(TARGETS cpp_node person_node
  DESTINATION lib/${PROJECT_NAME})#依然相对路径
# ${变量名} 是变量引用语法，表示获取这个变量的值
#*****

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()#表示ament_cmake指令结束，要放到最后，否则后面有些用到ament相关的指令可能不执行
