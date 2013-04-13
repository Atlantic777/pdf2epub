#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <block.h>
#include <QPainter>
#include <poppler-qt4.h>
#include <converter.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelectionModel>

using namespace Poppler;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_x_cut_clicked()
{
    int white_x = ui->spin_white_treshold->text().toInt();
    QVector<int> marks = c.main_block.x_cut(white_x);
    QPainter p((QPixmap*)ui->label_img->pixmap());
    p.setBrush(QBrush(QColor(255, 0, 0, 80)));

    QVector<Block>::Iterator b_list;

    for(b_list = c.main_block.BB_list.begin(); b_list != c.main_block.BB_list.end(); b_list++)
    {
        p.drawRect(QRect(b_list->topleft, b_list->size));
    }

    for(int i = 0; i < marks.size(); i++)
    {
        ui->textBrowser->append(QString::number(marks.at(i)));
    }
    ui->label_img->update();

}

void MainWindow::on_btn_y_cut_clicked()
{
    int white_y = ui->spin_white_y->text().toInt();
    QVector<int> marks = c.main_block.y_cut(white_y);
    QPainter p((QPixmap*)ui->label_img->pixmap());
    p.setBrush(QBrush(QColor(255, 0, 0, 80)));

    for(int i = 0; i < marks.size(); i++)
    {
        ui->textBrowser->append(QString::number(marks.at(i)));
    }

    QVector<Block>::Iterator b_list;


    for(b_list = c.main_block.BB_list.begin(); b_list != c.main_block.BB_list.end(); b_list++)
    {
        p.drawRect(QRect(b_list->topleft, b_list->size));
    }
    ui->label_img->update();
}

void MainWindow::on_btn_cut_clicked()
{
    c.change_page(c.current_page);
    update_label_img();

    c.all_blocks.clear();


    int white_x = ui->spin_white_treshold->text().toInt();
    int white_y = ui->spin_white_y->text().toInt();
    c.main_block.x_cut(white_x);
    QVector<Block>::Iterator b_list_x;
    QVector<Block>::Iterator b_list_y;

    QPainter p((QPixmap*)ui->label_img->pixmap());
    p.setBrush(QBrush(QColor(255, 0, 0, 80)));

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item = model->invisibleRootItem();

    int i = 0;
    for(b_list_x = c.main_block.BB_list.begin(); b_list_x != c.main_block.BB_list.end(); b_list_x++, i++)
    {
        b_list_x->y_cut(white_y);
        for(b_list_y = b_list_x->BB_list.begin(); b_list_y != b_list_x->BB_list.end(); b_list_y++)
        {
            p.drawRect(QRect(b_list_y->topleft, b_list_y->size));
            c.all_blocks.append(*b_list_y);
            item->appendRow(new QStandardItem(QString("blok n:").append(QString::number(i))));
        }
    }
    if(c.all_blocks.size()>0)
        ui->spin_block->setMaximum(c.all_blocks.size()-1);
    ui->label_img->update();
}

void MainWindow::on_btn_load_clicked()
{
    QString filename = ui->txt_filename->text();
    int page_num = ui->spin_page->text().toInt();
    c.load_file(filename);
    c.change_page(page_num);
    update_label_img();
    ui->spin_page->setMaximum(c.doc->numPages());
}

void MainWindow::on_spin_page_valueChanged(int page_num)
{
    c.change_page(page_num);
    c.current_page = page_num;
    update_label_img();
    on_btn_cut_clicked();
}

void MainWindow::update_label_img()
{
    ui->label_img->setPixmap(c.main_block.get_pixmap());
    ui->label_img->update();
}

void MainWindow::on_slider_white_treshold_actionTriggered(int action)
{
    ui->spin_white_treshold->setValue(ui->slider_white_treshold->value());
}

void MainWindow::on_spin_white_treshold_valueChanged(int arg1)
{
    ui->slider_white_treshold->setValue(arg1);
}

void MainWindow::on_spin_block_valueChanged(int arg1)
{
    on_btn_cut_clicked();
    int current = arg1;
    QRect roi(c.all_blocks.at(current).topleft, c.all_blocks.at(current).size);
    QString contents = c.doc->page(c.current_page)->text(roi);
    QRegExp exp1("      ");
    QRegExp exp2("[\\.]{3,200}");
    ui->textBrowser->setText(contents.replace("\n", " ").replace(exp1, "\n\t").replace(exp2, " -> "));

    QPainter p((QPixmap*)ui->label_img->pixmap());
    p.setBrush(QBrush(QColor(0, 255, 0, 80)));
    p.drawRect(roi);
    ui->label_img->update();

    Block b = c.all_blocks.at(arg1);
    float ratio = b.get_ratio();
    ui->label_ratio_res->setText(QString::number(ratio));
}
