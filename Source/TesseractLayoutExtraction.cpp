#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define TSV_PREAMBLE "level" "\t" "page_num" "\t" "block_num" "\t" \
					 "par_num" "\t" "line_num" "\t" "word_num" "\t" \
					 "left" "\t" "top" "\t" "width" "\t" "height" "\t" \
					 "conf" "\t" "text\n"

struct TesseractWord
{
	int level, page_num, block_num, par_num, line_num, word_num;
	int left, top, width, height, conf;
	char *text;
};

int main()
{
	const char *tesseractPath = "C:\\\"Program Files (x86)\"\\Tesseract-OCR";
	const char *inputpath = "F:\\GMRV\\DocumentSkewDeteector\\"
						"aiaps-fw\\SAAPI_FW\\skew_images\\book2\\00119.tif";
	const char *tessDataPath = "C:/Program Files (x86)/Tesseract-OCR/tessdata";
	char command[512];
	memset(command, 0, 512);
	sprintf(command, "%s\\tesseract.exe --tessdata-dir \"%s\" %s ./cevaTSV tsv"
			, tesseractPath, tessDataPath, inputpath);
	printf(command);
	printf("\n\n");
	system(command);
	///////////////////////////////////////////////

	FILE* fp;
	char buffer[512];

	fp = fopen("./cevaTSV.tsv", "r");
	int numWords = 0;

	fscanf(fp, TSV_PREAMBLE);	
	while (fgets(buffer, 512, (FILE*)fp))
		numWords++;

	TesseractWord *tessWords;
	tessWords = (TesseractWord*)malloc(numWords * sizeof(TesseractWord));

	fseek(fp, 0, SEEK_SET);
	fscanf(fp, TSV_PREAMBLE);
	char wordBuf[512];
	int i = 0;

	int num_blocks = -1;
	//int num_pars = -1;
	//int max_line_num = -1;

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

		//if (tessWords[i].par_num > num_pars)
		//	num_pars = tessWords[i].par_num;

		//if (tessWords[i].line_num > max_line_num)
		//	max_line_num = tessWords[i].line_num;
		i++;
	}
	fclose(fp);

	std::vector<int> pars_per_block(num_blocks, 0);// = (int*)calloc(num_blocks, sizeof(int));
	for (int i = 0; i < numWords; i++)
	{
		if (tessWords[i].par_num + 1 > pars_per_block[tessWords[i].block_num])
			pars_per_block[tessWords[i].block_num] = tessWords[i].par_num + 1;
	}

	int num_pars = 0;
	for (int i = 0; i < num_blocks; i++)
	{
		num_pars += pars_per_block[i];
	}

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

	int num_lines = 0;
	for (int i = 0; i < num_pars; i++)
	{
		num_lines += lines_per_par[i];
	}

	int *block_left = (int*)malloc(num_blocks * sizeof(int));
	for (int i = 0; i < num_blocks; i++)
	{
		block_left[i] = INT_MAX;
	}

	int *par_left = (int*)malloc(num_pars * sizeof(int));
	for (int i = 0; i < num_pars; i++)
	{
		par_left[i] = INT_MAX;
	}

	int *line_left = (int*)malloc(num_lines * sizeof(int));
	for (int i = 0; i < num_lines; i++)
	{
		line_left[i] = INT_MAX;
	}

	int *block_top = (int*)malloc(num_blocks * sizeof(int));
	for (int i = 0; i < num_blocks; i++)
	{
		block_top[i] = INT_MAX;
	}

	int *par_top = (int*)malloc(num_pars * sizeof(int));
	for (int i = 0; i < num_pars; i++)
	{
		par_top[i] = INT_MAX;
	}

	int *line_top = (int*)malloc(num_lines * sizeof(int));
	for (int i = 0; i < num_lines; i++)
	{
		line_top[i] = INT_MAX;
	}

	int *block_right = (int*)malloc(num_blocks * sizeof(int));
	for (int i = 0; i < num_blocks; i++)
	{
		block_right[i] = INT_MIN;
	}

	int *par_right = (int*)malloc(num_pars * sizeof(int));
	for (int i = 0; i < num_pars; i++)
	{
		par_right[i] = INT_MIN;
	}

	int *line_right = (int*)malloc(num_lines * sizeof(int));
	for (int i = 0; i < num_lines; i++)
	{
		line_right[i] = INT_MIN;
	}

	int *block_bottom = (int*)malloc(num_blocks * sizeof(int));
	for (int i = 0; i < num_blocks; i++)
	{
		block_bottom[i] = INT_MIN;
	}

	int *par_bottom = (int*)malloc(num_pars * sizeof(int));
	for (int i = 0; i < num_pars; i++)
	{
		par_bottom[i] = INT_MIN;
	}

	int *line_bottom = (int*)malloc(num_lines * sizeof(int));
	for (int i = 0; i < num_lines; i++)
	{
		line_bottom[i] = INT_MIN;
	}

	for (int i = 0; i < numWords; i++)
	{

	}

	for (int i = 0; i < numWords; i++)
	{
		if (!strcmp(tessWords[i].text, ""))
			continue;
		int par_num = 0;
		for (int j = 0; j < tessWords[i].block_num; j++)
			par_num += pars_per_block[j];
		par_num += tessWords[i].par_num;
		int line_num = 0;
		for (int j = 0; j < par_num; j++)
			line_num += lines_per_par[j];
		line_num += tessWords[i].line_num;

		if (tessWords[i].left < line_left[line_num])
			line_left[line_num] = tessWords[i].left;

		if (tessWords[i].left < block_left[tessWords[i].block_num])
			block_left[tessWords[i].block_num] = tessWords[i].left;

		if (tessWords[i].left < par_left[par_num])
			par_left[par_num] = tessWords[i].left;

		if (tessWords[i].top < line_top[line_num])
			line_top[line_num] = tessWords[i].top;

		if (tessWords[i].top < block_top[tessWords[i].block_num])
			block_top[tessWords[i].block_num] = tessWords[i].top;

		if (tessWords[i].top < par_top[par_num])
			par_top[par_num] = tessWords[i].top;



		if (tessWords[i].left + tessWords[i].width > line_right[line_num])
			line_right[line_num] = tessWords[i].left + tessWords[i].width;

		if (tessWords[i].left + tessWords[i].width > block_right[tessWords[i].block_num])
			block_right[tessWords[i].block_num] = tessWords[i].left + tessWords[i].width;

		if (tessWords[i].left + tessWords[i].width > par_right[par_num])
			par_right[par_num] = tessWords[i].left + tessWords[i].width;

		if (tessWords[i].top + tessWords[i].height > line_bottom[line_num])
			line_bottom[line_num] = tessWords[i].top + tessWords[i].height;

		if (tessWords[i].top + tessWords[i].height > block_bottom[tessWords[i].block_num])
			block_bottom[tessWords[i].block_num] = tessWords[i].top + tessWords[i].height;

		if (tessWords[i].top + tessWords[i].height > par_bottom[par_num])
			par_bottom[par_num] = tessWords[i].top + tessWords[i].height;

		//printf("%s %d\n", tessWords[i].text, tessWords[i].text[strlen(tessWords[i].text)]);
	}
	

	//printf("\n\nnumWords %d\n\n", numWords);

	
	cv::Mat image;
	//(512, 384, CV_8SC3, cvScalar(0, 0, 100));
	image = cv::imread(inputpath, CV_LOAD_IMAGE_COLOR);
	cv::Mat image2 = image.clone();
	cv::Mat image3 = image.clone();

	int rows = image.rows;
	int cols = image.cols;
	printf("\nmax_line_num %d\n", num_lines);
	printf("num_blocks %d\n", num_blocks);
	printf("num_pars %d\n", num_pars);
	

	for (int i = 0; i < num_lines; i++)
	{
		int G = rand() % 256;
		int B = rand() % 256;
		int R = rand() % 256;
		if (line_left[i] < 0 || line_right[i] > image.cols ||
			line_top[i] < 0 || line_bottom[i] > image.rows)
			continue;
		for (int x = line_left[i]; x < line_right[i]; x++)
		{
			for (int y = line_top[i]; y < line_bottom[i]; y++)
			{
				image.at<cv::Vec3b>(y,x) = cv::Vec3b(R, G, B) / 2 + image.at<cv::Vec3b>(y, x) / 2;
			}
		}
	}
	
	for (int i = 0; i < num_pars; i++)
	{
		int G = rand() % 256;
		int B = rand() % 256;
		int R = rand() % 256;
		if (par_left[i] < 0 || par_right[i] > image2.cols ||
			par_top[i] < 0 || par_bottom[i] > image2.rows)
			continue;
		for (int x = par_left[i]; x < par_right[i]; x++)
		{
			for (int y = par_top[i]; y < par_bottom[i]; y++)
			{
				image2.at<cv::Vec3b>(y, x) = cv::Vec3b(R, G, B) / 2 + image2.at<cv::Vec3b>(y, x) / 2;
			}
		}
	}

	for (int i = 0; i < num_blocks; i++)
	{
		int G = rand() % 256;
		int B = rand() % 256;
		int R = rand() % 256;
		if (block_left[i] < 0 || block_right[i] > image3.cols ||
			block_top[i] < 0 || block_bottom[i] > image3.rows)
			continue;
		for (int x = block_left[i]; x < block_right[i]; x++)
		{
			for (int y = block_top[i]; y < block_bottom[i]; y++)
			{
				image3.at<cv::Vec3b>(y, x) = cv::Vec3b(R, G, B) / 2 + image3.at<cv::Vec3b>(y, x) / 2;
			}
		}
	}

	cv::namedWindow("Lines", cv::WINDOW_FREERATIO);
	cv::imshow("Lines", image);

	cv::namedWindow("Paragraphs", cv::WINDOW_FREERATIO);
	cv::imshow("Paragraphs", image2);

	cv::namedWindow("Blocks", cv::WINDOW_FREERATIO);
	cv::imshow("Blocks", image3);
	//system("tesseract --tessdata-dir ./ ./ testing / eurotext.png . / testing / eurotext - eng - l eng tsv");
	//getc(stdin);
	cv::waitKey(0);
    return 0;
}