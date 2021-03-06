#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

enum RectBound { Left, Top, Right, Bottom, Count };

void CmpAndAssignIfGreater(int &dst, int src);
void CmpAndAssignIfLess(int &dst, int src);
void AllocIntArrayAndFill(int *&arr, int sz, int val);
void InitRectBoundsArray(int *&arr, int sz);

void DrawRectanglesOnDocument(cv::Mat &dstImage, int numRectangles, int *bounds);
void DrawRectanglesOnDocument(cv::Mat &dstImage, std::vector<int> &bounds);
void OCVdispImgInWin(cv::Mat &image, const char *winTypeName, const char *fileName,
	int x, int y, int width, int height);