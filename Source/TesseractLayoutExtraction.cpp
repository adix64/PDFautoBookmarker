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



////////////////////////////////////////////////////////////////////////////
///////////////// Tesseract Interface Data Structures///////////////////////
////////////////////////////////////////////////////////////////////////////
struct TSVentry
{
	int level;
	int page_num, block_num, par_num, line_num, word_num;

	int confidence;
	int bounds[RectBound::Count];
	int width, height;
	char *text;
};

struct TSVline
{
	std::vector<TSVentry*> words;
	int bounds[RectBound::Count];
};

struct TSVparagraph
{
	std::vector<TSVline> lines;
	int bounds[RectBound::Count];
};

struct TSVblock
{
	std::vector<TSVparagraph> paragraphs;
	int bounds[RectBound::Count];
};

struct TSVdocument
{
	TSVentry *tsvEntries;
	int numTSVentries = 0;
	int num_blocks = -1;
	int num_lines = 0;
	int num_pars = 0;
};
//<<<<<<<<<<<<<<<<<<<<<<////////////////////////////////////////////////////

class LayoutAnalyzer
{
private:
	//document input data structure
	std::vector<TSVentry> tsvEntries;
	std::vector<TSVblock> tsvBlocks;

	std::vector<int> winnerTitles;

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
			free(tsvEntries[i].text);
		
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
		//DrawRectanglesOnDocument(image3, num_pars, paragraphsBounds);
		DrawRectanglesOnDocument(image3, winnerTitles);


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


		tsvEntries = std::vector<TSVentry>(numTSVentries);

		fseek(fp, 0, SEEK_SET);
		fscanf(fp, TSV_PREAMBLE);
		char wordBuf[512];

		int i = 0;
		while (fgets(buffer, 512, (FILE*)fp))
		{
			memset(wordBuf, 0, 512);
			sscanf(buffer, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
				&tsvEntries[i].level, &tsvEntries[i].page_num, &tsvEntries[i].block_num,
				&tsvEntries[i].par_num, &tsvEntries[i].line_num, &tsvEntries[i].word_num,
				&tsvEntries[i].bounds[RectBound::Left], &tsvEntries[i].bounds[RectBound::Top],
				&tsvEntries[i].width, &tsvEntries[i].height, &tsvEntries[i].confidence, wordBuf);
			tsvEntries[i].text = (char*)malloc(strlen(wordBuf) + 1);
			strcpy(tsvEntries[i].text, wordBuf);

			if (tsvEntries[i].block_num + 1 > num_blocks)
				num_blocks = tsvEntries[i].block_num + 1;

			tsvEntries[i].bounds[RectBound::Right] = tsvEntries[i].bounds[RectBound::Left] + tsvEntries[i].width;
			tsvEntries[i].bounds[RectBound::Bottom] = tsvEntries[i].bounds[RectBound::Top] + tsvEntries[i].height;

			i++;
		}
		fclose(fp);

		std::vector<int> pars_per_block(num_blocks, 0);// = (int*)calloc(num_blocks, sizeof(int));
		for (int i = 0; i < numTSVentries; i++)
			if (tsvEntries[i].par_num + 1 > pars_per_block[tsvEntries[i].block_num])
				pars_per_block[tsvEntries[i].block_num] = tsvEntries[i].par_num + 1;

		tsvBlocks.resize(num_blocks);
		for (int i = 0; i < num_blocks; i++)
		{
			tsvBlocks[i].paragraphs.resize(pars_per_block[i]);
			num_pars += pars_per_block[i];
		}

		std::vector<int> lines_per_par(num_pars, 0);// = (int*)calloc(num_pars, sizeof(int));
		for (int i = 0; i < numTSVentries; i++)
		{

			int skipParagraphsCount = 0;
			for (int j = 0; j < tsvEntries[i].block_num; j++)
				skipParagraphsCount += pars_per_block[j];
			skipParagraphsCount += tsvEntries[i].par_num;

			if (tsvEntries[i].line_num + 1 > lines_per_par[skipParagraphsCount])
			{
				lines_per_par[skipParagraphsCount] = tsvEntries[i].line_num + 1;
				tsvBlocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].lines.resize(lines_per_par[skipParagraphsCount]);
			}
		}


		for (int i = 0; i < num_pars; i++)
			num_lines += lines_per_par[i];

		InitRectBoundsArray(blocksBounds, num_blocks * RectBound::Count);
		InitRectBoundsArray(linesBounds, num_lines * RectBound::Count);
		InitRectBoundsArray(paragraphsBounds, num_pars * RectBound::Count);


		int g_par_num, g_line_num;
		for (int i = 0; i < numTSVentries; i++)
		{
			if (tsvEntries[i].confidence < 0)//!strcmp(tsvEntries[i].text, "")) //it's a container
				continue;
			g_par_num = g_line_num = 0;

			for (int j = 0; j < tsvEntries[i].block_num; j++)
				g_par_num += pars_per_block[j];
			g_par_num += tsvEntries[i].par_num;

			for (int j = 0; j < g_par_num; j++)
				g_line_num += lines_per_par[j];
			g_line_num += tsvEntries[i].line_num;

			for (int bi = RectBound::Left; bi < RectBound::Right; bi++){
				CmpAndAssignIfLess(blocksBounds[tsvEntries[i].block_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
				CmpAndAssignIfLess(paragraphsBounds[g_par_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
				CmpAndAssignIfLess(linesBounds[g_line_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
			}
			for (int bi = RectBound::Right; bi < RectBound::Count; bi++) {
				CmpAndAssignIfGreater(blocksBounds[tsvEntries[i].block_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
				CmpAndAssignIfGreater(paragraphsBounds[g_par_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
				CmpAndAssignIfGreater(linesBounds[g_line_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
			}

			tsvBlocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].lines[tsvEntries[i].line_num].words.push_back(&tsvEntries[i]);
			for (int bi = 0; bi < RectBound::Count; bi++) {
				tsvBlocks[tsvEntries[i].block_num].bounds[bi] = blocksBounds[tsvEntries[i].block_num * RectBound::Count + bi];
				tsvBlocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].lines[tsvEntries[i].line_num].bounds[bi] = linesBounds[g_line_num * RectBound::Count + bi];
				tsvBlocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].bounds[bi] = paragraphsBounds[g_par_num * RectBound::Count + bi];
			}
		}

		for (int i = 0; i < num_blocks;i++)
		{
			int lft = tsvBlocks[i].bounds[RectBound::Left], rgt = tsvBlocks[i].bounds[RectBound::Right];
			int btm = tsvBlocks[i].bounds[RectBound::Bottom], tp = tsvBlocks[i].bounds[RectBound::Top];

			int blockArea = (rgt - lft) * (btm - tp);
			for (int j = 0, nPars = tsvBlocks[i].paragraphs.size(); j < nPars; j++)
			{
				int paragraphArea = (tsvBlocks[i].paragraphs[j].bounds[RectBound::Right] - tsvBlocks[i].paragraphs[j].bounds[RectBound::Left]) *
					(tsvBlocks[i].paragraphs[j].bounds[RectBound::Bottom] - tsvBlocks[i].paragraphs[j].bounds[RectBound::Top]);

				if (blockArea == paragraphArea && tsvBlocks[i].paragraphs[j].lines.size() < 4)
				{
					winnerTitles.insert(winnerTitles.end(), { lft, tp, rgt, btm });
				}
			}
		}

		Display(inputImageFilename);
	}
};



int main(int argc, char *argv[])
{
//	LPTSTR FileName = (LPTSTR)TEXT("F:\\GMRV\\CompGeometry&DocAnalysis\\AEACD-work\\TesseractLayoutExtraction\\TesseractLayoutExtraction\\Test1.pdf");
//	LPTSTR FileName2 = (LPTSTR)TEXT("F:\\GMRV\\CompGeometry&DocAnalysis\\AEACD-work\\TesseractLayoutExtraction\\TesseractLayoutExtraction\\Test2.pdf");
//
//	/////////////////////////////////////////////////////////////////////////////////
//	/////////////////////////////////////////////////////////////////////////////////
//	PDFLibHandle libHandle = InitPDFLibrary((char*)"whatever", (char*)"theFuckGoes");
//	auto readPDFfile = PDFDocLoadFromFile(libHandle, (char*)FileName);
//
//	auto outlineRoot = PDFDocGetOutlineRoot(readPDFfile);
//	auto newChild = PDFOutlineAddNewChild(outlineRoot);
//	PDFOutlineSetTitle(newChild, (char*)"un_bucmarc", strlen("un_bucmarc"));
//	PDFDocSaveToFile(readPDFfile, (char*)FileName2);
//	/////////////////////////////////////////////////////////////////////////////////
//	/////////////////////////////////////////////////////////////////////////////////
//
//
	char *inputDir = argv[1];
	char num[8];
	LayoutAnalyzer layoutAnalyzer;
	//layoutAnalyzer.RunOnDirectory("F:\\GMRV\\DocumentSkewDeteector\\aiaps-fw\\SAAPI_FW\\skew_images\\DITHERED");
	layoutAnalyzer.RunOnDirectory(".\\InputTest");
	cv::waitKey(0);
    return 0;
}
