#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QPixmap"
#include "QLabel"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spb_brightness->setMaximum(255);
    ui->spb_brightness->setMinimum(0);

    ui->spb_contrast->setMaximum(100);
    ui->spb_contrast->setMinimum(0);

    ui->spb_threshold->setMaximum(255);
    ui->spb_threshold->setMinimum(0);

    ui->spb_threshold->setValue(ui->spb_threshold->maximum()/2);

    ui->spb_mask_size->setMinimum(3);
    ui->spb_mask_size->setSingleStep(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_browse_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File",
                                                    "D:/",
                                                    "*.* All Files;;*.jpg");
    if (filename.isEmpty())
        return;
    ui->lne_browse->setText(filename);

    this->original_image = QImage(filename);

    makeGrayImage();

    DisplayImage(this->original_image, QFileInfo(filename).fileName());
}

void MainWindow::DisplayImage(QImage &img, QString title)
{
    QLabel *label = new QLabel();

    QPixmap pix = QPixmap::fromImage(img);
//    label->setPixmap(QPixmap::fromImage(img));
    label->setPixmap(
                pix.scaled(label->width(),
                           label->height(),
                           Qt::KeepAspectRatio)
                );
    label->setWindowTitle(title);
    label->show();
}

void MainWindow::makeGrayImage()
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
    this->gray_image = image_out;
    DisplayImage(this->gray_image);
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Session 1 //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_btn_brightness_clicked()
{
    int c = ui->spb_brightness->value();

    if (ui->rbtn_gray->isChecked())
    {
        QImage image_in = this->gray_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);

                int gray_out = gray_in + c;

                if (gray_out > 255) gray_out = 255;
                else if (gray_out < 0) gray_out = 0;

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
    else
    {
        QImage image_in = this->original_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color_in(rgb);
                int h, s, v;
                color_in.getHsv(&h, &s, &v);

                int v_out = v + c;

                if (v_out > 255) v_out = 255;
                else if (v_out < 0) v_out = 0;

                QColor color_out = QColor::fromHsv(h, s, v_out);
                image_out.setPixel(x, y, color_out.rgb());
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
}

void MainWindow::on_btn_negative_clicked()
{
    if (ui->rbtn_gray->isChecked())
    {
        QImage image_in = this->gray_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);

                int gray_out = 255 - gray_in;

                if (gray_out > 255) gray_out = 255;
                else if (gray_out < 0) gray_out = 0;

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
    else
    {
        QImage image_in = this->original_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color_in(rgb);
                int r, g, b;
                r = 255 - color_in.red();
                g = 255 - color_in.green();
                b = 255 - color_in.blue();

                QRgb color_out = qRgb(r, g, b);
                image_out.setPixel(x, y, color_out);
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
}

void MainWindow::on_btn_threshold_clicked()
{
    int c = ui->spb_threshold->value();
    thresholding(c);
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Session 2 //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void MainWindow::Show_Histogram(QImage &image_in, const int HEIGHT)
{
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
        QImage histogram(256, HEIGHT, QImage::Format_RGB888);
        histogram.fill(Qt::white);
        for (int x = 0; x < 256; x++)
        {
            lineHeight = HEIGHT*h[x]/max;
            for (int y = HEIGHT - 1; y >= HEIGHT - 1 - lineHeight; y--)
            {
                histogram.setPixel(x, y, qRgb(0, 0, 255));
            }
        }
        DisplayImage(histogram);
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
        DisplayImage(histogram);
    }
}

void MainWindow::on_btn_showHis_clicked()
{
    if (ui->rbtn_gray->isChecked())
    {
        QImage image_in = this->gray_image;

        Show_Histogram(image_in);
        DisplayImage(image_in);
    }
    else
    {
        QImage image_in = this->original_image;

        Show_Histogram(image_in);
        DisplayImage(image_in);
    }
}

void MainWindow::on_btn_contrast_clicked()
{
    float c = ui->spb_contrast->value();

    if (ui->rbtn_gray->isChecked())
    {
        QImage image_in = this->gray_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);

                int gray_out = gray_in * c;

                if (gray_out > 255) gray_out = 255;
                else if (gray_out < 0) gray_out = 0;

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
    else
    {
        QImage image_in = this->original_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color_in(rgb);
                int h, s, v;
                color_in.getHsv(&h, &s, &v);

                int v_out = v * c;

                if (v_out > 255) v_out = 255;
                else if (v_out < 0) v_out = 0;

                QColor color_out = QColor::fromHsv(h, s, v_out);
                image_out.setPixel(x, y, color_out.rgb());
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
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
    DisplayImage(image_in);
    Show_Histogram(image_in);
    DisplayImage(image_out);
    Show_Histogram(image_out);
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
    DisplayImage(image_in);
    Show_Histogram(image_in);
    DisplayImage(image_out);
    Show_Histogram(image_out);
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Session 3 //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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
    DisplayImage(image_in);
    DisplayImage(image_out);
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
    DisplayImage(image_in);
    DisplayImage(image_out);
}

void MainWindow::on_btn_add_noise_clicked()
{
    QImage image_in = this->original_image;
    float percentage = (ui->spb_noise_ratio->value())/100;
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_RGB32);

    if (ui->rbtn_gray->isChecked())
    {
        image_out = this->gray_image;
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
    DisplayImage(image_in);
    DisplayImage(image_out);

    QString fname = QFileDialog::getSaveFileName();
    image_out.save(fname);
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Session 4 //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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
        for (int i = 0; i < image_in.width() - 1; i++)
        {
            for (int j = 0; j < image_in.height(); j++)
            {
                QRgb color1 = image_in.pixel(i, j);
                QColor rgb1(color1);
                int r1, g1, b1;
                rgb1.getRgb(&r1, &g1, &b1);

                QRgb color2 = image_in.pixel(i + 1, j);
                QColor rgb2(color2);
                int r2, g2, b2;
                rgb2.getRgb(&r2, &g2, &b2);

                int r3, g3, b3;
                r3 = abs(r2 - r1);
                g3 = abs(g2 - g1);
                b3 = abs(b2 - b1);

                QRgb color_out = qRgb(r3, g3, b3);
                image_out_Gx.setPixel(i, j, color_out);

//                QRgb color1 = image_in.pixel(i, j);
//                QColor rgb1(color1);
//                int h1, s1, v1;
//                rgb1.getHsv(&h1, &s1, &v1);

//                QRgb color2 = image_in.pixel(i + 1, j);
//                QColor rgb2(color2);
//                int h2, s2, v2;
//                rgb2.getHsv(&h2, &s2, &v2);

//                int h3, s3, v3;
//                h3 = h1;
//                s3 = s1;
//                v3 = abs(v2 - v1);

//                QColor rgb3 = QColor::fromHsv(h3, s3, v3);
//                QRgb color_out = rgb3.rgb();
//                image_out_Gx.setPixel(i, j, color_out);

            }
        }
        for (int i = 0; i < image_in.width(); i++)
        {
            for (int j = 0; j < image_in.height() - 1; j++)
            {
                QRgb color1 = image_in.pixel(i, j);
                QColor rgb1(color1);
                int r1, g1, b1;
                rgb1.getRgb(&r1, &g1, &b1);

                QRgb color2 = image_in.pixel(i, j + 1);
                QColor rgb2(color2);
                int r2, g2, b2;
                rgb2.getRgb(&r2, &g2, &b2);

                int r3, g3, b3;
                r3 = abs(r2 - r1);
                g3 = abs(g2 - g1);
                b3 = abs(b2 - b1);

                QRgb color_out = qRgb(r3, g3, b3);
                image_out_Gy.setPixel(i, j, color_out);

//                QRgb color1 = image_in.pixel(i, j);
//                QColor rgb1(color1);
//                int h1, s1, v1;
//                rgb1.getHsv(&h1, &s1, &v1);

//                QRgb color2 = image_in.pixel(i, j + 1);
//                QColor rgb2(color2);
//                int h2, s2, v2;
//                rgb2.getHsv(&h2, &s2, &v2);

//                int h3, s3, v3;
//                h3 = h1;
//                s3 = s1;
//                v3 = abs(v2 - v1);

//                QColor rgb3 = QColor::fromHsv(h3, s3, v3);
//                QRgb color_out = rgb3.rgb();
//                image_out_Gy.setPixel(i, j, color_out);
            }
        }
    }
    DisplayImage(image_in);
    DisplayImage(image_out_Gx);
    DisplayImage(image_out_Gy);
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
    }
    else
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
                QRgb color2 = image_in.pixel(i + 1, j);
                QRgb color3 = image_in.pixel(i, j + 1);
                QRgb color4 = image_in.pixel(i + 1, j + 1);

                QColor rgb1(color1);
                QColor rgb2(color2);
                QColor rgb3(color3);
                QColor rgb4(color4);

                int new_red_gx = abs(rgb1.red() - rgb4.red());
                int new_red_gy = abs(rgb2.red() - rgb3.red());
                int new_green_gx = abs(rgb1.green() - rgb4.green());
                int new_green_gy = abs(rgb2.green() - rgb3.green());
                int new_blue_gx = abs(rgb1.blue() - rgb4.blue());
                int new_blue_gy = abs(rgb2.blue() - rgb3.blue());

                image_out_Gx.setPixel(i, j, qRgb(new_red_gx, new_green_gx, new_blue_gx));
                image_out_Gy.setPixel(i, j, qRgb(new_red_gy, new_green_gy, new_blue_gy));
            }
        }
    }
    DisplayImage(image_in);
    DisplayImage(image_out_Gx);
    DisplayImage(image_out_Gy);
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

    if (ui->rbtn_gray->isChecked())
    {
        int sum1, sum2;
        QRgb rgb;
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
        int sum_r_1, sum_g_1, sum_b_1;
        int sum_r_2, sum_g_2, sum_b_2;

        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum_r_1 = sum_g_1 = sum_b_1 = 0;
                sum_r_2 = sum_g_2 = sum_b_2 = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        QRgb rgb = image_in.pixel(x + i, y + j);
                        QColor color(rgb);

                        sum_r_1 += color.red() * mask[j + margin][i + margin];
                        sum_g_1 += color.green() * mask[j + margin][i + margin];
                        sum_b_1 += color.blue() * mask[j + margin][i + margin];

                        sum_r_2 += color.red() * mask2[j + margin][i + margin];
                        sum_g_2 += color.green() * mask2[j + margin][i + margin];
                        sum_b_2 += color.blue() * mask2[j + margin][i + margin];

//                        int gray = qGray(rgb);
//                        sum1 += gray * mask[j + margin][i + margin];
//                        sum2 += gray * mask2[j + margin][i + margin];


                    }
                }
                image_out_Gx.setPixel(x, y, qRgb(abs(sum_r_1), abs(sum_g_1), abs(sum_b_1)));
                image_out_Gy.setPixel(x, y, qRgb(abs(sum_r_2), abs(sum_g_2), abs(sum_b_2)));
            }
        }
    }
    DisplayImage(image_in);
    DisplayImage(image_out_Gx);
    DisplayImage(image_out_Gy);
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

    if (ui->rbtn_gray->isChecked())
    {
        QRgb rgb;
        int sum1, sum2;
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
        int sum_r_1, sum_g_1, sum_b_1;
        int sum_r_2, sum_g_2, sum_b_2;

        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum_r_1 = sum_g_1 = sum_b_1 = 0;
                sum_r_2 = sum_g_2 = sum_b_2 = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        QRgb rgb = image_in.pixel(x + i, y + j);
                        QColor color(rgb);

                        sum_r_1 += color.red() * mask[j + margin][i + margin];
                        sum_g_1 += color.green() * mask[j + margin][i + margin];
                        sum_b_1 += color.blue() * mask[j + margin][i + margin];

                        sum_r_2 += color.red() * mask2[j + margin][i + margin];
                        sum_g_2 += color.green() * mask2[j + margin][i + margin];
                        sum_b_2 += color.blue() * mask2[j + margin][i + margin];

                    }
                }
                image_out_Gx.setPixel(x, y, qRgb(abs(sum_r_1), abs(sum_g_1), abs(sum_b_1)));
                image_out_Gy.setPixel(x, y, qRgb(abs(sum_r_2), abs(sum_g_2), abs(sum_b_2)));
            }
        }
    }
    DisplayImage(image_in);
    DisplayImage(image_out_Gx);
    DisplayImage(image_out_Gy);
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

    if (ui->rbtn_gray->isChecked())
    {
        QRgb rgb;
        int sum1, sum2;
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
        int sum_r_1, sum_g_1, sum_b_1;
        int sum_r_2, sum_g_2, sum_b_2;

        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum_r_1 = sum_g_1 = sum_b_1 = 0;
                sum_r_2 = sum_g_2 = sum_b_2 = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        QRgb rgb = image_in.pixel(x + i, y + j);
                        QColor color(rgb);

                        sum_r_1 += color.red() * mask[j + margin][i + margin];
                        sum_g_1 += color.green() * mask[j + margin][i + margin];
                        sum_b_1 += color.blue() * mask[j + margin][i + margin];

                        sum_r_2 += color.red() * mask2[j + margin][i + margin];
                        sum_g_2 += color.green() * mask2[j + margin][i + margin];
                        sum_b_2 += color.blue() * mask2[j + margin][i + margin];

                    }
                }
                image_out_Gx.setPixel(x, y, qRgb(abs(sum_r_1), abs(sum_g_1), abs(sum_b_1)));
                image_out_Gy.setPixel(x, y, qRgb(abs(sum_r_2), abs(sum_g_2), abs(sum_b_2)));
            }
        }
    }
    DisplayImage(image_in);
    DisplayImage(image_out_Gx);
    DisplayImage(image_out_Gy);
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

    if (ui->rbtn_gray->isChecked())
    {
        QRgb rgb;
        int sum;
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
        int sum_r, sum_g, sum_b;

        for (int y = margin; y < image_in.height() - margin; y++)
        {
            for (int x = margin; x < image_in.width() - margin; x++)
            {
                sum_r = sum_g = sum_b = 0;
                sum_r = sum_g = sum_b = 0;
                for (int j = -margin; j <= margin; j++)
                {
                    for (int i = -margin; i <= margin; i++)
                    {
                        QRgb rgb = image_in.pixel(x + i, y + j);
                        QColor color(rgb);

                        sum_r += color.red() * mask[j + margin][i + margin];
                        sum_g += color.green() * mask[j + margin][i + margin];
                        sum_b += color.blue() * mask[j + margin][i + margin];

                    }
                }
                image_out.setPixel(x, y, qRgb(abs(sum_r), abs(sum_g), abs(sum_b)));
            }
        }
    }
    DisplayImage(image_in);
    DisplayImage(image_out);
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Session 5 //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void MainWindow::thresholding(int c)
{
    if (ui->rbtn_gray->isChecked())
    {
        QImage image_in = this->gray_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);

                int gray_out = gray_in;

                if (gray_out > c) gray_out = 255;
                else if (gray_out < c) gray_out = 0;

                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
    else
    {
        QImage image_in = this->original_image;
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        for (int x = 0; x < image_in.width(); x++)
        {
            for (int y = 0; y < image_in.height(); y++)
            {
                QRgb rgb = image_in.pixel(x, y);
                QColor color_in(rgb);
                int h, s, v;
                color_in.getHsv(&h, &s, &v);

                int v_out = v;

                if (v_out > c) v_out = 255;
                else if (v_out < c) v_out = 0;

                QColor color_out = QColor::fromHsv(h, s, v_out);
                image_out.setPixel(x, y, color_out.rgb());
            }
        }
        DisplayImage(image_in);
        DisplayImage(image_out);
    }
}

void MainWindow::on_btn_sequence_threshold_clicked()
{
    QImage image_in = this->original_image;

    int c = 127;
    int newC = c;

    do {
        int G1 = 0;
        int count1 = 0;
        int G2 = 0;
        int count2 = 0;
        c = newC;
        for (int i = 0; i < image_in.width(); i++)
        {
            for (int j = 0; j < image_in.height(); j++)
            {
                QRgb rgb = image_in.pixel(i, j);
                int gray = qGray(rgb);
                if (gray > c)
                {
                    G1 += gray;
                    count1++;
                }
                else
                {
                    G2 += gray;
                    count2++;
                }
            }
        }
        int M1 = G1/count1;
        int M2 = G2/count2;
        newC = (M1 + M2)/2;
    } while (abs(newC - c) != 0);

    qDebug() << newC;
    thresholding(newC);
}

void MainWindow::on_btn_otsu_threshold_clicked()
{
    QImage image_in = this->original_image;

    double maxDelta = -1000000000000;
    long maxC = 1;
//    double minDelta = 1000000000000;
//    long minC = 1;

    long histogram[256];
    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }

    for (int i = 0; i < image_in.width(); i++)
    {
        for (int j = 0; j < image_in.height(); j++)
        {
            QRgb rgb = image_in.pixel(i, j);
            int gray = qGray(rgb);
            histogram[gray]++;
        }
    }

    for (long c = 1; c < 255; c++)
    {
        long G1 = 0;
        long count1 = 0;
        long G2 = 0;
        long count2 = 0;

        for (int i = 0; i < 256; i++)
        {
            if (i <= c)
            {
                G1 += histogram[i]*i;
                count1 += histogram[i];
            }
            else
            {
                G2 += histogram[i]*i;
                count2 += histogram[i];
            }
        }
        double M1 = G1/count1;
        double M2 = G2/count2;

        double delta1 = 0;
        double delta2 = 0;
        for (int i = 0; i < 256; i++)
        {
            if (i <= c)
            {
                delta1 += pow(histogram[i]*i - M1, 2);
            }
            else
            {
                delta2 += pow(histogram[i]*i - M2, 2);
            }
        }
        double big_delta = delta1/(double)(count1 - 1) + delta2/(double)(count2 - 1);
//        qDebug() << "{delta1}" << delta1 << "{count1 - 1}" << (count1 - 1)  << "{BUG}: " << maxDelta - big_delta;
        if (big_delta > maxDelta)
        {
//            qDebug() << "maxDelta" << maxDelta << ": " << c;
            maxDelta = big_delta;
//            qDebug() << "big_delta" << big_delta;
            maxC = c;
        }
//        if (big_delta < minDelta)
//        {
//            qDebug() << "minDelta" << minDelta << ": " << c;
//            minDelta = big_delta;
//            qDebug() << "big_delta" << big_delta;
//            minC = c;
//        }
    }
    qDebug() << "maxC:" << maxC;
//    qDebug() << "minC:" << minC;
    thresholding(maxC);
}

QImage MainWindow::erosion(QImage image_in)
{
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    int kernel[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};
    int margin = 1;
    int c_gray;
    image_out.fill(Qt::white);

    for (int x = margin; x < image_in.width() - margin; x++)
    {
        for (int y = margin; y < image_in.height() - margin; y++)
        {
            int OK = 1;

            for (int i = -margin; i <= margin; i++)
            {
                for (int j = -margin; j <= margin; j++)
                {
                    c_gray = qGray(image_in.pixel(x+i, y+j));

                    OK = OK && (kernel[i][j] == 0 || c_gray <= 255/2);
                }
            }
            if (OK) image_out.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    return image_out;
}

QImage MainWindow::dilation(QImage image_in)
{
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
    int kernel[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};
    int margin = 1;
    int c_gray;
    image_out.fill(Qt::white);

    for (int x = margin; x < image_in.width() - margin; x++)
    {
        for (int y = margin; y < image_in.height() - margin; y++)
        {
            c_gray = qGray(image_in.pixel(x,y));

            if (c_gray <= 255/2)
            {
                for (int i = -margin; i <= margin; i++)
                {
                    for (int j = -margin; j <= margin; j++)
                    {
                        if (kernel[i + margin][j + margin])
                            image_out.setPixel(x + i, y + j, qRgb(0, 0, 0));
                    }
                }
            }
        }
    }

    return image_out;
}

void MainWindow::on_btn_erosion_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out = erosion(image_in);
    DisplayImage(image_in);
    DisplayImage(image_out);
}

void MainWindow::on_btn_dilation_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out = dilation(image_in);
    DisplayImage(image_in);
    DisplayImage(image_out);
}

void MainWindow::on_btn_open_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out = erosion(image_in);
    QImage image_out2 = dilation(image_out);
    DisplayImage(image_in);
    DisplayImage(image_out);
    DisplayImage(image_out2);
}

void MainWindow::on_btn_close_clicked()
{
    QImage image_in = this->original_image;
    QImage image_out = dilation(image_in);
    QImage image_out2 = erosion(image_out);
    DisplayImage(image_in);
    DisplayImage(image_out);
    DisplayImage(image_out2);
}
