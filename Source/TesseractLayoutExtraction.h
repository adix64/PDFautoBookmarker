#pragma once
#include <iostream>
#include <QtWidgets/QWidget>
#include "Utils.hpp"

#include <QtWidgets/qtreewidget.h>
#define TSV_PREAMBLE "level" "\t" "page_num" "\t" "block_num" "\t" \
					 "par_num" "\t" "line_num" "\t" "word_num" "\t" \
					 "left" "\t" "top" "\t" "width" "\t" "height" "\t" \
					 "conf" "\t" "text\n"

#define TESSERACT_PATH "C:\\\"Program Files (x86)\"\\Tesseract-OCR"
#define TESSDATA_PATH "C:/Program Files (x86)/Tesseract-OCR/tessdata"
#define TESSERACT_CMD "cd \"%s\" && (dir /b *png *jpg *bmp *tif *tiff *jpeg > test.txt) && %s\\tesseract.exe --tessdata-dir \"%s\"  \"%s\\%s\" \"%s\\%s\" tsv pdf"

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
	char* text;
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
	std::vector<int> winnerBlockIdxs;

	//bounding boxes for page elements
	int* blocksBounds, * linesBounds, * paragraphsBounds;
	std::vector<int> pars_per_block, lines_per_par;
	int width, height;

	int numTSVentries = 0;
	int num_blocks = -1;
	int num_lines = 0;
	int num_pars = 0;

	int crtEntryIdx = 0;
};
//<<<<<<<<<<<<<<<<<<<<<<////////////////////////////////////////////////////
struct LayoutAnalysisResult
{
	bool valid = false;
	std::string tsvFilename;
	std::vector<std::string> fileNames;
	std::vector<std::string> outlines;
	std::vector<int> pageNumbers;
	std::vector<float> nonAZdensity;
	std::vector<float> numbersDensity;
	std::vector<int> charCount;
};
class TSVtoMarkedPDFbackend
{
public:
	static LayoutAnalysisResult openWithDialog(QWidget *w);
	static void SaveBookmarkedPDFtoFile(LayoutAnalysisResult& result, QTreeWidget*& treeWidget);
};

float ComputeNonAZdensity(std::string& s);

float ComputeDigitDensity(std::string& s);
