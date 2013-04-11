#ifndef CONVERTER_H
#define CONVERTER_H
#include <QString>
#include <QImage>
#include <opencv2/core/core.hpp>
#include <block.h>
#include <poppler-qt4.h>

using namespace cv;

class Converter
{
public:
    Converter();
    void load_file(QString);
    static QImage Mat2QImage(Mat*);
    static Mat QImage2Mat(QImage*);
    Block main_block;
    Poppler::Document *doc;
    int current_page;
    void change_page(int);
    QVector<Block> all_blocks;
};

#endif // CONVERTER_H
