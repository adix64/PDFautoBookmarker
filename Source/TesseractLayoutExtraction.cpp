#define _CRT_SECURE_NO_WARNINGS
#define DBG 1
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "dirent.h"
#include "Utils.hpp"
#include "VSDocA.h"
#include "VSLibA.h"
#include "VSOutlineA.h"
//#include <Windows.h>
//#include <atlbase.h>
#define TSV_PREAMBLE "level" "\t" "page_num" "\t" "block_num" "\t" \
					 "par_num" "\t" "line_num" "\t" "word_num" "\t" \
					 "left" "\t" "top" "\t" "width" "\t" "height" "\t" \
					 "conf" "\t" "text\n"

#define TESSERACT_PATH "C:\\\"Program Files (x86)\"\\Tesseract-OCR"
#define TESSDATA_PATH "C:/Program Files (x86)/Tesseract-OCR/tessdata"
using namespace cv;

enum RectBound {Left, Top, Right, Bottom, Count};

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


struct TSVentry
{
	int level;
	int page_num, block_num, par_num, line_num, word_num;

	int confidence;
	int bounds[RectBound::Count];
	int width, height;
	char *text;
};

struct TSVdocument
{
	TSVentry *tessWords;
	int numTSVentries = 0;
	int num_blocks = -1;
	int num_lines = 0;
	int num_pars = 0;
};
class LayoutAnalyzer
{
private:
	//document input data structure
	TSVentry *tessWords;
	int numTSVentries = 0;
	int num_blocks = -1;
	int num_lines = 0;
	int num_pars = 0;
	
	//bounding boxes for page elements
	int *blocksBounds, *linesBounds, * paragraphsBounds;
	
	std::vector<std::string> inputFilenames, outputFilepaths;

private:
	// Run Tesseract on a given file and output its TSV into another file
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
	//CONSTRUCTOR
	LayoutAnalyzer() {}
	//DESTRUCTOR
	~LayoutAnalyzer()
	{
		free(blocksBounds);
		free(linesBounds);
		free(paragraphsBounds);

		for (int i = 0; i < numTSVentries; i++)
			free(tessWords[i].text);
		free(tessWords);
	}

	//CallTesseractOCR on every file in inputDir
	void RunTesseractInDir(const char *inputDir)
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(inputDir)) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") || !strcmp(ent->d_name, "ignore"))
					continue;
				inputFilenames.push_back(ent->d_name);
			}
			closedir(dir);
		}
		for (int i = 0; i < inputFilenames.size(); i++)
			CallTesseractOCR(inputDir, inputFilenames[i].c_str());
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


		DrawRectanglesOnDocument(image, num_blocks, blocksBounds);
		DrawRectanglesOnDocument(image2, num_lines, linesBounds);
		DrawRectanglesOnDocument(image3, num_pars, paragraphsBounds);


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
		numTSVentries = 0;
		num_blocks = -1;
		num_lines = 0;
		num_pars = 0;

		FILE* fp;
		char buffer[512];

		fp = fopen(inputTSVfilename, "r");

		fscanf(fp, TSV_PREAMBLE);
		while (fgets(buffer, 512, (FILE*)fp))
			numTSVentries++;


		tessWords = (TSVentry*)malloc(numTSVentries * sizeof(TSVentry));

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
				&tessWords[i].bounds[RectBound::Left], &tessWords[i].bounds[RectBound::Top],
				&tessWords[i].width, &tessWords[i].height,
				&tessWords[i].confidence, wordBuf);
			tessWords[i].text = (char*)malloc(strlen(wordBuf) + 1);
			strcpy(tessWords[i].text, wordBuf);

			if (tessWords[i].block_num + 1 > num_blocks)
				num_blocks = tessWords[i].block_num + 1;

			tessWords[i].bounds[RectBound::Right] = tessWords[i].bounds[RectBound::Left] + tessWords[i].width;
			tessWords[i].bounds[RectBound::Bottom] = tessWords[i].bounds[RectBound::Top] + tessWords[i].height;

			i++;
		}
		fclose(fp);

		std::vector<int> pars_per_block(num_blocks, 0);// = (int*)calloc(num_blocks, sizeof(int));
		for (int i = 0; i < numTSVentries; i++)
			if (tessWords[i].par_num + 1 > pars_per_block[tessWords[i].block_num])
				pars_per_block[tessWords[i].block_num] = tessWords[i].par_num + 1;

		for (int i = 0; i < num_blocks; i++)
			num_pars += pars_per_block[i];

		std::vector<int> lines_per_par(num_pars, 0);// = (int*)calloc(num_pars, sizeof(int));
		for (int i = 0; i < numTSVentries; i++)
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

		InitRectBoundsArray(blocksBounds, num_blocks * RectBound::Count);
		InitRectBoundsArray(linesBounds, num_lines * RectBound::Count);
		InitRectBoundsArray(paragraphsBounds, num_pars * RectBound::Count);


		int _par_num, _line_num, _right, _bottom;
		for (int i = 0; i < numTSVentries; i++)
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


			for (int bi = RectBound::Left; bi < RectBound::Right; bi++)
			{
				CmpAndAssignIfLess(blocksBounds[tessWords[i].block_num * RectBound::Count + bi], tessWords[i].bounds[bi]);
				CmpAndAssignIfLess(paragraphsBounds[_par_num * RectBound::Count + bi], tessWords[i].bounds[bi]);
				CmpAndAssignIfLess(linesBounds[_line_num * RectBound::Count + bi], tessWords[i].bounds[bi]);
			}

			for (int bi = RectBound::Right; bi < RectBound::Count; bi++)
			{
				CmpAndAssignIfGreater(blocksBounds[tessWords[i].block_num * RectBound::Count + bi], tessWords[i].bounds[bi]);
				CmpAndAssignIfGreater(paragraphsBounds[_par_num * RectBound::Count + bi], tessWords[i].bounds[bi]);
				CmpAndAssignIfGreater(linesBounds[_line_num * RectBound::Count + bi], tessWords[i].bounds[bi]);
			}

		}
		Display(inputImageFilename);
	}
};



int main(int argc, char *argv[])
{
	LPTSTR FileName = (LPTSTR)TEXT("F:\\GMRV\\CompGeometry&DocAnalysis\\AEACD-work\\TesseractLayoutExtraction\\TesseractLayoutExtraction\\Test1.pdf");
	LPTSTR FileName2 = (LPTSTR)TEXT("F:\\GMRV\\CompGeometry&DocAnalysis\\AEACD-work\\TesseractLayoutExtraction\\TesseractLayoutExtraction\\Test2.pdf");

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	PDFLibHandle libHandle = InitPDFLibrary((char*)"whatever", (char*)"theFuckGoes");
	auto readPDFfile = PDFDocLoadFromFile(libHandle, (char*)FileName);

	auto outlineRoot = PDFDocGetOutlineRoot(readPDFfile);
	auto newChild = PDFOutlineAddNewChild(outlineRoot);
	PDFOutlineSetTitle(newChild, (char*)"un_bucmarc", strlen("un_bucmarc"));
	PDFDocSaveToFile(readPDFfile, (char*)FileName2);
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	char *inputDir = argv[1];
	char num[8];
	LayoutAnalyzer layoutAnalyzer;
	//layoutAnalyzer.RunOnDirectory("F:\\GMRV\\DocumentSkewDeteector\\aiaps-fw\\SAAPI_FW\\skew_images\\DITHERED");
	layoutAnalyzer.RunOnDirectory(".\\InputTest");
	cv::waitKey(0);
    return 0;
}
