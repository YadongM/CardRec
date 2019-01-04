
// ImgRecDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgRec.h"
#include "ImgRecDlg.h"
#include "afxdialogex.h"
#include <set>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImgRecDlg �Ի���



CImgRecDlg::CImgRecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMGREC_DIALOG, pParent)
	/*, 1(1)
	, 360(360)
	, 105(105)
	, 90(90)
	, 40(40)
	, 100(100)
	, 25(25)
	, 70(70)
	, 50(50)
	, 190(190)*/
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CImgRecDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENIMG, &CImgRecDlg::OnBnClickedButtonOpenimg)
	ON_BN_CLICKED(IDC_BUTTON_EDGEDET, &CImgRecDlg::OnBnClickedButtonEdgedet)
	ON_BN_CLICKED(IDC_BUTTON_HOUGHTRAN, &CImgRecDlg::OnBnClickedButtonHoughtran)
	ON_BN_CLICKED(IDC_BUTTON_FILTER, &CImgRecDlg::OnBnClickedButtonFilter)
ON_BN_CLICKED(IDC_BUTTON_RECTREC, &CImgRecDlg::OnBnClickedButtonRectrec)
ON_BN_CLICKED(IDC_BUTTON_CARDREC, &CImgRecDlg::OnBnClickedButtonCardrec)
ON_BN_CLICKED(IDC_BUTTON_GETMODEL, &CImgRecDlg::OnBnClickedButtonGetmodel)
END_MESSAGE_MAP()


// CImgRecDlg ��Ϣ�������

BOOL CImgRecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	model_flag_pixel = Algorithm::load<KNearest>("flag_knn_pixel.xml");
	model_num_pixel = Algorithm::load<KNearest>("num_knn_pixel.xml");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImgRecDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImgRecDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImgRecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImgRecDlg::DrawcvMat(cv::Mat m_cvImg, UINT ID)
{
	cv::Mat img;
	CRect rect;

	GetDlgItem(ID)->GetClientRect(&rect);
	if (rect.Width() % 4 != 0)
	{
		rect.SetRect(rect.left, rect.top, rect.left + (rect.Width() + 3) / 4 * 4, rect.bottom);  //����ͼ����Ϊ4�ı���
		GetDlgItem(ID)->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOMOVE);
	}

	Rect dst(rect.left, rect.top, rect.right, rect.bottom);
	cv::resize(m_cvImg, img, cv::Size(rect.Width(), rect.Height()));  //ʹͼ����Ӧ�ؼ���С 
	
	unsigned int m_buffer[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
	BITMAPINFO* m_bmi = (BITMAPINFO*)m_buffer;
	BITMAPINFOHEADER* m_bmih = &(m_bmi->bmiHeader);
	memset(m_bmih, 0, sizeof(*m_bmih));
	m_bmih->biSize = sizeof(BITMAPINFOHEADER);
	m_bmih->biWidth = img.cols;   //����Ϊ4�ı���
	m_bmih->biHeight = -img.rows; //�����¶��ϵ�λͼ�� �߶�Ϊ��
	m_bmih->biPlanes = 1;
	m_bmih->biCompression = BI_RGB;
	m_bmih->biBitCount = 8 * img.channels();

	if (img.channels() == 1)  //��ͼ��Ϊ�Ҷ�ͼ��ʱ��Ҫ���õ�ɫ����ɫ
	{
		for (int i = 0; i < 256; i++)
		{
			m_bmi->bmiColors[i].rgbBlue = i;
			m_bmi->bmiColors[i].rgbGreen = i;
			m_bmi->bmiColors[i].rgbRed = i;
			m_bmi->bmiColors[i].rgbReserved = 0;
		}
	}

	CDC *pDC = GetDlgItem(ID)->GetDC();
	::StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, rect.Width(), rect.Height(), img.data, (BITMAPINFO*)m_bmi, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(pDC);
}


lineSegment lineAssess(const Mat& img, vector<Vec3i>& linePt) //����ĳһ��ֱ�������������ص�������룬����һ�����ص�ƫ��
{
	const uchar *p = img.ptr<uchar>(0);
	int width = img.size().width;
	//����Ҫ�����ǰ�linePt�еĵ㻮�ֵ�lineAreas�С�
	vector<vector<Vec3i>> lineAreas;
	lineAreas.reserve(10);
	vector<Vec3i>::iterator it;
	vector<int> newPt;
	newPt.reserve(8);
	vector<int> temp1;
	temp1.reserve(8);
	vector<Vec3i> temp2;	//temp2�����ڸ�lineAreas����һ��
	int arean = 0, i, j = 0, k = 0, dst, flag, lsize = (int)linePt.size(), lsizen = 0;
	Vec3i nearPt[8] = {Vec3i(-1,1,0),Vec3i(0,1,0), Vec3i(1,1,0),Vec3i(-1,0,0),Vec3i(1,0,0),Vec3i(-1,-1,0),Vec3i(0,-1,0),Vec3i(1,-1,0) };
	
	for (it = linePt.begin(); it != linePt.end(); ++it)
		if (p[(*it)[1] * width + (*it)[0]] == 0) { (*it)[2] = INT_MIN; ++lsizen; }

	while (lsizen != lsize)//�����ѭ������һ�Σ�����һƬ����
	{
		lineAreas.push_back(temp2);	//����һ��
		++arean;
		for (i = 0; i < lsize; ++i)
			if (linePt[i][2] != INT_MIN)
			{
				newPt.push_back(i);
				break;
			}
		lineAreas[arean - 1].reserve((lsize - lsizen)/2 +1);
		
		while (1)	//��ѭ������һ�Σ�Ŀ���������һ������
		{
			for (i = 0; i < newPt.size(); ++i)
			{
				if (linePt[newPt[i]][2] < 0 && linePt[newPt[i]][2] != INT_MIN)linePt[newPt[i]][2] = -linePt[newPt[i]][2];
				(lineAreas[arean - 1]).push_back(linePt[newPt[i]]);
				linePt[newPt[i]][2] = INT_MIN;
				++lsizen;
			}
			for (i = 0; i < newPt.size(); ++i)	//ÿ���������ĵ����newPt��ÿ���µ�������newPt�еĵ㿪ʼ���������ϴ������ĵ㣩
			{
				/*�����������8��ͨ��������жϣ������Ŀ��㼯linePt�У������õ�temp1�У������õ�newPt�У�*/
				for (k = newPt[i]; k < newPt[i] + 6 && k < lsize; ++k)
				{
					if (linePt[k][2] < 0)continue;
					flag = 0;
					for (dst = 0; dst < 8; ++dst)
					{
						if (linePt[k][0] == linePt[newPt[i]][0] + nearPt[dst][0] && linePt[k][1] == linePt[newPt[i]][1] + nearPt[dst][1])
						{
							flag = 1;
							temp1.push_back(k);
							linePt[k][2] = -linePt[k][2];
							break;
						}
					}
				}
				
			}
			if (temp1.empty())			//����û���µ������㡣�����ʱ��������temp1��i��һ��������Ƭ��������
			{
				newPt.clear();
				temp1.clear();
				++i;
				break;
			}
			else
			{						//�������µ������㡣�����������ĵ����newPt�������ʱ��������temp1
				newPt.clear();
				for (k = 0; k < temp1.size(); ++k)
				{
					newPt.push_back(temp1[k]);
				}
				temp1.clear();
			}

		}

	}

	int max = 0;
	int maxi;
	int startOrder, endOrder , startNum, endNum;
	Point startPt,endPt;
	for (i = 0; i < lineAreas.size(); ++i)
		if (lineAreas[i].size() > max)max = (int)lineAreas[i].size(), maxi = i;
	startOrder = endOrder = lineAreas[maxi][0][2];
	startNum = endNum = 0;
	for (i = 1; i < lineAreas[maxi].size(); ++i)
	{
		if (lineAreas[maxi][i][2] >= endOrder) endOrder = lineAreas[maxi][i][2], endNum = i;
		if (lineAreas[maxi][i][2] < startOrder) startOrder = lineAreas[maxi][i][2], startNum = i;
	}
	startPt = Point(lineAreas[maxi][startNum][0], lineAreas[maxi][startNum][1]);
	endPt = Point(lineAreas[maxi][endNum][0], lineAreas[maxi][endNum][1]);
	return lineSegment(startPt,endPt,max);
}


Vec2d findlinePt(const Vec2f& line, const Mat& img, vector<Vec3i>& linePt)//ĳֱ�߾��������ص㼰�����ڽӵ�
{
	double k, b;
	double rho = line[0], theta = line[1];
	int x, y, order;
	int xmin, xmax, ymin, ymax, temp;
	int width = img.size().width;
	int height = img.size().height;
	linePt.reserve(width*height / 1000);

	if (abs(theta - 0) < 0.00001)//k=�޵Ĵ�ֱ��
	{
		order = 0;
		x = (int)(abs(rho) + 0.5);
		for (y = 0; y < height; ++y)
		{
			linePt.push_back(Vec3i(x, y, order));
			if (x > 0)linePt.push_back(Vec3i(x - 1, y, order));
			if (x < width - 1)linePt.push_back(Vec3i(x + 1, y, order));
			++order;
		}
		return Vec2d((-DBL_MAX)/10000 / rho, DBL_MAX/10000);
	}
	if (abs(theta - CV_PI/2) < 0.00001)//k=0��ˮƽ��
	{
		order = 0;
		y = (int)(abs(rho) + 0.5);
		for (x = 0; x < width; ++x)
		{
			linePt.push_back(Vec3i(x - 1, y, order));
			if (y > 0)linePt.push_back(Vec3i(x, y - 1, order));
			if (y < height - 1)linePt.push_back(Vec3i(x, y + 1, order));
			++order;
		}
		return  Vec2d(0, rho);
	}

	k = -1.0 / tan(theta);
	b = rho / sin(theta);

	if (abs(k) >= 1)//��y��������x
	{
		ymin = (int)((0*k + b) + 0.5);
		ymax = (int)(((width - 1)*k + b) + 0.5);
		if (ymax < ymin) { temp = ymin; ymin = ymax; ymax = temp; }
		if (ymin < 0)ymin = 0;
		if (ymax > height - 1)ymax = height - 1;

		xmin = (int)((ymin - b) / k + 0.5);
		xmax = (int)((ymax - b) / k + 0.5);
		order = 0;

		linePt.push_back(Vec3i(xmin, ymin, order));
		if (xmin > 0)linePt.push_back(Vec3i(xmin - 1, ymin, order));
		if (xmin < width - 1)linePt.push_back(Vec3i(xmin + 1, ymin, order));
		++order;

		for (y = ymin + 1; y <= ymax - 1; ++y)
		{
			x = (int)((y - b) / k + 0.5); 
			linePt.push_back(Vec3i(x,y,order));
			linePt.push_back(Vec3i(x - 1, y, order));
			linePt.push_back(Vec3i(x + 1, y, order));
			++order;
		}
		linePt.push_back(Vec3i(xmax, ymax, order));
		if (xmax > 0)linePt.push_back(Vec3i(xmax - 1, ymax, order));
		if (xmax < width - 1)linePt.push_back(Vec3i(xmax + 1, ymax, order));

	}
	else
	{
		xmin = (int)((0 - b)/k + 0.5);
		xmax = (int)(((height - 1) - b)/k + 0.5);
		if (xmax < xmin) { temp = xmin; xmin = xmax; xmax = temp; }
		if (xmin < 0)xmin = 0;
		if (xmax > width - 1)xmax = width - 1;

		ymin = (int)(xmin * k + b + 0.5);
		ymax = (int)(xmax * k + b + 0.5);
		order = 0;

		linePt.push_back(Vec3i(xmin, ymin , order));
		if (ymin > 0)linePt.push_back(Vec3i(xmin, ymin - 1, order));
		if (ymin < height - 1)linePt.push_back(Vec3i(xmin, ymin + 1, order));
		++order;

		for (x = xmin + 1; x <= xmax - 1; ++x)
		{
			y = (int)(k*(x - 0.5) + b + 0.5);
			linePt.push_back(Vec3i(x,y,order));
			linePt.push_back(Vec3i(x, y - 1, order));
			linePt.push_back(Vec3i(x, y + 1, order));
			++order;
		}

		linePt.push_back(Vec3i(xmin, ymax, order));
		if (ymax > 0)linePt.push_back(Vec3i(xmax, ymax - 1, order));
		if (ymax < height - 1)linePt.push_back(Vec3i(xmax, ymax + 1, order));

	}
	return Vec2d(k, b);
}


//��ͼ��

void CImgRecDlg::OnBnClickedButtonOpenimg()
{
	
	CFileDialog dlg(true, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ͼ���ļ�(*.bmp;*.jpg)|*.bmp;*.jpg|ALL Files (*.*) |*.*||"));
	if (dlg.DoModal() != IDOK)return;

	USES_CONVERSION;
	Mat tempImage = imread(W2A(dlg.GetPathName()));

	//�ֱ��ʵ���
	double rate = pow(416 * 312.0 / (tempImage.cols*tempImage.rows),0.5);
	resize(tempImage, srcImage, Size(), rate, rate);
	//���ȵ���
	Vec3b *p = srcImage.ptr<Vec3b>(0);
	int i, j, k;
	int width = srcImage.size().width;
	int height = srcImage.size().height;
	double ave = 0, lightrate;
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				ave += p[i*width + j][k];
			}
		}
	ave /= (width*height*3);
	if (ave < 125) 
	{
		lightrate = 125.0 / ave;
		for (i = 0; i < height; ++i)
			for (j = 0; j < width; ++j)
			{
				for (k = 0; k < 3; ++k)
				{
					p[i*width + j][k] = saturate_cast<uchar>((int)(p[i*width + j][k] * lightrate));
				}
			}
	}


	DrawcvMat(srcImage, IDC_STATIC_CVMAT);
}



//��Ե���
void CImgRecDlg::OnBnClickedButtonEdgedet()
{
	UpdateData(true);
	Canny(srcImage, edgeImage, 40, 100);

	Mat tempImage;
	edgeImage.copyTo(tempImage);
	uchar *pE = edgeImage.ptr<uchar>(0);
	uchar *pt = tempImage.ptr<uchar>(0);
	Vec3b *pS = srcImage.ptr<Vec3b>(0);
	int i, j;
	int width = srcImage.size().width;
	int height = srcImage.size().height;
	double temp;
	/*��Ե��������*/
	for (i = 2; i < height - 2; ++i)//����һ�����ص����(������)
		for (j = 2; j < width - 2; ++j)
		{
			if (pE[i*width + j] == 255)
			{
				pt[i*width + j + 1] = 255;
				pt[i*width + j - 1] = 255;
				pt[(i + 1)*width + j] = 255;
				pt[(i - 1)*width + j] = 255;

				pt[(i - 1)*width + j - 1] = 255;
				pt[(i - 1)*width + j + 1] = 255;
				pt[(i + 1)*width + j - 1] = 255;
				pt[(i + 1)*width + j + 1] = 255;

				pt[i*width + j + 2] = 255;
				pt[i*width + j - 2] = 255;
				pt[(i + 2)*width + j ] = 255;
				pt[(i - 2)*width + j ] = 255;
			}
		}
	/*������ɫ��Ϣ���Ա�Ե�����������*/
	for (i = 1; i < height - 1; ++i)
		for (j = 1; j < width - 1; ++j)//��ֹԽ��
		{
			if (pt[i*width + j] == 255 /*|| pE[i*width + j+1] == 255 || pE[i*width + j-1] == 255 || pE[(i+1)*width + j] == 255 || pE[(i-1)*width + j] == 255*/ )
			{
				temp = (pS[i*width + j][0] + pS[i*width + j][1] + pS[i*width + j][2]) / 3.0;
				if (abs(pS[i*width + j][0] - temp) + abs(pS[i*width + j][1] - temp) + abs(pS[i*width + j][2] - temp) < 25 && temp>190)
				{
					pE[i*width + j] = 255;
				}
				else pE[i*width + j] = 0;
			}
		}

	//Canny(edgeImage, edgeImage, 40, 100);

	//DrawcvMat(edgeImage, IDC_STATIC_CVMAT);
	//imwrite("output\\edge.jpg", edgeImage);
}


void CImgRecDlg::OnBnClickedButtonHoughtran()
{
	UpdateData(true);
	lines.clear(); 
	HoughLines(edgeImage, lines, 1, CV_PI / 360, 105);
	//����ע��������������ʾ��ֵ����ֵԽ�󣬱�������Խ��׼���ٶ�Խ�죬�õ���ֱ��Խ�٣��õ���ֱ�߶��Ǻ��а��յ�ֱ�ߣ�
	//lines��һ���ΪVec3fʱ����������������ֱ�߾��������ص��������Ұ����޸�Ϊֱ�߾��������������������
	srcImage.copyTo(houghImage);
	float rho;//ֱ�ߵĳ���
	float theta;//ֱ�ߵĽǶ�
	double c1, c2;
	double x0, y0;
	Point pt1, pt2;
	for (size_t i = 0; i<lines.size(); i++)
	{
		rho = lines[i][0];
		theta = lines[i][1];
		c1 = cos(theta), c2 = sin(theta);
		x0 = c1*rho, y0 = c2*rho;
		pt1.x = cvRound(x0 - 1000 * c2);
		pt1.y = cvRound(y0 + 1000 * c1);
		pt2.x = cvRound(x0 + 1000 * c2);
		pt2.y = cvRound(y0 - 1000 * c1);
		line(houghImage, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA); //Scalar�������ڵ����߶���ɫ�����������⵽���߶���ʾ����ʲô��ɫ
	}
	/*imwrite("output\\hough.jpg", houghImage);
	DrawcvMat(houghImage, IDC_STATIC_CVMAT);*/
}


void CImgRecDlg::OnBnClickedButtonFilter()
{
	/*ͳ�ƾ���ÿһ��ֱ�ߵ��������ص�����*/
	int i, j;
	vector<Vec3i> linePt; //���򴢴�ĳһ��ֱ�߾������������ص�
	Vec2d temp;
	uchar *p = edgeImage.ptr<uchar>(0);

	linesS.clear();
	linesS.reserve(lines.size());
	for (i = 0; i < lines.size(); ++i)
	{
		//ÿһѭ������һ��ֱ��
		temp = findlinePt(lines[i], edgeImage, linePt); //����findlinePtѰ�Ҿ���ֱ�ߵ��������ص㣨������ֵ�Ƿ�Ϊ255��,���浽linePt��������ֱ�ߵ�б�ʺͽؾ�
		linesS.push_back(lineAssess(edgeImage, linePt)); //����lineAssess����ֱ�߾��������8��ͨ��������������ֵ������255��
		linesS[linesS.size() - 1].k = temp[0];
		linesS[linesS.size() - 1].b = temp[1];
		(*(linesS.end() - 1)).theta = lines[i][1];
		linePt.clear();
	}

	UpdateData(true);
	vector<lineSegment>::iterator it;
	for (it = linesS.begin(); it != linesS.end(); )
	{
		if ((*it).c < 90)//���ӵ�����ֵ
			it = linesS.erase(it);
		else ++it;
	}

	/*ȥ������ֱ��*/
	int size = linesS.size();
	if (size == 1);
	else 
	{
		int diff, temp1, temp2;
		for (i = 0; i < size; ++i)
		{
			if (linesS[i].c == -1)continue;
			for (j = 0; j < size; ++j)
			{
				if (j == i)continue;
				if (linesS[i].c == -1)continue;
				temp1 = abs(linesS[i].pt1.x - linesS[j].pt1.x) + abs(linesS[i].pt1.y - linesS[j].pt1.y) + abs(linesS[i].pt2.x - linesS[j].pt2.x) + abs(linesS[i].pt2.y - linesS[j].pt2.y);
				temp2 = abs(linesS[i].pt1.x - linesS[j].pt2.x) + abs(linesS[i].pt1.y - linesS[j].pt2.y) + abs(linesS[i].pt2.x - linesS[j].pt1.x) + abs(linesS[i].pt2.y - linesS[j].pt1.y);
				diff = temp1 < temp2 ? temp1 : temp2;
				if (diff < 70 )
				{
					if (linesS[i].c < linesS[j].c)
					{
						linesS[i].c = -1;
						break;
					}
					if (linesS[i].c >= linesS[j].c)
					{
						linesS[j].c = -1;
					}
				}
			}
		}
	}
	for (it = linesS.begin(); it != linesS.end(); )
	{
		if ((*it).c < 90)//���ӵ�����ֵ
			it = linesS.erase(it);
		else ++it;
	}
	/*ȥ������ֱ�����*/
	


	//���λ���ÿ���߶�
	srcImage.copyTo(lineImage);
	for (size_t i = 0; i<linesS.size(); i++)
	{
		line(lineImage, linesS[i].pt1, linesS[i].pt2, Scalar(55, 100, 195), 1, LINE_AA); //Scalar�������ڵ����߶���ɫ�����������⵽���߶���ʾ����ʲô��ɫ
	}
	//imwrite("output\\line.jpg", lineImage);
	//DrawcvMat(lineImage, IDC_STATIC_CVMAT);
}

int abs(Point a, Point b = Point(0,0))//��������֮��ĳ�������
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

void CImgRecDlg::OnBnClickedButtonRectrec()
{
	UpdateData(true);
	Point pt1, pt2; 
	int size = linesS.size();
	if (size < 4)
	{
		AfxMessageBox(CString("δ��⵽�㹻��ֱ�߱�Ե���޷�ʶ��"));
		return;
	}
	int i, j, k, dst, flag;
	vector<Vec4i> rect;//����ʶ��ͬһ��ͼƬ�ϵĶ�����Ρ��������������Ĵ���ֻ�Լ�⵽�ĵ�һ�����ν���ʶ��
	Vec3i temp;
	for (i = 0; i < size; ++i)
	{
		flag = 0;
		if (linesS[i].theta < 0)continue;
		for (j = 0; j < size; ++j)
		{
			if (j == i)continue;
			if (linesS[j].theta < 0)continue;
			if (abs(abs(linesS[j].theta - linesS[i].theta) - CV_PI / 2) < 0.2)//0.2����������ֱ�߽Ƕ����
			{
				if (abs(linesS[j].pt1, linesS[i].pt1) < 50 || abs(linesS[j].pt2, linesS[i].pt1) < 50)
				{
					flag += 1;
					break;
				}
			}
		}
		if (flag != 1) 
		{
			linesS[i].theta = -DBL_MAX;
			continue;
		}
		for (k = 0; k < size; ++k)
		{
			if (k == i|| k==j)continue;
			if (linesS[k].theta < 0)continue;
			if (abs(abs(linesS[k].theta - linesS[i].theta) - CV_PI / 2) < 0.2)
			{
				if (abs(linesS[k].pt1, linesS[i].pt2) < 50 || abs(linesS[k].pt2, linesS[i].pt2) < 50)
				{
					flag += 1;
					break;
				}
			}
		}
		if (flag != 2)
		{
			linesS[i].theta = -DBL_MAX;//������Ϊ-DBL_MAX��ʾ�����õ��߶�
			continue;
		}
		for (dst = 0; dst < size; ++dst)
		{
			if (dst == i || dst == j || dst == k)continue;
			if (linesS[dst].theta < 0)continue;
			if (abs(linesS[dst].theta - linesS[i].theta) < 0.2 || abs(abs(linesS[dst].theta - linesS[i].theta)-CV_PI) < 0.2)
			{
				if (abs(linesS[dst].pt1, linesS[j].pt2) < 50 || abs(linesS[dst].pt2, linesS[j].pt2) < 50 || abs(linesS[dst].pt1, linesS[j].pt1) < 50|| abs(linesS[dst].pt2, linesS[j].pt1) < 50)
				{
					flag += 1;
					break;
				}
			}
		}
		if (flag != 3)
		{
			linesS[i].theta = -DBL_MAX;//������Ϊ-DBL_MAX��ʾ�����õ��߶�
			continue;
		}
		else
		{
			rect.push_back(Vec4i(i,j,k,dst));//Ϊ�˼򻯣���3���߶�ȷ����ֱ��
			linesS[i].theta = -linesS[i].theta;
			linesS[j].theta = -linesS[j].theta;
			linesS[k].theta = -linesS[k].theta;
			linesS[dst].theta = -linesS[dst].theta;
			//3���߶α��������󣬽���thetaȡ����ʾ���߶��Ѿ�ʶ����ˣ����ٲ���ʶ��Ҳ�����Ժ�ԭ�������ѭ������������thetaΪ�����߶Σ�������ԭ���������ģ�
		}
	}

	if (rect.size() < 1)
	{
		AfxMessageBox(CString("δ��⵽�κ�ֽ�ƾ��α�Ե���޷�ʶ��"));
		return;
	}
	/*���ˣ����μ����ϡ�ע���ʱ���õ�thetaȫ����ȡ����*/

	/*���ڿ�ʼ������ε���ز���������ͼ�������ת�Ͳü�*/
	double rectWidth, rectHeight;//���εĿ�͸ߵ�ƽ��
	double tempdbl;
	double rotateDeg;//��Ҫ��ʱ����ת�ĽǶȣ������ƣ�

	Point corner[4];//����ֽ�Ʊ�Ե���ε��Ľ�����
	corner[0] = linesS[rect[0][0]].cal_pt(linesS[rect[0][1]]);
	corner[1] = linesS[rect[0][0]].cal_pt(linesS[rect[0][2]]);
	corner[2] = linesS[rect[0][1]].cal_pt(linesS[rect[0][3]]);
 	corner[3] = linesS[rect[0][2]].cal_pt(linesS[rect[0][3]]);

	/*����ֽ�ƾ��εĶ̱ߵ���ǣ��Ա���нǶ�����*/
	rectHeight = linesS[rect[0][0]].cal_len();
	rectWidth = (linesS[rect[0][1]].cal_len() + linesS[rect[0][2]].cal_len()) / 2;
	rotateDeg = (atan(linesS[rect[0][1]].k) + atan(linesS[rect[0][2]].k)) / 2;
	if (rectWidth > rectHeight)
	{
		rotateDeg = atan(linesS[rect[0][0]].k);
		tempdbl = rectHeight;
		rectHeight = rectWidth;
		rectWidth = tempdbl;
	}


	/*ɾ������thetaֵΪ-DBL_MAX�������߶Ρ�����ֵΪ���Ҳ�Ϊ-DBL_MAX��theataȡ������ԭ)*/
	vector<lineSegment>::iterator it;
	for (it = linesS.begin(); it != linesS.end();)
	{
		if ((*it).theta == -DBL_MAX)it = linesS.erase(it);
		else if ((*it).theta < 0)(*it).theta = -(*it).theta;
		else ++it;
	}


	/*���ڿ�ʼ������ת*/

	Size src_sz = srcImage.size();
	Size div_sz(src_sz.width, src_sz.height);

	Point srcCenter = Point(src_sz.width / 2, src_sz.height / 2);

	Mat rot_mat = getRotationMatrix2D(srcCenter, rotateDeg / CV_PI * 180, 1.0);
	Rect box = RotatedRect(srcCenter, src_sz, rotateDeg / CV_PI * 180).boundingRect();
	rot_mat.at<double>(0, 2) += box.width / 2 - srcCenter.x;
	rot_mat.at<double>(1, 2) += box.height / 2 - srcCenter.y;

	warpAffine(srcImage, divImage, rot_mat, box.size());
	
	//���÷���任���������ת���ͼƬ�ϣ�ֽ�ƾ��ν���������
	for (i = 0; i < 4; ++i)
	{
		corner[i] = Point((int)(rot_mat.at<double>(0, 0)*corner[i].x + rot_mat.at<double>(0, 1) *corner[i].y + rot_mat.at<double>(0, 2) + 0.5), (int)(rot_mat.at<double>(1, 0) * corner[i].x + rot_mat.at<double>(1, 1) * corner[i].y + rot_mat.at<double>(1, 2)));
	}

	//���ĸ��������бȽϣ��ҳ����ϽǺ����½�
	Point topleft = corner[0], loweright = corner[0];
	for (i = 1; i < 4; ++i)
	{
		if (abs(corner[i]) > abs(loweright))loweright = corner[i];
		if (abs(corner[i]) < abs(topleft))topleft = corner[i];
	}

	//�ü�ͼƬ
	divImage = divImage(Rect(topleft.x, topleft.y, loweright.x - topleft.x, loweright.y - topleft.y));
	
	//imwrite("output\\div.jpg", divImage);
	//DrawcvMat(divImage, IDC_STATIC_CVMAT);
}




//ֽ��ʶ��
void CImgRecDlg::OnBnClickedButtonCardrec()
{
	OnBnClickedButtonEdgedet();
	OnBnClickedButtonHoughtran();
	OnBnClickedButtonFilter();
	OnBnClickedButtonRectrec();

	Mat bigDiv;
	//��divImage�Ƚ��зŴ�Ӧ�ÿ��������ֵ�ָ�ͱ߽�Ѱ�ҵ�׼ȷ��
	resize(divImage, bigDiv, Size(), 3, 3);
	Mat dst = bigDiv(Rect(0, 0, (int)(bigDiv.cols / 6.5 + 0.5), (int)(bigDiv.rows / 3.0 + 0.5)));//ֽ�ƾ������Ͻǵ�����Ľ�ȡ��ע���ȡ�Ĳ���
	cvtColor(dst, dst, CV_RGB2GRAY);
	threshold(dst, dst, 0, 255, THRESH_BINARY_INV| CV_THRESH_OTSU);
	imwrite("output\\cardCorner.jpg", dst);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<vector<Point>>::iterator it_ct;
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	int up = 1000, down = 1000, right = 1000, left = 1000;
	double rectAreaThr = dst.cols*dst.rows*0.04;//С���ι�����ֵ�Ķ���
	Rect rect;
	//�����Դ�С����ʶ��
	int vaildAreaNum = 0;
	for (it_ct = contours.begin(); it_ct < contours.end(); it_ct++) 
	{
		rect = boundingRect(*it_ct);
		if (rect.area() < rectAreaThr)continue;//����Ϊ��ȥ��С��������ĸ��š�������ε���������Ͻ����������0.04���������ǿ϶��������ֻ�ɫ
		vaildAreaNum++;
		if (rect.x + rect.y < up + left)//���������ε����ϽǸ��ӽ�(0,0)(��������)����ʹ�øþ���
		{
			up = rect.y;
			left = rect.x;
			down = rect.y + rect.height;
			right = rect.x + rect.width;
		}
	}
	//if (down - up < 25 || right - left < 20) return; //�ֱ��ʵ���25*20�ж�Ϊ��ȡ����ʧ��
	//if (down - up > 50 || right - left > 50) return; //�ֱ��ʸ���50*50�ж�Ϊ��ȡ����ʧ��
	//����Ϊ�������Դ�С����ʶ�������ͨ�������3��������Ϊ���Ǵ�С����joker
	if (vaildAreaNum > 3)
	{
		AfxMessageBox(_T("��/С��"));
		return;
	}
	//���������right��ֵ�����˼򵥵��жϣ��Ա������Ķ�����10��ʶ������͵���İ취�����ݲ��ԣ������޸Ķ�10������������û��Ӱ��
	if (right <= dst.cols / 3 *2 )right = dst.cols - 1;
	Mat num = dst(Range(up, down), Range(left, right));
	Mat tmp = dst(Range(down, dst.rows), Range(left, right));
	/*imshow("num", num);*/
	//-------------------------��ȥ�������򣬴�����������ͬ��������ȡ��ɫ--------------------------------

	findContours(tmp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	up = 1000; down = 1000; right = 1000; left = 1000; rectAreaThr = tmp.cols*tmp.rows*0.04;
	for (it_ct = contours.begin(); it_ct < contours.end(); it_ct++) {
		rect = boundingRect(*it_ct);
		if (rect.area() < rectAreaThr)continue;//����Ϊ��ȥ��С��������ĸ��š�������ε���������Ͻ����������0.04���������ǿ϶��������ֻ�ɫ
		if (rect.x + rect.y < up + left)//���������ε����ϽǸ��ӽ�(0,0)(��������)����ʹ�øþ���
		{
			up = rect.y;
			left = rect.x;
			down = rect.y + rect.height;
			right = rect.x + rect.width;
		}
	}
	//if (down - up < 25) return; //�ֱ��ʵ���25*20�ж�Ϊ��ȡ��ɫʧ��
	Mat flag = tmp(Range(up, down), Range(left, right));
	/*imshow("flag", flag);*/
	imwrite("output\\num.jpg", num); //Ԥ�����ݱ���
	imwrite("output\\flag.jpg", flag);

	void train_pixel();
	int predictNum(Mat &m, Ptr<KNearest>);
	int predictFlag(Mat &m, Ptr<KNearest>);

	int resNum = predictNum(num, model_num_pixel);
	int resFlag = predictFlag(flag, model_flag_pixel);
	CString showRes;
	switch (resFlag) {
	case 1:
		showRes = "���� "; break;
	case 2:
		showRes = "���� "; break;
	case 3:
		showRes = "÷�� "; break;
	case 4:
		showRes = "���� "; break;
	default:
		break;
	}
	switch (resNum) {
	case 1:
		showRes += "A"; break;
	case 11:
		showRes += "J"; break;
	case 12:
		showRes += "Q"; break;
	case 13:
		showRes += "K"; break;
	default:
		CString temp;
		temp.Format(_T("%d"), resNum); 
		showRes += temp;
		break;
	}
	AfxMessageBox(showRes);
}


void train_pixel()
{
	Mat NumData, NumLabels, FlagData, FlagLabels;
	int trainNum = 40;
	string NumName[] = { "1_", "2_", "3_", "4_", "5_", "6_", "7_", "8_", "9_", "10_", "j_", "q_", "k_" };
	string FlagName[] = { "hongtao_", "fangkuai_", "meihua_", "heitao_" };
	for (int i = 0; i < trainNum * 13; i++) {
		Mat img, tmp;
		string path = "TrainSample\\";
		path.append(NumName[i / trainNum]).append(to_string((i%trainNum))).append(".jpg");
		img = imread(path, 0);
		resize(img, tmp, Size(30, 40));
		NumData.push_back(tmp.reshape(0, 1));  //���л��������������
		NumLabels.push_back(i / trainNum + 1);  //��Ӧ�ı�ע
	}
	NumData.convertTo(NumData, CV_32F); //uchar��ת��Ϊcv_32f
										//ʹ��KNN�㷨
	int K = 5;
	Ptr<TrainData> tData = TrainData::create(NumData, ROW_SAMPLE, NumLabels);
	Ptr<KNearest> NumModel = KNearest::create();
	NumModel->setDefaultK(K);
	NumModel->setIsClassifier(true);
	NumModel->train(tData);
	NumModel->save("./num_knn_pixel.xml");
	//-----------------------------------------------------------------------------------------------
	for (int i = 0; i < trainNum * 4; i++) {
		Mat img, tmp;
		string path = "TrainSample\\";
		path.append(FlagName[i / trainNum]).append(to_string((i%trainNum))).append(".jpg");
		img = imread(path, 0);
		resize(img, tmp, Size(30, 30));
		FlagData.push_back(tmp.reshape(0, 1));  //���л��������������
		FlagLabels.push_back(i / trainNum + 1);  //��Ӧ�ı�ע
	}
	FlagData.convertTo(FlagData, CV_32F); //uchar��ת��Ϊcv_32f
										  //ʹ��KNN�㷨
	int L = 5;
	Ptr<TrainData> tFlag = TrainData::create(FlagData, ROW_SAMPLE, FlagLabels);
	Ptr<KNearest> FlagModel = KNearest::create();
	FlagModel->setDefaultK(L);
	FlagModel->setIsClassifier(true);
	FlagModel->train(tFlag);
	FlagModel->save("./flag_knn_pixel.xml");
}

int predictNum(Mat &m, Ptr<KNearest> model_num_pixel) 
{
	Mat temp;
	resize(m, temp, Size(30, 40));
	Mat vec1;
	vec1.push_back(temp.reshape(0, 1));
	vec1.convertTo(vec1, CV_32F);
	int r1 = model_num_pixel->predict(vec1);   //�������н���Ԥ��
	return r1;
}

int predictFlag(Mat &m, Ptr<KNearest> model_flag_pixel) 
{
	Mat temp;
	resize(m, temp, Size(30, 30));
	Mat vec1;
	vec1.push_back(temp.reshape(0, 1));
	vec1.convertTo(vec1, CV_32F);
	int r1 = model_flag_pixel->predict(vec1);   //�������н���Ԥ��
	return r1;
}

void CImgRecDlg::OnBnClickedButtonGetmodel()
{
	train_pixel();
}
