#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class MODE { ORIGINAL, MODIFIED, ORIGINAL_HIS, MODIFIED_HIS, GX, GY };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DisplayImage(QImage &img, MODE mode);

private slots:
    void on_btn_browse_clicked();

    void on_btn_brightness_clicked();

    void on_btn_negative_clicked();

    void on_btn_threshold_clicked();

    void on_rbtn_gray_toggled(bool checked);

    void on_hsld_brightness_valueChanged(int value);

    void on_spb_brightness_valueChanged(int arg1);

    void on_hsld_brightness_sliderReleased();

    void on_hsld_threshold_valueChanged(int value);

    void on_spb_threshold_valueChanged(int arg1);

    void on_btn_showHis_clicked();

    void on_spb_contrast_valueChanged(int arg1);

    void on_btn_contrast_clicked();

    void on_btn_linearHis_clicked();

    void on_btn_balanceHis_clicked();

    void showHistogram(QImage, MODE);

    void on_btn_mean_filter_clicked();

    void on_btn_median_filter_clicked();

    void on_btn_add_noise_clicked();

    void on_btn_save_clipboard_clicked();

    void on_btn_gradient_clicked();

    void on_btn_robert_clicked();

    void on_btn_sobel_clicked();

    void on_btn_prewitt_clicked();

    void on_btn_laplace_clicked();

    void on_btn_gauss_n_laplace_clicked();

private:
    Ui::MainWindow *ui;
    QImage original_image, modified_image, backup_image;
    QImage gx_image, gy_image;
    QPixmap pix;
};
#endif // MAINWINDOW_H
