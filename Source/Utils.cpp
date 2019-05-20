#include "Utils.hpp"
#include <stdlib.h>


void CmpAndAssignIfGreater(int &dst, int src)
{
	if (src > dst)
		dst = src;
}
void CmpAndAssignIfLess(int &dst, int src)
{
	if (src < dst)
		dst = src;
}

void AllocIntArrayAndFill(int *&arr, int sz, int val)
{
	arr = (int*)malloc(sz * sizeof(int));
	for (int i = 0; i < sz; i++)
		arr[i] = val;
}

void InitRectBoundsArray(int *&arr, int sz)
{
	arr = (int*)malloc(sz * sizeof(int));
	int boundsInitValues[] = { INT_MAX, INT_MAX, INT_MIN, INT_MIN };

	for (int i = 0; i < sz; i++)
		arr[i] = boundsInitValues[i%4];
}


void DrawRectanglesOnDocument(cv::Mat &dstImage, int numRectangles, int *bounds)
{
	for (int i = 0; i < numRectangles; i++)
	{
		int G = rand() % 256;
		int B = rand() % 256;
		int R = rand() % 256;
		if (bounds[4 * i + RectBound::Left] < 0 || bounds[4 * i + RectBound::Right] > dstImage.cols ||
			bounds[4 * i + RectBound::Top] < 0 || bounds[4 * i + RectBound::Bottom] > dstImage.rows)
			continue;
		for (int x = bounds[4 * i + RectBound::Left]; x < bounds[4 * i + RectBound::Right]; x++)
			for (int y = bounds[4 * i + RectBound::Top]; y < bounds[4 * i + RectBound::Bottom]; y++)
				dstImage.at<cv::Vec3b>(y, x) = cv::Vec3b(R, G, B) / 2
				+ dstImage.at<cv::Vec3b>(y, x) / 2;
	}
}

void DrawRectanglesOnDocument(cv::Mat &dstImage, std::vector<int> &bounds)
{
	for (int i = 0, nBounds = bounds.size() / RectBound::Count; i < nBounds; i++)
	{
		int G = rand() % 256;
		int B = rand() % 256;
		int R = rand() % 256;
		if (bounds[4 * i + RectBound::Left] < 0 || bounds[4 * i + RectBound::Right] > dstImage.cols ||
			bounds[4 * i + RectBound::Top] < 0 || bounds[4 * i + RectBound::Bottom] > dstImage.rows)
			continue;
		for (int x = bounds[4 * i + RectBound::Left]; x < bounds[4 * i + RectBound::Right]; x++)
			for (int y = bounds[4 * i + RectBound::Top]; y < bounds[4 * i + RectBound::Bottom]; y++)
				dstImage.at<cv::Vec3b>(y, x) = cv::Vec3b(R, G, B) / 2
				+ dstImage.at<cv::Vec3b>(y, x) / 2;
	}
}