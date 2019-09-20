/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

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
    QAction *exitAction;
    QAction *insertRowAction;
    QAction *removeRowAction;
    QAction *insertColumnAction;
    QAction *removeColumnAction;
    QAction *insertChildAction;
    QAction *actionOpen_Folder_with_scanned_Pages;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_10;
    QPushButton *openImageFolderPushBtn;
    QGroupBox *outlinesGrpBox;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *charsGrpBox;
    QVBoxLayout *verticalLayout_4;
    QLabel *maxNonAZdensityLbl;
    QDoubleSpinBox *maxNonAZdensity;
    QLabel *maxNumDensityLbl;
    QDoubleSpinBox *max09densitySBox;
    QLabel *minCharCntLbl;
    QSpinBox *minCharCount;
    QLabel *label;
    QSpinBox *spinBox;
    QPushButton *outlineCharConstrainPushBtn;
    QGroupBox *hierarchyGrpBox;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *autoNestingChkBox;
    QFrame *line_5;
    QCheckBox *numberedHeadingsChkBox;
    QCheckBox *latinNumeralsChkBox;
    QCheckBox *abcListingsChkBox;
    QPushButton *hierarchyPushBtn;
    QGroupBox *preprocessGrpBox;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *deskewChkBox;
    QCheckBox *removeImgsChkBox;
    QPushButton *preprocessPushBtn;
    QPushButton *revertPushBtn;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *prevPageBtn;
    QLabel *pageLabel;
    QPushButton *nextPageBtn;
    QPushButton *savePDFbutton;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *actionsMenu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 800);
        MainWindow->setMinimumSize(QSize(800, 800));
        MainWindow->setStyleSheet(QLatin1String("*{color:rgb(192,192,192); background-color:rgb(64,64,64)}\n"
"QHeaderView::section { color: white; background: rgb(32,32,32); }\n"
"QCheckBox{background-color: rgb(32,32,32); border-radius: 5px; padding: 3}\n"
"QPushButton:pressed { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 186, 162, 255), stop:1 rgba(98, 211, 162, 255)) } QPushButton { color: white; background-color: rgb(32,32,32); border: 1px solid white; border-radius: 8px; padding: 6;} QPushButton:disabled { background-color: rgb(170, 170, 127) }\n"
"QProgressBar:horizontal {\n"
"	border: 0px solid cyan;\n"
"	color:cyan;\n"
"	background: black;\n"
"	margin: 0px 2px 0 0px;\n"
"	padding:0px\n"
"}\n"
"QProgressBar::chunk:horizontal {\n"
"	background: cyan;\n"
"	/*margin-right: 2px;  space */\n"
"	/*width: 10px;*/\n"
"\n"
"}\n"
""));
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
        openImageFolderPushBtn = new QPushButton(centralwidget);
        openImageFolderPushBtn->setObjectName(QStringLiteral("openImageFolderPushBtn"));

        verticalLayout_10->addWidget(openImageFolderPushBtn);

        outlinesGrpBox = new QGroupBox(centralwidget);
        outlinesGrpBox->setObjectName(QStringLiteral("outlinesGrpBox"));
        outlinesGrpBox->setStyleSheet(QStringLiteral(""));
        verticalLayout_3 = new QVBoxLayout(outlinesGrpBox);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        treeWidget = new QTreeWidget(outlinesGrpBox);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
		treeWidget->setStyleSheet(QLatin1String("QHeaderView::section { color: white; background: rgb(32,32,32); }\n"));
//"QTreeWidget::item { border: 0.5px ; border-style: solid ; border-color: lightgray ;}"));
        treeWidget->setHeaderHidden(false);
        treeWidget->setColumnCount(2);
        treeWidget->header()->setProperty("showSortIndicator", QVariant(false));

        verticalLayout_3->addWidget(treeWidget);


        verticalLayout_10->addWidget(outlinesGrpBox);


        horizontalLayout->addLayout(verticalLayout_10);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        charsGrpBox = new QGroupBox(centralwidget);
        charsGrpBox->setObjectName(QStringLiteral("charsGrpBox"));
        charsGrpBox->setMinimumSize(QSize(105, 310));
        charsGrpBox->setMaximumSize(QSize(16777215, 310));
        verticalLayout_4 = new QVBoxLayout(charsGrpBox);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        maxNonAZdensityLbl = new QLabel(charsGrpBox);
        maxNonAZdensityLbl->setObjectName(QStringLiteral("maxNonAZdensityLbl"));
        maxNonAZdensityLbl->setAlignment(Qt::AlignCenter);
        maxNonAZdensityLbl->setWordWrap(true);

        verticalLayout_4->addWidget(maxNonAZdensityLbl);

        maxNonAZdensity = new QDoubleSpinBox(charsGrpBox);
        maxNonAZdensity->setObjectName(QStringLiteral("maxNonAZdensity"));
        maxNonAZdensity->setStyleSheet(QStringLiteral("*{background-color: rgb(32,32,32); border-radius: 5px; padding: 3}"));
        maxNonAZdensity->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        maxNonAZdensity->setDecimals(3);
        maxNonAZdensity->setMaximum(1);
        maxNonAZdensity->setSingleStep(0.01);
        maxNonAZdensity->setValue(0.3);

        verticalLayout_4->addWidget(maxNonAZdensity);

        maxNumDensityLbl = new QLabel(charsGrpBox);
        maxNumDensityLbl->setObjectName(QStringLiteral("maxNumDensityLbl"));
        maxNumDensityLbl->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(maxNumDensityLbl);

        max09densitySBox = new QDoubleSpinBox(charsGrpBox);
        max09densitySBox->setObjectName(QStringLiteral("max09densitySBox"));
        max09densitySBox->setStyleSheet(QStringLiteral("*{background-color: rgb(32,32,32); border-radius: 5px; padding: 3}"));
        max09densitySBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        max09densitySBox->setDecimals(3);
        max09densitySBox->setMaximum(1);
        max09densitySBox->setSingleStep(0.01);
        max09densitySBox->setValue(0.3);

        verticalLayout_4->addWidget(max09densitySBox);

        minCharCntLbl = new QLabel(charsGrpBox);
        minCharCntLbl->setObjectName(QStringLiteral("minCharCntLbl"));
        minCharCntLbl->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(minCharCntLbl);

        minCharCount = new QSpinBox(charsGrpBox);
        minCharCount->setObjectName(QStringLiteral("minCharCount"));
        minCharCount->setStyleSheet(QStringLiteral("*{background-color: rgb(32,32,32); border-radius: 5px; padding: 3}"));
        minCharCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        minCharCount->setMaximum(256);
        minCharCount->setValue(4);

        verticalLayout_4->addWidget(minCharCount);

        label = new QLabel(charsGrpBox);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_4->addWidget(label);

        spinBox = new QSpinBox(charsGrpBox);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setStyleSheet(QStringLiteral("*{background-color: rgb(32,32,32); border-radius: 5px; padding: 3}"));
        spinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox->setMinimum(5);
        spinBox->setMaximum(1024);
        spinBox->setValue(80);

        verticalLayout_4->addWidget(spinBox);

        outlineCharConstrainPushBtn = new QPushButton(charsGrpBox);
        outlineCharConstrainPushBtn->setObjectName(QStringLiteral("outlineCharConstrainPushBtn"));

        verticalLayout_4->addWidget(outlineCharConstrainPushBtn);


        verticalLayout_2->addWidget(charsGrpBox);

        hierarchyGrpBox = new QGroupBox(centralwidget);
        hierarchyGrpBox->setObjectName(QStringLiteral("hierarchyGrpBox"));
        hierarchyGrpBox->setMaximumSize(QSize(16777215, 245));
        verticalLayout_6 = new QVBoxLayout(hierarchyGrpBox);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        autoNestingChkBox = new QCheckBox(hierarchyGrpBox);
        autoNestingChkBox->setObjectName(QStringLiteral("autoNestingChkBox"));
        autoNestingChkBox->setEnabled(false);

        verticalLayout_6->addWidget(autoNestingChkBox);

        line_5 = new QFrame(hierarchyGrpBox);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(line_5);

        numberedHeadingsChkBox = new QCheckBox(hierarchyGrpBox);
        numberedHeadingsChkBox->setObjectName(QStringLiteral("numberedHeadingsChkBox"));
        numberedHeadingsChkBox->setEnabled(false);
        numberedHeadingsChkBox->setLayoutDirection(Qt::RightToLeft);

        verticalLayout_6->addWidget(numberedHeadingsChkBox);

        latinNumeralsChkBox = new QCheckBox(hierarchyGrpBox);
        latinNumeralsChkBox->setObjectName(QStringLiteral("latinNumeralsChkBox"));
        latinNumeralsChkBox->setEnabled(false);
        latinNumeralsChkBox->setLayoutDirection(Qt::RightToLeft);

        verticalLayout_6->addWidget(latinNumeralsChkBox);

        abcListingsChkBox = new QCheckBox(hierarchyGrpBox);
        abcListingsChkBox->setObjectName(QStringLiteral("abcListingsChkBox"));
        abcListingsChkBox->setEnabled(false);
        abcListingsChkBox->setLayoutDirection(Qt::RightToLeft);

        verticalLayout_6->addWidget(abcListingsChkBox);

        hierarchyPushBtn = new QPushButton(hierarchyGrpBox);
        hierarchyPushBtn->setObjectName(QStringLiteral("hierarchyPushBtn"));
        hierarchyPushBtn->setEnabled(false);

        verticalLayout_6->addWidget(hierarchyPushBtn);


        verticalLayout_2->addWidget(hierarchyGrpBox);

        preprocessGrpBox = new QGroupBox(centralwidget);
        preprocessGrpBox->setObjectName(QStringLiteral("preprocessGrpBox"));
        preprocessGrpBox->setMaximumSize(QSize(16777215, 125));
        verticalLayout_5 = new QVBoxLayout(preprocessGrpBox);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        deskewChkBox = new QCheckBox(preprocessGrpBox);
        deskewChkBox->setObjectName(QStringLiteral("deskewChkBox"));
        deskewChkBox->setEnabled(false);

        verticalLayout_5->addWidget(deskewChkBox);

        removeImgsChkBox = new QCheckBox(preprocessGrpBox);
        removeImgsChkBox->setObjectName(QStringLiteral("removeImgsChkBox"));
        removeImgsChkBox->setEnabled(false);

        verticalLayout_5->addWidget(removeImgsChkBox);

        preprocessPushBtn = new QPushButton(preprocessGrpBox);
        preprocessPushBtn->setObjectName(QStringLiteral("preprocessPushBtn"));
        preprocessPushBtn->setEnabled(false);

        verticalLayout_5->addWidget(preprocessPushBtn);


        verticalLayout_2->addWidget(preprocessGrpBox);

        revertPushBtn = new QPushButton(centralwidget);
        revertPushBtn->setObjectName(QStringLiteral("revertPushBtn"));

        verticalLayout_2->addWidget(revertPushBtn);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_5);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        prevPageBtn = new QPushButton(centralwidget);
        prevPageBtn->setObjectName(QStringLiteral("prevPageBtn"));

        horizontalLayout_2->addWidget(prevPageBtn);

        pageLabel = new QLabel(centralwidget);
        pageLabel->setObjectName(QStringLiteral("pageLabel"));

        horizontalLayout_2->addWidget(pageLabel);

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
        menubar->setGeometry(QRect(0, 0, 800, 26));
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
        actionsMenu->addAction(removeRowAction);
        actionsMenu->addAction(insertChildAction);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Editable Tree Model", Q_NULLPTR));
        exitAction->setText(QApplication::translate("MainWindow", "E&xit", Q_NULLPTR));
        exitAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
        insertRowAction->setText(QApplication::translate("MainWindow", "Insert Row", Q_NULLPTR));
        insertRowAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+I, R", Q_NULLPTR));
        removeRowAction->setText(QApplication::translate("MainWindow", "Remove Row", Q_NULLPTR));
        removeRowAction->setShortcut(QApplication::translate("MainWindow", "Del", Q_NULLPTR));
        insertColumnAction->setText(QApplication::translate("MainWindow", "Insert Column", Q_NULLPTR));
        insertColumnAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+I, C", Q_NULLPTR));
        removeColumnAction->setText(QApplication::translate("MainWindow", "Remove Column", Q_NULLPTR));
        removeColumnAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+R, C", Q_NULLPTR));
        insertChildAction->setText(QApplication::translate("MainWindow", "Insert Child", Q_NULLPTR));
        insertChildAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", Q_NULLPTR));
        actionOpen_Folder_with_scanned_Pages->setText(QApplication::translate("MainWindow", "Open Folder with scanned Pages", Q_NULLPTR));
        openImageFolderPushBtn->setText(QApplication::translate("MainWindow", "Open Folder with Scanned Doc. Images ...", Q_NULLPTR));
        outlinesGrpBox->setTitle(QApplication::translate("MainWindow", "Outlines", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "Page", Q_NULLPTR));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Heading Text", Q_NULLPTR));
        charsGrpBox->setTitle(QApplication::translate("MainWindow", "Characters", Q_NULLPTR));
        maxNonAZdensityLbl->setText(QApplication::translate("MainWindow", "non-letter chars\n"
" max density", Q_NULLPTR));
        maxNumDensityLbl->setText(QApplication::translate("MainWindow", "number chars\n"
"max density", Q_NULLPTR));
        minCharCntLbl->setText(QApplication::translate("MainWindow", "min char count", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "max char count", Q_NULLPTR));
        outlineCharConstrainPushBtn->setText(QApplication::translate("MainWindow", "Process File", Q_NULLPTR));
        hierarchyGrpBox->setTitle(QApplication::translate("MainWindow", "Hierarchy", Q_NULLPTR));
        autoNestingChkBox->setText(QApplication::translate("MainWindow", "AutoNesting", Q_NULLPTR));
        numberedHeadingsChkBox->setText(QApplication::translate("MainWindow", "Numbered\n"
"Headings", Q_NULLPTR));
        latinNumeralsChkBox->setText(QApplication::translate("MainWindow", "Latin(iv IV)\n"
"Numerals", Q_NULLPTR));
        abcListingsChkBox->setText(QApplication::translate("MainWindow", "a. b. c. ...\n"
"Listings", Q_NULLPTR));
        hierarchyPushBtn->setText(QApplication::translate("MainWindow", "Process File", Q_NULLPTR));
        preprocessGrpBox->setTitle(QApplication::translate("MainWindow", "Preprocess", Q_NULLPTR));
        deskewChkBox->setText(QApplication::translate("MainWindow", "DeSkew", Q_NULLPTR));
        removeImgsChkBox->setText(QApplication::translate("MainWindow", "RmvImgs", Q_NULLPTR));
        preprocessPushBtn->setText(QApplication::translate("MainWindow", "Process File", Q_NULLPTR));
        revertPushBtn->setText(QApplication::translate("MainWindow", "Revert All Changes", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Document View", Q_NULLPTR));
        prevPageBtn->setText(QApplication::translate("MainWindow", "\342\227\204", Q_NULLPTR));
        pageLabel->setText(QApplication::translate("MainWindow", "Page:", Q_NULLPTR));
        nextPageBtn->setText(QApplication::translate("MainWindow", "\342\226\272", Q_NULLPTR));
        savePDFbutton->setText(QApplication::translate("MainWindow", "Save Outlined PDF ...", Q_NULLPTR));
        fileMenu->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
        actionsMenu->setTitle(QApplication::translate("MainWindow", "&Actions", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
