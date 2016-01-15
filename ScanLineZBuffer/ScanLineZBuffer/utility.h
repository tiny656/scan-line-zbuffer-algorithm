#ifndef __UTILITY_H__
#define __UTILITY_H__
#define __GXX_EXPERIMENTAL_CXX0X__
#undef min 
#undef max
#define NOMINMAX
#define mmin(a, b) ((a)<(b)?(a):(b))
#define mmax(a, b) ((a)>(b)?(a):(b))
// Windows
#include <windows.h>

// OpenGL
#include <gl/GL.h>
#include <gl/GLU.h>

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif

// OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Handles.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<> PolyMesh;

/**
* 公共功能函数
*/

/**
* unsigned short 转换为 string
* @todo
* @param WORD(unsigned short) value, 传入要进行转换的值.
* @return string, 返回转换后的值.
*/
std::string uInt2String(WORD value);

/**
* 返回当前时间
* @todo
* @param, empty.
* @return string, 返回格式化当前时间.
*/
std::string logTime();

/**
* 对浮点数val四舍五入
* @todo
* @param double, 要进行四舍五入的值
* @return int, 返回四舍五入后结果
*/
int convertToInt(double val);


#endif // __UTILITY_H__