// TransSocket.cpp : implementation file
//

#include "TransSocket.h"
#include <string>
#include <assert.h>

// CTransSocket


CTransSocket ::CTransSocket(){
	
    m_pRawImg = new unsigned char[64 * 2 * 1024];
    
	m_pPackBuf = new unsigned char[4200];
    
	m_nStreamLen = 0;
	m_pStreamBuf = new unsigned char[128*1024];

	m_nLength = 0;
    
    m_nPackIndex = 0;
    
    m_nNeedFrame = 0;
    m_nNeedLine = 0;

}



// CTransSocket member functions





bool CTransSocket::Package(unsigned char* pucPack, unsigned long ulPackLen)
{
	//	复制数据到流缓冲中
	bool bRet = false;
    if (!(ulPackLen+m_nStreamLen<128*1024)) {
        return false;
    }
	assert(ulPackLen + m_nStreamLen < 128*1024);
    
	memcpy(m_pStreamBuf + m_nStreamLen, pucPack, ulPackLen);
	m_nStreamLen += ulPackLen;
    
     
	//TRACE(_T("---------PACKAGE........ nStreamLen=%d  ---- nPackLen=%d  =======\n"),m_nStreamLen,ulPackLen);

	while (m_nStreamLen >= 525) {

		//	找包头、对齐包头
		int head = 0;
		for (head = 0; head <= m_nStreamLen - 8; head++) {
			if ((m_pStreamBuf[head] == 0x5A)
				&& (m_pStreamBuf[head + 1] == 0xA5)
				&& (m_pStreamBuf[head + 2] == 0xFF)
				&& (m_pStreamBuf[head + 3] == 0x00)
				&& (m_pStreamBuf[head + 4] == 0x5A)
				&& (m_pStreamBuf[head + 5] == 0xA5)
				&& (m_pStreamBuf[head + 6] == 0xFF)
				&& (m_pStreamBuf[head + 7] == 0x00)
				)
			{
				//TRACE("memmove: m_pStreamBuf = 0x%x, m_nStreamLen = %d\n", m_pStreamBuf, m_nStreamLen);
				if (head > 0) {
					for (int b = 0; b < m_nStreamLen - head; b++) {
						m_pStreamBuf[b] = m_pStreamBuf[b + head];
					}
					//memmove(m_pStreamBuf, m_pStreamBuf + head, m_nStreamLen - head);
					m_nStreamLen -= head;
				}
				break;
			}
		}

		//	没有找到
		if (head >= m_nStreamLen - 8) {
			//TRACE("XXXxx   FAILED IN FINDING __________ m_nStreamLen = %d____________xxxx\n",m_nStreamLen);
			memmove(m_pStreamBuf, m_pStreamBuf + head, m_nStreamLen - head);
			m_nStreamLen -= head;
			break;
		}

		//	如果找到了包头，则填充线，此时数据流长度超过525
		if (m_nStreamLen >= 525) {
           
    
			//	确定线是否可用
			unsigned char nCurFrame = m_pStreamBuf[8];
			unsigned char nCurLine = m_pStreamBuf[9];
            
            
            nCurLine |= ( (nCurFrame & 0x01) << 7 );
            nCurFrame >>= 1;
            
           // printf("\n m_nNeedFrame = %d, m_nNeedLine = %d \n",m_nNeedFrame,m_nNeedLine);
           // printf("\n nCurFrame = %d, nCurLine = %d \n",nCurFrame,nCurLine);

			if (	(m_nNeedFrame != nCurFrame)
				||	(m_nNeedLine != nCurLine)
				)
			{
                //printf("\n m_nNeedFrame = %d, nCurFrame = %d \n",m_nNeedFrame,nCurFrame);

				//	线号或帧号对不上时的处理
				m_nNeedFrame = nCurFrame;
                
				m_nNeedLine = 0;
                
				/*
				if (m_nStreamLen > 525)
					memmove(m_pStreamBuf, m_pStreamBuf + 525, m_nStreamLen - 525);
				m_nStreamLen -= 525;
				*/
			}

			if (	(m_nNeedFrame != nCurFrame)
				||	(m_nNeedLine != nCurLine)
				)
			{	//	线号和帧号还是对不上则废弃该线
                //printf("\n m_nStreamLen = %ld",m_nStreamLen);


				if (m_nStreamLen > 525) {
					memmove(m_pStreamBuf, m_pStreamBuf + 525, m_nStreamLen - 525);
				}
				m_nStreamLen -= 525;
			}
			else {
                
                //printf("\n m_nNeedFrame = %d, m_nNeedLine = %d \n",m_nNeedFrame,m_nNeedLine);

				assert(m_nNeedLine < 256);
					
                memcpy(m_pRawImg + 512 * m_nNeedLine, m_pStreamBuf + 10, 512);

                //printf("\n m_pRawImg=%s \n",m_pRawImg);
                
                assert(m_nStreamLen < 128 * 1024);
                
				if (m_nStreamLen > 525)
					memmove(m_pStreamBuf, m_pStreamBuf + 525, m_nStreamLen - 525);
				m_nStreamLen -= 525;

				
				if (m_nNeedLine == 255) {
					//TRACE("==================== FRAME ID: %d GOT.========================\n", m_nNeedFrame);
					m_nNeedFrame++;
					m_nNeedLine = 0;
					bRet = true;
					break;
				}
                else {
                    m_nNeedLine++;
                }
			}
		}
	}

	return bRet;
}

