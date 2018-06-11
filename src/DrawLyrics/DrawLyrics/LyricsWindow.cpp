// LyricsWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LyricsWindow.h"


// CLyricsWindow

IMPLEMENT_DYNAMIC(CLyricsWindow, CWnd)

CLyricsWindow::CLyricsWindow()
{
	HDC hDC=::GetDC(NULL);
	m_hCacheDC=::CreateCompatibleDC(hDC);
	::ReleaseDC(NULL,hDC);
	//---------------------------------
	m_lpszLyrics=NULL ; //Unicode��ʽ�ĸ��
	m_nHighlight=NULL ; //������ʵİٷֱ� 0--100
	m_TextGradientMode=LyricsGradientMode_Two ; //��ͨ��ʽ���ģʽ
	m_pTextPen=NULL ; //��ͨ��ʱ߿򻭱�
	m_HighlightGradientMode=LyricsGradientMode_Two ; //������ʽ���ģʽ
	m_pHighlightPen=NULL ; //������ʱ߿򻭱�
	m_pShadowBrush=NULL ; //��Ӱ��ˢ,GDIPlus��ˢ 
	m_nShadowOffset=LyricsGradientMode_Two ; //��Ӱƫ��
	m_pFont=NULL ; //GDIPlus����
	m_FontStyle=NULL ; 
	m_FontSize=NULL ; 
	m_pTextFormat=NULL;
	//---------------------------------
	m_pFontFamily=new Gdiplus::FontFamily();
	m_pTextFormat=new Gdiplus::StringFormat();
	m_pTextFormat->SetFormatFlags(StringFormatFlagsNoWrap);//������
	m_pTextFormat->SetAlignment(StringAlignmentCenter); //��ˮƽ���뷽ʽ
	m_pTextFormat->SetLineAlignment(StringAlignmentNear); //�ô�ֱ���뷽ʽ
	//---------------------------------
	SetLyricsFont(L"����",50,FontStyleBold);
	SetLyricsColor(Gdiplus::Color::Red,Gdiplus::Color(255,172,0),LyricsGradientMode_Three);
	SetLyricsBorder(Gdiplus::Color::Black,1);
	SetLyricsShadow(Gdiplus::Color(150,0,0,0),2);
	SetHighlightColor(Gdiplus::Color(255,100,26),Gdiplus::Color(255,255,0),LyricsGradientMode_Three);
	SetHighlightBorder(Gdiplus::Color::Black,1);
	
}

CLyricsWindow::~CLyricsWindow()
{
	if(m_lpszLyrics){
		delete  m_lpszLyrics;
		m_lpszLyrics=NULL;
	}
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(m_pFontFamily){
		delete m_pFontFamily;
		m_pFontFamily=NULL;
	}	
	if(m_pTextFormat){
		delete m_pTextFormat;
		m_pTextFormat=NULL;
	}	
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
}


BEGIN_MESSAGE_MAP(CLyricsWindow, CWnd)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



BOOL CLyricsWindow::Create()
{
	return CLyricsWindow::Create(_T("CometLyricsWindow"),-1,-1);
}
BOOL CLyricsWindow::Create(LPCTSTR lpszClassName)
{
	return CLyricsWindow::Create(lpszClassName,-1,-1);
}
BOOL CLyricsWindow::Create(LPCTSTR lpszClassName,int nWidth,int nHeight)
{
	if(!RegisterWndClass(lpszClassName))
	{
		TRACE("Class��Registration��Failedn");
	}
	//--------------------------------------------
	//ȡ�����湤������
	RECT rcWork;
	SystemParametersInfo (SPI_GETWORKAREA,NULL,&rcWork,NULL);
	int nWorkWidth=rcWork.right-rcWork.left;
	int nWorkHeight=rcWork.bottom-rcWork.top;
	//δ���ݿ�ȡ��߶Ȳ���ʱ���ø�Ĭ��ֵ
	if(nWidth<0)nWidth=nWorkWidth;
	if(nHeight<0)nHeight=150;
	//������ߡ�����λ��,�ô�������Ļ�·�
	int x=rcWork.left+( (nWorkWidth-nWidth)/2 );
	int y=rcWork.bottom-nHeight;
	//--------------------------------------------
	DWORD dwStyle=WS_POPUP|WS_VISIBLE;
	DWORD dwExStyle=WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED;
	if(!CWnd::CreateEx(dwExStyle,lpszClassName,NULL,dwStyle,x,y,nWidth,nHeight,NULL,NULL))
		return FALSE;
}
BOOL CLyricsWindow::RegisterWndClass(LPCTSTR lpszClassName)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	WNDCLASSEX wndcls;
	memset(&wndcls,0,sizeof(WNDCLASSEX));
	wndcls.cbSize=sizeof(WNDCLASSEX);
	if(GetClassInfoEx(hInstance,lpszClassName,&wndcls))
	{
		return TRUE;
	}
	if(GetClassInfoEx(NULL,lpszClassName,&wndcls))
	{
		return TRUE;
	}

	wndcls.style=CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wndcls.lpfnWndProc=::DefWindowProc;
	wndcls.hInstance=hInstance;
	wndcls.hIcon=NULL;
	wndcls.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wndcls.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	wndcls.lpszMenuName=NULL;
	wndcls.lpszClassName=lpszClassName;
	if(!RegisterClassEx(&wndcls))
	{
		return FALSE;
	}
	return TRUE;
}


//���¸��(����ı�,�������Ȱٷֱ�)
void CLyricsWindow::UpdateLyrics(LPCSTR lpszLyrics,int nHighlight)
{
	if(m_lpszLyrics){
		delete  m_lpszLyrics;
		m_lpszLyrics=NULL;
	}
	if(lpszLyrics){
		//���ݽ�����ANSI�汾���ַ���,��Ҫת����Unicode
		int nLen=MultiByteToWideChar(CP_ACP,0,lpszLyrics,-1, NULL,NULL);
		if(nLen){
			m_lpszLyrics = new WCHAR[nLen+1];
			ZeroMemory(m_lpszLyrics,sizeof(WCHAR)*(nLen+1));
			MultiByteToWideChar(CP_ACP,0,lpszLyrics,-1,m_lpszLyrics,nLen); 
		}		
	}
	UpdateLyrics(nHighlight);
}
void CLyricsWindow::UpdateLyrics(LPCWSTR lpszLyrics,int nHighlight)
{
	if(m_lpszLyrics){
		delete  m_lpszLyrics;
		 m_lpszLyrics=NULL;
	}
	int nLen=0;
	if(lpszLyrics){
		nLen=lstrlenW(lpszLyrics);
	}
	if(nLen>0){
		m_lpszLyrics=new WCHAR[nLen+1];
		ZeroMemory(m_lpszLyrics,sizeof(WCHAR)*(nLen+1));
		CopyMemory(m_lpszLyrics,lpszLyrics,sizeof(WCHAR)*(nLen));
	}
	UpdateLyrics(nHighlight);
}
//���¸�������(�������Ȱٷֱ�)
void CLyricsWindow::UpdateLyrics(int nHighlight)
{
	m_nHighlight=nHighlight;
	if(m_nHighlight<0)
		m_nHighlight=0;
	if(m_nHighlight>100)
		m_nHighlight=100;
	Draw();
}
//�ػ���ʴ���
void CLyricsWindow::Draw()
{
	CRect rcClient;
	GetClientRect(rcClient);
	m_nWidth=rcClient.Width();
	m_nHeight=rcClient.Height();
	//----------------------------------
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = m_nHeight;
	bitmapinfo.bmiHeader.biWidth = m_nWidth;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression=BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter=0;
	bitmapinfo.bmiHeader.biYPelsPerMeter=0;
	bitmapinfo.bmiHeader.biClrUsed=0;
	bitmapinfo.bmiHeader.biClrImportant=0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
	HBITMAP hBitmap=CreateDIBSection (m_hCacheDC,&bitmapinfo, 0,NULL, 0, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject (m_hCacheDC,hBitmap);
	//----------------------------------
	Gdiplus::Graphics* pGraphics=new Gdiplus::Graphics(m_hCacheDC);
	pGraphics->SetSmoothingMode (SmoothingModeAntiAlias);
	pGraphics->SetTextRenderingHint (TextRenderingHintAntiAlias);
	DrawLyrics(pGraphics);
	delete pGraphics;
	//----------------------------------
	//����͸������
	CPoint DestPt(0,0);
	CSize psize(m_nWidth,m_nHeight);
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = 255;
	HDC hDC=::GetDC(m_hWnd);
	::UpdateLayeredWindow(m_hWnd,hDC,NULL,&psize,m_hCacheDC,&DestPt,0,&blendFunc32bpp,ULW_ALPHA);
	//----------------------------------
	//�ͷ���Դ
	::SelectObject (m_hCacheDC,hOldBitmap);
	::DeleteObject(hBitmap);
	::ReleaseDC(m_hWnd,hDC);
}
//���Ƹ��
void CLyricsWindow::DrawLyrics(Gdiplus::Graphics* pGraphics)
{
	//��ȡ�����ֿ�Ⱥ͸߶�
	RectF layoutRect(0,0,0,0);
	RectF boundingBox;
	pGraphics->MeasureString (m_lpszLyrics, -1, m_pFont,layoutRect, m_pTextFormat,&boundingBox, 0, 0);
	//�����ʻ�����λ��
	RectF dstRect((m_nWidth - boundingBox.Width) / 2,(m_nHeight - boundingBox.Height) / 2,boundingBox.Width,boundingBox.Height);
	if(dstRect.X<0)dstRect.X=0;
	if(dstRect.Width>m_nWidth)dstRect.Width=m_nWidth;

	//-----------------------------------------------------------
	//������Ӱ
	if(m_pShadowBrush){
		layoutRect=dstRect;
		layoutRect.X=layoutRect.X + m_nShadowOffset;
		layoutRect.Y=layoutRect.Y + m_nShadowOffset;
		Gdiplus::GraphicsPath* pShadowPath=new Gdiplus::GraphicsPath(FillModeAlternate);//����·��
		pShadowPath->AddString (m_lpszLyrics, -1, m_pFontFamily, m_FontStyle, m_FontSize, layoutRect, m_pTextFormat); //�����ּ���·��
		pGraphics->FillPath (m_pShadowBrush,pShadowPath);//���·��
		delete pShadowPath; //����·��
	}

	//-----------------------------------------------------------
	//�������
	Gdiplus::GraphicsPath* pStringPath=new Gdiplus::GraphicsPath(FillModeAlternate);//����·��
	pStringPath->AddString (m_lpszLyrics, -1, m_pFontFamily, m_FontStyle, m_FontSize,dstRect, m_pTextFormat); //�����ּ���·��
	if(m_pTextPen){
		pGraphics->DrawPath (m_pTextPen,pStringPath);//��·��,���ֱ߿�
	}
	Gdiplus::Brush* pBrush = CreateGradientBrush(m_TextGradientMode, m_TextColor1,m_TextColor2,dstRect);
	pGraphics->FillPath (pBrush,pStringPath);//���·��
	delete pBrush;//���ٻ�ˢ
	DrawHighlightLyrics(pGraphics,pStringPath,dstRect);
	delete pStringPath; //����·��

}
//���Ƹ������
void CLyricsWindow::DrawHighlightLyrics(Gdiplus::Graphics* pGraphics,Gdiplus::GraphicsPath* pPath,RectF& dstRect)
{
	if(m_nHighlight<=0)return;
	Gdiplus::Region* pRegion=NULL;
	if(m_nHighlight<100){
		RectF CliptRect(dstRect);
		CliptRect.Width=CliptRect.Width * (REAL)m_nHighlight / (REAL)100.0f;
		pRegion=new Gdiplus::Region(CliptRect);
		pGraphics->SetClip(pRegion,CombineModeReplace);
	}
	//--------------------------------------------
	if(m_pHighlightPen){
		pGraphics->DrawPath (m_pHighlightPen,pPath);//��·��,���ֱ߿�
	}
	Gdiplus::Brush* pBrush = CreateGradientBrush(m_HighlightGradientMode, m_HighlightColor1,m_HighlightColor2,dstRect);
	pGraphics->FillPath (pBrush,pPath);//���·��
	delete pBrush;//���ٻ�ˢ
	//--------------------------------------------
	if(pRegion){
		pGraphics->ResetClip();
		delete pRegion;
	}
}
//�������仭ˢ
Gdiplus::Brush* CLyricsWindow::CreateGradientBrush(LyricsGradientMode TextGradientMode,Gdiplus::Color& Color1,Gdiplus::Color& Color2,RectF& dstRect)
{
	PointF pt1;
	PointF pt2;
	Gdiplus::Brush* pBrush=NULL;
	switch (TextGradientMode)
	{
	case LyricsGradientMode_Two://��ɫ����
		{
			PointF point1(dstRect.X,dstRect.Y);
			PointF point2(dstRect.X,dstRect.Y+dstRect.Height);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((LinearGradientBrush*)pBrush)->SetWrapMode(WrapModeTileFlipXY);
			break;
		}

	case LyricsGradientMode_Three://��ɫ����
		{
			PointF point1(dstRect.X,dstRect.Y);
			PointF point2(dstRect.X,dstRect.Y+dstRect.Height/2);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((LinearGradientBrush*)pBrush)->SetWrapMode(WrapModeTileFlipXY);
			break;
		}

	default://�޽���
		{
			pBrush=new Gdiplus::SolidBrush(Color1);
			break;
		}
	}
	return pBrush;
}

//���ø����ɫ
void CLyricsWindow::SetLyricsColor(Gdiplus::Color TextColor1)
{
	CLyricsWindow::SetLyricsColor(TextColor1,Gdiplus::Color::Black,LyricsGradientMode_None);
}
void CLyricsWindow::SetLyricsColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode)
{
	m_TextColor1=TextColor1;
	m_TextColor2=TextColor2;
	m_TextGradientMode=TextGradientMode;

}
//���ø�ʱ߿�
void CLyricsWindow::SetLyricsBorder(Gdiplus::Color BorderColor,REAL BorderWidth)
{
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pTextPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//���ø��������ɫ
void CLyricsWindow::SetHighlightColor(Gdiplus::Color TextColor1)
{
	CLyricsWindow::SetHighlightColor(TextColor1,Gdiplus::Color::Black,LyricsGradientMode_None);
}
void CLyricsWindow::SetHighlightColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode)
{
	m_HighlightColor1=TextColor1;
	m_HighlightColor2=TextColor2;
	m_HighlightGradientMode=TextGradientMode;

}
//���ø�����ʱ߿�
void CLyricsWindow::SetHighlightBorder(Gdiplus::Color BorderColor,REAL BorderWidth)
{
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pHighlightPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//���ø����Ӱ
void CLyricsWindow::SetLyricsShadow(Gdiplus::Color ShadowColor,int nShadowOffset)
{
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(ShadowColor.GetA()>0 && nShadowOffset>0){
		m_nShadowOffset=nShadowOffset;
		m_pShadowBrush=new Gdiplus::SolidBrush(ShadowColor);
	}else{
		m_nShadowOffset=0;
	}
}
//���ø������
void CLyricsWindow::SetLyricsFont(const WCHAR * familyName,REAL emSize,INT style,Unit unit)
{
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
	FontFamily family(familyName,NULL);
	Status lastResult = family.GetLastStatus();
	if (lastResult != Ok)
	{
		HFONT hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONTW lf;
		ZeroMemory(&lf,sizeof(LOGFONTW));
		GetObjectW(hFont,sizeof(LOGFONTW),&lf);
		FontFamily family2(lf.lfFaceName,NULL);
		m_pFont=new Gdiplus::Font(&family2,emSize,style,unit);
	}else{
		m_pFont=new Gdiplus::Font(&family,emSize,style,unit);
	}
	 //----------------
	//����һЩ��������,����·��ʱҪ�õ�
	m_pFont->GetFamily (m_pFontFamily);
	m_FontSize=m_pFont->GetSize ();
	m_FontStyle=m_pFont->GetStyle ();

	
	
}

void CLyricsWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	ReleaseCapture();
	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,NULL);
}

void CLyricsWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnLButtonUp(nFlags, point);
}
