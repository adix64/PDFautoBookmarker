/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H_H
#define UI_MAINWINDOW_H_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
	QAction* exitAction;
	QAction* insertRowAction;
	QAction* removeRowAction;
	QAction* insertColumnAction;
	QAction* removeColumnAction;
	QAction* insertChildAction;
	QAction* actionOpen_Folder_with_scanned_Pages;
	QWidget* centralwidget;
	QHBoxLayout* horizontalLayout;
	QVBoxLayout* verticalLayout_10;
	QPushButton* pushButton;
	QGroupBox* outlinesGrpBox;
	QVBoxLayout* verticalLayout_3;
	QTreeWidget* treeWidget;
	QVBoxLayout* verticalLayout_2;
	QGroupBox* charrsGroupBox;
	QVBoxLayout* verticalLayout_4;
	QLabel* maxNonAZdensityLabel;
	QDoubleSpinBox* maxNonAZdensity;
	QFrame* line;
	QFrame* line_2;
	QLabel* label_4;
	QDoubleSpinBox* max09densitySBox;
	QFrame* line_3;
	QFrame* line_4;
	QLabel* label_2;
	QSpinBox* minCharCount;
	QGroupBox* groupBox_4;
	QVBoxLayout* verticalLayout_6;
	QCheckBox* autoNestingChkBox;
	QFrame* line_5;
	QCheckBox* numberedHeadingsChkBox;
	QCheckBox* latinNumeralsChkBox;
	QCheckBox* abcListingsChkBox;
	QGroupBox* groupBox_3;
	QVBoxLayout* verticalLayout_5;
	QCheckBox* deskewChkBox;
	QCheckBox* removeImgsChkBox;
	QVBoxLayout* verticalLayout;
	QLabel* label_5;
	QGraphicsView* graphicsView;
	QHBoxLayout* horizontalLayout_2;
	QPushButton* prevPageBtn;
	QLabel* PageLabel;
	QPushButton* nextPageBtn;
	QPushButton* savePDFbutton;
	QMenuBar* menubar;
	QMenu* fileMenu;
	QMenu* actionsMenu;
	QStatusBar* statusbar;

	void setupUi(QMainWindow* MainWindow)
	{
		if (MainWindow->objectName().isEmpty())
			MainWindow->setObjectName(QStringLiteral("MainWindow"));
		MainWindow->resize(765, 662);
		MainWindow->setStyleSheet(QStringLiteral("*{color:rgb(192,192,192); background-color:rgb(64,64,64)}"));
		MainWindow->setToolButtonStyle(Qt::ToolButtonTextOnly);
		exitAction = new QAction(MainWindow);
		exitAction->setObjectName(QStringLiteral("exitAction"));
		insertRowAction = new QAction(MainWindow);
		insertRowAction->setObjectName(QStringLiteral("insertRowAction"));
		removeRowAction = new QAction(MainWindow);
		removeRowAction->setObjectName(QStringLiteral("removeRowAction"));
		insertColumnAction = new QAction(MainWindow);
		insertColumnAction->setObjectName(QStringLiteral("insertColumnAction"));
		removeColumnAction = new QAction(MainWindow);
		removeColumnAction->setObjectName(QStringLiteral("removeColumnAction"));
		insertChildAction = new QAction(MainWindow);
		insertChildAction->setObjectName(QStringLiteral("insertChildAction"));
		actionOpen_Folder_with_scanned_Pages = new QAction(MainWindow);
		actionOpen_Folder_with_scanned_Pages->setObjectName(QStringLiteral("actionOpen_Folder_with_scanned_Pages"));
		centralwidget = new QWidget(MainWindow);
		centralwidget->setObjectName(QStringLiteral("centralwidget"));
		horizontalLayout = new QHBoxLayout(centralwidget);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		verticalLayout_10 = new QVBoxLayout();
		verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
		pushButton = new QPushButton(centralwidget);
		pushButton->setObjectName(QStringLiteral("pushButton"));

		verticalLayout_10->addWidget(pushButton);

		outlinesGrpBox = new QGroupBox(centralwidget);
		outlinesGrpBox->setObjectName(QStringLiteral("outlinesGrpBox"));
		verticalLayout_3 = new QVBoxLayout(outlinesGrpBox);
		verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
		treeWidget = new QTreeWidget(outlinesGrpBox);
		QTreeWidgetItem* __qtreewidgetitem = new QTreeWidgetItem();
		__qtreewidgetitem->setText(0, QStringLiteral("1"));
		treeWidget->setHeaderItem(__qtreewidgetitem);
		treeWidget->setObjectName(QStringLiteral("treeWidget"));

		verticalLayout_3->addWidget(treeWidget);


		verticalLayout_10->addWidget(outlinesGrpBox);


		horizontalLayout->addLayout(verticalLayout_10);

		verticalLayout_2 = new QVBoxLayout();
		verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
		charrsGroupBox = new QGroupBox(centralwidget);
		charrsGroupBox->setObjectName(QStringLiteral("charrsGroupBox"));
		verticalLayout_4 = new QVBoxLayout(charrsGroupBox);
		verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
		maxNonAZdensityLabel = new QLabel(charrsGroupBox);
		maxNonAZdensityLabel->setObjectName(QStringLiteral("maxNonAZdensityLabel"));
		maxNonAZdensityLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
		maxNonAZdensityLabel->setWordWrap(true);

		verticalLayout_4->addWidget(maxNonAZdensityLabel);

		maxNonAZdensity = new QDoubleSpinBox(charrsGroupBox);
		maxNonAZdensity->setObjectName(QStringLiteral("maxNonAZdensity"));

		verticalLayout_4->addWidget(maxNonAZdensity);

		line = new QFrame(charrsGroupBox);
		line->setObjectName(QStringLiteral("line"));
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);

		verticalLayout_4->addWidget(line);

		line_2 = new QFrame(charrsGroupBox);
		line_2->setObjectName(QStringLiteral("line_2"));
		line_2->setFrameShape(QFrame::HLine);
		line_2->setFrameShadow(QFrame::Sunken);

		verticalLayout_4->addWidget(line_2);

		label_4 = new QLabel(charrsGroupBox);
		label_4->setObjectName(QStringLiteral("label_4"));
		label_4->setAlignment(Qt::AlignCenter);

		verticalLayout_4->addWidget(label_4);

		max09densitySBox = new QDoubleSpinBox(charrsGroupBox);
		max09densitySBox->setObjectName(QStringLiteral("max09densitySBox"));

		verticalLayout_4->addWidget(max09densitySBox);

		line_3 = new QFrame(charrsGroupBox);
		line_3->setObjectName(QStringLiteral("line_3"));
		line_3->setFrameShape(QFrame::HLine);
		line_3->setFrameShadow(QFrame::Sunken);

		verticalLayout_4->addWidget(line_3);

		line_4 = new QFrame(charrsGroupBox);
		line_4->setObjectName(QStringLiteral("line_4"));
		line_4->setFrameShape(QFrame::HLine);
		line_4->setFrameShadow(QFrame::Sunken);

		verticalLayout_4->addWidget(line_4);

		label_2 = new QLabel(charrsGroupBox);
		label_2->setObjectName(QStringLiteral("label_2"));
		label_2->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

		verticalLayout_4->addWidget(label_2);

		minCharCount = new QSpinBox(charrsGroupBox);
		minCharCount->setObjectName(QStringLiteral("minCharCount"));

		verticalLayout_4->addWidget(minCharCount);


		verticalLayout_2->addWidget(charrsGroupBox);

		groupBox_4 = new QGroupBox(centralwidget);
		groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
		verticalLayout_6 = new QVBoxLayout(groupBox_4);
		verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
		autoNestingChkBox = new QCheckBox(groupBox_4);
		autoNestingChkBox->setObjectName(QStringLiteral("autoNestingChkBox"));

		verticalLayout_6->addWidget(autoNestingChkBox);

		line_5 = new QFrame(groupBox_4);
		line_5->setObjectName(QStringLiteral("line_5"));
		line_5->setFrameShape(QFrame::HLine);
		line_5->setFrameShadow(QFrame::Sunken);

		verticalLayout_6->addWidget(line_5);

		numberedHeadingsChkBox = new QCheckBox(groupBox_4);
		numberedHeadingsChkBox->setObjectName(QStringLiteral("numberedHeadingsChkBox"));

		verticalLayout_6->addWidget(numberedHeadingsChkBox);

		latinNumeralsChkBox = new QCheckBox(groupBox_4);
		latinNumeralsChkBox->setObjectName(QStringLiteral("latinNumeralsChkBox"));

		verticalLayout_6->addWidget(latinNumeralsChkBox);

		abcListingsChkBox = new QCheckBox(groupBox_4);
		abcListingsChkBox->setObjectName(QStringLiteral("abcListingsChkBox"));

		verticalLayout_6->addWidget(abcListingsChkBox);


		verticalLayout_2->addWidget(groupBox_4);

		groupBox_3 = new QGroupBox(centralwidget);
		groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
		verticalLayout_5 = new QVBoxLayout(groupBox_3);
		verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
		deskewChkBox = new QCheckBox(groupBox_3);
		deskewChkBox->setObjectName(QStringLiteral("deskewChkBox"));

		verticalLayout_5->addWidget(deskewChkBox);

		removeImgsChkBox = new QCheckBox(groupBox_3);
		removeImgsChkBox->setObjectName(QStringLiteral("removeImgsChkBox"));

		verticalLayout_5->addWidget(removeImgsChkBox);


		verticalLayout_2->addWidget(groupBox_3);


		horizontalLayout->addLayout(verticalLayout_2);

		verticalLayout = new QVBoxLayout();
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		label_5 = new QLabel(centralwidget);
		label_5->setObjectName(QStringLiteral("label_5"));
		label_5->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

		verticalLayout->addWidget(label_5);

		graphicsView = new QGraphicsView(centralwidget);
		graphicsView->setObjectName(QStringLiteral("graphicsView"));

		verticalLayout->addWidget(graphicsView);

		horizontalLayout_2 = new QHBoxLayout();
		horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
		prevPageBtn = new QPushButton(centralwidget);
		prevPageBtn->setObjectName(QStringLiteral("prevPageBtn"));

		horizontalLayout_2->addWidget(prevPageBtn);

		PageLabel = new QLabel(centralwidget);
		PageLabel->setObjectName(QStringLiteral("PageLabel"));

		horizontalLayout_2->addWidget(PageLabel);

		nextPageBtn = new QPushButton(centralwidget);
		nextPageBtn->setObjectName(QStringLiteral("nextPageBtn"));

		horizontalLayout_2->addWidget(nextPageBtn);


		verticalLayout->addLayout(horizontalLayout_2);

		savePDFbutton = new QPushButton(centralwidget);
		savePDFbutton->setObjectName(QStringLiteral("savePDFbutton"));
		savePDFbutton->setStyleSheet(QStringLiteral(""));

		verticalLayout->addWidget(savePDFbutton);


		horizontalLayout->addLayout(verticalLayout);

		MainWindow->setCentralWidget(centralwidget);
		menubar = new QMenuBar(MainWindow);
		menubar->setObjectName(QStringLiteral("menubar"));
		menubar->setGeometry(QRect(0, 0, 765, 26));
		fileMenu = new QMenu(menubar);
		fileMenu->setObjectName(QStringLiteral("fileMenu"));
		actionsMenu = new QMenu(menubar);
		actionsMenu->setObjectName(QStringLiteral("actionsMenu"));
		MainWindow->setMenuBar(menubar);
		statusbar = new QStatusBar(MainWindow);
		statusbar->setObjectName(QStringLiteral("statusbar"));
		MainWindow->setStatusBar(statusbar);

		menubar->addAction(fileMenu->menuAction());
		menubar->addAction(actionsMenu->menuAction());
		fileMenu->addAction(exitAction);
		fileMenu->addAction(actionOpen_Folder_with_scanned_Pages);
		actionsMenu->addAction(insertRowAction);
		actionsMenu->addAction(insertColumnAction);
		actionsMenu->addSeparator();
		actionsMenu->addAction(removeRowAction);
		actionsMenu->addAction(removeColumnAction);
		actionsMenu->addSeparator();
		actionsMenu->addAction(insertChildAction);

		retranslateUi(MainWindow);

		QMetaObject::connectSlotsByName(MainWindow);
	} // setupUi

	void retranslateUi(QMainWindow* MainWindow)
	{
		MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Editable Tree Model", Q_NULLPTR));
		exitAction->setText(QApplication::translate("MainWindow", "E&xit", Q_NULLPTR));
		exitAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
		insertRowAction->setText(QApplication::translate("MainWindow", "Insert Row", Q_NULLPTR));
		insertRowAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+I, R", Q_NULLPTR));
		removeRowAction->setText(QApplication::translate("MainWindow", "Remove Row", Q_NULLPTR));
		removeRowAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+R, R", Q_NULLPTR));
		insertColumnAction->setText(QApplication::translate("MainWindow", "Insert Column", Q_NULLPTR));
		insertColumnAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+I, C", Q_NULLPTR));
		removeColumnAction->setText(QApplication::translate("MainWindow", "Remove Column", Q_NULLPTR));
		removeColumnAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+R, C", Q_NULLPTR));
		insertChildAction->setText(QApplication::translate("MainWindow", "Insert Child", Q_NULLPTR));
		insertChildAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", Q_NULLPTR));
		actionOpen_Folder_with_scanned_Pages->setText(QApplication::translate("MainWindow", "Open Folder with scanned Pages", Q_NULLPTR));
		pushButton->setText(QApplication::translate("MainWindow", "Browse Folder for Scanned Documents", Q_NULLPTR));
		outlinesGrpBox->setTitle(QApplication::translate("MainWindow", "Outlines", Q_NULLPTR));
		charrsGroupBox->setTitle(QApplication::translate("MainWindow", "Characters", Q_NULLPTR));
		maxNonAZdensityLabel->setText(QApplication::translate("MainWindow", "max density\n"
			"\302\254[a..zA..Z]", Q_NULLPTR));
		label_4->setText(QApplication::translate("MainWindow", "max density\n"
			"[0..9]", Q_NULLPTR));
		label_2->setText(QApplication::translate("MainWindow", "min outline\n"
			"char count", Q_NULLPTR));
		groupBox_4->setTitle(QApplication::translate("MainWindow", "Hierarchy", Q_NULLPTR));
		autoNestingChkBox->setText(QApplication::translate("MainWindow", "Automatic\n"
			"nesting", Q_NULLPTR));
		numberedHeadingsChkBox->setText(QApplication::translate("MainWindow", "Numbered\n"
			"Headings", Q_NULLPTR));
		latinNumeralsChkBox->setText(QApplication::translate("MainWindow", "Latin\n"
			"numerals", Q_NULLPTR));
		abcListingsChkBox->setText(QApplication::translate("MainWindow", "a. b. c.\n"
			"listings", Q_NULLPTR));
		groupBox_3->setTitle(QApplication::translate("MainWindow", "Preprocess", Q_NULLPTR));
		deskewChkBox->setText(QApplication::translate("MainWindow", "DeSkew", Q_NULLPTR));
		removeImgsChkBox->setText(QApplication::translate("MainWindow", "RmvImgs", Q_NULLPTR));
		label_5->setText(QApplication::translate("MainWindow", "DOCUMENT View", Q_NULLPTR));
		prevPageBtn->setText(QApplication::translate("MainWindow", "\342\227\204", Q_NULLPTR));
		PageLabel->setText(QApplication::translate("MainWindow", "Page", Q_NULLPTR));
		nextPageBtn->setText(QApplication::translate("MainWindow", "\342\226\272", Q_NULLPTR));
		savePDFbutton->setText(QApplication::translate("MainWindow", "Save PDF", Q_NULLPTR));
		fileMenu->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
		actionsMenu->setTitle(QApplication::translate("MainWindow", "&Actions", Q_NULLPTR));
	} // retranslateUi

};

namespace Ui {
	class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H_H
