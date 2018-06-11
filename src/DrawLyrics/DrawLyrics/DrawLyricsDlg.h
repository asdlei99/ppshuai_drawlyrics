// DrawLyricsDlg.h : 头文件
//

#pragma once
#include "LyricsWindow.h"
#include "afxwin.h"
#include "colorbutton.h"
#include "LinkStatic.h"

#define ID_UPDATELYRICS_TIMER 100 //更新歌词的时钟ID

// CDrawLyricsDlg 对话框
class CDrawLyricsDlg : public CDialog
{
// 构造
public:
	CDrawLyricsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAWLYRICS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	HICON m_hIcon;
	CLyricsWindow m_LyricsWindow;
	CEdit m_TextBox1;
	CEdit m_TextBox2;
	CButton m_Button1;
	int m_nHighlight;//高亮歌词的百分比 0--100
	CButton m_Check1;
	CComboBox m_cb_FontFace;
	CComboBox m_cb_FontSize;
	CColorButton m_ColorBtn1;
	CColorButton m_ColorBtn2;
	CColorButton m_ColorBtn3;
	CColorButton m_ColorBtn4;
	CColorButton m_ColorBtn5;
	CColorButton m_ColorBtn6;
	CComboBox m_cb_TextGradientMode;
	CComboBox m_cb_HighlightGradientMode;
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
