
// ImgRecDlg.h : ͷ�ļ�
//

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> //ʹ��resize
#include <opencv2/ml.hpp>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;
using namespace cv::ml;



class lineSegment
{
public:
	lineSegment() {}
	lineSegment(Point a, Point b, int c) :pt1(a), pt2(b),theta(0), c(c) {}
	Point pt1;
	Point pt2;
	double theta;
	double k;
	double b;
	int c;
	Point cal_pt(const lineSegment& other)//��������һ���߶εĽ���
	{
		double k2 = other.k;
		double b2 = other.b;
		double x = (b2 - b) / (k - k2);
		double y;
		if(abs(k)<abs(k2))y = k*x + b;//����б��̫�������������bug�����о�
		else y = k2*x + b2;
		if (y>INT_MAX||y<INT_MIN||x>INT_MAX||x<INT_MIN)return Point(INT_MIN, INT_MIN);//����int�ı�ʾ��Χ������Ϊû�н���
		return Point(int(x+0.5), int(y+0.5));//�����������Խ�磨ͼ��߽磩�������Ӧ���ڽ��㲻��Խ������ʹ�á�
	}
	double cal_len()//���������߶εĳ���
	{
		return pow(((double)pt2.x - (double)pt1.x)*((double)pt2.x - (double)pt1.x) + ((double)pt2.y - (double)pt1.y)*((double)pt2.y - (double)pt1.y),0.5);
	}
};


// CImgRecDlg �Ի���
class CImgRecDlg : public CDialogEx
{
// ����
public:
	CImgRecDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGREC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void DrawcvMat(cv::Mat m_cvImg, UINT ID);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenimg();
	afx_msg void OnBnClickedButtonEdgedet();
	afx_msg void OnBnClickedButtonHoughtran();
	afx_msg void OnBnClickedButtonFilter();
public:
	Mat srcImage;
	Mat edgeImage;//ʹ��canny��Ե����õ��ı�ԵͼƬ��Ԫ��������uchar
	Mat houghImage;//�����˻���任��⵽��ֱ�ߵ�ԭͼƬ��Ԫ��������Vec3b
	vector<Vec2f> lines;//����任�õ���ԭʼֱ����Ϣ
	vector<lineSegment> linesS;//ԭʼֱ��lines��������õ����߶�
	Mat lineImage;//�Ի���任�õ���ֱ�߹��˺�õ����߶�ͼƬ��Ԫ��������Vec3b
	Mat divImage;//��ֽ�Ʒָ������ͼƬ��Ԫ��������Vec3b
	int houghC1;
	int houghC2;
	int houghC3;
	int lineFilterThr;
	double cannyC1;
	double cannyC2;
	double whiteColorThr;
	afx_msg void OnBnClickedButtonRectrec();
	int lineSimThr;  //������߶�a,b��a���˵��Ӧb���˵�ĳ�������֮��С�ڸ���ֵ������Ϊ���߶��������߶Σ�ȥ����϶Ƚϲ����һ�������ֶ˵��Ӧ��ʽ��ȡ��������С�ģ�
	int rectLinkThr;//��ʾ�������߶εĶ˵�ĳ�������С�ڸ�ֵʱ�������߶ν��ƴ�ֱ��������Ϊ�������˵��������Ϊ���ε�һ��ֱ��
	afx_msg void OnBnClickedButtonCardrec();
	afx_msg void OnBnClickedButtonGetmodel();
	int whiteColorThr2;

	Ptr<KNearest> model_flag_pixel;
	Ptr<KNearest> model_num_pixel;
};
