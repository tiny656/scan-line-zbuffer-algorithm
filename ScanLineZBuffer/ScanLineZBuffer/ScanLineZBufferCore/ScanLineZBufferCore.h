#ifndef __SCANLINEZBUFFERCORE_H__
#define __SCANLINEZBUFFERCORE_H__

#include "../utility.h"
#include "ScanLineZBufferStructure.h"

class ScanLineZBufferCore 
{

public:
	ScanLineZBufferCore(PolyMesh &mesh);
	~ScanLineZBufferCore();
	void Run();
	void SetScene();
	void ProjectModelToScene();
	void BuildPolygonAndEdgeTable();
	void ScanLineZBufferAlgorithm();
	void DrawFrame();

//辅助函数
	void addEdgeToActiveEdgeTable(int y, const PolygonTableElem *pte, int addType);

private:
	PolyMesh *pMesh;
	int m_viewport[4];		// 视口矩阵, [0, 0, width, height]
	double m_modelview[16];	// 模型变换矩阵
	double m_projection[16];	// 投影矩阵
	std::vector<Point> m_projectionVertex;	// 投影坐标

	double *zBuffer;// zBuffer数组
	ColorType *colorBuffer; // 颜色数组
	GLubyte *frame; // 当前图像 
	std::vector<std::vector<PolygonTableElem>> m_polygonTable; // 分类多边形表
	std::vector<std::vector<EdgeTableElem>> m_edgeTable; // 分类边表
	std::vector<PolygonTableElem> m_activePolygonTable; // 活化多边形表

};


#endif // __SCANLINEZBUFFERCORE_H__