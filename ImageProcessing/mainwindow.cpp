#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QtAlgorithms>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spb_brightness->setMaximum(255);
    ui->spb_brightness->setMinimum(-255);
    ui->hsld_brightness->setMaximum(255);
    ui->hsld_brightness->setMinimum(-255);

    ui->spb_contrast->setMaximum(100);
    ui->spb_contrast->setMinimum(0);

    ui->spb_threshold->setMaximum(255);
    ui->spb_threshold->setMinimum(0);
    ui->hsld_threshold->setMaximum(255);
    ui->hsld_threshold->setMinimum(0);

    ui->spb_threshold->setValue(ui->spb_threshold->maximum()/2);

    ui->spb_mask_size->setMaximum(255);
    ui->spb_mask_size->setMinimum(0);

    ui->spb_noise_ratio->setMaximum(100);
    ui->spb_noise_ratio->setMinimum(0);

    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_save_clipboard_clicked()
{
    this->backup_image = this->original_image;
    this->original_image = this->modified_image;
    DisplayImage(this->original_image, MODE::ORIGINAL);
}

void MainWindow::on_btn_browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    "D:/Users/Pictures/Photos",
                                                    "*.* All files");
    if (fileName.isEmpty())
        return;
    ui->lne_browse->setText(fileName);
    ui->rbtn_gray->setChecked(false); //make sure to uncheck rbtn_gray after get image

    this->original_image = QImage(fileName);
    ui->lbl_dimensions->setText(
                        QString::number(this->original_image.width()) +
                        " x " +
                        QString::number(this->original_image.height()) +
                        " pixels"
                        );
    this->pix = QPixmap::fromImage(this->original_image);

    DisplayImage(this->original_image, MODE::ORIGINAL);
}

void MainWindow::on_btn_brightness_clicked()    //truot to chuc do
{
    QImage image_in = this->original_image;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    int c = ui->spb_brightness->value();

    if (ui->rbtn_gray->isChecked())
    {
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);
                int gray_out = (gray_in + c);

                if (gray_out > 255) gray_out = 255;
                else if (gray_out < 0) gray_out = 0;

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
    }
    else
    {
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color_in(rgb);
                int h, s, v;
                color_in.getHsv(&h, &s, &v);
                int v_out = v + c;
                if (v_out > 255)
                {
                    v_out = 255;
                }
                else if (v_out < 0)
                {
                    v_out = 0;
                }
                QColor color_out = QColor::fromHsv(h, s, v_out);
                image_out.setPixel(x, y, color_out.rgb());
            }
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}

void MainWindow::DisplayImage(QImage &img, MODE mode)
{
    QPixmap pix = QPixmap::fromImage(img);
    if (mode == MODE::ORIGINAL)
    {
        ui->lbl_originalPic->setPixmap(
                    pix.scaled(ui->lbl_originalPic->width(),
                               ui->lbl_originalPic->height(),
                               Qt::KeepAspectRatio)
                    );
    }
    if (mode == MODE::MODIFIED)
    {
        ui->lbl_modifiedPic->setPixmap(
                    pix.scaled(
                        ui->lbl_modifiedPic->width(),
                        ui->lbl_modifiedPic->height(),
                        Qt::KeepAspectRatio)
                    );
    }
    if (mode == MODE::ORIGINAL_HIS)
    {
        ui->lbl_originalHis->setPixmap(
                    pix.scaled(ui->lbl_originalHis->width(),
                               ui->lbl_originalHis->height(),
                               Qt::KeepAspectRatio)
                    );
    }
    if (mode == MODE::MODIFIED_HIS)
    {
        ui->lbl_modifiedHis->setPixmap(
                    pix.scaled(
                        ui->lbl_modifiedHis->width(),
                        ui->lbl_modifiedHis->height(),
                        Qt::KeepAspectRatio)
                    );
    }
    if (mode == MODE::GX)
    {
        ui->lbl_gx->setPixmap(
                    pix.scaled(
                        ui->lbl_gx->width(),
                        ui->lbl_gx->height(),
                        Qt::KeepAspectRatio)
                    );
    }
    if (mode == MODE::GY)
    {
        ui->lbl_gy->setPixmap(
                    pix.scaled(
                        ui->lbl_gy->width(),
                        ui->lbl_gy->height(),
                        Qt::KeepAspectRatio)
                    );
    }
}

void MainWindow::on_btn_negative_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    for (int x = 0; x < image_in.width(); x++)
    {
        for (int y = 0; y < image_in.height(); y++)
        {
            if (ui->rbtn_gray->isChecked())
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);
                int gray_out = 255 - gray_in;
                if (gray_out > 255) gray_out = 255;
                else if (gray_out < 0) gray_out = 0;

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
            else
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color_in(rgb);
                int r, g, b;
                color_in.getRgb(&r, &g, &b);
                int r_out = 255 - r;
                int g_out = 255 - g;
                int b_out = 255 - b;
                if (r_out > 255)
                {
                    r_out = 255;
                }
                else if (r_out < 0)
                {
                    r_out = 0;
                }
                if (g_out > 255)
                {
                    g_out = 255;
                }
                else if (g_out < 0)
                {
                    g_out = 0;
                }
                if (b_out > 255)
                {
                    b_out = 255;
                }
                else if (b_out < 0)
                {
                    b_out = 0;
                }
                QColor color_out = QColor::fromRgb(r_out, g_out, b_out);
                image_out.setPixel(x, y, color_out.rgb());
            }
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}

void MainWindow::on_btn_threshold_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    int c = ui->spb_threshold->value();
    for (int x = 0; x < image_in.width(); x++)
    {
        for (int y = 0; y < image_in.height(); y++)
        {
            /*
            QRgb rgb = image_in.pixel(x, y);
            QColor color_in(rgb);
            int h, s, v;
            color_in.getHsv(&h, &s, &v);
            int v_out = 0;
            if (v >= c)
            {
                v_out = 255;
            }
            QColor color_out = QColor::fromHsv(h, s, v_out);
            image_out.setPixel(x, y, color_out.rgb());
            */

            QRgb color = image_in.pixel(x, y);
            int gray_in = qGray(color);
            int gray_out = 0;
            if (gray_in >= c) gray_out = 255;
            QRgb color_out = qRgb(gray_out, gray_out, gray_out);
            image_out.setPixel(x, y, color_out);
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}

void MainWindow::on_rbtn_gray_toggled(bool checked)
{
    if (checked)
    {
        QImage image_in = this->original_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray = qGray(color);

                QRgb color_out = qRgb(gray, gray, gray);
                image_out.setPixel(x, y, color_out);
            }
        }
        this->modified_image = image_out;
        DisplayImage(image_out, MODE::ORIGINAL);
    }
    else
    {
        DisplayImage(this->original_image, MODE::ORIGINAL);
    }
}

void MainWindow::on_hsld_brightness_valueChanged(int value)
{
    ui->spb_brightness->setValue(value);
}

void MainWindow::on_spb_brightness_valueChanged(int arg)
{
    ui->hsld_brightness->setValue(arg);
    ui->btn_brightness->click();
}

void MainWindow::on_hsld_brightness_sliderReleased()
{
    //ui->btn_brightness->click();
}

void MainWindow::on_hsld_threshold_valueChanged(int value)
{
    ui->spb_threshold->setValue(value);
}

void MainWindow::on_spb_threshold_valueChanged(int arg)
{
    ui->hsld_threshold->setValue(arg);
    ui->btn_threshold->click();
}

void MainWindow::showHistogram(QImage image_in, MODE mode)
{
    const int HEIGHT = 128;
    if (ui->rbtn_gray->isChecked())
    {
        // calculate histogram
        int h[256];
        for (int i = 0; i < 256; i++)
        {
            h[i] = 0;
        }
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                int gray = qGray(rgb);
                h[gray]++;
            }
        }
        // calculate histogram end
        int max = 0;
        for (int i = 0; i < 256; i++)
            if (h[i] > max) max = h[i];
        int lineHeight = 0;
        QImage histogram(256, HEIGHT, QImage::Format_ARGB32);
        histogram.fill(Qt::white);
        for (int x = 0; x < 256; x++)
        {
            lineHeight = HEIGHT*h[x]/max;
            for (int y = HEIGHT - 1; y > HEIGHT - 1 - lineHeight; y--)
            {
                histogram.setPixel(x, y, qRgb(0, 0, 255));
            }
        }
        DisplayImage(histogram, mode);
    }
    else
    {
        // calculate histogram
        int h_red[256], h_blue[256], h_green[256];
        for (int i = 0; i < 256; i++)
        {
            h_red[i] = h_blue[i] = h_green[i] = 0;
        }
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QColor color = image_in.pixel(x, y);
                int red = color.red();
                int green = color.green();
                int blue = color.blue();
                h_red[red]++;
                h_green[green]++;
                h_blue[blue]++;
            }
        }
        // calculate histogram end
        int max = 0;
        for (int i = 0; i < 256; i++)
        {
            if (max < h_red[i]) max = h_red[i];
            if (max < h_green[i]) max = h_green[i];
            if (max < h_blue[i]) max = h_blue[i];
        }
        QImage histogram = QImage(256, HEIGHT*3, QImage::Format_RGB32);
        histogram.fill(Qt::white);
        int lineHeight;
        for (int i = 0; i < 256; i++)
        {
            lineHeight = HEIGHT*h_red[i]/max;
            for (int j = HEIGHT - 1; j > HEIGHT - lineHeight; j--)
            {
                histogram.setPixel(i, j, qRgb(255, 0, 0));
            }

            lineHeight = HEIGHT*h_green[i]/max;
            for (int j = 2*HEIGHT - 1; j > 2*HEIGHT - lineHeight; j--)
            {
                histogram.setPixel(i, j, qRgb(0, 255, 0));
            }

            lineHeight = HEIGHT*h_red[i]/max;
            for (int j = 3*HEIGHT - 1; j > 3*HEIGHT - lineHeight; j--)
            {
                histogram.setPixel(i, j, qRgb(0, 0, 255));
            }
        }
        DisplayImage(histogram, mode);
    }
}

void MainWindow::on_btn_showHis_clicked()
{
    showHistogram(this->original_image, MODE::ORIGINAL_HIS);
    if (!this->modified_image.isNull())
    {
        showHistogram(this->modified_image, MODE::MODIFIED_HIS);
    }
}

void MainWindow::on_spb_contrast_valueChanged(int arg)
{
    ui->btn_contrast->click();
}

void MainWindow::on_btn_contrast_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    float c = ui->spb_contrast->value();
    for (int x = 0; x < image_in.width(); x++)
    {
        for (int y = 0; y < image_in.height(); y++)
        {
            if (ui->rbtn_gray->isChecked())
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);
                int gray_out = (gray_in * c);
                if (gray_out > 255) gray_out = 255;
                else if (gray_out < 0) gray_out = 0;

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
            else
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color_in(rgb);
                int h, s, v;
                color_in.getHsv(&h, &s, &v);
                int v_out = v * c;
                if (v_out > 255)
                {
                    v_out = 255;
                }
                else if (v_out < 0)
                {
                    v_out = 0;
                }
                QColor color_out = QColor::fromHsv(h, s, v_out);
                image_out.setPixel(x, y, color_out.rgb());
            }
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}

void MainWindow::on_btn_linearHis_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    int max_gray = 0;
    int min_gray = 255;
    int max_red, max_green, max_blue;
    int min_red, min_green, min_blue;
    max_red = max_green = max_blue = 0;
    min_red = min_green = min_blue = 255;

    if (ui->rbtn_gray->isChecked())
    {
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);

                if (gray_in < min_gray)
                {
                    min_gray = gray_in;
                }
                if (gray_in > max_gray)
                {
                    max_gray = gray_in;
                }
            }
        }

        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);

                int gray_out = (int)(((float)(gray_in - min_gray)/(max_gray - min_gray))*255);

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
    }
    else
    {
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color(rgb);
                int red = color.red();
                int green = color.green();
                int blue = color.blue();

                if (red < min_red)
                {
                    min_red = red;
                }
                if (red > max_red)
                {
                    max_red = red;
                }

                if (green < min_green)
                {
                    min_green = green;
                }
                if (green > max_green)
                {
                    max_green = green;
                }

                if (blue < min_blue)
                {
                    min_blue = blue;
                }
                if (blue > max_blue)
                {
                    max_blue = blue;
                }
                //qDebug() << "It's runnin'";
            }
        }
        qDebug() << max_red << "; " << max_green << "; " << max_blue;
        qDebug() << min_red << "; " << min_green << "; " << min_blue;
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color(rgb);
                int red = color.red();
                int green = color.green();
                int blue = color.blue();
                ///
                int red_out = (int)(((float)(red - min_red)/(max_red - min_red))*255);
                int green_out = (int)(((float)(green - min_green)/(max_green - min_green))*255);
                int blue_out = (int)(((float)(blue - min_blue)/(max_blue - min_blue))*255);

                QRgb color_out = qRgb(red_out, green_out, blue_out);
                image_out.setPixel(x, y, color_out);
            }
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}

void MainWindow::on_btn_balanceHis_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    int max_gray = 0;
    int min_gray = 255;
    int max_red, max_green, max_blue;
    int min_red, min_green, min_blue;
    max_red = max_green = max_blue = 0;
    min_red = min_green = min_blue = 255;
    float h_gray[256];
    float h_red[256], h_blue[256], h_green[256];
    float hn_gray[256];
    float hn_red[256], hn_blue[256], hn_green[256];
    float C_gray[256];
    float C_red[256], C_green[256], C_blue[256];
    int O_gray[256];
    int O_red[256], O_green[256], O_blue[256];

    if (ui->rbtn_gray->isChecked())
    {
        //### Initialize
        for (int i = 0; i < 256; i++)
        {
            h_gray[i] = 0;
            hn_gray[i] = 0;
            C_gray[i] = 0;
            O_gray[i] = 0;
        }
        //### Calculate Histogram
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);
                h_gray[gray_in]++;

                if (gray_in < min_gray)
                {
                    min_gray = gray_in;
                }
                if (gray_in > max_gray)
                {
                    max_gray = gray_in;
                }
            }
        }
        //### Normalize Histogram
        for (int i = 0; i < 256; i++)
        {
            hn_gray[i] = h_gray[i]/(image_in.width()*image_in.height());
        }
        //### Probability density function
        C_gray[0] = hn_gray[0];
        for (int i = 1; i < 256; i++)
        {
            C_gray[i] = hn_gray[i] + C_gray[i-1];
        }
        //###
        for (int i = 0; i < 256; i++)
        {
            O_gray[i] = int(round(C_gray[i]  * 255));
        }
        //### rendering
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);

                int gray_out = O_gray[gray_in];

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
    }
    else
    {
        //### Initialize
        for (int i = 0; i < 256; i++)
        {
            h_red[i] = h_blue[i] = h_green[i] = 0;
            hn_red[i] = hn_blue[i] = hn_green[i] = 0;
            C_red[i] = C_green[i] = C_blue[i] = 0;
            O_red[i] = O_green[i] = O_blue[i] = 0;
        }
        //### Calculate Histogram
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                //QRgb rgb = image_in.pixel(x, y);
                //QColor color(rgb);
                QColor color = image_in.pixel(x, y);
                int red = color.red();
                int green = color.green();
                int blue = color.blue();
                h_red[red]++;
                h_green[green]++;
                h_blue[blue]++;

                if (red < min_red)
                {
                    min_red = red;
                }
                if (red > max_red)
                {
                    max_red = red;
                }

                if (green < min_green)
                {
                    min_green = green;
                }
                if (green > max_green)
                {
                    max_green = green;
                }

                if (blue < min_blue)
                {
                    min_blue = blue;
                }
                if (blue > max_blue)
                {
                    max_blue = blue;
                }
                //qDebug() << "It's runnin'";
            }
        }
        qDebug() << max_red << "; " << max_green << "; " << max_blue;
        qDebug() << min_red << "; " << min_green << "; " << min_blue;
        //### Normalize Histogram
        for (int i = 0; i < 256; i++)
        {
            hn_red[i] = h_red[i]/(image_in.width()*image_in.height());
            hn_green[i] = h_green[i]/(image_in.width()*image_in.height());
            hn_blue[i] = h_blue[i]/(image_in.width()*image_in.height());
        }
        //### Probability density function
        C_red[0] = hn_red[0];
        C_green[0] = hn_green[0];
        C_blue[0] = hn_blue[0];
        for (int i = 1; i < 256; i++)
        {
            C_red[i] = hn_red[i] + C_red[i-1];
            C_green[i] = hn_green[i] + C_green[i-1];
            C_blue[i] = hn_blue[i] + C_blue[i-1];
        }
        //###
        for (int i = 0; i < 256; i++)
        {
            O_red[i] = int(round(C_red[i]  * 255));
            O_green[i] = int(round(C_green[i]  * 255));
            O_blue[i] = int(round(C_blue[i]  * 255));
        }
        //### rendering
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color(rgb);
                int red = color.red();
                int green = color.green();
                int blue = color.blue();
                ///
                int red_out = O_red[red];
                int green_out = O_green[green];
                int blue_out = O_blue[blue];

                QRgb color_out = qRgb(red_out, green_out, blue_out);
                image_out.setPixel(x, y, color_out);
            }
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}


void MainWindow::on_btn_mean_filter_clicked()
{
    QImage image_in = this->original_image;
    int size = ui->spb_mask_size->value();
    int margin = int((size - 1)/2);
    int cells = size * size;
    QImage image_out(image_in.width() - margin, image_in.height() - margin, QImage::Format_RGB32);
    int sumR, sumG, sumB, sum;
    QRgb rgb;

    if (ui->rbtn_gray->isChecked())
    {
        for (int x = margin; x < image_in.width() - margin; x++)
        {
            for (int y = margin; y < image_in.height() - margin; y++)
            {
                sum = 0;
                for (int i = -margin; i <= margin; i++)
                {
                    for (int j = -margin; j <= margin; j++)
                    {
                        rgb = image_in.pixel(x+1, y+1);
                        sum += qGray(rgb);
                    }
                }
                image_out.setPixel(x, y, qRgb(sum/cells, sum/cells, sum/cells));
            }
        }
    }
    else
    {
        for (int x = margin; x < image_in.width() - margin; x++)
        {
            for (int y = margin; y < image_in.height() - margin; y++)
            {
                sumR = sumG = sumB = 0;
                for (int i = -margin; i <= margin; i++)
                {
                    for (int j = -margin; j <= margin; j++)
                    {
                        rgb = image_in.pixel(x+1, y+1);
                        QColor color(rgb);
                        sumR += color.red();
                        sumG += color.green();
                        sumB += color.blue();
                    }
                }
                image_out.setPixel(x, y, qRgb(sumR/cells, sumG/cells, sumB/cells));
            }
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}

void MainWindow::on_btn_median_filter_clicked()
{
    QImage image_in = this->original_image;
    int size = ui->spb_mask_size->value();
    int margin = int((size - 1)/2);
    int cells = size * size;
    QImage image_out(image_in.width() - margin, image_in.height() - margin, QImage::Format_RGB32);

    if (ui->rbtn_gray->isChecked())
    {
        for (int x = margin; x < image_in.width() - margin; x++)
        {
            for (int y = margin; y < image_in.height() - margin; y++)
            {
                int h[cells];
                int k = 0;
                for (int i = -margin; i <= margin; i++)
                {
                    for (int j = -margin; j <= margin; j++)
                    {
                        QRgb color = image_in.pixel(x + i, y + j);
                        int gray = qGray(color);
                        h[k] = gray;
                        k++;
                    }
                }
                std::sort(h + 0, h + cells);
                int meanV = h[int((cells - 1) / 2)];
                image_out.setPixel(x, y, qRgb(meanV, meanV, meanV));
            }
        }
    }
    else
    {
        for (int x = margin; x < image_in.width() - margin; x++)
        {
            for (int y = margin; y < image_in.height() - margin; y++)
            {
                int h_r[cells];
                int h_g[cells];
                int h_b[cells];
                int k = 0;
                for (int i = -margin; i <= margin; i++)
                {
                    for (int j = -margin; j <= margin; j++)
                    {
                        QRgb rgb = image_in.pixel(x + i, y + j);
                        QColor color(rgb);
                        int red = color.red();
                        int green = color.green();
                        int blue = color.blue();
                        h_r[k] = red;
                        h_g[k] = green;
                        h_b[k] = blue;
                        k++;
                    }
                }
                std::sort(h_r + 0, h_r + cells);
                std::sort(h_g + 0, h_g + cells);
                std::sort(h_b + 0, h_b + cells);
                int meanR = h_r[int((cells - 1) / 2)];
                int meanG = h_g[int((cells - 1) / 2)];
                int meanB = h_b[int((cells - 1) / 2)];
                image_out.setPixel(x, y, qRgb(meanR, meanG, meanB));
            }
        }
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}

void MainWindow::on_btn_add_noise_clicked()
{
    QImage image_in = this->original_image;
    float percentage = (ui->spb_noise_ratio->value())/100;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_RGB32);

    if (ui->rbtn_gray->isChecked())
    {
        image_out = this->modified_image;
        ///
        int noise_point = image_out.width() * image_in.height() * percentage;

        for (int i = 0; i < noise_point; i++)
        {
            int x = rand() % image_in.width();
            int y = rand() % image_in.height();
            int a = rand() % 2;
            if (a == 0)
            {
                image_out.setPixel(x, y, qRgb(0, 0, 0));
            }
            else
            {
                image_out.setPixel(x, y, qRgb(255, 255, 255));
            }
        }
        ///
    }
    else
    {
        image_out = this->original_image;
        ///
        int noise_point = image_out.width() * image_in.height() * percentage;

        for (int i = 0; i < noise_point; i++)
        {
            int x = rand() % image_in.width();
            int y = rand() % image_in.height();
            int a = rand() % 2;
            if (a == 0)
            {
                image_out.setPixel(x, y, qRgb(0, 0, 0));
            }
            else
            {
                image_out.setPixel(x, y, qRgb(255, 255, 255));
            }
        }
        ///
    }
    this->modified_image = image_out;
    DisplayImage(image_out, MODE::MODIFIED);
}


void MainWindow::on_btn_gradient_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out_Gx(image_in.width() - 1, image_in.height(), QImage::Format_RGB32);
    QImage image_out_Gy(image_in.width(), image_in.height() - 1, QImage::Format_RGB32);

    if (ui->rbtn_gray->isChecked())
    {
        for (int i = 0; i < image_in.width() - 1; i++)
        {
            for (int j = 0; j < image_in.height(); j++)
            {
                QRgb color1 = image_in.pixel(i, j);
                int gray1 = qGray(color1);

                QRgb color_in = qRgb(gray1, gray1, gray1);
                image_in.setPixel(i, j, color_in);

                QRgb color2 = image_in.pixel(i + 1, j);
                int gray2 = qGray(color2);
                int newgray = abs(gray2 - gray1);

                image_out_Gx.setPixel(i, j, qRgb(newgray, newgray, newgray));
            }
        }
        for (int i = 0; i < image_in.width(); i++)
        {
            for (int j = 0; j < image_in.height() - 1; j++)
            {
                QRgb color1 = image_in.pixel(i, j);
                int gray1 = qGray(color1);

                QRgb color_in = qRgb(gray1, gray1, gray1);
                image_in.setPixel(i, j, color_in);

                QRgb color2 = image_in.pixel(i, j + 1);
                int gray2 = qGray(color2);
                int newgray = abs(gray2 - gray1);

                image_out_Gy.setPixel(i, j, qRgb(newgray, newgray, newgray));
            }
        }
        ///
    }
    else
    {
        ///
    }
    this->gx_image = image_out_Gx;
    DisplayImage(image_out_Gx, MODE::GX);
    this->gy_image = image_out_Gy;
    DisplayImage(image_out_Gx, MODE::GY);
}


void MainWindow::on_btn_robert_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out_Gx(image_in.width() - 1, image_in.height() - 1, QImage::Format_RGB32);
    QImage image_out_Gy(image_in.width() - 1, image_in.height() - 1, QImage::Format_RGB32);

    if (ui->rbtn_gray->isChecked())
    {
        for (int i = 0; i < image_in.width() - 1; i++)
        {
            for (int j = 0; j < image_in.height() - 1; j++)
            {
                /*
                |
                |
                */
                QRgb color1 = image_in.pixel(i, j);
                int gray1 = qGray(color1);
                QRgb color2 = image_in.pixel(i + 1, j);
                int gray2 = qGray(color2);
                QRgb color3 = image_in.pixel(i, j + 1);
                int gray3 = qGray(color3);
                QRgb color4 = image_in.pixel(i + 1, j + 1);
                int gray4 = qGray(color4);

                int newgray_gx = abs(gray1 - gray4);
                int newgray_gy = abs(gray2 - gray3);

                image_out_Gx.setPixel(i, j, qRgb(newgray_gx, newgray_gx, newgray_gx));
                image_out_Gy.setPixel(i, j, qRgb(newgray_gy, newgray_gy, newgray_gy));
            }
        }
        ///
    }
    else
    {
        ///
    }
    this->gx_image = image_out_Gx;
    DisplayImage(image_out_Gx, MODE::GX);
    this->gy_image = image_out_Gy;
    DisplayImage(image_out_Gx, MODE::GY);
}


void MainWindow::on_btn_sobel_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out_Gx(image_in.width(), image_in.height(), QImage::Format_RGB32);
    QImage image_out_Gy(image_in.width(), image_in.height(), QImage::Format_RGB32);

    int maskSize = 3;
    int margin = maskSize / 2;

    int mask[maskSize][maskSize];
    mask[0][0] = -1; mask[0][1] = 0; mask[0][2] = 1;
    mask[1][0] = -2; mask[1][1] = 0; mask[1][2] = 2;
    mask[2][0] = -1; mask[2][1] = 0; mask[2][2] = 1;
    int mask2[maskSize][maskSize];
    mask2[0][0] = -1; mask2[0][1] = -2; mask2[0][2] = -1;
    mask2[1][0] = 0; mask2[1][1] = 0; mask2[1][2] = 0;
    mask2[2][0] = 1; mask2[2][1] = 2; mask2[2][2] = 1;
    QRgb rgb;
    int sum1, sum2;

    if (ui->rbtn_gray->isChecked())
    {
        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum1 = sum2 = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        rgb = image_in.pixel(x + i, y + j);
                        int gray = qGray(rgb);
                        sum1 += gray * mask[j + margin][i + margin];
                        sum2 += gray * mask2[j + margin][i + margin];
                    }
                }
                image_out_Gx.setPixel(x, y, qRgb(abs(sum1), abs(sum1), abs(sum1)));
                image_out_Gy.setPixel(x, y, qRgb(abs(sum2), abs(sum2), abs(sum2)));
            }
        }
        ///
    }
    else
    {
        ///
    }
    this->gx_image = image_out_Gx;
    DisplayImage(image_out_Gx, MODE::GX);
    this->gy_image = image_out_Gy;
    DisplayImage(image_out_Gx, MODE::GY);
}


void MainWindow::on_btn_prewitt_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out_Gx(image_in.width(), image_in.height(), QImage::Format_RGB32);
    QImage image_out_Gy(image_in.width(), image_in.height(), QImage::Format_RGB32);

    int maskSize = 3;
    int margin = maskSize / 2;

    int mask[maskSize][maskSize];
    mask[0][0] = -1; mask[0][1] = 0; mask[0][2] = 1;
    mask[1][0] = -1; mask[1][1] = 0; mask[1][2] = 1;
    mask[2][0] = -1; mask[2][1] = 0; mask[2][2] = 1;
    int mask2[maskSize][maskSize];
    mask2[0][0] = -1; mask2[0][1] = -1; mask2[0][2] = -1;
    mask2[1][0] = 0; mask2[1][1] = 0; mask2[1][2] = 0;
    mask2[2][0] = 1; mask2[2][1] = 1; mask2[2][2] = 1;
    QRgb rgb;
    int sum1, sum2;

    if (ui->rbtn_gray->isChecked())
    {
        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum1 = sum2 = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        rgb = image_in.pixel(x + i, y + j);
                        int gray = qGray(rgb);
                        sum1 += gray * mask[j + margin][i + margin];
                        sum2 += gray * mask2[j + margin][i + margin];
                    }
                }
                image_out_Gx.setPixel(x, y, qRgb(abs(sum1), abs(sum1), abs(sum1)));
                image_out_Gy.setPixel(x, y, qRgb(abs(sum2), abs(sum2), abs(sum2)));
            }
        }
        ///
    }
    else
    {
        ///
    }
    this->gx_image = image_out_Gx;
    DisplayImage(image_out_Gx, MODE::GX);
    this->gy_image = image_out_Gy;
    DisplayImage(image_out_Gx, MODE::GY);
}


void MainWindow::on_btn_laplace_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out_Gx(image_in.width(), image_in.height(), QImage::Format_RGB32);
    QImage image_out_Gy(image_in.width(), image_in.height(), QImage::Format_RGB32);

    int maskSize = 3;
    int margin = (int)maskSize / 2;

    int mask[maskSize][maskSize];
    mask[0][0] = 0; mask[0][1] = 1; mask[0][2] = 0;
    mask[1][0] = 1; mask[1][1] = -4; mask[1][2] = 1;
    mask[2][0] = 0; mask[2][1] = 1; mask[2][2] = 0;
    int mask2[maskSize][maskSize];
    mask2[0][0] = 1; mask2[0][1] = 1; mask2[0][2] = 1;
    mask2[1][0] = 1; mask2[1][1] = -8; mask2[1][2] = 1;
    mask2[2][0] = 1; mask2[2][1] = 1; mask2[2][2] = 1;
    QRgb rgb;
    int sum1, sum2;

    if (ui->rbtn_gray->isChecked())
    {
        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum1 = sum2 = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        rgb = image_in.pixel(x + i, y + j);
                        int gray = qGray(rgb);
                        sum1 += gray * mask[j + margin][i + margin];
                        sum2 += gray * mask2[j + margin][i + margin];
                    }
                }
                image_out_Gx.setPixel(x, y, qRgb(abs(sum1), abs(sum1), abs(sum1)));
                image_out_Gy.setPixel(x, y, qRgb(abs(sum2), abs(sum2), abs(sum2)));
            }
        }
        ///
    }
    else
    {
        ///
    }
    this->gx_image = image_out_Gx;
    DisplayImage(image_out_Gx, MODE::GX);
    this->gy_image = image_out_Gy;
    DisplayImage(image_out_Gx, MODE::GY);
}


void MainWindow::on_btn_gauss_n_laplace_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_RGB32);

    int maskSize = 5;
    int margin = (int)maskSize / 2;

    int mask[maskSize][maskSize];
    mask[0][0] = 0; mask[0][1] = 0; mask[0][2] = -1; mask[0][3] = 0; mask[0][4] = 0;
    mask[1][0] = 0; mask[1][1] = -1; mask[1][2] = -2; mask[1][3] = -1; mask[1][4] = 0;
    mask[2][0] = -1; mask[2][1] = -2; mask[2][2] = 16; mask[2][3] = -2; mask[2][4] = -1;
    mask[3][0] = 0; mask[3][1] = -1; mask[3][2] = -2; mask[3][3] = -1; mask[3][4] = 0;
    mask[4][0] = 0; mask[4][1] = 0; mask[4][2] = -1; mask[4][3] = 0; mask[4][4] = 0;
    QRgb rgb;
    int sum;

    if (ui->rbtn_gray->isChecked())
    {
        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        rgb = image_in.pixel(x + i, y + j);
                        int gray = qGray(rgb);
                        sum += gray * mask[j + margin][i + margin];
                    }
                }
                image_out.setPixel(x, y, qRgb(abs(sum), abs(sum), abs(sum)));
            }
        }
        ///
    }
    else
    {
        ///
    }
    this->gx_image = image_out;
    DisplayImage(image_out, MODE::GX);
}

