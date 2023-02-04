#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Считываем исходное изображение в массив Байт
    QFile file("C:\\Qt\\Project\\GRI_Lab2\\PERTSEV.bmp");
    file.open(QIODevice::ReadOnly);
    arr = file.readAll();
    file.close();
    // Получаем данные о картинки из определенных Байт заголовка
    inf_size = sum_Byts(2, 5);
    inf_wight = sum_Byts(18, 21);
    inf_height = sum_Byts(22, 25);
    inf_depth = sum_Byts(28, 29);
    inf_num_col = sum_Byts(46, 49);
    inf_point_arr = sum_Byts(10, 13);
    // Выводим данные на форму
    QImage img("C:\\Qt\\Project\\GRI_Lab2\\PERTSEV.bmp");
    set_image("C:\\Qt\\Project\\GRI_Lab2\\PERTSEV.bmp");
    QString str;
    str = "Размер файла: " + QString::number(inf_size) + " Байт"
            + "\nРазмер изображения: " + QString::number(inf_wight) + "x"
            + QString::number(inf_height)
            + "\nГлубина изображения: " + QString::number(inf_depth) + " бит"
            + "\nФормат изображения: " + format_image(img.format())
            + "\nКол-во цветов в палитре: ";
    if(inf_num_col == 0){
        str += "Палитра отсутствует";
    } else{
        str += QString::number(inf_num_col);
    }

    ui->label_2->setText(str);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(slot()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(slot1()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), SLOT(slot2()));
}

void MainWindow::edit_data(int data, int begin, int length){
    int ff;
    for(int i = 0, shift = 0; i < length; i++, shift += 8){
        ff = data;
        ff = ff >> shift;
        ff = ff & 0xff;
        arr[begin + i] = (char)ff;
    }
}

void MainWindow::add_pixel(int r, int g, int b, int a){
    arr.push_back((char)r);
    arr.push_back((char)g);
    arr.push_back((char)b);
    arr.push_back((char)a);
}

void MainWindow::add_frame(int wight, int height, int fr){
    QByteArray picture;
    int pint_arr = sum_Byts(10, 13);
    // Меняем размер картинки в заголовке
    edit_data(wight + fr * 2, 18, 4);
    edit_data(height + fr * 2, 22, 4);
    // Переписываем массив пикселей в отдельный массив
    for(int i = pint_arr; i < arr.size(); i++){
        picture.push_back(arr[i]);
    }
    // Удаляем массив пикселей из оригинального массива
    arr.remove(pint_arr, picture.size());
    // Рисуем нижнюю грань
    for(int i = 0; i < fr; i++){
        for(int j = 0; j < wight + fr * 2; j++){
            add_pixel(color_frame.red(), color_frame.green(),
                      color_frame.blue(), color_frame.alpha());
        }
    }
    // Переписываем картинку с добавлением рамки
    for(int i = 0, i_pict = 0; i < height; i++){
        for(int j = 0; j < fr; j++){
            add_pixel(color_frame.red(), color_frame.green(),
                      color_frame.blue(), color_frame.alpha());
        }
        for(int j = 0; j < wight * 4; j++){
            arr.push_back(picture[i_pict]);
            i_pict++;
        }
        for(int j = 0; j < fr; j++){
            add_pixel(color_frame.red(), color_frame.green(),
                      color_frame.blue(), color_frame.alpha());
        }
    }
    // Рисуем верхнюю грань
    for(int i = 0; i < fr; i++){
        for(int j = 0; j < wight + fr * 2; j++){
            add_pixel(color_frame.red(), color_frame.green(),
                      color_frame.blue(), color_frame.alpha());
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_image(QString path)
{
    ui->label->setPixmap(QPixmap(path).scaled(400,
                                              400,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
}

QString MainWindow::format_image(int format)
{
    switch(format){
    case 0:
        return "Invalid";
    case 1:
        return "Mono";
    case 2:
        return "MonoLSB";
    case 3:
        return "Indexed8";
    case 4:
        return "RGB32";
    case 5:
        return "ARGB32";
    case 6:
        return "ARGB32_Premultiplied";
    case 7:
        return "RGB16";
    case 8:
        return "ARGB8565_Premultiplied";
    case 9:
        return "RGB666";
    case 10:
        return "ARGB6666_Premultiplied";
    case 11:
        return "RGB555";
    case 12:
        return "ARGB8555_Premultiplied";
    case 13:
        return "RGB888";
    case 14:
        return "RGB444";
    default:
        return "Other";
    }
}

int MainWindow::sum_Byts(int begin, int end)
{
    int rez = 0;
    for(int i = begin, shift = 0; i <= end; i++, shift += 8){
        rez += QByteArray::fromRawData(&arr[i], 1).toHex().toInt(nullptr, 16) << shift;
    }
    return rez;
}

void MainWindow::slot()
{
    QFile file("C:\\Qt\\Project\\GRI_Lab2\\PERTSEV.bmp");
    file.open(QIODevice::ReadOnly);
    arr = file.readAll();
    file.close();
    // Создаем файл для итогового изображения
    QFile file1("C:\\Qt\\Project\\GRI_Lab2\\PERTSEV1.bmp");
    file1.open(QIODevice::WriteOnly);
    add_frame(inf_wight, inf_height, frame_size);
    for(int i = 0; i < arr.size(); i++){
        file1.putChar(arr[i]);
    }
    file1.close();
    set_image("C:\\Qt\\Project\\GRI_Lab2\\PERTSEV1.bmp");
}

void MainWindow::slot1()
{
    qDebug("fsfzs");
    color_frame = QColorDialog::getColor(Qt::white,
                                          this,
                                          "Выбор цвета рамки",
                                          QColorDialog::ShowAlphaChannel);
//color_frame = QColorDialog::getColor(Qt::red, this);
}

void MainWindow::slot2()
{
    frame_size = ui->horizontalSlider->value();
    ui->label_3->setText(QString::number(frame_size));
}

