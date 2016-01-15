
// ScanLineZBufferView.cpp : CScanLineZBufferView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ScanLineZBuffer.h"
#endif

#include "ScanLineZBufferDoc.h"
#include "ScanLineZBufferView.h"

// my header files
#include "utility.h"
#include "ScanLineZBufferCore/ScanLineZBufferStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanLineZBufferView

IMPLEMENT_DYNCREATE(CScanLineZBufferView, CView)

BEGIN_MESSAGE_MAP(CScanLineZBufferView, CView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CScanLineZBufferView 构造/析构

CScanLineZBufferView::CScanLineZBufferView()
{
	// TODO:  在此处添加构造代码
	this->m_GLPixelIndex = 0;
	this->m_hGLContext = NULL;
	this->pZBufferCore = NULL;
	this->m_centerPoint.x = this->m_centerPoint.y = this->m_centerPoint.z = 0;
	this->m_modelScale = 0;
	this->m_z_delta = 0;
	// std::cout << logTime() << " View构造函数" << std::endl;
}

CScanLineZBufferView::~CScanLineZBufferView()
{
	// std::cout << logTime() << " View析构函数" << std::endl;
}

BOOL CScanLineZBufferView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	// std::cout << logTime() << " " << "PreCreateWindow" << std::endl;
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	return CView::PreCreateWindow(cs);
}

// CScanLineZBufferView 绘制

void CScanLineZBufferView::OnDraw(CDC* /*pDC*/)
{
	// std::cout << logTime() << " OnDraw" << std::endl;
	CScanLineZBufferDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// std::cout << logTime() << "模型信息" << pDoc->m_mesh.n_vertices() << " " << pDoc->m_mesh.n_edges() << " " << pDoc->m_mesh.n_faces() << std::endl;

	// 绘制模型
	// 如果未载入模型
	if (pZBufferCore == NULL) {
		// 清空背景色为黑色
		SetBackgroundColor();
		return;
	}
	pZBufferCore->Run();
	SwapBuffers(wglGetCurrentDC());
}


// CScanLineZBufferView 诊断

#ifdef _DEBUG
void CScanLineZBufferView::AssertValid() const
{
	CView::AssertValid();
}

void CScanLineZBufferView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScanLineZBufferDoc* CScanLineZBufferView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanLineZBufferDoc)));
	return (CScanLineZBufferDoc*)m_pDocument;
}
#endif //_DEBUG


// CScanLineZBufferView 消息处理程序

void CScanLineZBufferView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// std::cout << logTime() << " " << "OnSize" << std::endl;
	// 设置视口
	glViewport(0, 0, cx, cy);
}

void CScanLineZBufferView::OnDestroy()
{
	CView::OnDestroy();

	if (wglGetCurrentContext())
	{
		wglMakeCurrent(NULL, NULL);
	}
	if (this->m_hGLContext)
	{
		wglDeleteContext(this->m_hGLContext);
		this->m_hGLContext = NULL;
	}
}

void CScanLineZBufferView::OnMouseMove(UINT nFlags, CPoint point)
{
	// std::cout << logTime() << " 当前屏幕坐标点:" << point.x << " " << point.y << std::endl;
	// 没有导入模型
	if ( GetDocument()->m_mesh.n_vertices() <= 0)	return;
	
	// 处理模型旋转事件
	// 保存上一次点击位置
	
	static CPoint prevPointPos(-1, -1);
	if (nFlags & MK_LBUTTON)
	{
		// std::cout << logTime() << " " << "Model Move" << std::endl;
		// std::cout << "(" << prevPointPos.x << ", " << prevPointPos.y << ")" << " " << "(" << point.x << ", " << point.y << ")" << std::endl;
		if (prevPointPos.x < 0 || prevPointPos.y < 0) prevPointPos = point; // 处理一次点击
		// 旋转模型
		double x = prevPointPos.x - point.x;
		double y = prevPointPos.y - point.y;
		double modelview_matrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslated(0., 0., m_z_delta); // 移动到原点
		glRotated(sqrt((double)(x*x + y*y)), y, x, 0.0); // 旋转
		glTranslated(0., 0., -m_z_delta); // 移回原处
		glMultMatrixd(modelview_matrix);
		prevPointPos = point;
		RedrawWindow();
	}
	else
	{
		prevPointPos.x = prevPointPos.y = -1;
	}
	
	// CView::OnMouseMove(nFlags, point);
}

BOOL CScanLineZBufferView::OnEraseBkgnd(CDC* pDC)
{
	// return CView::OnEraseBkgnd(pDC);
	// 取消背景擦除，防止闪烁
	// std::cout << logTime() << " " << "OnEraseBkgnd" << std::endl;
	return TRUE;
}




void CScanLineZBufferView::OnTimer(UINT_PTR nIDEvent)
{
	// 状态指示器，更新模型的顶点数,面数
	// 如果没有载入模型
	char info[256];
	CString infoCString;
	CStatusBar* barPtr = (CStatusBar*)(AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR));
	if (barPtr) {
		if (GetDocument()->m_mesh.n_vertices() <= 0) {
			sprintf_s(info, "No mesh model data!");
		}
		else {
			sprintf_s(info, "vertices: %d  faces: %d", GetDocument()->m_mesh.n_vertices(), GetDocument()->m_mesh.n_faces());
		}
		infoCString = info;
		barPtr->SetPaneText(0, (LPCTSTR)infoCString);
	}
	CView::OnTimer(nIDEvent);
}


int CScanLineZBufferView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 得到一个窗口对象
	HWND hwnd = this->GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);

	if (!this->SetWindowPixelFormat(hdc)) 
	{
		return 0;
	}
	if (!this->CreateViewGLContext(hdc))
	{
		return 0;
	}
	if (!this->InitializeGL())
	{
		return 0;
	}
	SetTimer(1, 500, NULL);
	return 0;
}

BOOL CScanLineZBufferView::SetWindowPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pixelDesc =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,
		::GetDeviceCaps(hDC, BITSPIXEL), //得到像素颜色位数
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		32,
		0, 0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	this->m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	if (!this->m_GLPixelIndex)
	{
		this->m_GLPixelIndex = 1; // 默认像素格式
		if (!DescribePixelFormat(hDC, this->m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc))
		{
			return FALSE;
		}
	}
	if (!SetPixelFormat(hDC, this->m_GLPixelIndex, &pixelDesc))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CScanLineZBufferView::CreateViewGLContext(HDC hDC)
{
	this->m_hGLContext = wglCreateContext(hDC); // 创建RC
	if (!this->m_hGLContext)
	{
		return FALSE;
	}
	if (!wglMakeCurrent(hDC, this->m_hGLContext)) // 选为当前RC
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CScanLineZBufferView::InitializeGL()
{
	glDisable(GL_DEPTH_TEST);// 关闭Buffer深度测试
	glClearColor(0.,0.,0.,1.); // 屏幕清空
	glShadeModel(GL_SMOOTH);
	m_z_delta = 0.0; // 模型放在世界坐标的深度
	return TRUE;
}

void CScanLineZBufferView::InitMeshInfo(PolyMesh &mesh)
{
	this->pZBufferCore = new ScanLineZBufferCore(mesh);
	this->CalcModelInfo();
	this->FocusModel();
}

void CScanLineZBufferView::DestoryMeshInfo()
{
	this->m_centerPoint.x = this->m_centerPoint.y = this->m_centerPoint.z = 0;
	this->m_modelScale = 0;
	this->m_z_delta = 0;
	delete pZBufferCore;
	pZBufferCore = NULL;
}

void CScanLineZBufferView::SetBackgroundColor()
{
	// 设置背景颜色
	glClearColor(0., 0., 0., 1.); // 黑色
	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(wglGetCurrentDC());
}

void CScanLineZBufferView::CalcModelInfo()
{
	// 计算模型的中心坐标
	m_centerPoint.x = m_centerPoint.y = m_centerPoint.z = 0;
	CScanLineZBufferDoc *pDoc = GetDocument();
	Point min_xyz, max_xyz;
	auto first_point = (pDoc->m_mesh).point((pDoc->m_mesh).vertices_begin().handle());
	min_xyz.x = max_xyz.x = first_point.data()[0];
	min_xyz.y = max_xyz.y = first_point.data()[1];
	min_xyz.z = max_xyz.z = first_point.data()[2];
	for (auto vertex_it = (pDoc->m_mesh).vertices_begin(); vertex_it != (pDoc->m_mesh).vertices_end(); ++vertex_it)
	{
		auto point = (pDoc->m_mesh).point(vertex_it.handle());
		auto x = point.data()[0], y = point.data()[1], z = point.data()[2];
		m_centerPoint.x += x;
		m_centerPoint.y += y;
		m_centerPoint.z += z;
		min_xyz.x = mmin(min_xyz.x, x);
		min_xyz.y = mmin(min_xyz.y, y);
		min_xyz.z = mmin(min_xyz.z, z);
		max_xyz.x = mmax(max_xyz.x, x);
		max_xyz.y = mmax(max_xyz.y, y);
		max_xyz.z = mmax(max_xyz.z, z);
	}
	int n_verteice = (pDoc->m_mesh).n_vertices();
	m_centerPoint.x /= n_verteice;
	m_centerPoint.y /= n_verteice;
	m_centerPoint.z /= n_verteice;
	double vec[] = { max_xyz.x - min_xyz.x, max_xyz.y - min_xyz.y, max_xyz.z - min_xyz.z }; 
	// 计算模型的尺寸
	m_modelScale = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

void CScanLineZBufferView::FocusModel()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(-m_centerPoint.x, -m_centerPoint.y, -m_centerPoint.z - m_modelScale);
	m_z_delta = -m_modelScale;
}


