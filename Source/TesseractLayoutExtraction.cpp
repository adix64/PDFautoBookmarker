#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "dirent.h"
#include "Utils.hpp"

#define TSV_PREAMBLE "level" "\t" "page_num" "\t" "block_num" "\t" \
					 "par_num" "\t" "line_num" "\t" "word_num" "\t" \
					 "left" "\t" "top" "\t" "width" "\t" "height" "\t" \
					 "conf" "\t" "text\n"

#define TESSERACT_PATH "C:\\\"Program Files (x86)\"\\Tesseract-OCR"
#define TESSDATA_PATH "C:/Program Files (x86)/Tesseract-OCR/tessdata"
using namespace cv;

void DrawRectanglesOnDocument(cv::Mat &image, int count,
	int *left, int *right, int *top, int *bottom)
{
	for (int i = 0; i < count; i++)
	{
		int G = rand() % 256;
		int B = rand() % 256;
		int R = rand() % 256;
		if (left[i] < 0 || right[i] > image.cols ||
			top[i] < 0 || bottom[i] > image.rows)
			continue;
		for (int x = left[i]; x < right[i]; x++)
			for (int y = top[i]; y < bottom[i]; y++)
				image.at<cv::Vec3b>(y, x) = cv::Vec3b(R, G, B) / 2
				+ image.at<cv::Vec3b>(y, x) / 2;
	}
}


struct TesseractWord
{
	int level, page_num, block_num, par_num, line_num, word_num;
	int left, top, width, height, conf;
	char *text;
};

struct TSVdocument
{
	TesseractWord *tessWords;
	int numWords = 0;
	int num_blocks = -1;
	int num_lines = 0;
	int num_pars = 0;
};

class LayoutAnalyzer
{
private:
	//document input data structure
	TesseractWord *tessWords;
	int numWords = 0;
	int num_blocks = -1;
	int num_lines = 0;
	int num_pars = 0;
	
	//bounding boxes for page elements
	int *block_left, *block_right, *block_top, *block_bottom;
	int *line_left, *line_right, *line_top, *line_bottom;
	int *par_left, *par_right, *par_top, *par_bottom;
	
	std::vector<std::string> inputFilenames, outputFilepaths;

private:

	void CallTesseractOCR(const char *inputDir, const char *inputFile)
	{
		char command[512];
		memset(command, 0, 512);
		const char *end = &inputDir[strlen(inputDir) - 1];
		while (end[0] != '\\' && end > inputDir)
			end--;

		sprintf(command, "%s\\tesseract.exe --tessdata-dir \"%s\" %s\\%s .%s_TSVs\\%s tsv"
			, TESSERACT_PATH, TESSDATA_PATH, inputDir, inputFile, end, inputFile);

		outputFilepaths.push_back("." + std::string(end) + "_TSVs\\" + inputFile + ".tsv");

		printf("\n>>>\n%s\n<<<\n", command);
		system(command);
	}

public:

	LayoutAnalyzer() {}
	~LayoutAnalyzer()
	{
		free(block_left);
		free(par_left);
		free(line_left);

		free(block_top);
		free(par_top);
		free(line_top);

		free(block_right);
		free(par_right);
		free(line_right);

		free(block_bottom);
		free(par_bottom);
		free(line_bottom);
		
		for (int i = 0; i < numWords; i++)
			free(tessWords[i].text);
		free(tessWords);
	}

	void RunTesseractInDir(const char *inputDir)
	{
		DIR *dir;
		struct dirent *ent;

		if ((dir = opendir(inputDir)) != NULL)
		{
			while ((ent = readdir(dir)) != NULL) {
				if (!strcmp(ent->d_name, "."))
					continue;
				if (!strcmp(ent->d_name, ".."))
					continue;

				inputFilenames.push_back(ent->d_name);
			}
			closedir(dir);
		}

		for (int i = 0; i < inputFilenames.size(); i++)
		{
			CallTesseractOCR(inputDir, inputFilenames[i].c_str());
		}
	}

	void Display(const char *filename)
	{
		cv::Mat image;	//(512, 384, CV_8SC3, cvScalar(0, 0, 100));
		image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		cv::Mat image2 = image.clone();
		cv::Mat image3 = image.clone();

		int rows = image.rows;
		int cols = image.cols;
		printf("\nmax_line_num %d\n", num_lines);
		printf("num_blocks %d\n", num_blocks);
		printf("num_pars %d\n", num_pars);


		DrawRectanglesOnDocument(image, num_blocks, block_left, block_right, block_top, block_bottom);
		DrawRectanglesOnDocument(image2, num_lines, line_left, line_right, line_top, line_bottom);
		DrawRectanglesOnDocument(image3, num_pars, par_left, par_right, par_top, par_bottom);


		int _height = (rows * 400) / cols;

		char winName[256];
		memset(winName, 0, 256);
		sprintf(winName, "BLOCKS %s", filename);
		cv::namedWindow(winName, cv::WINDOW_FREERATIO);
		cv::imshow(winName, image);
		cv::resizeWindow(winName, 400, _height);
		cv::moveWindow(winName, 0, 100);

		memset(winName, 0, 256);
		sprintf(winName, "PARAGRAPHS %s", filename);
		cv::namedWindow(winName, cv::WINDOW_FREERATIO);
		cv::imshow(winName, image3);
		cv::resizeWindow(winName, 400, _height);
		cv::moveWindow(winName, 400, 100);

		memset(winName, 0, 256);
		sprintf(winName, "LINES %s", filename);
		cv::namedWindow(winName, cv::WINDOW_FREERATIO);
		cv::imshow(winName, image2);
		cv::resizeWindow(winName, 400, _height);
		cv::moveWindow("Lines", 800, 100);

		
	}

	void RunOnDirectory(const char *inputDir)
	{
		RunTesseractInDir(inputDir);

		for (int i = 0; i < outputFilepaths.size(); i++)
			Run((std::string(inputDir) +"\\"+ inputFilenames[i]).c_str(),
				outputFilepaths[i].c_str());


		
	}

	void Run(const char *inputImageFilename, const char *inputTSVfilename)
	{
		numWords = 0;
		num_blocks = -1;
		num_lines = 0;
		num_pars = 0;

		FILE* fp;
		char buffer[512];

		fp = fopen(inputTSVfilename, "r");

		fscanf(fp, TSV_PREAMBLE);
		while (fgets(buffer, 512, (FILE*)fp))
			numWords++;


		tessWords = (TesseractWord*)malloc(numWords * sizeof(TesseractWord));

		fseek(fp, 0, SEEK_SET);
		fscanf(fp, TSV_PREAMBLE);
		char wordBuf[512];

		int i = 0;
		while (fgets(buffer, 512, (FILE*)fp))
		{
			memset(wordBuf, 0, 512);
			sscanf(buffer, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
				&tessWords[i].level, &tessWords[i].page_num, &tessWords[i].block_num,
				&tessWords[i].par_num, &tessWords[i].line_num, &tessWords[i].word_num,
				&tessWords[i].left, &tessWords[i].top, &tessWords[i].width, &tessWords[i].height,
				&tessWords[i].conf, wordBuf);
			tessWords[i].text = (char*)malloc(strlen(wordBuf) + 1);
			strcpy(tessWords[i].text, wordBuf);

			if (tessWords[i].block_num + 1 > num_blocks)
				num_blocks = tessWords[i].block_num + 1;
			i++;
		}
		fclose(fp);

		std::vector<int> pars_per_block(num_blocks, 0);// = (int*)calloc(num_blocks, sizeof(int));
		for (int i = 0; i < numWords; i++)
			if (tessWords[i].par_num + 1 > pars_per_block[tessWords[i].block_num])
				pars_per_block[tessWords[i].block_num] = tessWords[i].par_num + 1;

		for (int i = 0; i < num_blocks; i++)
			num_pars += pars_per_block[i];

		std::vector<int> lines_per_par(num_pars, 0);// = (int*)calloc(num_pars, sizeof(int));
		for (int i = 0; i < numWords; i++)
		{
			int skip = 0;
			for (int j = 0; j < tessWords[i].block_num; j++)
				skip += pars_per_block[j];
			skip += tessWords[i].par_num;

			if (tessWords[i].line_num + 1 > lines_per_par[skip])
			{
				lines_per_par[skip] = tessWords[i].line_num + 1;
			}
		}

		for (int i = 0; i < num_pars; i++)
			num_lines += lines_per_par[i];

		AllocIntArrayAndFill(block_left, num_blocks, INT_MAX);
		AllocIntArrayAndFill(par_left, num_pars, INT_MAX);
		AllocIntArrayAndFill(line_left, num_lines, INT_MAX);

		AllocIntArrayAndFill(block_top, num_blocks, INT_MAX);
		AllocIntArrayAndFill(par_top, num_pars, INT_MAX);
		AllocIntArrayAndFill(line_top, num_lines, INT_MAX);

		AllocIntArrayAndFill(block_right, num_blocks, INT_MIN);
		AllocIntArrayAndFill(par_right, num_pars, INT_MIN);
		AllocIntArrayAndFill(line_right, num_lines, INT_MIN);

		AllocIntArrayAndFill(block_bottom, num_blocks, INT_MIN);
		AllocIntArrayAndFill(par_bottom, num_pars, INT_MIN);
		AllocIntArrayAndFill(line_bottom, num_lines, INT_MIN);

		int _par_num, _line_num, _right, _bottom;

		for (int i = 0; i < numWords; i++)
		{
			if (!strcmp(tessWords[i].text, ""))
				continue;
			_par_num = _line_num = 0;

			for (int j = 0; j < tessWords[i].block_num; j++)
				_par_num += pars_per_block[j];
			_par_num += tessWords[i].par_num;

			for (int j = 0; j < _par_num; j++)
				_line_num += lines_per_par[j];
			_line_num += tessWords[i].line_num;

			CmpAndAssignIfLess(line_left[_line_num], tessWords[i].left);
			CmpAndAssignIfLess(block_left[tessWords[i].block_num], tessWords[i].left);
			CmpAndAssignIfLess(par_left[_par_num], tessWords[i].left);

			CmpAndAssignIfLess(line_top[_line_num], tessWords[i].top);
			CmpAndAssignIfLess(block_top[tessWords[i].block_num], tessWords[i].top);
			CmpAndAssignIfLess(par_top[_par_num], tessWords[i].top);

			_right = tessWords[i].left + tessWords[i].width;
			_bottom = tessWords[i].top + tessWords[i].height;

			CmpAndAssignIfGreater(line_right[_line_num], _right);
			CmpAndAssignIfGreater(block_right[tessWords[i].block_num], _right);
			CmpAndAssignIfGreater(par_right[_par_num], _right);
			CmpAndAssignIfGreater(line_bottom[_line_num], _bottom);
			CmpAndAssignIfGreater(block_bottom[tessWords[i].block_num], _bottom);
			CmpAndAssignIfGreater(par_bottom[_par_num], _bottom);
		}
		Display(inputImageFilename);
	}
};



int main(int argc, char *argv[])
{
	char *inputDir = argv[1];
	char num[8];
	LayoutAnalyzer layoutAnalyzer;
	//layoutAnalyzer.RunOnDirectory("F:\\GMRV\\DocumentSkewDeteector\\aiaps-fw\\SAAPI_FW\\skew_images\\DITHERED");
	layoutAnalyzer.RunOnDirectory(".\\InputTest");
	cv::waitKey(0);
    return 0;
}
