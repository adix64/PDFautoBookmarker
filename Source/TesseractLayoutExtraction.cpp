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
#define TESSERACT_CMD "%s\\tesseract.exe --tessdata-dir \"%s\" -c textonly_pdf=1 %s\\%s %s\\%s tsv"
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

struct TSVpage
{
	std::vector<TSVblock> blocks;
	std::vector<TSVentry> tsvEntries;
	std::vector<int> winnerTitles;

	//bounding boxes for page elements
	int *blocksBounds, *linesBounds, *paragraphsBounds;
	std::vector<int> pars_per_block, lines_per_par;
	int width, height;

	int numTSVentries = 0;
	int num_blocks = -1;
	int num_lines = 0;
	int num_pars = 0;

	int crtEntryIdx = 0;
};
//<<<<<<<<<<<<<<<<<<<<<<////////////////////////////////////////////////////

class LayoutAnalyzer
{
private:
	//document input data structure

	std::vector<TSVpage> tsvPages;
	int numPages = 0;
	
	char inputFileListPath[512];
	
	std::vector<std::string> inputFilenames, outputFilepaths;

public:

	//CONSTRUCTOR
	LayoutAnalyzer() {}
	//DESTRUCTOR
	~LayoutAnalyzer()
	{
		for (int i = 0; i < numPages; i++) {
			free(tsvPages[i].blocksBounds);
			free(tsvPages[i].linesBounds);
			free(tsvPages[i].paragraphsBounds);
			for (int j = 0; j < tsvPages[i].numTSVentries; j++)
				free(tsvPages[i].tsvEntries[j].text);
		}
		
	}



	//CallTesseractOCR on every file in inputDir
	void RunOnFileList(const char *directory, const char *listFile)
	{
		FILE* fp;
		char buffer[512];
		memset(buffer, 0, 512);
		
		fp = fopen((std::string(directory) + "\\" +  listFile).c_str(), "r");

		while (fgets(buffer, 512, (FILE*)fp))
		{
			buffer[strlen(buffer) - 1] = '\0';
			inputFilenames.push_back(buffer);
			outputFilepaths.push_back(std::string(directory) + "\\" + buffer + ".tsv");
			memset(buffer, 0, 512);
		}
		fclose(fp);
//Run Tesseract...
		char command[512];
		memset(command, 0, 512);
		sprintf(command, TESSERACT_CMD, TESSERACT_PATH, TESSDATA_PATH, directory, listFile, directory, listFile);
		printf("\n>>>\n%s\n<<<\n", command);
		system(command);
//...<
		Run((std::string(directory) + "\\" + listFile + ".tsv").c_str());
	}

	void DisplayResultImgs(const char *filename, int pageIndex)
	{
		cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		cv::Mat image2 = image.clone(), image3 = image.clone();
		TSVpage &page = tsvPages[pageIndex];
		int rows = image.rows, cols = image.cols, _height = (rows * 400) / cols;;
		printf("\n max_line_num %d num_blocks %d\n num_pars \n", page.num_lines, page.num_blocks, page.num_pars);

		DrawRectanglesOnDocument(image, page.num_blocks, page.blocksBounds);
		DrawRectanglesOnDocument(image2, page.num_lines, page.linesBounds);
		//DrawRectanglesOnDocument(image3, num_pars, paragraphsBounds);
		DrawRectanglesOnDocument(image3, page.winnerTitles);

		OCVdispImgInWin(image, "BLOCKS", filename, 0, 100, 400, _height);
		OCVdispImgInWin(image3, "PARAGRAPHS", filename, 400, 100, 400, _height);
		OCVdispImgInWin(image2, "LINES", filename, 800, 100, 400, _height);
	}

	void Run(const char *inputTSVpath)
	{
		FILE* fp;
		char buffer[512];
		char wordBuf[512];

		fp = fopen(inputTSVpath, "r");

		fscanf(fp, TSV_PREAMBLE);
		int dummies[10], crtPage, prevPage = -1;
		std::vector<int> entriesPerPage;
		while (fgets(buffer, 512, (FILE*)fp))
		{
			memset(wordBuf, 0, 512);
			sscanf(buffer, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
				&dummies[0], &crtPage, &dummies[1], &dummies[2], &dummies[3], &dummies[4],
				&dummies[5], &dummies[6], &dummies[7], &dummies[8], &dummies[9], wordBuf);
			if (crtPage != prevPage)
			{
				numPages++;
				entriesPerPage.push_back(0);
			}
			else
				entriesPerPage[crtPage - 1]++;

			prevPage = crtPage;
		}

		tsvPages = std::vector<TSVpage>(numPages);
		for (int pageIdx = 0; pageIdx < numPages; pageIdx++)
		{
			tsvPages[pageIdx].numTSVentries = entriesPerPage[pageIdx];
			tsvPages[pageIdx].tsvEntries.resize(tsvPages[pageIdx].numTSVentries);
		}

		fseek(fp, 0, SEEK_SET);
		fscanf(fp, TSV_PREAMBLE);

		int tLevel, tBlock_num, tPar_num, tLine_num, tWord_num, tLeft, tTop, tWidth, tHeight, tConfidence;
		prevPage = -1;
		while (fgets(buffer, 512, (FILE*)fp))
		{
			memset(wordBuf, 0, 512);
			sscanf(buffer, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
				&tLevel, &crtPage, &tBlock_num, &tPar_num, &tLine_num, &tWord_num,
				&tLeft, &tTop, &tWidth, &tHeight, &tConfidence, wordBuf);

			if (crtPage == prevPage)
			{
				TSVpage &tsvPage = tsvPages[crtPage - 1];
				TSVentry &tsvEntry = tsvPage.tsvEntries[tsvPage.crtEntryIdx];
				char *&textPtr = tsvEntry.text;
				textPtr = (char*)malloc(strlen(wordBuf) + 1);
				strcpy(textPtr, wordBuf);
				tsvEntry.level = tLevel; tsvEntry.page_num = crtPage; tsvEntry.block_num = tBlock_num;
				tsvEntry.par_num = tPar_num; tsvEntry.line_num = tLine_num; tsvEntry.word_num = tWord_num;
				tsvEntry.confidence = tConfidence;

				tsvEntry.bounds[RectBound::Left] = tLeft;
				tsvEntry.bounds[RectBound::Top] = tTop;
				tsvEntry.width = tWidth;	tsvEntry.height = tHeight;

				if (tsvEntry.block_num + 1 > tsvPage.num_blocks)
					tsvPage.num_blocks = tsvEntry.block_num + 1;

				tsvEntry.bounds[RectBound::Right] = tsvEntry.bounds[RectBound::Left] + tsvEntry.width;
				tsvEntry.bounds[RectBound::Bottom] = tsvEntry.bounds[RectBound::Top] + tsvEntry.height;

				tsvPage.crtEntryIdx++;
			}
			prevPage = crtPage;
		}
		fclose(fp);

		for (int pageIdx = 0; pageIdx < numPages; pageIdx++)
		{
			TSVpage &tsvPage = tsvPages[pageIdx];
			std::vector<TSVentry> &tsvEntries = tsvPage.tsvEntries;
			tsvPage.pars_per_block.resize(tsvPage.num_blocks, 0);// = (int*)calloc(num_blocks, sizeof(int));
			for (int i = 0; i < tsvPage.numTSVentries; i++)
				if (tsvEntries[i].par_num + 1 > tsvPage.pars_per_block[tsvEntries[i].block_num])
					tsvPage.pars_per_block[tsvEntries[i].block_num] = tsvEntries[i].par_num + 1;

			tsvPage.blocks.resize(tsvPage.num_blocks);
			for (int i = 0; i < tsvPage.num_blocks; i++)
			{
				tsvPage.blocks[i].paragraphs.resize(tsvPage.pars_per_block[i]);
				tsvPage.num_pars += tsvPage.pars_per_block[i];
			}

			tsvPage.lines_per_par.resize(tsvPage.num_pars, 0);// = (int*)calloc(num_pars, sizeof(int));
			for (int i = 0; i < tsvPage.numTSVentries; i++)
			{

				int skipParagraphsCount = 0;
				for (int j = 0; j < tsvEntries[i].block_num; j++)
					skipParagraphsCount += tsvPage.pars_per_block[j];
				skipParagraphsCount += tsvEntries[i].par_num;

				if (tsvEntries[i].line_num + 1 > tsvPage.lines_per_par[skipParagraphsCount])
				{
					tsvPage.lines_per_par[skipParagraphsCount] = tsvEntries[i].line_num + 1;
					tsvPage.blocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].lines.resize(tsvPage.lines_per_par[skipParagraphsCount]);
				}
			}


			for (int i = 0; i < tsvPage.num_pars; i++)
				tsvPage.num_lines += tsvPage.lines_per_par[i];

			InitRectBoundsArray(tsvPage.blocksBounds, tsvPage.num_blocks * RectBound::Count);
			InitRectBoundsArray(tsvPage.linesBounds, tsvPage.num_lines * RectBound::Count);
			InitRectBoundsArray(tsvPage.paragraphsBounds, tsvPage.num_pars * RectBound::Count);


			int g_par_num, g_line_num;
			for (int i = 0; i < tsvPage.numTSVentries; i++)
			{
				//if (tsvEntries[i].confidence < 0)//!strcmp(tsvEntries[i].text, "")) //it's a container
				//	continue;
				g_par_num = g_line_num = 0;

				for (int j = 0; j < tsvEntries[i].block_num; j++)
					g_par_num += tsvPage.pars_per_block[j];
				g_par_num += tsvEntries[i].par_num;

				for (int j = 0; j < g_par_num; j++)
					g_line_num += tsvPage.lines_per_par[j];
				g_line_num += tsvEntries[i].line_num;

				//compute AABBs
				for (int bi = RectBound::Left; bi < RectBound::Right; bi++) {
					CmpAndAssignIfLess(tsvPage.blocksBounds[tsvEntries[i].block_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
					CmpAndAssignIfLess(tsvPage.paragraphsBounds[g_par_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
					CmpAndAssignIfLess(tsvPage.linesBounds[g_line_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
				}
				for (int bi = RectBound::Right; bi < RectBound::Count; bi++) {
					CmpAndAssignIfGreater(tsvPage.blocksBounds[tsvEntries[i].block_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
					CmpAndAssignIfGreater(tsvPage.paragraphsBounds[g_par_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
					CmpAndAssignIfGreater(tsvPage.linesBounds[g_line_num * RectBound::Count + bi], tsvEntries[i].bounds[bi]);
				}

				tsvPage.blocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].lines[tsvEntries[i].line_num].words.push_back(&tsvEntries[i]);
				for (int bi = 0; bi < RectBound::Count; bi++) {
					tsvPage.blocks[tsvEntries[i].block_num].bounds[bi] = tsvPage.blocksBounds[tsvEntries[i].block_num * RectBound::Count + bi];
					tsvPage.blocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].lines[tsvEntries[i].line_num].bounds[bi] = tsvPage.linesBounds[g_line_num * RectBound::Count + bi];
					tsvPage.blocks[tsvEntries[i].block_num].paragraphs[tsvEntries[i].par_num].bounds[bi] = tsvPage.paragraphsBounds[g_par_num * RectBound::Count + bi];
				}
			}

			for (int b = 0; b < tsvPage.num_blocks; b++)
			{
				int numParagraphsWithText = 0, numLinesInParagraph, maxNumWordsInLine = 0, winnerParagraph, parIdx;
				bool breakFromParagraph;
				TSVblock &tBlock = tsvPage.blocks[b];
				for (int p = 0, nPars = tBlock.paragraphs.size(); p < nPars; p++)
				{
					TSVparagraph &tPar = tBlock.paragraphs[p];
					breakFromParagraph = false;
					for (int l = 0, nLines = tPar.lines.size(); l < nLines; l++)
					{
						std::vector<TSVentry *> &words = tPar.lines[l].words;;
						for (int w = 0, nWords = words.size(); w < nWords; w++)
						{
							if (words[w]->confidence != -1)
							{
								numParagraphsWithText++;
								breakFromParagraph = true;
								break;
							}
						}
						if (breakFromParagraph)
						{
							numLinesInParagraph = nLines;
							parIdx = p;
							break;
						}
					}

				}



				if (numParagraphsWithText == 1 && numLinesInParagraph < 4) 
				{
					TSVparagraph &tPar = tBlock.paragraphs[parIdx];
					
					for (int l_ = 0, nLines = tPar.lines.size(); l_ < nLines; l_++)
					{
						if (tPar.lines[l_].words.size() > maxNumWordsInLine)
							maxNumWordsInLine = tPar.lines[l_].words.size();
					}
					
					if (maxNumWordsInLine < 16)
					{
						int lft = tsvPage.blocks[b].bounds[RectBound::Left], rgt = tsvPage.blocks[b].bounds[RectBound::Right];
						int btm = tsvPage.blocks[b].bounds[RectBound::Bottom], tp = tsvPage.blocks[b].bounds[RectBound::Top];
						tsvPage.winnerTitles.insert(tsvPage.winnerTitles.end(), { lft, tp, rgt, btm });
					}
				}
			}
			DisplayResultImgs(inputFilenames[pageIdx].c_str(), pageIdx);
		}
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
	layoutAnalyzer.RunOnFileList(".\\paperTest", "test.txt");
	cv::waitKey(0);
    return 0;
}
