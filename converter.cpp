#include "converter.h"
#include "poppler-qt4.h"
#include <opencv2/opencv.hpp>
#include <QDebug>

Converter::Converter()
{

}

void Converter::load_file(QString filename)
{
    doc = Poppler::Document::load(filename);
    current_page = 0;
}

QImage Converter::Mat2QImage(Mat *src)
{
    QImage dest(src->cols, src->rows, QImage::Format_Indexed8);

    for(int y = 0; y < src->rows; y++)
    {
        memcpy(dest.scanLine(y), src->data + y*src->cols, src->cols);
    }

    return dest;
}

Mat Converter::QImage2Mat(QImage *src)
{
    cv::Mat dest = cv::Mat(src->height(), src->width(), CV_8UC4, (uchar*)src->bits(), src->bytesPerLine());
    cv::Mat dst;
    cv::cvtColor(dest, dst, CV_RGB2GRAY);

    return dst;
}

void Converter::change_page(int page = 0)
{
    QImage tmp_img = doc->page(page)->renderToImage();
    main_block = Block(QImage2Mat(&tmp_img));
    main_block.size = tmp_img.size();
    main_block.topleft = QPoint(0, 0);
    main_block.root = &main_block;
}
