#ifndef __SCANLINEZBUFFER_STRUCTURE_H__
#define __SCANLINEZBUFFER_STRUCTURE_H__


// 点定义
struct Point {
	double x, y, z;
};


// 颜色定义
struct ColorType {
	GLubyte r, g, b, a;
};

// 活化边表 元素, 单独记录一个边表元素
struct ActiveEdgeTableElem
{
	double xl; // 左交点的x坐标
	double dxl; // (左交点边上)两相邻扫描线交点的x坐标之差
	int dyl; // 以和左交点所在边相交的扫描线数为初值，以后向下每处理一条扫描线减1
	// 右边的交点的三个对应分量；含义与左边交点相同
	// double xr;
	// double dxr;
	// int dyr;
	double zl; // 左交点处多边形所在平面的深度值
	double dzx; // 沿扫描线向右走过一个像素时，多边形所在平面的深度增量。对于平面方程，dzx = -a/c(c!=0)
	double dzy; // 沿y方向向下移过一根扫描线时，多边形所在平面的深度增量。对于平面方程，dzy = b/c(c!=0)
	int id; // 交点对所在的多边形的编号
};

// 分类多边形表 || 活化多边形表 元素
struct PolygonTableElem
{
	double a, b, c, d; // 多边形所在的平面方程系数,ax+by+cz+d=0
	int id; // 多边形的编号
	int dy; // 多边形跨越的剩余扫面线数目 
	std::vector<ActiveEdgeTableElem> activeEdgeTable; // 针对活化多边形表中的活化边表
	ColorType color;
};

// 分类边表 元素
struct EdgeTableElem
{
	double x; // 边的上端点的x坐标
	double dx; // 相邻两条扫描线交点的x坐标差dx (-1/k)
	int dy; // 边跨越的扫描线数目
	int id; // 边所属多边形的编号
};


#endif //  __SCANLINEZBUFFER_STRUCTURE_H__
