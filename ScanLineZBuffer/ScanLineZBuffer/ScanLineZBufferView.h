
// ScanLineZBufferView.h : CScanLineZBufferView 类的接口
//

#pragma once

#include "ScanLineZBufferCore/ScanLineZBufferStructure.h"
#include "ScanLineZBufferCore/ScanLineZBufferCore.h"

class CScanLineZBufferView : public CView
{
protected: // 仅从序列化创建
	CScanLineZBufferView();
	DECLARE_DYNCREATE(CScanLineZBufferView)

// 特性
public:
	CScanLineZBufferDoc* GetDocument() const;

// 自定义操作
public:
	void InitMeshInfo(PolyMesh &mesh);
	void DestoryMeshInfo();
	void CalcModelInfo(); // 计算模型的中心位置和尺寸 
	void FocusModel(); // 模型放在视口中
protected:
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
	BOOL InitializeGL();
	void SetBackgroundColor();
// 自定义变量
private:
	int m_GLPixelIndex; // 像素格式的索引值 
	HGLRC m_hGLContext; // 绘制环境，HGLRC指向一个rendering context句柄
	ScanLineZBufferCore *pZBufferCore; // ZBuffer算法核心
	double m_z_delta; // 模拟放在世界坐标系的深度
	Point m_centerPoint; // 模型的中间位置
	double m_modelScale; // 模型尺寸
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CScanLineZBufferView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ScanLineZBufferView.cpp 中的调试版本
inline CScanLineZBufferDoc* CScanLineZBufferView::GetDocument() const
   { return reinterpret_cast<CScanLineZBufferDoc*>(m_pDocument); }
#endif

