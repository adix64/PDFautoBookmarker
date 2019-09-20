#include "mainwindow.h"
#include "TesseractLayoutExtraction.h"
#include <QtCore/qfile.h>
#include <QtCore/qsignalmapper.h>
#include <QtGui/qpixmap.h>
#include <QtWidgets/QGraphicsPixmapItem>
#include "zoomer.h"
#include <iostream>
#include <thread>
#include <QtCore/qthread.h>

/////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	setupUi(this);

	mScene = new QGraphicsScene();
	graphicsView->setScene(mScene);


	connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

	connect(actionsMenu, &QMenu::aboutToShow, this, &MainWindow::updateActions);
	connect(insertRowAction, &QAction::triggered, this, &MainWindow::insertRow);
	//connect(insertColumnAction, &QAction::triggered, this, &MainWindow::insertColumn);
	connect(removeRowAction, &QAction::triggered, this, &MainWindow::removeRow);
	//connect(removeColumnAction, &QAction::triggered, this, &MainWindow::removeColumn);
	connect(insertChildAction, &QAction::triggered, this, &MainWindow::insertChild);
//	//connect(actionOpen_Folder_with_scanned_Pages, &QAction::triggered, this, [this] { TSVtoMarkedPDFbackend::openWithDialog(this); });
	connect(actionOpen_Folder_with_scanned_Pages, &QAction::triggered, this, &MainWindow::LoadFolder);
	connect(prevPageBtn, &QPushButton::released, this, &MainWindow::PrevPage);
	connect(nextPageBtn, &QPushButton::released, this, &MainWindow::NextPage);
	connect(savePDFbutton, &QPushButton::released, this, &MainWindow::SaveBookmarked);
	connect(openImageFolderPushBtn, &QPushButton::released, this, &MainWindow::LoadFolder);
	connect(revertPushBtn, &QPushButton::released, this, &MainWindow::RevertAllChanges);
	connect(outlineCharConstrainPushBtn, &QPushButton::released, this, &MainWindow::RevertAllChanges);
	connect(treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::onTreeWidgetItemClicked);

	updateActions();
	
	treeWidget->setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
	treeWidget->setDragEnabled(true);

	treeWidget->setColumnCount(2);
	treeWidget->setColumnWidth(0, 700);

	QList<QString> sl = { QString("Heading Text"), QString("Page") };
	treeWidget->setHeaderLabels(QStringList(sl));
	this->showMaximized();
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::onTreeWidgetItemClicked(QTreeWidgetItem* item, int column)
{
	const char* pstr = item->text(1).toUtf8().constData();
	int pageNum;
	sscanf(pstr, "Page %d", &pageNum);
	SetCurrentPage(pageNum - 1);
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::RevertAllChanges()
{
	ResetTree(treeWidget);
	QList<QTreeWidgetItem*> items;
	for (int i = 0; i < result.outlines.size(); i++)
	{
		if (result.nonAZdensity[i] > (float)maxNonAZdensity->value() ||
			result.numbersDensity[i] > (float)max09densitySBox->value()
			|| result.outlines[i].size() > spinBox->value()
			|| result.outlines[i].size() < minCharCount->value())
			continue;

		auto newItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(result.outlines[i].c_str())));
		newItem->setText(1, QString("Page %1").arg(result.pageNumbers[i] + 1));
		newItem->setFlags(newItem->flags() | Qt::ItemFlag::ItemIsEditable);
		items.append(newItem);

	}
	treeWidget->insertTopLevelItems(0, items);
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::ResetTree(QTreeWidget*& treeWidget)
{	
	int numberOfTopLevelItems = treeWidget->topLevelItemCount();
	while (numberOfTopLevelItems > 0)
	{
		for (int topLevelindex = 0; topLevelindex < numberOfTopLevelItems; topLevelindex++)
		{
			QTreeWidgetItem* item = treeWidget->topLevelItem(topLevelindex);
			//		
			_deleteRecursiveTreeItem(item, 0);
		}
		numberOfTopLevelItems = treeWidget->topLevelItemCount();
	}
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::_deleteRecursiveTreeItem(QTreeWidgetItem* item, int level)
{
	if (!item)
		return;
	for (int childIndex = 0; childIndex < item->childCount(); childIndex++)
	{
		QTreeWidgetItem* child = item->child(childIndex);
		_deleteRecursiveTreeItem(child, level + 1);
	}
	auto pparent = item->parent();
	if (pparent != NULL)
		pparent->removeChild(item);
	delete item;
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::PrevPage()
{
	if (pages.size() <= 0)
		return;
	if (currentPageIndex > 0)
	{
		currentPageIndex--;
		SetCurrentPage(currentPageIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::SetCurrentPage(int idx)
{
	mScene->removeItem(currentItem);
	currentItem = pageItems[idx];
	mScene->addItem(currentItem);
	char pgName[64];
	memset(pgName, 0, 64);
	sprintf(pgName, "Page: %d / %d", idx + 1, pages.size());
	pageLabel->setText(QString(pgName));
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::NextPage()
{
	if (pages.size() <= 0)
		return;
	if (currentPageIndex < pages.size()-1)
	{
		currentPageIndex++;
		SetCurrentPage(currentPageIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::SaveBookmarked()
{
	TSVtoMarkedPDFbackend::SaveBookmarkedPDFtoFile(result, treeWidget);
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::LoadFolder()
{
	openImageFolderPushBtn->setText("Processing Folder, Please Wait ...");
	auto newresult = TSVtoMarkedPDFbackend::openWithDialog(this);
	openImageFolderPushBtn->setEnabled(false);

	if (!newresult.valid)
	{
		openImageFolderPushBtn->setText("Open Folder with Scanned Doc. Images ...");
		openImageFolderPushBtn->setEnabled(true);
		return;
	}
	result = newresult;
	for (int i = 0; i < pages.size(); i++)
	{
		if (pages[i])
			delete pages[i];
		if (pageItems[i])
			delete pageItems[i];
	}
	if (currentItem != NULL)
	{
		mScene->removeItem(currentItem);
		currentItem = NULL;
	}

	pages.clear();
	pageItems.clear();
	for (int i = 0; i < result.fileNames.size(); i++)
	{
		pages.push_back(new QImage());
		pages[i]->load(result.fileNames[i].c_str());
		pageItems.push_back(new QGraphicsPixmapItem(QPixmap::fromImage(*pages[i])));
	}

	//////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//treeWidget->setEditTriggers(QFlags<QAbstractItemView::EditTrigger>(QAbstractItemView::EditTrigger::NoEditTriggers));
	//RevertAllChanges();
	QList<QTreeWidgetItem*> items;
	for (int i = 0; i < result.outlines.size(); i++)
	{
		if (result.nonAZdensity[i] > (float)maxNonAZdensity->value() ||
			result.numbersDensity[i] > (float)max09densitySBox->value()
			|| result.outlines[i].size() > spinBox->value() 
			|| result.outlines[i].size() < minCharCount->value() )
			continue;
		auto newItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(result.outlines[i].c_str())));
		newItem->setText(1, QString("Page %1").arg(result.pageNumbers[i] + 1));
		newItem->setFlags(newItem->flags() | Qt::ItemFlag::ItemIsEditable);
		items.append(newItem);

	}
	treeWidget->insertTopLevelItems(0, items);
	Graphics_view_zoom* z = new Graphics_view_zoom(graphicsView);
	z->set_modifiers(Qt::NoModifier);
	mScene->addItem(pageItems[0]);
	graphicsView->show();
	openImageFolderPushBtn->setText("Open Folder with Scanned Doc. Images ...");
	openImageFolderPushBtn->setEnabled(true);
	char pgName[64];
	memset(pgName, 0, 64);
	sprintf(pgName, "Page: 1 / %d", pages.size());
	pageLabel->setText(QString(pgName));
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::insertChild()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::insertRow()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::removeRow()
{
	auto selItem = treeWidget->selectedItems();
	if (selItem.size() <= 0)
		return;
	if (selItem[0] != NULL)
	{
		_deleteRecursiveTreeItem(selItem[0], 0);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateActions()
{
}