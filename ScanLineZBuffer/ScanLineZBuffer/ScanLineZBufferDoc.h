
// ScanLineZBufferDoc.h : CScanLineZBufferDoc 类的接口
//


#pragma once

#include "utility.h"

class CScanLineZBufferDoc : public CDocument
{
protected: // 仅从序列化创建
	CScanLineZBufferDoc();
	DECLARE_DYNCREATE(CScanLineZBufferDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CScanLineZBufferDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	// 载入模型文件
	bool ReadMesh(const std::string filePath);

public:
	// 模型变量
	PolyMesh m_mesh;

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	afx_msg void OnFileClose();
};
