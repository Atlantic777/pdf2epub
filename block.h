#ifndef ANALISER_H
#define ANALISER_H
#include <opencv2/core/core.hpp>
#include <QImage>
#include <QString>
#include <QVector>

using namespace cv;

class Block
{
public:
    Block(Mat);
    Block();
    void show(QString);
    void operator=(Mat);
    QVector<int> x_cut(int);
    QVector<int> sum_x();
    QVector<int> sum_y();
    QVector<int> y_cut(int);
    QVector<int> get_marks(QVector<int>, int);
    Mat original;
    QPixmap get_pixmap();
    QVector<Block> BB_list;
    QPoint topleft;
    QSize size;
    Block *root;
};

#endif // ANALISER_H
