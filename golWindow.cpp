#include <QtWidgets>

#include "golWindow.h"
#include "golBoard.h"

GolWindow::GolWindow()
{
    board = new GolBoard;
    startBtn = new QPushButton(tr("&Start"));
    pauseBtn = new QPushButton(tr("&Pause"));
    populateBtn = new QPushButton(tr("&Populate"));
    clearBtn = new QPushButton(tr("&Clear"));

    connect(startBtn, SIGNAL(clicked()), board, SLOT(start()));
    connect(pauseBtn, SIGNAL(clicked()), board, SLOT(pause()));
    connect(populateBtn, SIGNAL(clicked()), board, SLOT(populate()));
    connect(clearBtn, SIGNAL(clicked()), board, SLOT(clear()));

    QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(startBtn);
    vbox->addWidget(pauseBtn);
    vbox->addWidget(populateBtn);
    vbox->addWidget(clearBtn);
    groupBox->setLayout(vbox);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(groupBox, 0, 0);
    layout->addWidget(board, 0, 1, 4, 4);
    setLayout(layout);

    setWindowTitle(tr("Game of Life"));
    resize(550, 370);
}
