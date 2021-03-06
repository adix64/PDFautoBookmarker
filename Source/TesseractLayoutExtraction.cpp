#define _CRT_SECURE_NO_WARNINGS
#define DBG 1
#include "TesseractLayoutExtraction.h"
#include <iostream>
#include "dirent.h"
#include "VSDocA.h"
#include "VSLibA.h"
#include "VSOutlineA.h"
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qaction.h>
#include <QtWidgets/qfiledialog.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qobject.h>
#include <QtWidgets/qlabel.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

float ComputeNonAZdensity(std::string& s)
{
	int numbersCount = 0, dummy;
	auto dt = s.data();
	for (int i = 0, sz = s.size(); i < sz; i++)
		if (!((97 <= s[i] && s[i] <= 122) || (65 <= s[i] && s[i] <= 90) || s[i] == 32))
			numbersCount++;

	return (float)numbersCount / (float)s.size();
}

float ComputeDigitDensity(std::string& s)
{
	int numbersCount = 0, dummy;
	auto dt = s.data();
	for (int i = 0, sz = s.size(); i < sz; i++)
		if (48 <= s[i] && s[i] <= 57)
			numbersCount++;

	return (float)numbersCount / (float)s.size();
}

class LayoutAnalyzer
{
private:
	//document input data structure

	std::vector<TSVpage> tsvPages;
	int numPages = 0;
	
	char inputFileListPath[2048];
	
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
	void RunOnFileList(const char *directory, const char *listFile, LayoutAnalysisResult &result)
	{
//Run Tesseract...
		char command[2048];
		memset(command, 0, 2048);
		sprintf(command, TESSERACT_CMD, directory, TESSERACT_PATH, TESSDATA_PATH,  directory, listFile, directory, listFile);
		std::string helperS(command);
		char crtDriveLetter = qApp->applicationDirPath().toUtf8().data()[0];
		char changeDriveLetter[] = "?: && ";
		changeDriveLetter[0] = directory[0];
		
		helperS = changeDriveLetter + helperS;

		system(helperS.c_str());
		//char Filename[MAX_PATH]; //this is a char buffer
		//GetModuleFileNameA(GetModuleHandle(0), Filename, sizeof(Filename));
		//printf("\n%s\nRunning Tesseract-OCR in %s\n<<<\n", Filename, directory);
		//

		FILE* fp;
		char buffer[2048];
		memset(buffer, 0, 2048);

		fp = fopen((std::string(directory) + "\\" + listFile).c_str(), "r");
		if (fp == NULL)
		{
			std::cout << "file does not exist\n";
			return;
		};
		while (fgets(buffer, 2048, (FILE*)fp))
		{
			buffer[strlen(buffer) - 1] = '\0';
			inputFilenames.push_back(buffer);
			outputFilepaths.push_back(std::string(directory) + "\\" + buffer + ".tsv");
			result.fileNames.push_back(std::string(directory) + "\\" + buffer);
			memset(buffer, 0, 2048);
		}
		fclose(fp);
//...<
		Run(std::string(directory), (std::string(directory) + "/" + listFile + ".tsv").c_str(), result);
	}

	void DisplayResultImgs(std::string directory, char *filename, int pageIndex)
	{
		cv::Mat image = cv::imread(directory + "/" + filename, cv::IMREAD_COLOR);
		cv::Mat image2 = image.clone(), image3 = image.clone();
		TSVpage &page = tsvPages[pageIndex];
		int rows = image.rows, cols = image.cols, _height = (rows * 400) / cols;;
		printf("\n max_line_num %d num_blocks %d\n num_pars \n", page.num_lines, page.num_blocks, page.num_pars);

		DrawRectanglesOnDocument(image, page.num_blocks, page.blocksBounds);
		DrawRectanglesOnDocument(image2, page.num_lines, page.linesBounds);
		//DrawRectanglesOnDocument(image3, num_pars, paragraphsBounds);
		DrawRectanglesOnDocument(image3, page.winnerTitles);

		return;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		OCVdispImgInWin(image, "BLOCKS", filename, 0, 100, 400, _height);
		OCVdispImgInWin(image3, "PARAGRAPHS", filename, 400, 100, 400, _height);
		OCVdispImgInWin(image2, "LINES", filename, 800, 100, 400, _height);
	}

	void Run(std::string directory, const char *inputTSVpath, LayoutAnalysisResult &result)
	{
		FILE* fp;
		char buffer[2048];
		char wordBuf[2048];

		fp = fopen(inputTSVpath, "r");

		fscanf(fp, TSV_PREAMBLE);
		int dummies[10], crtPage, prevPage = -1;
		std::vector<int> entriesPerPage;
		while (fgets(buffer, 2048, (FILE*)fp))
		{
			memset(wordBuf, 0, 2048);
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
		while (fgets(buffer, 2048, (FILE*)fp))
		{
			memset(wordBuf, 0, 2048);
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
				int numParagraphsWithText = 0, numLinesInParagraph, maxNumWordsInLine = 0, parIdx;
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
						tsvPage.winnerBlockIdxs.push_back(b);
					}
				}
			}
			//DisplayResultImgs(directory, (char*)inputFilenames[pageIdx].c_str(), pageIdx);
		}

		char *inPdf = _strdup(inputTSVpath);
		int inFileNameSz = strlen(inPdf);
		sprintf(inPdf + inFileNameSz - 3, "pdf");

		char *outPdf = (char*)malloc((inFileNameSz+3) * sizeof(char));
		strcpy(outPdf, inPdf);
		sprintf(outPdf + inFileNameSz - 4, ".~OUT.pdf");

		TCHAR *wname= (TCHAR*)calloc(inFileNameSz + 1, sizeof(TCHAR));
		for (int i = 0; i < inFileNameSz; i++)
			wname[i] = inPdf[i];
		LPTSTR FileName = const_cast<LPTSTR>(wname);

		/////////////////////////////////////////////////////////////////////////////////
		PDFLibHandle libHandle = InitPDFLibrary((char*)"someLic", (char*)"someKey");
		auto readPDFfile = PDFDocLoadFromFile(libHandle, (char*)FileName);
			
		auto outlineRoot = PDFDocGetOutlineRoot(readPDFfile);

		for (int i = 0; i < numPages; i++)
		{
			TSVpage &tsvPage = tsvPages[i];
			for (int b = 0; b < tsvPage.winnerBlockIdxs.size(); b++)
			{
				std::string bookmark;
				TSVblock &wBlock = tsvPage.blocks[tsvPage.winnerBlockIdxs[b]];
				for (int p = 0; p < wBlock.paragraphs.size(); p++)
				{
					TSVparagraph &tPar = wBlock.paragraphs[p];
					for (int l = 0, nLines = tPar.lines.size(); l < nLines; l++)
					{
						std::vector<TSVentry *> &words = tPar.lines[l].words;;
						for (int w = 0, nWords = words.size(); w < nWords; w++)
						{
							if (words[w]->confidence < 0)
								continue;
							bookmark += words[w]->text;
							bookmark += " ";
						}
					}
				}
				if (bookmark.size() < 3)
					continue;

				//std::cout << "detected HEADING: " << bookmark << "\n";
				result.outlines.push_back(bookmark);
				result.pageNumbers.push_back(i);
				result.charCount.push_back(bookmark.size());
				result.nonAZdensity.push_back(ComputeNonAZdensity(bookmark));
				result.numbersDensity.push_back(ComputeDigitDensity(bookmark));
			}
		}
		result.tsvFilename = std::string(inputTSVpath);

		DonePDFLibrary(&libHandle);
	}

	/////////////////////////////////////////////////////////////////////////////////

	void WritePDFtoFile(LayoutAnalysisResult &result, QTreeWidget*& treeWidget)
	{
		char* inPdf = _strdup(result.tsvFilename.c_str());
		int inFileNameSz = strlen(inPdf);
		sprintf(inPdf + inFileNameSz - 3, "pdf");

		char* outPdf = (char*)malloc((inFileNameSz + 3) * sizeof(char));
		strcpy(outPdf, inPdf);
		sprintf(outPdf + inFileNameSz - 4, ".~OUT.pdf");

		QString selFilter = "PDF files (*.PDF)";

		QString filePath = QFileDialog::getSaveFileName(treeWidget->parentWidget(), "Save Outlined PDF As",
			QDir::currentPath(), "PDF files (*.pdf)");// &selFilter);
		
		if (filePath.size() <= 0)
			return;
		
		TCHAR* wname = (TCHAR*)calloc(inFileNameSz + 1, sizeof(TCHAR));
		for (int i = 0; i < inFileNameSz; i++)
			wname[i] = inPdf[i];
		LPTSTR FileName = const_cast<LPTSTR>(wname);

		PDFLibHandle libHandle = InitPDFLibrary((char*)"someLic", (char*)"someKey");
		auto readPDFfile = PDFDocLoadFromFile(libHandle, (char*)FileName);

		auto outlineRoot = PDFDocGetOutlineRoot(readPDFfile);

		SaveHelper(treeWidget, outlineRoot, readPDFfile);



		PDFDocSaveToFile(readPDFfile, filePath.toUtf8().data());
		DonePDFLibrary(&libHandle);
		
		QDir d = QFileInfo(filePath).absoluteDir();
		QString absolute = d.absolutePath();
		
		//system((QString("explorer \"") + absolute + "\" &").toUtf8().data());
	}

	void SaveHelper(QTreeWidget*& treeWidget, PDFOutlineHandle &rootHandle, PDFDocHandle &docHandle)
	{
		int numberOfTopLevelItems = treeWidget->topLevelItemCount();
		for (int topLevelindex = 0; topLevelindex < numberOfTopLevelItems; topLevelindex++)
		{
			QTreeWidgetItem* item = treeWidget->topLevelItem(topLevelindex);
			auto newChild = PDFOutlineAddNewChild(rootHandle);

			std::string s = item->text(0).toUtf8().constData();
			PDFOutlineSetTitle(newChild, (char*)(s.c_str()), s.size());
			PDFExplicitDest dest;
			dest.Type = edtFitV;
			const char *pstr = item->text(1).toUtf8().constData();
			sscanf(pstr, "Page %d", &(dest.Page));
			dest.Page--;
			dest.a = dest.b = dest.c = dest.d = 0;
			auto realDest = PDFDestinationFromExplit(docHandle, dest, false, false);
			auto action = PDFActionNewGoToDestination(docHandle, realDest);
			PDFOutlineSetAction(newChild, action);
			processItemM(item, 0, newChild, docHandle);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////

	void processItemM(QTreeWidgetItem* parent, int level, PDFOutlineHandle &outlineHandle, PDFDocHandle &docHandle)
	{
		for (int childIndex = 0; childIndex < parent->childCount(); childIndex++)
		{
			QTreeWidgetItem* child = parent->child(childIndex);
	
			auto newChild = PDFOutlineAddNewChild(outlineHandle);
			std::string s = child->text(0).toUtf8().constData();
			PDFOutlineSetTitle(newChild, (char*)(s.c_str()), s.size());
			PDFExplicitDest dest;
			dest.Type = edtFitV;
			const char* pstr = child->text(1).toUtf8().constData();
			sscanf(pstr, "Page %d", &(dest.Page));
			dest.Page--;
			dest.a = dest.b = dest.c = dest.d = 0;
			auto realDest = PDFDestinationFromExplit(docHandle, dest, false, false);
			auto action = PDFActionNewGoToDestination(docHandle, realDest);
			PDFOutlineSetAction(newChild, action);
			processItemM(child, level + 1, newChild, docHandle);
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////

LayoutAnalysisResult TSVtoMarkedPDFbackend::openWithDialog(QWidget *mWidget)
{
	LayoutAnalysisResult result;
	QString files = QFileDialog::getExistingDirectory(mWidget, "Open File for Deskew");
	if (files.size() <= 0)
	{
		std::cout << "NO FILE SELECTED!\n";
		return result;
	}
	result.valid = true;
	std::string __filename = files.toUtf8().constData();

	LayoutAnalyzer layoutAnalyzer;
	layoutAnalyzer.RunOnFileList(__filename.c_str(), "test.txt", result);

	return result;
}

void TSVtoMarkedPDFbackend::SaveBookmarkedPDFtoFile(LayoutAnalysisResult &result, QTreeWidget *&treeWidget)
{
	LayoutAnalyzer layoutAnalyzer;
	layoutAnalyzer.WritePDFtoFile(result, treeWidget);
}