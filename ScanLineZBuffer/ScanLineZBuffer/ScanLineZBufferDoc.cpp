
// ScanLineZBufferDoc.cpp : CScanLineZBufferDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ScanLineZBuffer.h"
#endif

#include "ScanLineZBufferDoc.h"
#include "ScanLineZBufferView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CScanLineZBufferDoc

IMPLEMENT_DYNCREATE(CScanLineZBufferDoc, CDocument)

BEGIN_MESSAGE_MAP(CScanLineZBufferDoc, CDocument)
	ON_COMMAND(ID_FILE_CLOSE, &CScanLineZBufferDoc::OnFileClose)
END_MESSAGE_MAP()


// CScanLineZBufferDoc 构造/析构

CScanLineZBufferDoc::CScanLineZBufferDoc()
{
	// TODO:  在此添加一次性构造代码

}

CScanLineZBufferDoc::~CScanLineZBufferDoc()
{
}

BOOL CScanLineZBufferDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	// 第一次打开显示标题
	SetTitle(_T("当前无显示模型"));
	return TRUE;
}




// CScanLineZBufferDoc 序列化

void CScanLineZBufferDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CScanLineZBufferDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CScanLineZBufferDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CScanLineZBufferDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CScanLineZBufferDoc 诊断

#ifdef _DEBUG
void CScanLineZBufferDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScanLineZBufferDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}



#endif //_DEBUG


// CScanLineZBufferDoc 命令
bool CScanLineZBufferDoc::ReadMesh(const std::string filePath)
{
	m_mesh.clear();
	bool result = OpenMesh::IO::read_mesh(m_mesh, filePath);
	if (result) {
		// std::cout << logTime() << "模型信息" << m_mesh.n_vertices() << " " << m_mesh.n_edges() << " " << m_mesh.n_faces() << std::endl;
		POSITION pos = GetFirstViewPosition();
		CScanLineZBufferView *pView = (CScanLineZBufferView*)GetNextView(pos);
		pView->InitMeshInfo(m_mesh);
		pView->CalcModelInfo();
		pView->FocusModel();
	}
	return result && m_mesh.n_vertices() > 0;
}

void CScanLineZBufferDoc::OnFileClose()
{
	POSITION pos = GetFirstViewPosition();
	CScanLineZBufferView *pView = (CScanLineZBufferView*)GetNextView(pos);
	
	if (m_mesh.n_vertices() <= 0)
	{
		MessageBox(NULL, _T("当前未打开任何文件，无法关闭！"), _T("Error"), MB_OK);
		pView->DestoryMeshInfo();
		return;
	}
	if ((MessageBox(NULL, _T("确认关闭当前模型文件？"), _T("文件关闭"), MB_ICONWARNING | MB_OKCANCEL) == IDOK)) 
	{
		pView->DestoryMeshInfo();
		this->SetTitle(_T("当前无显示模型"));
		this->m_mesh.clear();
		this->UpdateAllViews(0, 0, 0);
		return;
	}
}
