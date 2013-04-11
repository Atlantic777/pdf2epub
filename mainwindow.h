#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <converter.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    Converter c;
    void update_label_img();
    ~MainWindow();
    
private slots:
    void on_btn_x_cut_clicked();

    void on_btn_y_cut_clicked();

    void on_btn_cut_clicked();

    void on_btn_load_clicked();

    void on_spin_page_valueChanged(int page_num);

    void on_slider_white_treshold_actionTriggered(int action);

    void on_spin_white_treshold_valueChanged(int arg1);

    void on_spin_block_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
