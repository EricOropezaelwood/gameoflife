#include <QtWidgets>
#include <iostream>

#include "golBoard.h"

GolBoard::GolBoard(QWidget *parent) : QFrame(parent)
{
    std::srand(time(0));
    setFrameStyle(QFrame::Panel);
    setMouseTracking(true);
    height = 200;
    width = 200;
    cellsize = 3;
    isPaused = true;
    iteration = 0;
    timeoutTime = 25;
    popRatio = 0.4f;
    resize(width*cellsize+2*frameWidth(), height*cellsize+2*frameWidth());
    populate();
}

void GolBoard::clear() {
    for (int i=0; i<width*height; i++) {
        grid[i] = 0;
    }
    isPaused = true;
    timer.stop();
    iteration = 0;
    emit changeLabel("iterationLabel", QString("Iteration: %1").arg(iteration));
    emit changeLabel("aliveCellsLabel", QString("Alive cells: 0"));
    emit checkPauseBtn();
    update();
}

void GolBoard::drawCell(QPainter &painter, int x, int y) {
    QColor color = Qt::black;
    painter.fillRect(x*cellsize+1, y*cellsize+1, cellsize, cellsize, color);
}

void GolBoard::iterate() {
    int aliveCells = 0;
    tmp_grid.clear();
    for (int i=0; i<width*height; i++) {
        tmp_grid << 0;
    }
    for (int n=0; n<grid.size(); n++) {
        int ncount = neighbor_count(n);

        if ((0 < grid.at(n)) && (2 == ncount || 3 == ncount)) {
            /* Any live cell with two or three live neighbours
             * lives on */
            tmp_grid[n] = 1;
            aliveCells++;
        }
        else if ((0 == grid.at(n)) && (3 == ncount)) {
            /* Any dead cell with exactly three live neighbours becomes
             * alive */
            tmp_grid[n] = 1;
            aliveCells++;
        }
    }
    grid = tmp_grid;

    iteration++;
    emit changeLabel("iterationLabel", QString("Iteration: %1").arg(iteration));
    emit changeLabel("aliveCellsLabel", QString("Alive cells: %1").arg(aliveCells));
}

QSize GolBoard::minimumSizeHint() const
{
    return QSize(width+2*frameWidth(), height+2*frameWidth());
}

void GolBoard::mouseMoveEvent(QMouseEvent *event){
    int x = (event->x()-frameWidth())/cellsize;
    int y = (event->y()-frameWidth())/cellsize;
    QToolTip::showText(event->globalPos(), QString(tr("(%1,%2)").arg(x).arg(y)));
}

void GolBoard::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        int x = (event->x()-frameWidth())/cellsize;
        int y = (event->y()-frameWidth())/cellsize;
        int idx = (x*width + y);

        if (idx < 0 || idx >= width*height)
            return;

        if (grid[idx]) {
            grid[idx] = 0;
        }
        else {
            grid[idx] = 1;
        }
        update();
    }
}

int GolBoard::neighbor_count(int n) {

    /* | n-w-1 | n-w | n-w+1 | */
    /* | n-1   | n   | n+1   | */
    /* | n+w-1 | n+w | n+w+1 | */

    int w = width;
    int ncount = 0;
    /* x- and y-coordinate of n */
    int xCoord = n%w;
    int yCoord = n/w;

    /* check row above n */
    if (yCoord > 0) {
        /* check neighbor NW */
        if (xCoord > 0 && grid.at(n-w-1)) ncount++;
        /* check neighbor N */
        if (grid.at(n-w)) ncount++;
        /* check neighbor NE */
        if (xCoord+1 < w && grid.at(n-w+1)) ncount++;
    }

    /* check neighbor W */
    if (xCoord > 0 && grid.at(n-1)) ncount++;
    /* check neighbor E */
    if (xCoord+1 < w && grid.at(n+1)) ncount++;

    /* check row below n */
    if (yCoord+1 < height) {
        /* check neighbor SW */
        if (xCoord > 0 && grid.at(n+w-1)) ncount++;
        /* check neighbor S */
        if (grid.at(n+w)) ncount++;
        /* check neighbor SE */
        if (xCoord+1 < w && grid.at(n+w+1)) ncount++;
    }

    return ncount;
}

void GolBoard::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    for (int i=0; i < height; i++) {
        for (int j=0; j < width; j++) {
            if (grid[i*width + j]) {
                drawCell(painter, i, j);
            }
        }
    }
}

void GolBoard::pause() {
    if (isPaused) {
        iterate();
        update();
        return;
    }
    isPaused = true;
    timer.stop();
    update();
}

void GolBoard::populate() {
    grid.clear();
    for (int i=0; i<width*height; i++) {
        grid << (rand() < popRatio * ((double)RAND_MAX + 1.0));
    }
    update();
}

void GolBoard::resizeEvent(QResizeEvent *event) {
    QSize s = event->size();
    int t = std::min(s.width(), s.height());
    cellsize = t/width;
    resize(width*cellsize+2*frameWidth(), height*cellsize+2*frameWidth());
    update();
}

void GolBoard::setBoardSize(int w, int h) {
    width = w;
    height = h;
}

void GolBoard::setPopRatio(int p) {
    popRatio = p / 100.0f;
}

void GolBoard::setTimeoutTime(int timeout) {
    timeoutTime = 1000/timeout;
}

QSize GolBoard::sizeHint() const
{
    return QSize(width*cellsize+2*frameWidth(), height*cellsize+2*frameWidth());
}

void GolBoard::start() {
    if (isPaused) {
        isPaused = false;
    }
    timer.start(timeoutTime, this);
}

void GolBoard::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timer.timerId()) {
        iterate();
        timer.start(timeoutTime, this);
        update();
    } else {
        QFrame::timerEvent(event);
    }
}

void GolBoard::wheelEvent(QWheelEvent *event) {
    int tmp_cellsize = cellsize + (event->angleDelta()/120).y();
    cellsize = std::min(size().width()/width, std::max(1, tmp_cellsize));
    update();
}
