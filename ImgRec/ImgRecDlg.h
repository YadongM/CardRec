
// ImgRecDlg.h : 头文件
//

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> //使用resize
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
	Point cal_pt(const lineSegment& other)//计算与另一条线段的交点
	{
		double k2 = other.k;
		double b2 = other.b;
		double x = (b2 - b) / (k - k2);
		double y;
		if(abs(k)<abs(k2))y = k*x + b;//修正斜率太大计算误差带来的bug？待研究
		else y = k2*x + b2;
		if (y>INT_MAX||y<INT_MIN||x>INT_MAX||x<INT_MIN)return Point(INT_MIN, INT_MIN);//超过int的表示范围，就认为没有交点
		return Point(int(x+0.5), int(y+0.5));//简单起见不考虑越界（图像边界）的情况。应该在交点不会越界的情况使用。
	}
	double cal_len()//计算这条线段的长度
	{
		return pow(((double)pt2.x - (double)pt1.x)*((double)pt2.x - (double)pt1.x) + ((double)pt2.y - (double)pt1.y)*((double)pt2.y - (double)pt1.y),0.5);
	}
};


// CImgRecDlg 对话框
class CImgRecDlg : public CDialogEx
{
// 构造
public:
	CImgRecDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGREC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	Mat edgeImage;//使用canny边缘检测后得到的边缘图片，元素类型是uchar
	Mat houghImage;//绘制了霍夫变换检测到的直线的原图片，元素类型是Vec3b
	vector<Vec2f> lines;//霍夫变换得到的原始直线信息
	vector<lineSegment> linesS;//原始直线lines经过计算得到的线段
	Mat lineImage;//对霍夫变换得到的直线过滤后得到的线段图片，元素类型是Vec3b
	Mat divImage;//将纸牌分割出来的图片，元素类型是Vec3b
	int houghC1;
	int houghC2;
	int houghC3;
	int lineFilterThr;
	double cannyC1;
	double cannyC2;
	double whiteColorThr;
	afx_msg void OnBnClickedButtonRectrec();
	int lineSimThr;  //如果两线段a,b，a两端点对应b两端点的城区距离之和小于该阈值，则认为两线段是相似线段，去掉拟合度较差的那一条（两种端点对应方式中取计算结果较小的）
	int rectLinkThr;//表示，当两线段的端点的城区距离小于该值时（且两线段近似垂直），才认为这两个端点可以连接为矩形的一个直角
	afx_msg void OnBnClickedButtonCardrec();
	afx_msg void OnBnClickedButtonGetmodel();
	int whiteColorThr2;

	Ptr<KNearest> model_flag_pixel;
	Ptr<KNearest> model_num_pixel;
};
