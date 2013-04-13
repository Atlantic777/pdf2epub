#include "block.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <converter.h>
#include <QPixmap>

using namespace cv;

Block::Block()
{

}

Block::Block(Mat crop)
{
    crop.copyTo(original);
}

void Block::show(const QString wname)
{
    namedWindow(wname.toStdString());
    imshow(wname.toStdString(), this->original);
}

void Block::operator=(Mat src)
{
    original = src;
}


QVector<int> Block::sum_x()
{
        QVector<int> result;
        int row_sum = 0;

        for(int y = 0; y < original.rows; y++)
        {
            for(int x = 0; x < original.cols; x++)
            {
                row_sum += original.data[y*original.cols+x];
            }
            result.push_back(row_sum/original.cols);
            row_sum = 0;
        }
        return result;
}

QVector<int> Block::sum_y()
{
        QVector<int> result;
        int col_sum = 0;

        for(int x = 0; x < original.cols; x++)
        {
            for(int y = 0; y < original.rows; y++)
            {
                col_sum += original.data[y*original.cols+x];
            }
            result.push_back(col_sum/original.rows);
            col_sum = 0;
        }
        return result;
}

QVector<int> Block::x_cut(int treshold)
{
    QVector<int> sum = sum_x();
    QVector<int> marks = get_marks(sum, treshold);

    for(int i = 0; i < marks.size(); i+=2)
    {
        QSize new_size = QSize(size.width(), marks.at(i+1)-marks.at(i));
        QPoint new_topleft = QPoint(topleft.x(), marks.at(i));

       int x1 = topleft.x();
       int y1 = topleft.y()+marks.at(i);
       int x2 = new_size.width();
       int y2 = new_size.height();

       cv::Mat tmp_mat = original(cv::Rect(x1, y1, x2, y2)).clone();

        Block tmp_block(tmp_mat);
        tmp_block.root = root;
        tmp_block.size = new_size;
        tmp_block.topleft = new_topleft;
        BB_list.append(tmp_block);

    }
    return marks;
}

QVector<int> Block::y_cut(int treshold)
{
    QVector<int> sum = sum_y();
    QVector<int> marks = get_marks(sum, treshold);

    for(int i = 0; i < marks.size(); i+=2)
    {
        QSize new_size = QSize(marks.at(i+1)-marks.at(i), size.height());
        QPoint new_topleft = QPoint(marks.at(i), topleft.y());

       int x1 = topleft.x()+marks.at(i);
       int y1 = topleft.y();
       int x2 = new_size.width();
       int y2 = new_size.height();

       cv::Mat tmp_mat = root->original(cv::Rect(x1, y1, x2, y2)).clone();

        Block tmp_block(tmp_mat);
        tmp_block.root = root;
        tmp_block.size = new_size;
        tmp_block.topleft = new_topleft;
        BB_list.append(tmp_block);

    }
    return marks;
}

/*
QImage Block::ilustrate()
{
    Mat tmp;
    original.copyTo(tmp);

    QVector<int> sum = sum_h();

    for(int y = 0; y < original.rows; y++)
    {
        for(int x = 0; x < original.cols; x++)
        {
            tmp.data[y*original.cols+x] = sum.at(y);
        }
    }

    return Mat2QImage(&tmp);
}
*/


QVector<int> Block::get_marks(QVector<int> sum, int white_limit)
{
    QVector<int> result;
    int black_counter = 0;
    int white_counter = 0;
    int state = 1;

    for(int i = 0; i < sum.size(); i++)
    {
        //white zone
        if((sum.at(i) == 255) && (state == 1))
        {
            white_counter++;
        }
        //black zone
        else if((sum.at(i) < 255) && (state == 0))
        {
            white_counter = 0;
            black_counter++;
        }
        //to white and gray zone
        else if((sum.at(i) == 255) && (state == 0))
        {
            if(white_counter > white_limit)
            {
                result.push_back(i-black_counter-3);
                result.push_back(i-white_limit+3);
                black_counter = 0;
                state = 1;
            }
            else if(i == sum.size()-1)
            {
                result.push_back(i-black_counter-3);
                result.push_back(i-white_counter+3);
            }

            else
            {
                black_counter++;
                white_counter++;
            }
        }
        //to black
        else if((sum.at(i) < 255) && (state == 1))
        {
            black_counter = 1;
            white_counter = 0;
            state = 0;
        }
    }
    qSort(result);
    return result;
}

QPixmap Block::get_pixmap()
{
    return QPixmap::fromImage(Converter::Mat2QImage(&original));
}

float Block::get_ratio()
{
    float black = 0;
    float white = 0;
    int sum = original.cols*original.rows;

    for(int i = 0; i < original.cols; i++)
    {
        for(int j = 0; j < original.rows; j++)
        {
            if(original.data[j*original.cols + i] < 255)
            {
                black++;
            }
            else
            {
                white++;
            }
        }
    }

    return black/(original.cols*original.rows)*100;
}
