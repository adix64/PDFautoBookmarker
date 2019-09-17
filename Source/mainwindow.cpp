#include "mainwindow.h"
#include "TesseractLayoutExtraction.h"
#include <QtCore/qfile.h>
#include <QtCore/qsignalmapper.h>
#include <QtGui/qpixmap.h>
#include <QtWidgets/QGraphicsPixmapItem>
#include "zoomer.h"
#include <iostream>

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
	connect(insertColumnAction, &QAction::triggered, this, &MainWindow::insertColumn);
	connect(removeRowAction, &QAction::triggered, this, &MainWindow::removeRow);
	connect(removeColumnAction, &QAction::triggered, this, &MainWindow::removeColumn);
	connect(insertChildAction, &QAction::triggered, this, &MainWindow::insertChild);
//	//connect(actionOpen_Folder_with_scanned_Pages, &QAction::triggered, this, [this] { TSVtoMarkedPDFbackend::openWithDialog(this); });
	connect(actionOpen_Folder_with_scanned_Pages, &QAction::triggered, this, &MainWindow::LoadFolder);
	connect(prevPageBtn, &QPushButton::released, this, &MainWindow::PrevPage);
	connect(nextPageBtn, &QPushButton::released, this, &MainWindow::NextPage);
	connect(savePDFbutton, &QPushButton::released, this, &MainWindow::SaveBookmarked);
	connect(pushButton, &QPushButton::released, this, &MainWindow::LoadFolder);
	//connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onTreeWidgetItemDoubleClicked);

	updateActions();
	
	treeWidget->setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
	treeWidget->setDragEnabled(true);

	treeWidget->setColumnCount(2);
	treeWidget->setColumnWidth(0, 700);

	QList<QString> sl = { QString("Heading Text"), QString("Page") };
	treeWidget->setHeaderLabels(QStringList(sl));
	treeWidget->header()->setStyleSheet(QString("QHeaderView::section { color: white; background: rgb(32,32,32); }"));
	QString btnStyle = "QPushButton:pressed { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 186, 162, 255), stop:1 rgba(98, 211, 162, 255)) } QPushButton { color: white; background-color: rgb(32,32,32); border: 1px solid white; border-radius: 8px; padding: 6;} QPushButton:disabled { background-color: rgb(170, 170, 127) }";
	pushButton->setStyleSheet(btnStyle);
	savePDFbutton->setStyleSheet(btnStyle);
	nextPageBtn->setStyleSheet(btnStyle);
	prevPageBtn->setStyleSheet(btnStyle);
	this->showMaximized();
}

void MainWindow::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	//if (isEditable(column)) {
		treeWidget->editItem(item, column);
	//}
}
/////////////////////////////////////////////////////////////////////////////////

void MainWindow::PrevPage()
{
	if (currentPageIndex > 0)
	{
		mScene->removeItem(currentItem);
		currentPageIndex--;
		currentItem = pageItems[currentPageIndex];
		mScene->addItem(currentItem);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::NextPage()
{
	if (currentPageIndex < pages.size()-1)
	{
		mScene->removeItem(currentItem);
		currentPageIndex++;
		currentItem = pageItems[currentPageIndex];
		mScene->addItem(currentItem);

	}
}

/////////////////////////////////////////////////////////////////////////////////

void MainWindow::SaveBookmarked()
{
	
	//for(int i = 0; i < )
	TSVtoMarkedPDFbackend::SaveBookmarkedPDFtoFile(result, treeWidget);
}



/////////////////////////////////////////////////////////////////////////////////

void MainWindow::LoadFolder()
{
	result = TSVtoMarkedPDFbackend::openWithDialog(this);
	if (!result.valid)
		return;
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
	
	QList<QTreeWidgetItem*> items;
	for (int i = 0; i < result.outlines.size(); i++)
	{
		auto newItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(result.outlines[i].first.c_str())));
		newItem->setText(1, QString("Page %1").arg(result.outlines[i].second));
		newItem->setFlags(newItem->flags() | Qt::ItemFlag::ItemIsEditable);
		items.append(newItem);

	}
	treeWidget->insertTopLevelItems(0, items);

	/*for (int i = 0; i < result.outlines.size(); i++)
	{
		insertRow();
		
	}
	{
		{
			QModelIndex index = model->index(i, 0);
			model->setData(index, QVariant(tr(result.outlines[i].first.c_str())), Qt::EditRole);
		}
		{
			QModelIndex index = model->index(i, 1);
			char pageNumTxt[64];
			memset(pageNumTxt, 0, 64);
			sprintf(pageNumTxt, "Page %d", result.outlines[i].second);
			model->setData(index, QVariant(tr(pageNumTxt)), Qt::EditRole);
		}
	}*/
	Graphics_view_zoom* z = new Graphics_view_zoom(graphicsView);
	z->set_modifiers(Qt::NoModifier);
	//QGraphicsView* view = new QGraphicsView(scene);
	mScene->addItem(pageItems[0]);
	//((QScrollBar*)graphicsView->scrollBarWidgets[0])->
	graphicsView->show();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::insertChild()
{
	//const QModelIndex index = view->selectionModel()->currentIndex();
	//QAbstractItemModel* model = view->model();

	//if (model->columnCount(index) == 0) {
	//	if (!model->insertColumn(0, index))
	//		return;
	//}

	//if (!model->insertRow(0, index))
	//	return;

	//for (int column = 0; column < model->columnCount(index); ++column) {
	//	const QModelIndex child = model->index(0, column, index);
	//	model->setData(child, QVariant(tr("[No data]")), Qt::EditRole);
	//	if (!model->headerData(column, Qt::Horizontal).isValid())
	//		model->setHeaderData(column, Qt::Horizontal, QVariant(tr("[No header]")), Qt::EditRole);
	//}

	//view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
	//	QItemSelectionModel::ClearAndSelect);
	//updateActions();
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool MainWindow::insertColumn()
{
	return true;
	//QAbstractItemModel* model = view->model();
	//int column = view->selectionModel()->currentIndex().column();

	//// Insert a column in the parent item.
	//bool changed = model->insertColumn(column + 1);
	//if (changed)
	//	model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);

	//updateActions();

	//return changed;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::insertRow()
{
	//const QModelIndex index = view->selectionModel()->currentIndex();
	//QAbstractItemModel* model = view->model();

	//if (!model->insertRow(index.row() + 1, index.parent()))
	//	return;

	//updateActions();

	//for (int column = 0; column < model->columnCount(index.parent()); ++column) {
	//	const QModelIndex child = model->index(index.row() + 1, column, index.parent());
	//	model->setData(child, QVariant(tr("[No data]")), Qt::EditRole);
	//}
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool MainWindow::removeColumn()
{
	return true;
	//QAbstractItemModel* model = view->model();
	//const int column = view->selectionModel()->currentIndex().column();

	//// Insert columns in each child of the parent item.
	//const bool changed = model->removeColumn(column);
	//if (changed)
	//	updateActions();

	//return changed;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::removeRow()
{
	//const QModelIndex index = view->selectionModel()->currentIndex();
	//QAbstractItemModel* model = view->model();
	//if (model->removeRow(index.row(), index.parent()))
	//	updateActions();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateActions()
{
	
	//const bool hasSelection = !view->selectionModel()->selection().isEmpty();
	//removeRowAction->setEnabled(hasSelection);
	//removeColumnAction->setEnabled(hasSelection);

	//const bool hasCurrent = view->selectionModel()->currentIndex().isValid();
	//insertRowAction->setEnabled(hasCurrent);
	//insertColumnAction->setEnabled(hasCurrent);

	//if (hasCurrent) {
	//	view->closePersistentEditor(view->selectionModel()->currentIndex());

	//	const int row = view->selectionModel()->currentIndex().row();
	//	const int column = view->selectionModel()->currentIndex().column();
	//	if (view->selectionModel()->currentIndex().parent().isValid())
	//		statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
	//	else
	//		statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
	//}
}