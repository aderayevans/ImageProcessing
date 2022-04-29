#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_browse_clicked();
    void DisplayImage(QImage &img, QString title = "Image");
    //Session 1
    void makeGrayImage();
    void on_btn_brightness_clicked();
    void on_btn_negative_clicked();
    void on_btn_threshold_clicked();

    //Session 2
    void Show_Histogram(QImage &, const int HEIGHT = 128);
    void on_btn_showHis_clicked();
    void on_btn_contrast_clicked(); //căng tổ chức đồ
    void on_btn_linearHis_clicked();
    void on_btn_balanceHis_clicked();

    //Session 3
    void on_btn_mean_filter_clicked();
    void on_btn_median_filter_clicked();
    void on_btn_add_noise_clicked();

    //Session 4
    void on_btn_gradient_clicked();
    void on_btn_robert_clicked();
    void on_btn_sobel_clicked();
    void on_btn_prewitt_clicked();
    void on_btn_laplace_clicked();
    void on_btn_gauss_n_laplace_clicked();

    //Session 5
    void thresholding(int c);
    void on_btn_sequence_threshold_clicked();

    void on_btn_otsu_threshold_clicked();

    QImage erosion(QImage image_in); //an mon
    QImage dilation(QImage image_in); //gian no

    void on_btn_erosion_clicked();

    void on_btn_dilation_clicked();

    void on_btn_open_clicked();

    void on_btn_close_clicked();

private:
    Ui::MainWindow *ui;
    QImage original_image, gray_image;
};

#endif // MAINWINDOW_H
