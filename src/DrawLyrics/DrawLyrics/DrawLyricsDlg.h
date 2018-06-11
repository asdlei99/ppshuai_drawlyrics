// DrawLyricsDlg.h : ͷ�ļ�
//

#pragma once
#include "LyricsWindow.h"
#include "afxwin.h"
#include "colorbutton.h"
#include "LinkStatic.h"

#define ID_UPDATELYRICS_TIMER 100 //���¸�ʵ�ʱ��ID

// CDrawLyricsDlg �Ի���
class CDrawLyricsDlg : public CDialog
{
// ����
public:
	CDrawLyricsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DRAWLYRICS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	HICON m_hIcon;
	CLyricsWindow m_LyricsWindow;
	CEdit m_TextBox1;
	CEdit m_TextBox2;
	CButton m_Button1;
	int m_nHighlight;//������ʵİٷֱ� 0--100
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
	// ���ɵ���Ϣӳ�亯��
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
