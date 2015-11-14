
// WLBScanDesktopDlg.cpp : 实现文件
//

#include "WLBScanDesktopDlg.h"

#include <math.h>
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框





// CWLBScanDesktopDlg 对话框







// CWLBScanDesktopDlg 消息处理程序

CWLBScanDesktopDlg::CWLBScanDesktopDlg()
{

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
			// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_pImage = new unsigned char[640 * 480 * 4];
	memset(m_pImage, 0x00, 640 * 480 * 4);
	m_pDscIndex = new DSC_INDEX[640 * 480];
	memset(m_pDscIndex, 0, sizeof(DSC_INDEX)* 640 * 480);

	InitDscIndex();

	m_bRunning = false;

}



// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。



//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。



//	初始化DSC系数
#define PI		(3.14159265359)

#define WIDTH   640
#define HEIGHT  480
void CWLBScanDesktopDlg::InitDscIndex()
{
	double dbAngle = 80.0 / 180.0*PI;	//	扫描角度为80度
	double dbDeadRgn = 15.0;			//	死区长度为15mm
	double dbScaleSample = 780.0 / 2000000.0*1000.0;	//	每个采样点的长度，单位： mm，采样率为2MHz
	double dbScalePixel = ((1.0 - cos(dbAngle / 2.0))*dbDeadRgn + 512.0*dbScaleSample) / (double)HEIGHT;
	double dbCenterY = -cos(dbAngle / 2.0)*dbDeadRgn/dbScalePixel;
	double dbCenterX = (double)WIDTH / 2.0;

	/*
	//	debug
	for (int x = 0; x < 640; x++) {
		if (x >= 256) {
			for (int y = 0; y < 480; y++) {
				m_pDscIndex[y * 640 + x].bIn = false;
			}
		}
		else {
			for (int y = 0; y < 480; y++) {
				m_pDscIndex[y * 640 + x].bIn = true;
				m_pDscIndex[y * 640 + x].nLine = x / 2;
				m_pDscIndex[y * 640 + x].nPixel = (int)(y*(480.0/512.0));
				m_pDscIndex[y * 640 + x].fPartLine = 1.0;
				m_pDscIndex[y * 640 + x].fPartPixel = 1.0;

				if (x / 2 >= 127)
					m_pDscIndex[y * 640 + x].bIn = false;
				if (y >= 511)
					m_pDscIndex[y * 640 + x].bIn = false;
			}
		}
	}
	*/

	
	double dbAngleGrid = dbAngle / 127.0;
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			double dbPosX = x - dbCenterX;
			double dbPosY = y - dbCenterY;
			double angle = atan(dbPosX / dbPosY);
			if ((angle <= -dbAngle/2.0)
				|| (angle >= dbAngle/2.0)
				)
			{
				m_pDscIndex[y * WIDTH + x].bIn = false;
				continue;
			}

			m_pDscIndex[y * WIDTH + x].bIn = true;
			double dbLine = (angle - (-dbAngle / 2.0)) / dbAngleGrid;
			m_pDscIndex[y * WIDTH + x].nLine = (int)dbLine;
			m_pDscIndex[y * WIDTH + x].fPartLine = 1.0 - (dbLine - (double)((int)dbLine));

			double len = sqrt(dbPosX*dbPosX + dbPosY*dbPosY) * dbScalePixel;
			double samp = (len-dbDeadRgn) / dbScaleSample;
			if (samp <= 0.0 || samp >= 511.0) {
				m_pDscIndex[y * WIDTH + x].bIn = false;
				continue;
			}
			m_pDscIndex[y * WIDTH + x].nPixel = (int)samp;
			m_pDscIndex[y * WIDTH + x].fPartPixel =1.0 - (samp - (double)((int)samp));
			if (m_pDscIndex[y * WIDTH + x].nLine >= 127)
				m_pDscIndex[y * WIDTH + x].bIn = false;
			if (m_pDscIndex[y*WIDTH+x].nPixel >= 511)
				m_pDscIndex[y * WIDTH + x].bIn = false;
            
            int nPos = y*WIDTH+x;
            if (m_pDscIndex[nPos].bIn) {
                m_pDscIndex[nPos].ulPartLine = (unsigned long)(m_pDscIndex[nPos].fPartLine*256.0);
                m_pDscIndex[nPos].ulPartPixel = (unsigned long)(m_pDscIndex[nPos].fPartPixel*256.0);
            }
		}
	}
}

//	计算DSC，其结果输出到 m_pImage 中
void CWLBScanDesktopDlg::DscImage(unsigned char* pRawData)
{
    static unsigned char g = 0; g += 32;

    unsigned long cmp_line,row_line;
    unsigned long gray;
	//for (int x = 0; x < WIDTH; x++) {
	//	for (int y = 0; y < HEIGHT; y++) {
	//		int nPos = y * WIDTH + x;
    memset(m_pImage,0,WIDTH*HEIGHT*4);
    for (int nPos=0;nPos<WIDTH*HEIGHT;++nPos)
    {
        DSC_INDEX dscindex = m_pDscIndex[nPos];
			if (dscindex.bIn) {
				unsigned long tl, tr;
                unsigned long npp = (dscindex.nLine << 9) + dscindex.nPixel;
                tl = pRawData[npp];

                npp += 512;
                tr = pRawData[npp];
                
				
                row_line = dscindex.ulPartLine;
                //cmp_pixel = (256 - dscindex.ulPartPixel);
                cmp_line = (256 - dscindex.ulPartLine);
                //l = tl * row_pixel + bl * cmp_pixel;
                //r = tr * row_pixel + br * cmp_pixel;
                //gray = l * row_line + r * cmp_line;
                gray = tl * row_line + tr * cmp_line;
                unsigned char rgb = (gray >> 8) & 0x00FF;
                
                int nPixelPos = nPos << 2;
                m_pImage[nPixelPos] = m_pImage[nPixelPos+1] = m_pImage[nPixelPos+2] = rgb;
                m_pImage[nPixelPos+3] = 255;
                
                //gray >>= 16;
                //if (gray > 255) {
                //gray = 255;
                //}
                //if (gray < 0.0)
                //gray = 0.0;
                //else if (gray > 255.0)
                //    gray = 255.0;
                
				//m_pImage[nPos] = (gray >> 16) & 0x00FF;
			}
			else {
                m_pImage[nPos*4+3] = 255;
                
                //
				//m_pImage[nPos] = 0x5A;
			}
    }
    //	}
	//}
	
	//memcpy(m_pImage, pRawData, 512 * 128);
}

//	绘制图像






