#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/imgproc.hpp>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include <QTimer>


using namespace cv;
cv::VideoCapture camara(1);
Mat imagen;


QImage MatToQImage(const cv::Mat& src) {
      QImage dest(src.cols, src.rows, QImage::Format_ARGB32);

  const float scale = 255.0;

  if (src.depth() == CV_8U) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = src.at<quint8>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3b bgr = src.at<cv::Vec3b>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  } else if (src.depth() == CV_32F) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = scale * src.at<float>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3f bgr = scale * src.at<cv::Vec3f>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  }

  return dest;
}

void MainWindow::videoT(){


    camara >> imagen;
    QImage qimg = MatToQImage(imagen);

    // Convertir QImage a QPixmap
    QPixmap pixmap = QPixmap::fromImage(qimg);
    ui->label->setPixmap(pixmap);
}

// Función para convertir cv::Mat a QImage

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camara.set(CAP_PROP_FRAME_WIDTH,640);
    camara.set(CAP_PROP_FRAME_HEIGHT,480);

    QTimer *cronos = new QTimer(this);
    connect(cronos, SIGNAL(timeout()), this, SLOT(videoT()));
    cronos->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

