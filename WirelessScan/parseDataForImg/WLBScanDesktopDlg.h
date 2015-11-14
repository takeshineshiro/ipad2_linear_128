
// WLBScanDesktopDlg.h : 头文件
//

#pragma once

#include "TransSocket.h"
// CWLBScanDesktopDlg 对话框
class CWLBScanDesktopDlg
{
   
// 构造
public:
    
	 CWLBScanDesktopDlg();
		// DDX/DDV 支持

//	网络接口
	
	bool		m_bRunning;

	CTransSocket m_TransSocket;


//	图像DSC
	typedef struct _tagDSC_INDEX{
		bool bIn;
		int  nLine;
		int	 nPixel;
		float fPartLine;
		float fPartPixel;
        unsigned long ulPartLine;
        unsigned long ulPartPixel;
	} DSC_INDEX;

	DSC_INDEX* m_pDscIndex;
	//	初始化DSC系数
	void InitDscIndex();

	unsigned char* m_pImage;
	//	计算DSC，其结果输出到 m_pImage 中
	void DscImage(unsigned char* pRawData);
	
	
};
