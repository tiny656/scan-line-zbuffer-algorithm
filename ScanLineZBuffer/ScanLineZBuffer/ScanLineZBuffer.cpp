
// ScanLineZBuffer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ScanLineZBuffer.h"
#include "MainFrm.h"

#include "ScanLineZBufferDoc.h"
#include "ScanLineZBufferView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanLineZBufferApp

BEGIN_MESSAGE_MAP(CScanLineZBufferApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CScanLineZBufferApp::OnAppAbout)
	// 基于文件的标准文档命令
	// ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	// ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CScanLineZBufferApp::OnFileOpen)
END_MESSAGE_MAP()


// CScanLineZBufferApp 构造

CScanLineZBufferApp::CScanLineZBufferApp()
{
	// TODO:  将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ScanLineZBuffer.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CScanLineZBufferApp 对象

CScanLineZBufferApp theApp;


// CScanLineZBufferApp 初始化

BOOL CScanLineZBufferApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	// SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	// LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CScanLineZBufferDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CScanLineZBufferView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CScanLineZBufferApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileOpen();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_COMMAND(ID_FILE_OPEN, &CAboutDlg::OnFileOpen)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CScanLineZBufferApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CScanLineZBufferApp 消息处理程序





//void CAboutDlg::OnFileOpen()
//{
//	// TODO:  在此添加命令处理程序代码
//}


// 处理打开文件
void CScanLineZBufferApp::OnFileOpen()
{
	// 文件类型
	CString filter;
	filter = "OBJ files (*.obj)|*.obj|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER, filter);
	if (fileOpenDlg.DoModal() == IDOK)
	{
		CScanLineZBufferDoc *pDoc = (CScanLineZBufferDoc*)AfxGetMainWnd()->GetTopLevelFrame()->GetActiveView()->GetDocument();
		std::string filePath = CT2A(fileOpenDlg.GetPathName().GetBuffer());
		CString fileName = fileOpenDlg.GetFileName();
		if (pDoc->ReadMesh(filePath))
		{
			MessageBox(NULL, _T("载入obj模型文件成功!"), _T("Success"), MB_OK);
			// 修改文件标题
			pDoc->SetTitle(fileName);
			// 发送重绘消息
			pDoc->UpdateAllViews(0, 0, 0);
		} 
		else 
		{
			MessageBox(NULL, _T("读入obj模型文件出错!(请输入PolyMesh模型)"), _T("Error"), MB_OK);
		}
	}

}
