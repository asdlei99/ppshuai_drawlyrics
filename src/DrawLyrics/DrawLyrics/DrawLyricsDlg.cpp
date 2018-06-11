// DrawLyricsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawLyrics.h"
#include "DrawLyricsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawLyricsDlg 对话框




CDrawLyricsDlg::CDrawLyricsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawLyricsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nHighlight=0;
}

void CDrawLyricsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_TextBox1);
	DDX_Control(pDX, IDC_EDIT2, m_TextBox2);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_COMBO1, m_cb_FontFace);
	DDX_Control(pDX, IDC_COMBO2, m_cb_FontSize);
	DDX_Control(pDX, IDC_COMBO3, m_cb_TextGradientMode);
	DDX_Control(pDX, IDC_COMBO4, m_cb_HighlightGradientMode);
	DDX_Control(pDX, IDC_COLORBTN1, m_ColorBtn1);
	DDX_Control(pDX, IDC_COLORBTN2, m_ColorBtn2);
	DDX_Control(pDX, IDC_COLORBTN3, m_ColorBtn3);
	DDX_Control(pDX, IDC_COLORBTN4, m_ColorBtn4);
	DDX_Control(pDX, IDC_COLORBTN5, m_ColorBtn5);
	DDX_Control(pDX, IDC_COLORBTN6, m_ColorBtn6);
}

BEGIN_MESSAGE_MAP(CDrawLyricsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDrawLyricsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CDrawLyricsDlg::OnBnClickedCheck1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CDrawLyricsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDrawLyricsDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDrawLyricsDlg 消息处理程序

BOOL CDrawLyricsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_TextBox1.SetWindowText(_T("你是我的小苹果"));
	m_TextBox2.SetWindowText(_T("40"));

	m_cb_FontFace.AddString(_T("黑体"));
	m_cb_FontFace.AddString(_T("楷体"));
	m_cb_FontFace.AddString(_T("微软雅黑"));
	m_cb_FontFace.SetCurSel(0);
	m_cb_FontSize.AddString(_T("30"));
	m_cb_FontSize.AddString(_T("40"));
	m_cb_FontSize.AddString(_T("45"));
	m_cb_FontSize.AddString(_T("50"));
	m_cb_FontSize.AddString(_T("55"));
	m_cb_FontSize.AddString(_T("60"));
	m_cb_FontSize.SetCurSel(3);
	m_cb_TextGradientMode.AddString(_T("无渐变"));
	m_cb_TextGradientMode.AddString(_T("两色渐变"));
	m_cb_TextGradientMode.AddString(_T("三色渐变"));
	m_cb_TextGradientMode.SetCurSel(1);
	m_cb_HighlightGradientMode.AddString(_T("无渐变"));
	m_cb_HighlightGradientMode.AddString(_T("两色渐变"));
	m_cb_HighlightGradientMode.AddString(_T("三色渐变"));
	m_cb_HighlightGradientMode.SetCurSel(1);

	m_ColorBtn1.SetColor(0x0000FF);
	m_ColorBtn2.SetColor(0x00FFFF);
	m_ColorBtn3.SetColor(0x000000);
	m_ColorBtn4.SetColor(0xFF8800);
	m_ColorBtn5.SetColor(0xFFFF00);
	m_ColorBtn6.SetColor(0x000000);

	//创建歌词窗口
	m_LyricsWindow.Create();
	//必须更新歌词,否则无歌词无法显示;
	m_LyricsWindow.UpdateLyrics(_T("GDIPlus绘制歌词"),50);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrawLyricsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CDrawLyricsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDrawLyricsDlg::OnBnClickedButton1()
{
	CString strText;
	GetDlgItemText(IDC_EDIT1,strText);
	m_nHighlight=GetDlgItemInt(IDC_EDIT2);
	m_LyricsWindow.UpdateLyrics((LPCTSTR)strText,m_nHighlight);
}

void CDrawLyricsDlg::OnBnClickedCheck1()
{
	OnBnClickedButton1();
	BOOL bCheck=m_Check1.GetCheck();
	if(bCheck){
		m_TextBox1.EnableWindow(FALSE);
		m_TextBox2.EnableWindow(FALSE);
		m_Button1.EnableWindow(FALSE);
		SetTimer(ID_UPDATELYRICS_TIMER,80,NULL);
	}else{
		m_TextBox1.EnableWindow(TRUE);
		m_TextBox2.EnableWindow(TRUE);
		m_Button1.EnableWindow(TRUE);
		KillTimer(ID_UPDATELYRICS_TIMER);
	}
}

void CDrawLyricsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==ID_UPDATELYRICS_TIMER){
		m_nHighlight++;
		if(m_nHighlight>100)
			m_nHighlight=0;
		m_LyricsWindow.UpdateLyrics(m_nHighlight);
	}


	CDialog::OnTimer(nIDEvent);
}

void CDrawLyricsDlg::OnBnClickedButton2()
{
	CString strFontFace;
	CString strFontSize;
	INT nFontStyle=0;
	m_cb_FontFace.GetLBText(m_cb_FontFace.GetCurSel(),strFontFace);
	m_cb_FontSize.GetLBText(m_cb_FontSize.GetCurSel(),strFontSize);
	int nFontSize=_wtoi((LPTSTR)(LPCTSTR)strFontSize);
	if(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		nFontStyle |= FontStyleBold;
	}
	if(((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck())
	{
		nFontStyle |= FontStyleItalic;
	}
	if(((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())
	{
		nFontStyle |= FontStyleUnderline;
	}
	//-----------------
	//设置歌词字体
	m_LyricsWindow.SetLyricsFont((LPTSTR)(LPCTSTR)strFontFace,nFontSize,nFontStyle);
	//设置后要重新绘制歌词
	m_LyricsWindow.Draw();
}

void CDrawLyricsDlg::OnBnClickedButton3()
{
	COLORREF crText1;
	COLORREF crText2;
	COLORREF crBorder;
	int nCurSel;
	Gdiplus::Color TextColor1;
	Gdiplus::Color TextColor2;
	Gdiplus::Color BorderColor;
	LyricsGradientMode TextGradientMode;
	//-------------------------------------
	crText1=m_ColorBtn1.GetColor();
	crText2=m_ColorBtn2.GetColor();
	crBorder=m_ColorBtn3.GetColor();
	nCurSel=m_cb_TextGradientMode.GetCurSel();
	TextColor1.SetFromCOLORREF(crText1);
	TextColor2.SetFromCOLORREF(crText2);
	BorderColor.SetFromCOLORREF(crBorder);
	switch (nCurSel)
	{
	case 0:
		TextGradientMode=LyricsGradientMode_None;
		break;
	case 1:
		TextGradientMode=LyricsGradientMode_Two;
		break;
	case 2:
		TextGradientMode=LyricsGradientMode_Three;
		break;
	}
	//设置歌词颜色
	m_LyricsWindow.SetLyricsColor(TextColor1,TextColor2,TextGradientMode);
	//设置歌词边框
	m_LyricsWindow.SetLyricsBorder(BorderColor);
	//-------------------------------------
	crText1=m_ColorBtn4.GetColor();
	crText2=m_ColorBtn5.GetColor();
	crBorder=m_ColorBtn6.GetColor();
	nCurSel=m_cb_HighlightGradientMode.GetCurSel();
	TextColor1.SetFromCOLORREF(crText1);
	TextColor2.SetFromCOLORREF(crText2);
	BorderColor.SetFromCOLORREF(crBorder);
	switch (nCurSel)
	{
	case 0:
		TextGradientMode=LyricsGradientMode_None;
		break;
	case 1:
		TextGradientMode=LyricsGradientMode_Two;
		break;
	case 2:
		TextGradientMode=LyricsGradientMode_Three;
		break;
	}
	//设置高亮歌词颜色
	m_LyricsWindow.SetHighlightColor(TextColor1,TextColor2,TextGradientMode);
	//设置高亮歌词边框
	m_LyricsWindow.SetHighlightBorder(BorderColor);
	//-------------------------------------
	//设置后要重新绘制歌词
	m_LyricsWindow.Draw();
}
